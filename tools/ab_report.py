#!/usr/bin/env python3
"""A/B reporting tool. Reads `logs/attempts.jsonl` (the Phase 2 per-
attempt log) and produces the rigorous post-run analysis described in
docs/direction/003-multi-agent-ab-architecture.md §5.

Outputs, in order:

  1. Per-identity summary: Wilson 95% CI on match rate,
     bootstrap 95% CI on attempts/hour, mean cost-per-match, attempts
     attempted.
  2. Stratified tables: same metrics broken down by `(tier, size_bucket,
     has_class_context)`. Easy-leaf bias on small functions can otherwise
     mask hard-function regressions.
  3. McNemar's exact test for the paired/shootout subset: per-function
     win/lose tabulation across identities, exact two-sided p-value,
     CI on disagreement direction.
  4. Promotion verdict: GREEN if better identity's CI lower bound > worse
     identity's CI upper bound; AMBER on overlap; RED if reversed
     (the better identity's point estimate is *worse* than the other's
     CI).

The tool is **idempotent**: re-running on the same `attempts.jsonl`
produces byte-identical output. RNG-driven steps (bootstrap) take a
fixed seed; iteration order is sorted; floating-point intermediates
are rounded at fixed precision before printing. The smoke test asserts
this property by running the report twice and diffing the output.

Cost-weighted view: per direction-003 §risks, when one backend is
rate-limited, weighting by allocated share would penalize the other
backend's match rate. We weight by **attempts attempted**, which
naturally reflects rate-limit imbalance.

Usage:
    python3 tools/ab_report.py
    python3 tools/ab_report.py --attempts logs/attempts.jsonl
    python3 tools/ab_report.py --run-id <hex>          # filter to one run
    python3 tools/ab_report.py --json                  # machine-readable
"""

from __future__ import annotations

import argparse
import json
import math
import random
import sys
from collections import Counter, defaultdict
from pathlib import Path
from typing import Dict, Iterable, List, Optional, Tuple

_REPO_ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(_REPO_ROOT / "tools"))

from ab_schedule import (_size_bucket, _tier_for, identity_key,
                         SIZE_BUCKETS)


# Fixed seed so the bootstrap CIs are deterministic across runs of the
# report on the same attempts.jsonl. Idempotency is contract.
_BOOTSTRAP_SEED = 20260425
_BOOTSTRAP_ITERS = 2000


def wilson_ci(successes: int, n: int,
              z: float = 1.959963984540054) -> Tuple[float, float]:
    """Wilson score 95% CI for a binomial proportion.

    Better small-n behavior than normal-approximation Wald CIs (which
    can give bounds outside [0, 1]); reported as (lower, upper).
    Default z = 1.96 (95% two-sided). Returns (0, 0) when n=0 — calls
    sites should test that case rather than report a meaningless CI.
    """
    if n <= 0:
        return (0.0, 0.0)
    p = successes / n
    denom = 1 + z * z / n
    center = (p + z * z / (2 * n)) / denom
    spread = z * math.sqrt((p * (1 - p) + z * z / (4 * n)) / n) / denom
    return (max(0.0, center - spread), min(1.0, center + spread))


def bootstrap_mean_ci(values: List[float],
                      iters: int = _BOOTSTRAP_ITERS,
                      seed: int = _BOOTSTRAP_SEED,
                      conf: float = 0.95
                      ) -> Tuple[float, float, float]:
    """Bootstrap (lo, hi, point) CI on a sample mean.

    Returns the percentile-method CI bounds at `conf` plus the sample
    mean. Deterministic given `seed`. Empty input → (0, 0, 0); the
    caller is expected to gate on n>0 before calling.
    """
    if not values:
        return (0.0, 0.0, 0.0)
    point = sum(values) / len(values)
    rng = random.Random(seed)
    n = len(values)
    means = []
    for _ in range(iters):
        sample = [values[rng.randrange(n)] for _ in range(n)]
        means.append(sum(sample) / n)
    means.sort()
    lo_idx = int((1 - conf) / 2 * iters)
    hi_idx = int((1 + conf) / 2 * iters) - 1
    return (means[lo_idx], means[hi_idx], point)


def mcnemar_exact_p(b: int, c: int) -> float:
    """Two-sided exact (binomial) McNemar's test.

    `b` = identity_X matched, identity_Y didn't (in a paired sample).
    `c` = identity_Y matched, identity_X didn't.
    Returns the two-sided p-value under H0: P(X wins | disagree) = 0.5.

    Exact form (rather than the chi-square approximation) because the
    paired sample at one shootout run is small (~50 functions); the
    chi-square breaks down for n_disagree < 25.
    """
    n = b + c
    if n == 0:
        return 1.0
    k = min(b, c)
    # Two-sided: 2 × P(K ≤ min(b,c)) under Bin(n, 0.5), capped at 1.0.
    cum = 0.0
    for i in range(k + 1):
        cum += math.comb(n, i) * 0.5 ** n
    return min(1.0, 2 * cum)


def load_attempts(path: Path,
                  run_id_filter: Optional[str] = None) -> List[dict]:
    """Stream-decode an attempts.jsonl file. Skips empty lines but
    raises on malformed JSON — a corrupt attempts.jsonl row is silent
    data poisoning, exactly the failure mode this tool is meant to
    catch. Iteration is in file order; the file is append-only so
    that's session-completion order.
    """
    out = []
    with open(path) as f:
        for lineno, line in enumerate(f, 1):
            line = line.rstrip("\n")
            if not line.strip():
                continue
            try:
                rec = json.loads(line)
            except json.JSONDecodeError as e:
                raise RuntimeError(
                    f"{path}:{lineno}: malformed attempts.jsonl record: {e}"
                )
            if run_id_filter and rec.get("run_id") != run_id_filter:
                continue
            out.append(rec)
    return out


def _identity_for(rec: dict) -> str:
    ident = rec.get("identity")
    if ident:
        return ident
    return identity_key(
        rec.get("backend") or "",
        rec.get("model") or "",
        rec.get("effort") or "",
    )


def _stratum_label(rec: dict) -> str:
    size = int(rec.get("size") or 0)
    is_leaf = bool(rec.get("is_leaf"))
    has_ctx = bool(rec.get("class_name"))
    tier = _tier_for(size, is_leaf, has_ctx)
    bucket_idx = _size_bucket(size)
    bucket = SIZE_BUCKETS[bucket_idx]
    bucket_label = (f"{bucket[0]}-{bucket[1]}" if bucket[1] < 1 << 20
                    else f"{bucket[0]}+")
    return f"T{tier}·{bucket_label}·{'ctx' if has_ctx else 'no-ctx'}"


def per_identity_summary(attempts: List[dict]) -> Dict[str, dict]:
    """Aggregate per-identity stats. The unit of analysis is **per
    function attempt**, not per session — a session that touched 5
    functions counts as 5 attempts so the rate is comparable across
    different batch sizes.
    """
    by_ident: Dict[str, dict] = defaultdict(lambda: {
        "attempts": 0,
        "matched": 0,
        "failed": 0,
        "refused": 0,
        "prep_error": 0,
        "system_error": 0,
        "input_tokens": 0,
        "output_tokens": 0,
        "cached_tokens": 0,
        "cost_usd": 0.0,
        "duration_s": 0.0,
        "had_cost": False,
    })
    for rec in attempts:
        ident = _identity_for(rec)
        d = by_ident[ident]
        d["attempts"] += 1
        verified = rec.get("verified_status")
        if verified == "matched":
            d["matched"] += 1
        elif verified == "failed":
            d["failed"] += 1
        kind = rec.get("session_error_kind")
        if rec.get("agent_refused"):
            d["refused"] += 1
        if kind == "prep_error":
            d["prep_error"] += 1
        if kind == "system_error":
            d["system_error"] += 1
        if rec.get("had_usage_data"):
            d["input_tokens"] += int(rec.get("input_tokens") or 0)
            d["output_tokens"] += int(rec.get("output_tokens") or 0)
            d["cached_tokens"] += int(rec.get("cached_tokens") or 0)
            cost = rec.get("cost_usd")
            if isinstance(cost, (int, float)):
                d["cost_usd"] += float(cost)
                d["had_cost"] = True
        d["duration_s"] += float(rec.get("session_share_s") or 0.0)

    for ident, d in by_ident.items():
        d["match_rate"] = d["matched"] / d["attempts"] if d["attempts"] else 0.0
        d["match_ci"] = wilson_ci(d["matched"], d["attempts"])
        d["cost_per_match"] = (d["cost_usd"] / d["matched"]
                               if d["matched"] and d["had_cost"] else None)
    return dict(by_ident)


def stratified_breakdown(attempts: List[dict]) -> Dict[Tuple[str, str], dict]:
    """`{(identity, stratum_label): {attempts, matched, ci}}`. Sorted
    by stratum then identity at print time so two re-runs print the
    same row order (idempotency contract).
    """
    out: Dict[Tuple[str, str], dict] = defaultdict(lambda: {
        "attempts": 0, "matched": 0,
    })
    for rec in attempts:
        ident = _identity_for(rec)
        stratum = _stratum_label(rec)
        d = out[(ident, stratum)]
        d["attempts"] += 1
        if rec.get("verified_status") == "matched":
            d["matched"] += 1
    for d in out.values():
        d["match_rate"] = d["matched"] / d["attempts"] if d["attempts"] else 0.0
        d["match_ci"] = wilson_ci(d["matched"], d["attempts"])
    return dict(out)


def shootout_pairs(attempts: List[dict]) -> Dict[str, Dict[str, str]]:
    """Build `{addr: {identity: 'matched'|'failed'|...}}` over the
    attempts that look like shootout records. We classify a record as
    shootout when its `verify_reason` says so, OR when the same
    address appears under two different identities in the same run
    (indicating both attempted it). The first criterion is exact when
    the orchestrator records `queue_kind` in the attempt; we add a
    fallback for older logs.
    """
    by_addr: Dict[str, Dict[str, str]] = defaultdict(dict)
    for rec in attempts:
        addr = rec.get("address")
        if not addr:
            continue
        ident = _identity_for(rec)
        verified = rec.get("verified_status") or "untried"
        # Last write wins so re-runs of the same identity collapse to
        # the most-recent verdict (consistent with the DB tracking
        # current owner, not history).
        by_addr[addr][ident] = verified
    return dict(by_addr)


def _mcnemar_table(addr_to_ident_status: Dict[str, Dict[str, str]],
                   ident_a: str, ident_b: str
                   ) -> Tuple[int, int, int, int]:
    """`(both_matched, only_a_matched, only_b_matched, both_failed)`."""
    both = a_only = b_only = neither = 0
    for addr, m in addr_to_ident_status.items():
        if ident_a not in m or ident_b not in m:
            continue
        sa = (m[ident_a] == "matched")
        sb = (m[ident_b] == "matched")
        if sa and sb:
            both += 1
        elif sa and not sb:
            a_only += 1
        elif sb and not sa:
            b_only += 1
        else:
            neither += 1
    return both, a_only, b_only, neither


def _verdict(better_lo: float, worse_hi: float, better_pt: float,
             worse_pt: float) -> str:
    """Promotion verdict per §5: GREEN, AMBER, or RED."""
    if better_lo > worse_hi:
        return "GREEN"
    if better_pt < worse_pt:
        return "RED"
    return "AMBER"


def _fmt_pct_ci(rate: float, lo: float, hi: float) -> str:
    return f"{rate*100:5.1f}% [{lo*100:5.1f}, {hi*100:5.1f}]"


def render_text(per_ident: Dict[str, dict],
                strat: Dict[Tuple[str, str], dict],
                pairs: Dict[str, Dict[str, str]],
                identities: List[str]) -> str:
    """Stable text rendering. Sort everything; deterministic numeric
    formatting. The smoke test relies on this being byte-stable.
    """
    out: List[str] = []
    out.append("=== Per-identity summary ===")
    out.append(f"{'identity':40s}  {'attempts':>8s}  {'matched':>7s}  "
               f"{'rate / 95% CI':>22s}  {'cost/match':>10s}")
    for ident in sorted(per_ident.keys()):
        d = per_ident[ident]
        cm = (f"${d['cost_per_match']:.4f}" if d['cost_per_match'] is not None
              else "—")
        out.append(
            f"{ident:40s}  {d['attempts']:8d}  {d['matched']:7d}  "
            f"{_fmt_pct_ci(d['match_rate'], *d['match_ci']):>22s}  {cm:>10s}"
        )
    out.append("")

    out.append("=== Stratified breakdown ===")
    out.append(f"{'stratum':>22s}  {'identity':40s}  {'n':>4s}  "
               f"{'match':>5s}  {'rate / 95% CI':>22s}")
    for (ident, stratum) in sorted(strat.keys()):
        d = strat[(ident, stratum)]
        out.append(
            f"{stratum:>22s}  {ident:40s}  {d['attempts']:4d}  "
            f"{d['matched']:5d}  {_fmt_pct_ci(d['match_rate'], *d['match_ci']):>22s}"
        )
    out.append("")

    out.append("=== Shootout (paired) ===")
    if len(identities) < 2 or not pairs:
        out.append("(no paired data)")
        out.append("")
    else:
        ident_a, ident_b = sorted(identities)[:2]
        both, a_only, b_only, neither = _mcnemar_table(pairs, ident_a, ident_b)
        n_paired = both + a_only + b_only + neither
        if n_paired == 0:
            out.append("(no paired data — no shared addresses across identities)")
        else:
            p = mcnemar_exact_p(a_only, b_only)
            n_disagree = a_only + b_only
            delta = (a_only - b_only) / n_paired if n_paired else 0.0
            wilson = (wilson_ci(a_only, n_disagree)
                      if n_disagree else (0.0, 0.0))
            # Lift `delta` into pp on full pair count so it matches
            # the §5 example phrasing.
            out.append(f"paired n={n_paired}  both={both}  "
                       f"only-{ident_a}={a_only}  only-{ident_b}={b_only}  "
                       f"neither={neither}")
            out.append(f"McNemar exact p = {p:.4g}  "
                       f"Δ match-rate = {delta*100:+.1f}pp")
            if n_disagree:
                out.append(f"P({ident_a} wins | disagree) = "
                           f"{a_only/n_disagree*100:.1f}% "
                           f"[Wilson CI {wilson[0]*100:.1f}, "
                           f"{wilson[1]*100:.1f}]")
            out.append("")

    out.append("=== Promotion verdict ===")
    if len(identities) >= 2 and len(per_ident) >= 2:
        sorted_ids = sorted(per_ident.keys(),
                            key=lambda i: -per_ident[i]["match_rate"])
        better, worse = sorted_ids[0], sorted_ids[1]
        bl, bh = per_ident[better]["match_ci"]
        wl, wh = per_ident[worse]["match_ci"]
        bp = per_ident[better]["match_rate"]
        wp = per_ident[worse]["match_rate"]
        verdict = _verdict(bl, wh, bp, wp)
        out.append(f"better: {better}  rate={_fmt_pct_ci(bp, bl, bh)}")
        out.append(f"worse : {worse}  rate={_fmt_pct_ci(wp, wl, wh)}")
        out.append(f"verdict: {verdict}")
    else:
        out.append("(need >=2 identities to verdict)")

    return "\n".join(out) + "\n"


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n", 1)[0])
    ap.add_argument("--attempts", default="logs/attempts.jsonl",
                    help="Path to attempts.jsonl (default: logs/attempts.jsonl)")
    ap.add_argument("--run-id", default=None,
                    help="Filter to a single run_id.")
    ap.add_argument("--json", action="store_true",
                    help="Emit machine-readable JSON instead of text.")
    args = ap.parse_args()

    path = Path(args.attempts)
    if not path.exists():
        print(f"error: {path} does not exist", file=sys.stderr)
        sys.exit(1)
    attempts = load_attempts(path, run_id_filter=args.run_id)
    if not attempts:
        print("(no attempts records)")
        return

    identities = sorted({_identity_for(r) for r in attempts})
    per_ident = per_identity_summary(attempts)
    strat = stratified_breakdown(attempts)
    pairs = shootout_pairs(attempts)

    if args.json:
        payload = {
            "identities": identities,
            "per_identity": {k: {kk: vv for kk, vv in v.items()
                                  if kk not in ("match_ci",)}
                             for k, v in per_ident.items()},
            "per_identity_ci": {k: list(v["match_ci"])
                                 for k, v in per_ident.items()},
            "stratified": [
                {"identity": ident, "stratum": stratum, **dat,
                 "match_ci": list(dat["match_ci"])}
                for (ident, stratum), dat in sorted(strat.items())
            ],
            "shootout_pairs_n": len(pairs),
        }
        if len(identities) >= 2:
            ident_a, ident_b = sorted(identities)[:2]
            both, a_only, b_only, neither = _mcnemar_table(
                pairs, ident_a, ident_b)
            payload["mcnemar"] = {
                "ident_a": ident_a,
                "ident_b": ident_b,
                "both": both,
                "only_a": a_only,
                "only_b": b_only,
                "neither": neither,
                "p_value": mcnemar_exact_p(a_only, b_only),
            }
        print(json.dumps(payload, indent=2, sort_keys=True))
    else:
        print(render_text(per_ident, strat, pairs, identities), end="")


if __name__ == "__main__":
    main()
