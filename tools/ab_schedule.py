#!/usr/bin/env python3
"""A/B run schedule pre-computer.

Phase 3 of docs/direction/003-multi-agent-ab-architecture.md. At run
start the orchestrator hands a candidate pool + a list of
(backend, model) identities to `build_schedule()`; it returns a
`Schedule` whose queues the picker draws from for the duration.

Three modes:

  * Mode A — disjoint, stratified. Each function is assigned to exactly
    one identity. Strata are `(tier, size_bucket, has_class_context)`;
    within each stratum, the assignment is round-robin over identities,
    so each identity sees roughly the same workload mix. Strata with
    fewer than `MIN_STRATUM_SIZE` (= 4) functions merge into the
    next-larger size_bucket within the same (tier, has_class_context)
    pair before assignment.

  * Mode B — shootout. Every function is given to every identity. The
    `disjoint` queues are empty; `shootout` carries the deterministic
    function order all identities work through.

  * Mode C — hybrid. `paired_reserve_n` functions are held out as a
    shootout reserved set (stratified to mirror the overall pool's
    distribution); the remainder is scheduled per Mode A. The picker
    consults whichever queue is non-empty for a given identity.

The output is deterministic given (functions, identities, mode,
paired_reserve_n, seed). That property is load-bearing for the smoke
test and makes operator debugging tractable — re-running with the same
inputs reproduces the exact same assignment.

The schedule is a pure-data object. Every mutation (popping a function
off a queue, recording an attempt) goes through Schedule helpers so
that the orchestrator's lock-discipline stays centralized.
"""

from __future__ import annotations

import random
import threading
from dataclasses import dataclass, field
from typing import Dict, Iterable, List, Optional, Set, Tuple

MODE_DISJOINT = "A"
MODE_SHOOTOUT = "B"
MODE_HYBRID = "C"
ALL_MODES = (MODE_DISJOINT, MODE_SHOOTOUT, MODE_HYBRID)

# Strata-merge floor from docs/direction/003 §risks. Below this and the
# stratum can't be split fairly across identities (a 1-function stratum
# would just go to whichever identity drew first). Operator-tunable
# only — agents must not soften it without operator say-so.
MIN_STRATUM_SIZE = 4

# size_bucket boundaries. Inclusive lower, inclusive upper. The buckets
# are independent of `tier` (which already encodes size+leaf+context);
# they exist so larger functions don't all collapse into a single
# tier-3 stratum that hides backend differences on long tails.
SIZE_BUCKETS: Tuple[Tuple[int, int], ...] = (
    (0, 16),
    (17, 64),
    (65, 256),
    (257, 1024),
    (1025, 1 << 30),
)


def identity_key(backend: str, model: str) -> str:
    """Canonical string key for a (backend, model) identity. Used as
    the dict key everywhere so Mode A's per-identity queues, Mode B's
    shootout-attempted sets, and the report's group-by columns all
    line up without re-hashing.
    """
    return f"{backend}/{model}"


def safe_identity_tag(identity: str) -> str:
    """Filesystem-safe rendition of `backend/model` for branch +
    worktree paths (e.g. `claude/claude-opus-4-7` →
    `claude-claude-opus-4-7`, `codex/gpt-5.5` → `codex-gpt-5_5`).
    Used by both the orchestrator (worktree creation) and ab_promote
    (branch lookup) — keeping them identical here prevents drift.
    """
    return identity.replace("/", "-").replace(".", "_")


def _size_bucket(size: int) -> int:
    for i, (lo, hi) in enumerate(SIZE_BUCKETS):
        if lo <= size <= hi:
            return i
    return len(SIZE_BUCKETS) - 1


def _tier_for(size: int, is_leaf: bool, has_class_context: bool) -> int:
    """Same priority tiers as orchestrator.pick_next_batch, lifted out
    so the schedule and the picker don't drift apart silently.
    """
    if size <= 8:
        return 0
    if is_leaf and size <= 64:
        return 1
    if has_class_context:
        return 2
    return 3


def _stratum_key(func: dict, matched_classes: Set[str]) -> Tuple[int, int, bool]:
    size = int(func.get("size") or 0)
    is_leaf = bool(func.get("is_leaf", False))
    cls = func.get("class_name") or ""
    has_class_context = bool(cls and cls in matched_classes)
    return (_tier_for(size, is_leaf, has_class_context),
            _size_bucket(size),
            has_class_context)


def _merge_thin_strata(strata: Dict[Tuple[int, int, bool], List[dict]]
                       ) -> Dict[Tuple[int, int, bool], List[dict]]:
    """Merge any stratum with < MIN_STRATUM_SIZE into the next-larger
    `size_bucket` within the same (tier, has_class_context). If no
    larger bucket exists, fold downward instead so the residue isn't
    silently dropped — losing thin strata would skew the schedule's
    class-context distribution.
    """
    merged: Dict[Tuple[int, int, bool], List[dict]] = {}
    for (tier, bucket, ctx), funcs in strata.items():
        merged[(tier, bucket, ctx)] = list(funcs)

    # Walk from the smallest bucket upward. A thin stratum gets folded
    # into the next-larger bucket within the same (tier, ctx). If the
    # absorbing bucket is itself thin after the merge, the next pass
    # will fold it again.
    by_group: Dict[Tuple[int, bool], List[int]] = {}
    for (tier, bucket, ctx) in merged:
        by_group.setdefault((tier, ctx), []).append(bucket)
    for group in by_group.values():
        group.sort()

    for (tier, ctx), buckets in by_group.items():
        if len(buckets) <= 1:
            continue
        # Bottom-up merge: fold thin lower buckets into the next-larger.
        i = 0
        while i < len(buckets) - 1:
            this_b = buckets[i]
            next_b = buckets[i + 1]
            this_key = (tier, this_b, ctx)
            next_key = (tier, next_b, ctx)
            if len(merged.get(this_key, [])) < MIN_STRATUM_SIZE:
                merged.setdefault(next_key, []).extend(merged.pop(this_key, []))
                buckets.pop(i)
                continue
            i += 1
        # Top-down: a thin top bucket has no larger neighbor. Fold it
        # into the bucket below so it doesn't get assigned alone.
        if buckets:
            top_b = buckets[-1]
            top_key = (tier, top_b, ctx)
            while len(buckets) >= 2 and len(merged.get(top_key, [])) < MIN_STRATUM_SIZE:
                prev_b = buckets[-2]
                prev_key = (tier, prev_b, ctx)
                merged.setdefault(prev_key, []).extend(merged.pop(top_key, []))
                buckets.pop()
                if not buckets:
                    break
                top_b = buckets[-1]
                top_key = (tier, top_b, ctx)

    # Drop empty entries the merging may have left behind.
    return {k: v for k, v in merged.items() if v}


def _build_strata(functions: Iterable[dict],
                  matched_classes: Set[str]
                  ) -> Dict[Tuple[int, int, bool], List[dict]]:
    strata: Dict[Tuple[int, int, bool], List[dict]] = {}
    for f in functions:
        key = _stratum_key(f, matched_classes)
        strata.setdefault(key, []).append(f)
    return _merge_thin_strata(strata)


def _round_robin_assign(funcs: List[dict],
                        identities: List[str],
                        rng: random.Random) -> Dict[str, List[dict]]:
    """Round-robin assignment with a shuffled rotation per stratum.

    Shuffling the function order before the round-robin removes any
    address-ordering bias (untried lists tend to come in by .obj order,
    which would systematically hand one backend the same .obj). The
    rotation start is also drawn from rng so two strata of size N
    don't both hand identity 0 the first slot.
    """
    out: Dict[str, List[dict]] = {ident: [] for ident in identities}
    if not funcs or not identities:
        return out
    shuffled = list(funcs)
    rng.shuffle(shuffled)
    rotation = rng.randrange(len(identities))
    for i, f in enumerate(shuffled):
        ident = identities[(i + rotation) % len(identities)]
        out[ident].append(f)
    return out


def _stratified_holdout(strata: Dict[Tuple[int, int, bool], List[dict]],
                        n: int,
                        rng: random.Random
                        ) -> Tuple[List[dict],
                                   Dict[Tuple[int, int, bool], List[dict]]]:
    """Sample `n` functions stratified to mirror the input distribution.

    Returns (held_out, remaining_strata). Allocates per-stratum quota
    proportional to the stratum's share of the total pool. Rounding
    residuals get distributed to the largest strata so the held-out
    count exactly matches `n` (caller asked for N — silently returning
    N-1 is the kind of off-by-one that ruins an analysis).
    """
    total = sum(len(v) for v in strata.values())
    if total == 0 or n <= 0:
        remaining = {k: list(v) for k, v in strata.items()}
        return [], remaining
    n = min(n, total)

    # Initial proportional allocation, rounded down. The leftover
    # (n - sum_alloc) is dealt out to strata in descending fractional
    # remainder order so larger strata absorb the residual rather than
    # picking up extras from already-saturated thin ones.
    fractions: List[Tuple[Tuple[int, int, bool], int, float]] = []
    for k, v in strata.items():
        share = len(v) * n / total
        floor = int(share)
        fractions.append((k, floor, share - floor))
    quota: Dict[Tuple[int, int, bool], int] = {k: f for k, f, _ in fractions}
    leftover = n - sum(quota.values())
    fractions.sort(key=lambda t: (-t[2], -len(strata[t[0]])))
    i = 0
    while leftover > 0 and fractions:
        k, _, _ = fractions[i % len(fractions)]
        if quota[k] < len(strata[k]):
            quota[k] += 1
            leftover -= 1
        i += 1
        if i > len(fractions) * 4:
            # Safety: every stratum saturated. The min(n, total) cap
            # above should make this unreachable; raise loud if not.
            raise RuntimeError(
                "stratified_holdout: unable to place residual quota — "
                "saturation logic bug")

    held: List[dict] = []
    remaining: Dict[Tuple[int, int, bool], List[dict]] = {}
    for k, funcs in strata.items():
        funcs = list(funcs)
        rng.shuffle(funcs)
        q = quota.get(k, 0)
        held.extend(funcs[:q])
        if funcs[q:]:
            remaining[k] = funcs[q:]
    return held, remaining


@dataclass
class Schedule:
    """Pre-computed assignment for one orchestrator run.

    Threadsafe: every mutator acquires `_lock`. The orchestrator's
    coordinator thread holds db_lock when calling these (per Phase 1
    discipline), but the lock here is independent so call sites in the
    commit thread / report tool don't have to know about db_lock.

    `disjoint[ident]` is the per-identity queue for Mode A / Mode C
    remainder. `shootout` is the shared reserved set; `shootout_done`
    tracks which (identity, addr) pairs have been picked so neither
    identity attempts the same shootout function twice.
    """

    mode: str
    identities: List[str]
    disjoint: Dict[str, List[dict]] = field(default_factory=dict)
    shootout: List[dict] = field(default_factory=list)
    shootout_done: Dict[str, Set[str]] = field(default_factory=dict)
    paired_reserve_n: int = 0
    seed: int = 0
    _lock: threading.Lock = field(default_factory=threading.Lock, repr=False)
    # Cached per-identity address sets — `disjoint` is set at
    # construction and never mutated, `shootout_done` only grows
    # monotonically. The picker hits these on every batch pick, so
    # rebuilding them per call would be O(pool) hot-path bloat.
    _disjoint_addrs_cache: Dict[str, Set[str]] = field(default_factory=dict, repr=False)
    _shootout_all_addrs: Set[str] = field(default_factory=set, repr=False)

    def _rebuild_caches(self) -> None:
        self._disjoint_addrs_cache = {
            ident: {f["address"] for f in funcs}
            for ident, funcs in self.disjoint.items()
        }
        self._shootout_all_addrs = {f["address"] for f in self.shootout}

    def disjoint_addrs_for(self, ident: str) -> Set[str]:
        """Per-identity disjoint set, cached. Returns the live set —
        callers must not mutate; `disjoint` is immutable post-build
        but the cache is shared across pick calls.
        """
        with self._lock:
            return self._disjoint_addrs_cache.get(ident, set())

    @property
    def shootout_addrs(self) -> Set[str]:
        with self._lock:
            return set(self._shootout_all_addrs)

    def shootout_remaining(self, ident: str) -> Set[str]:
        """Addresses still un-attempted by `ident` from the shootout
        queue. Excludes `shootout_done[ident]` set members. Returns a
        copy — caller can't accidentally mutate the schedule's state.
        """
        with self._lock:
            done = self.shootout_done.get(ident, set())
            return self._shootout_all_addrs - done

    def disjoint_remaining(self, ident: str,
                           untried_addrs: Set[str]) -> List[dict]:
        """Functions still in `ident`'s disjoint queue whose address
        appears in `untried_addrs`. The orchestrator passes the live
        untried set so previously-popped batches don't bounce back.
        """
        with self._lock:
            return [f for f in self.disjoint.get(ident, [])
                    if f["address"] in untried_addrs]

    def mark_shootout_attempted(self, ident: str,
                                addrs: Iterable[str]) -> None:
        with self._lock:
            done = self.shootout_done.setdefault(ident, set())
            done.update(addrs)

    def unmark_shootout_attempted(self, ident: str,
                                  addrs: Iterable[str]) -> None:
        """Revert an in_progress claim — called when a session aborts
        before producing results so the function returns to the queue.
        """
        with self._lock:
            done = self.shootout_done.get(ident)
            if not done:
                return
            for a in addrs:
                done.discard(a)

    def to_summary_dict(self) -> dict:
        """Operator-readable summary; used by the orchestrator at run
        start to log what the schedule looks like.
        """
        with self._lock:
            return {
                "mode": self.mode,
                "identities": list(self.identities),
                "paired_reserve_n": self.paired_reserve_n,
                "seed": self.seed,
                "disjoint_counts": {ident: len(funcs)
                                    for ident, funcs in self.disjoint.items()},
                "shootout_count": len(self.shootout),
            }


def build_schedule(functions: Iterable[dict],
                   identities: List[str],
                   mode: str,
                   paired_reserve_n: int = 0,
                   seed: int = 42,
                   matched_classes: Optional[Set[str]] = None,
                   ) -> Schedule:
    """Construct a Schedule from the candidate pool.

    `functions` is the candidate set the picker would otherwise see —
    typically the un-tried, eligible-by-args slice. The schedule does
    NOT re-filter by match_status; the caller is expected to have
    already filtered. This keeps the schedule's job to "assignment"
    and avoids two sources of truth on what's eligible.

    `matched_classes` — set of class names with at least one match in
    main. Used to compute `has_class_context` per stratum. The picker
    uses the same field for tier=2 priority; passing it in here keeps
    the stratification consistent with prioritization.
    """
    if mode not in ALL_MODES:
        raise ValueError(f"build_schedule: unknown mode {mode!r}")
    identities = list(dict.fromkeys(identities))
    if not identities:
        raise ValueError("build_schedule: identities must be non-empty")
    if mode == MODE_SHOOTOUT and paired_reserve_n:
        raise ValueError(
            "build_schedule: paired_reserve_n only valid in Mode C; "
            "Mode B uses the full pool as the shootout set")
    if mode == MODE_HYBRID and paired_reserve_n <= 0:
        raise ValueError(
            "build_schedule: Mode C requires paired_reserve_n > 0; "
            "use Mode A if no shootout slice is wanted")

    matched_classes = matched_classes or set()
    rng = random.Random(seed)
    funcs_list = list(functions)

    sched = Schedule(
        mode=mode,
        identities=identities,
        paired_reserve_n=paired_reserve_n,
        seed=seed,
    )
    sched.shootout_done = {ident: set() for ident in identities}

    if mode == MODE_SHOOTOUT:
        # Deterministic order so each identity walks the shootout list
        # in the same sequence — controls for ordering effects on
        # context-building.
        sched.shootout = sorted(funcs_list, key=_priority_sort_key)
        sched.disjoint = {ident: [] for ident in identities}
        sched._rebuild_caches()
        return sched

    strata = _build_strata(funcs_list, matched_classes)

    if mode == MODE_HYBRID:
        held, remaining_strata = _stratified_holdout(
            strata, paired_reserve_n, rng)
        sched.shootout = sorted(held, key=_priority_sort_key)
    else:
        remaining_strata = strata

    disjoint: Dict[str, List[dict]] = {ident: [] for ident in identities}
    # Sort strata deterministically so the round-robin start rotates
    # across reproducible groups. Using the tuple key directly yields
    # ascending-tier-then-bucket order.
    for key in sorted(remaining_strata.keys()):
        funcs = remaining_strata[key]
        assigned = _round_robin_assign(funcs, identities, rng)
        for ident, lst in assigned.items():
            disjoint[ident].extend(lst)
    for ident in identities:
        disjoint[ident].sort(key=_priority_sort_key)
    sched.disjoint = disjoint
    sched._rebuild_caches()
    return sched


def _priority_sort_key(func: dict) -> Tuple[int, int]:
    """Same key shape as orchestrator.pick_next_batch's priority sort.

    The schedule itself doesn't compute `has_class_context` here
    (that's used for stratification, not within-queue prioritization);
    keeping the within-queue sort by (size, addr) means the picker's
    batch grouping behavior is unchanged for any given identity's
    queue.
    """
    return (int(func.get("size") or 0), int(func.get("address", "0x0"), 16))


if __name__ == "__main__":
    # Self-check: run a tiny build and print the summary. Lets an
    # operator eyeball schedule fairness without spinning up the
    # orchestrator.
    import argparse
    import json
    import sys

    ap = argparse.ArgumentParser()
    ap.add_argument("--mode", default=MODE_DISJOINT, choices=ALL_MODES)
    ap.add_argument("--identities", default="claude/claude-opus-4-7,codex/gpt-5.5",
                    help="Comma-separated backend/model pairs.")
    ap.add_argument("--reserve", type=int, default=0,
                    help="paired_reserve_n for Mode C.")
    ap.add_argument("--seed", type=int, default=42)
    ap.add_argument("--db", default="config/functions.json")
    args = ap.parse_args()

    with open(args.db) as f:
        all_funcs = json.load(f)
    untried = [f for f in all_funcs if f.get("match_status") == "untried"]
    matched_classes = {f["class_name"] for f in all_funcs
                       if f.get("match_status") == "matched"
                       and f.get("class_name")}
    idents = [s.strip() for s in args.identities.split(",") if s.strip()]
    sched = build_schedule(untried, idents, args.mode,
                           paired_reserve_n=args.reserve, seed=args.seed,
                           matched_classes=matched_classes)
    json.dump(sched.to_summary_dict(), sys.stdout, indent=2)
    sys.stdout.write("\n")
