#!/usr/bin/env python3
"""Overnight autonomous matching orchestrator.

Picks functions from the database, prepares context, invokes Claude Code
in headless mode to write and match C source, and tracks results.

Usage:
    python3 tools/orchestrator.py --hours 8                  # 8-hour run
    python3 tools/orchestrator.py --hours 2 --size-max 8     # trivial only
    python3 tools/orchestrator.py --dry-run --limit 3        # test run
    python3 tools/orchestrator.py --hours 8 --class eWorld   # target class
    python3 tools/orchestrator.py --hours 8 --targets config/finetune_targets.json  # targeted mode
"""

import argparse
import concurrent.futures
import glob
import importlib
import json
import os
import queue
import random
import re
import subprocess
import sys
import threading
import time
import uuid
from dataclasses import dataclass, field
from datetime import datetime, timedelta, timezone
from typing import List, Optional, Set

from common import (DB_PATH, EBOOT_PATH, OBJDUMP,
                    canonical_method_pattern, load_db, save_db,
                    filter_functions, build_addr_map,
                    fix_vfpu_disassembly, strip_cpp_comments)
from byte_match import CompileFailed, check_byte_match
from backends import AgentRefused, AVAILABLE_BACKENDS, get_backend, run_session
from ab_schedule import (Schedule, build_schedule, identity_key,
                         safe_identity_tag,
                         MODE_DISJOINT, MODE_SHOOTOUT, MODE_HYBRID)

# db_lock: every DB read/write goes through this; workers never touch
# the DB directly. git_lock: serializes git subprocess calls — git's
# index.lock collides under concurrent mutations. See
# docs/direction/003-multi-agent-ab-architecture.md.
db_lock = threading.Lock()
git_lock = threading.Lock()

# Sentinel class key for free-function batches (no class_name in the DB).
# Uses a value that can't collide with a real class name.
FREE_CLASS_KEY = "<free>"

# apply_outcome return-kind constants. The coordinator dispatch reads
# these to drive the circuit breaker and backoff. Constants beat raw
# strings because typos here become silent miscategorization.
OUTCOME_SUCCESS = "success"
OUTCOME_REFUSAL = "refusal"
OUTCOME_AGENT_FAIL = "agent_fail"
OUTCOME_SYSTEM_ERROR = "system_error"
OUTCOME_PREP_ERROR = "prep_error"


def apply_decisions_to_funcs(addr_index, decisions, session_id,
                              backend_name, backend_model,
                              matched_at, variant):
    """Mutate `addr_index`-resolved DB rows in-place per session
    decisions. Used both for the in-memory DB (disjoint sessions) and
    the worktree's on-disk DB (shootout commits) so the two paths
    can't drift on what `matched_by_*` looks like.

    Raises if `addr_index` is missing a decision's address — that's
    desync between batch and DB and should fail loud.
    """
    for d in decisions:
        target = addr_index.get(d.address)
        if target is None:
            raise RuntimeError(
                f"apply_decisions_to_funcs: decision references unknown "
                f"address {d.address}"
            )
        target["match_status"] = d.status
        if d.src_file is not None:
            target["src_file"] = d.src_file
        if d.symbol_name is not None:
            target["symbol_name"] = d.symbol_name
        if d.status == "matched":
            target["matched_by_backend"] = backend_name
            target["matched_by_session_id"] = session_id
            target["matched_by_model"] = backend_model
            target["matched_at"] = matched_at
        if d.failure_note:
            target.setdefault("failure_notes", []).append({
                "session": session_id,
                "notes": d.failure_note,
                "backend": backend_name,
                "model": backend_model,
                "variant": variant,
                "timestamp": matched_at,
            })
            target["failure_notes"] = target["failure_notes"][-5:]


@dataclass
class FunctionDecision:
    """Worker's final verdict for one batch address — post-verify,
    post-quality-gates. None-valued fields mean "don't update", not
    "set to None"; apply_outcome only writes fields that are not None.
    """
    address: str
    status: str
    src_file: Optional[str] = None
    symbol_name: Optional[str] = None
    failure_note: Optional[str] = None
    claimed_status: Optional[str] = None
    verify_reason: Optional[str] = None
    rejected_extern_c: bool = False


@dataclass
class WorkContext:
    """Inputs handed to a worker thread. `peer_query` is queried at
    partition time (not captured at submit) so peers that join mid-
    session are still observed — without it, two concurrent workers
    each classify the other's still-dirty `src/Bar.cpp` as out-of-scope
    and revert it (bug #4 reborn under multi-agent).

    Phase 3 fields:
    `worktree` — None for shared main tree (Mode A), or absolute path
    for shootout worktrees (Mode B/C reserved slice). Threaded into
    every git op + the agent's spawn cwd so isolated trees don't
    leak edits into main.
    `identity` — `"backend/model"` string the schedule uses as a key
    so attempts.jsonl and ab_report can group consistently.
    `queue_kind` — "disjoint" | "shootout"; tells apply_outcome
    whether to update the in-memory `functions` (disjoint, main DB)
    or skip the in-memory write (shootout — DB write is committed
    inside the worktree by the commit thread).
    """
    batch: list
    backend: object
    variant: str
    classes: set
    session_id: str
    log_path: str
    session_timeout: int
    functions: list
    exact_paths: set
    sibling_prefixes: tuple
    peer_query: object = None
    worktree: Optional[str] = None
    identity: str = ""
    queue_kind: str = "disjoint"


@dataclass
class SessionOutcome:
    """Worker → coordinator return payload. Worker is read-only on the
    DB; the coordinator applies `decisions` under db_lock and runs the
    git commit. `session_usage` is empty when no usage events were
    captured (prep_error, agent crash before first turn).
    """
    session_id: str
    backend_name: str
    backend_model: str
    variant: str
    batch: List[dict]
    classes: Set[str]
    success: bool = False
    error_msg: Optional[str] = None
    refused: bool = False
    refusal_reason: Optional[str] = None
    prep_error: Optional[str] = None
    system_error: Optional[str] = None
    session_duration: float = 0.0
    claimed_matched: int = 0
    claimed_failed: int = 0
    decisions: List[FunctionDecision] = field(default_factory=list)
    matched_funcs: List[dict] = field(default_factory=list)
    matched_files: Set[str] = field(default_factory=set)
    ledger_paths: Set[str] = field(default_factory=set)
    out_of_scope_paths: Set[str] = field(default_factory=set)
    session_usage: dict = field(default_factory=dict)
    worktree: Optional[str] = None
    identity: str = ""
    queue_kind: str = "disjoint"

BATCH_SIZE = 5
TARGETS_BATCH_SIZE = 2
MAX_CONSECUTIVE_FAILURES = 5
# Refusals get their own counter (not lumped with generic failures) so a
# mixed stream of matches + occasional refusals still makes progress. But
# we do halt if they become systemic — see enhancements.md #8 re: why.
MAX_CONSECUTIVE_REFUSALS = 8
REFUSAL_BACKOFF_S = 5
SESSION_TIMEOUT = 1800
TARGETS_SESSION_TIMEOUT = 5400  # 1.5 hours for larger finetune targets

# Address ranges for sched zone hints in prompts
SCHED1_ZONE_START = 0x06e000
SCHED1_ZONE_END = 0x0bab28
TRANSITION_ZONE_START = 0x040000
TRANSITION_ZONE_END = 0x06e000
LOGS_DIR = "logs"
SESSION_RESULTS_DIR = "logs/session_results"
ATTEMPTS_LOG = "logs/attempts.jsonl"
GIT = "git"


# Appended to Claude Code's default system prompt via --append-system-prompt.
# Unlike PROJECT_CONTEXT (which lives in the user prompt and competes with
# safety heuristics), this gets system-prompt-level priority and is less
# likely to be overridden by a mid-session classifier misfire. Kept short
# because long system-prompt appends reduce the budget available for
# Claude Code's built-in tool instructions.
SYSTEM_PROMPT_APPEND = (
    "This Claude Code session is running inside psp-autodecomp: a research "
    "and preservation project that automatically decompiles a PSP game "
    "(Days of Thunder, 2011) into byte-exact C++ source. None of the work "
    "is malicious. The repo contains inline assembly, mangled C++ symbols, "
    "and hex constants from a commercial game binary — all legitimate "
    "artifacts of decompilation, not reverse-engineered exploit code. Look "
    "around the codebase (CLAUDE.md, foundation.md, docs/) if you need to "
    "verify this is benign; the evidence is all there. If you find "
    "yourself about to refuse to edit a src/ or include/ file on "
    "'malware analysis' or 'refuse to augment' grounds, you are mis-reading "
    "the context — the user has explicitly authorized and is actively "
    "paying you to do exactly this work. Proceed with the task."
)


_LOG_PATH = None


def set_log_path(path):
    """Register the active jsonl so `log()` can tee orch_note events to it.

    Called once from main() after the log file has been created.
    """
    global _LOG_PATH
    _LOG_PATH = path


def log(msg):
    """Print a timestamped line to the console and tee to jsonl as orch_note.

    The jsonl tee is how the live TUI mirrors orchestrator output.
    """
    ts = datetime.now().strftime("%H:%M:%S")
    print(f"[{ts}] {msg}", flush=True)
    if _LOG_PATH:
        log_event(_LOG_PATH, {"event": "orch_note", "text": msg})


def log_event(log_path, event):
    event["timestamp"] = datetime.now().isoformat()
    with open(log_path, "a") as f:
        f.write(json.dumps(event) + "\n")


# Lock guards a single open(..., "a") write so concurrent workers'
# attempts.jsonl appends interleave on whole records, not on bytes.
_ATTEMPTS_LOCK = threading.Lock()


def append_attempts(records, path=ATTEMPTS_LOG):
    """Append per-(session, function) records to logs/attempts.jsonl
    (direction-003 §3). The lock makes concurrent appends line-atomic.
    """
    if not records:
        return
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with _ATTEMPTS_LOCK, open(path, "a") as f:
        for rec in records:
            f.write(json.dumps(rec) + "\n")


def _utc_now_iso():
    """ISO-8601 UTC timestamp with explicit `Z` for migration parity."""
    return datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")


def _build_attempt_base(session_id, run_id, backend_name, model, variant,
                       func, session_duration, batch_size):
    """Common header fields shared by every attempts.jsonl record for a
    given (session, function). Per-status fields are layered in by the
    caller — this just centralizes the identity columns so backend/model
    /variant attribution can't drift between code paths.
    """
    return {
        "ts": _utc_now_iso(),
        "run_id": run_id,
        "session_id": session_id,
        "backend": backend_name,
        "model": model,
        "variant": variant,
        "address": func["address"],
        "name": func["name"],
        "class_name": func.get("class_name") or "",
        "size": func.get("size", 0),
        "is_leaf": bool(func.get("is_leaf", False)),
        "obj_file": func.get("obj_file") or "",
        "session_duration_s": round(session_duration, 3),
        "session_share_s": round(session_duration / max(1, batch_size), 3),
    }


def _per_attempt_usage_share(session_usage, batch_size):
    """Split the session's summed token counts evenly across attempts so
    each attempts.jsonl row carries its share, not the session total.
    When the backend never emitted usage (refused before first turn,
    prep_error, etc.), tokens come through as None — distinct from a
    real-zero session, which the report tool needs to tell apart.
    """
    n = max(1, batch_size)
    had_usage = bool(session_usage.get("had_usage_data"))
    if not had_usage:
        return {
            "input_tokens": None,
            "output_tokens": None,
            "cached_tokens": None,
            "cost_usd": None,
            "had_usage_data": False,
        }
    in_t = int(session_usage.get("input_tokens", 0))
    out_t = int(session_usage.get("output_tokens", 0))
    cached_t = int(session_usage.get("cached_tokens", 0))
    cost = session_usage.get("cost_usd")
    return {
        "input_tokens": in_t // n,
        "output_tokens": out_t // n,
        "cached_tokens": cached_t // n,
        "cost_usd": (cost / n) if isinstance(cost, (int, float)) else None,
        "had_usage_data": True,
    }


def emit_attempts_for_outcome(outcome, run_id, error_kind=None,
                              path=ATTEMPTS_LOG):
    """Append one attempts.jsonl record per (session_id, function).
    `error_kind` (set on the abort path) is the OUTCOME_* string the
    coordinator attributed the session-level failure to; success sets
    it None.
    """
    decision_by_addr = {d.address: d for d in outcome.decisions}
    usage_share = _per_attempt_usage_share(
        outcome.session_usage, len(outcome.batch))

    records = []
    for func in outcome.batch:
        rec = _build_attempt_base(
            outcome.session_id, run_id, outcome.backend_name,
            outcome.backend_model, outcome.variant, func,
            outcome.session_duration, len(outcome.batch),
        )
        rec.update(usage_share)

        d = decision_by_addr.get(func["address"])
        rec["claimed_status"] = d.claimed_status if d else None
        rec["verified_status"] = d.status if d else None
        rec["verify_reason"] = d.verify_reason if d else None
        rec["rejected_extern_c"] = bool(d and d.rejected_extern_c)
        rec["agent_refused"] = bool(outcome.refused)
        rec["prep_error"] = outcome.prep_error
        rec["session_error_kind"] = error_kind
        records.append(rec)

    append_attempts(records, path=path)


def set_batch_status(functions, batch, status, addr_index=None):
    """Set match_status for all functions in a batch. O(1) per function."""
    if addr_index is None:
        addr_index = build_addr_map(functions)
    for func in batch:
        target = addr_index.get(func["address"])
        if target:
            target["match_status"] = status


def revert_in_progress(functions, batch, addr_index=None):
    """Revert in_progress functions back to untried."""
    if addr_index is None:
        addr_index = build_addr_map(functions)
    for func in batch:
        target = addr_index.get(func["address"])
        if target and target["match_status"] == "in_progress":
            target["match_status"] = "untried"


def load_targets_file(path):
    """Load a targets JSON file (e.g., finetune_targets.json).

    Returns list of target dicts with at least 'address' fields.
    Validates that every entry has an 'address' field.
    """
    with open(path, "r") as f:
        targets = json.load(f)
    if not isinstance(targets, list):
        raise ValueError(f"Targets file {path} must be a JSON array")
    for i, t in enumerate(targets):
        if "address" not in t:
            raise ValueError(f"Target entry {i} in {path} missing 'address' field: {t}")
    return targets


def _group_and_fill_batch(candidates, batch_size):
    """Group candidates by class when possible, fill remaining slots.

    Takes a pre-sorted candidate list and returns a batch up to batch_size.
    """
    batch = []
    if candidates[0].get("class_name"):
        target_class = candidates[0]["class_name"]
        batch = [f for f in candidates if f["class_name"] == target_class][:batch_size]

    if len(batch) < batch_size:
        batch_addrs = {f["address"] for f in batch}
        for c in candidates:
            if c["address"] not in batch_addrs:
                batch.append(c)
                if len(batch) >= batch_size:
                    break

    return batch


def _class_key(func):
    """Return the lock-key for a function (class name or FREE_CLASS_KEY)."""
    cls = func.get("class_name") or ""
    return cls if cls else FREE_CLASS_KEY


def compute_batch_classes(batch, class_to_header=None):
    """Set of lock keys to acquire for this batch.

    Pre-Phase-3 this returned only class names. Phase 3 expands to
    include each class's *declaring header* (when known) so two
    concurrent batches on classes sharing a header
    (e.g., cFilePlatform + cBufferedFile both in include/cFile.h)
    are mutually exclusive — without this, both batches would
    legitimately need to edit the same file and race.

    Free functions collapse to a single FREE_CLASS_KEY so any
    free-function batch blocks any other free-function batch (they
    all share src/free_functions.c).
    """
    keys = {_class_key(f) for f in batch}
    if class_to_header:
        for f in batch:
            cls = f.get("class_name") or ""
            if not cls:
                continue
            hdr = class_to_header.get(cls)
            if hdr:
                keys.add(hdr)
    return keys


def _candidate_lock_keys(func, class_to_header=None):
    """Lock keys a candidate function would need to acquire — used
    by the picker to filter candidates whose declaring header is
    already in flight.
    """
    keys = {_class_key(func)}
    cls = func.get("class_name") or ""
    if cls and class_to_header:
        hdr = class_to_header.get(cls)
        if hdr:
            keys.add(hdr)
    return keys


def _safe_class_filename(class_name):
    """Convert a C++ class name to its src/include base filename."""
    cs = class_name.replace("::", "_")
    return "".join(c for c in cs if c.isalnum() or c in "_-")


# Matches `class Foo {`, `class Foo : public Bar`, `struct Foo {`,
# `struct Foo : Bar`. Skips forward declarations (`class Foo;`) by
# requiring `:` or `{` immediately following the name. Multi-line
# definitions where `{` lands on the next line are caught by an
# alternate match below; we run both passes per file.
_CLASS_DEF_RE = re.compile(
    r'^\s*(?:class|struct)\s+(\w+)\s*[:{]', re.MULTILINE)
# Same intent but for the multi-line case: `class Foo\n{`. The
# negative lookahead `(?!\s*;)` filters `class Foo /* ... */;`
# forward decls that span lines.
_CLASS_DEF_NEWLINE_RE = re.compile(
    r'^\s*(?:class|struct)\s+(\w+)\s*$\s*\{',
    re.MULTILINE)


def scan_class_headers(include_dir="include"):
    """Return `{class_name: relative_header_path}` for every class
    *definition* found under `include_dir`. Forward declarations are
    skipped — we want the file where the class body lives so the
    file ledger can include it as in-scope when an agent's match
    needs to add a member declaration.

    On a class declared in multiple headers (genuine duplicate
    definition, rare in this codebase), first-found wins. Operator-
    visible: a warning is logged when a duplicate is detected so
    the operator notices structural ambiguity before it bites.
    """
    mapping: dict = {}
    duplicates: list = []
    for path in sorted(glob.glob(os.path.join(include_dir, "*.h"))):
        try:
            with open(path) as f:
                content = f.read()
        except OSError:
            continue
        rel = path
        seen_in_file: set = set()
        for m in _CLASS_DEF_RE.finditer(content):
            cls = m.group(1)
            if cls in seen_in_file:
                continue
            seen_in_file.add(cls)
            if cls in mapping and mapping[cls] != rel:
                duplicates.append((cls, mapping[cls], rel))
            else:
                mapping.setdefault(cls, rel)
        for m in _CLASS_DEF_NEWLINE_RE.finditer(content):
            cls = m.group(1)
            if cls in seen_in_file:
                continue
            seen_in_file.add(cls)
            if cls in mapping and mapping[cls] != rel:
                duplicates.append((cls, mapping[cls], rel))
            else:
                mapping.setdefault(cls, rel)
    return mapping, duplicates


def _src_to_o_path(src_file, worktree=None):
    """Mirror byte_match.compile_src's mapping for `src/`-prefixed
    paths only: `src/foo.cpp` → `build/src/foo.cpp.o` (worktree-
    relative when set). Returns None for any other prefix —
    intentionally narrower than `compile_src`'s basename fallback,
    since the re-verify only ever invalidates `src/*` files (the
    file ledger restricts agent writes to `src/` and `include/`,
    and `include/*` produces no .o).
    """
    rel = src_file.lstrip("./")
    if not rel.startswith("src/"):
        return None
    o_rel = "build/src/" + rel[len("src/"):] + ".o"
    if worktree:
        return os.path.join(worktree, o_rel)
    return o_rel


def compute_allowed_paths(batch, class_to_header=None):
    """Per-session file ledger: paths the agent is allowed to modify.

    Conventional set: `{src/<Class>.cpp, include/<Class>.h,
    src/<Class>_*.cpp}` for each class in the batch.

    Phase-3 expansion via `class_to_header` (built once per run by
    `scan_class_headers`): if a class's body actually lives in a
    differently-named header — `cFilePlatform` declared in
    `include/cFile.h`, `eRigidBodyState` in `include/eRigidBody.h`,
    etc. — that file is added too so the agent can legitimately add
    a member declaration without the partition reverting it as out-
    of-scope. Without this, ~half of "secondary" classes (whose
    declaration shares a header with their parent class) couldn't
    match without falsely tripping the post-revert re-verify.

    Returned as (exact_paths, sibling_prefixes). The sibling pattern
    `src/<Class>_*.cpp` is matched at partition time, not enumerated
    at pick time, so files the agent creates mid-session are still
    in scope.
    """
    classes = {f.get("class_name") for f in batch}
    classes.discard(None)
    classes.discard("")
    if not classes:
        return ({"src/free_functions.c"}, ())
    exact = set()
    prefixes = []
    for cls in classes:
        cs = _safe_class_filename(cls)
        exact.add(f"src/{cs}.cpp")
        exact.add(f"include/{cs}.h")
        prefixes.append(f"src/{cs}_")
        # Add the actual declaring header (could be the same as
        # include/<Class>.h, in which case the set absorbs it).
        if class_to_header:
            actual = class_to_header.get(cls)
            if actual:
                exact.add(actual)
    return (exact, tuple(prefixes))


def is_path_allowed(path, exact_paths, sibling_prefixes):
    """Predicate matching the (exact_paths, sibling_prefixes) ledger."""
    if path in exact_paths:
        return True
    if path.endswith(".cpp") and path.startswith(sibling_prefixes):
        return True
    return False


def partition_dirty_paths(exact_paths, sibling_prefixes, peer_query=None,
                          worktree=None):
    """Returns (ledger_paths, out_of_scope_paths) over dirty `src/` and
    `include/` paths only. Non-src/non-include paths (config/, logs/,
    tools/) are skipped: the coordinator legitimately mutates
    config/functions.json during the same session and reverting it
    would clobber the in-progress DB; agent overstepping into tools/ or
    Makefile is caught by the next pre-flight build instead.

    `peer_query` returns other in-flight workers' allowed-set tuple.
    Paths matching a peer's set are dropped, not classified as
    out-of-scope — they're a peer's still-in-flight ledger.

    `worktree` (Phase 3 shootout) scopes the dirty-paths read to a
    specific worktree. Without it, the read is from the orchestrator's
    CWD — the shared main worktree — exactly the Phase 1 behavior.
    """
    # --untracked-files=all: git's default `normal` collapses a fully-
    # untracked dir to one `?? dir/` line, hiding the individual files
    # the partition needs to see.
    ok, out, err = git_run("status", "--porcelain", "--untracked-files=all",
                           cwd=worktree)
    if not ok:
        raise RuntimeError(
            f"git status failed — can't determine dirty paths: {err}"
        )
    if peer_query is not None:
        peer_exact, peer_prefixes = peer_query()
    else:
        peer_exact, peer_prefixes = set(), ()
    ledger = set()
    out_of_scope = set()
    for line in out.splitlines():
        if len(line) < 4:
            continue
        path = line[3:]
        if " -> " in path:
            path = path.split(" -> ", 1)[1]
        if not path.startswith(("src/", "include/")):
            continue
        if is_path_allowed(path, exact_paths, sibling_prefixes):
            ledger.add(path)
        elif is_path_allowed(path, peer_exact, peer_prefixes):
            # Peer's still-in-flight ledger — leave it alone.
            continue
        else:
            out_of_scope.add(path)
    return ledger, out_of_scope


def revert_paths(paths, worktree=None):
    """Revert dirty paths. Tracked → `git checkout`; untracked → unlink.
    Raises RuntimeError on any failure: a stuck out-of-scope path
    survives into the next session's partition and risks committing
    cross-session contamination — the exact failure Phase 1 is meant
    to prevent.

    `worktree` (Phase 3 shootout) targets a specific worktree's index
    + working tree; untracked-file removal also resolves against that
    tree. None means the orchestrator's CWD, today's behavior.
    """
    for p in paths:
        ok, _, _ = git_run("ls-files", "--error-unmatch", "--", p,
                           cwd=worktree)
        if ok:
            ok2, out, err = git_run("checkout", "--", p, cwd=worktree)
            if not ok2:
                raise RuntimeError(
                    f"revert_paths: git checkout -- {p} failed: {err or out}"
                )
        else:
            full = os.path.join(worktree, p) if worktree else p
            if not os.path.exists(full):
                continue
            try:
                os.remove(full)
            except OSError as e:
                raise RuntimeError(
                    f"revert_paths: os.remove({full}) failed: {e}"
                ) from e


def pick_next_batch_targeted(functions, targets, addr_index, batch_size,
                             excluded_classes=None, allowed_addrs=None,
                             check_status=True, class_to_header=None):
    """Select the next batch from a specific targets list.

    Only picks targets that are untried in the database.
    Priority order matches the targets file order (critical first).
    Warns about target addresses not found in the database.
    `excluded_classes` filters out functions whose class is currently
    locked by another in-flight session.

    `allowed_addrs` (Phase 3) restricts the candidate pool to the
    schedule-assigned subset for the calling identity. None means no
    restriction — the disjoint queue's allowed-set ⊇ untried in
    Mode A, so passing None preserves Phase 1/2 behavior.

    `check_status` toggles the `match_status == "untried"` filter.
    Shootout queries pass False because shootout DB rows stay
    `untried` in main while still being attempted in worktrees —
    the schedule's `shootout_done` is the per-identity authority.
    """
    excluded_classes = excluded_classes or set()
    candidates = []
    for t in targets:
        addr = t["address"]
        func = addr_index.get(addr)
        if not func:
            log(f"  WARNING: target {addr} ({t.get('name', '?')}) not in database — skipping")
            continue
        if check_status and func["match_status"] != "untried":
            continue
        if _candidate_lock_keys(func, class_to_header) & excluded_classes:
            continue
        if allowed_addrs is not None and addr not in allowed_addrs:
            continue
        candidates.append(func)

    if not candidates:
        return []

    return _group_and_fill_batch(candidates, batch_size)


def pick_next_batch(functions, args, batch_size=BATCH_SIZE,
                    excluded_classes=None, allowed_addrs=None,
                    check_status=True, class_to_header=None):
    """Select the next batch of functions to attempt.

    Priority:
    1. Untried ≤8 byte functions (trivial, highest success rate)
    2. Untried leaf functions ≤64 bytes
    3. Untried functions in classes with existing matches (context available)
    4. Untried functions by ascending size

    `excluded_classes` filters out functions whose class is currently
    locked by another in-flight session.

    `allowed_addrs` (Phase 3) restricts to schedule-assigned addrs for
    the calling identity. None preserves pre-Phase-3 behavior.
    `check_status` toggles the `untried` filter — shootout queries
    pass False since their DB stays `untried` while attempts happen
    in isolated worktrees.
    """
    excluded_classes = excluded_classes or set()
    candidates = filter_functions(
        functions,
        status=("untried" if check_status else None),
        class_name=getattr(args, 'class_name', None),
        name=getattr(args, 'func_name', None),
        obj=getattr(args, 'obj', None),
        size_min=getattr(args, 'size_min', None) if getattr(args, 'size_min', None) is not None else 4,
        size_max=getattr(args, 'size_max', None),
    )

    if allowed_addrs is not None:
        candidates = [c for c in candidates if c["address"] in allowed_addrs]

    if not candidates:
        return []

    if excluded_classes:
        candidates = [c for c in candidates
                      if not (_candidate_lock_keys(c, class_to_header)
                              & excluded_classes)]
        if not candidates:
            return []

    matched_classes = {f["class_name"] for f in functions
                       if f["match_status"] == "matched" and f["class_name"]}

    def priority_key(f):
        size = f["size"]
        is_leaf = f.get("is_leaf", False)
        has_class_context = f["class_name"] in matched_classes if f["class_name"] else False

        if size <= 8:
            tier = 0
        elif is_leaf and size <= 64:
            tier = 1
        elif has_class_context:
            tier = 2
        else:
            tier = 3

        return (tier, size)

    candidates.sort(key=priority_key)

    return _group_and_fill_batch(candidates, batch_size)


def disassemble_function(addr, size):
    """Extract disassembly for a function from the original binary.

    Raises RuntimeError if objdump fails.
    """
    result = subprocess.run(
        [OBJDUMP, "-d", "-j", ".text",
         f"--start-address=0x{addr:x}",
         f"--stop-address=0x{addr + size:x}",
         EBOOT_PATH],
        capture_output=True, text=True
    )
    if result.returncode != 0:
        raise RuntimeError(f"objdump failed for 0x{addr:x}: {result.stderr.strip()}")

    # Fix VFPU instructions that objdump mis-decodes
    output = fix_vfpu_disassembly(result.stdout)

    lines = []
    for line in output.split("\n"):
        line = line.strip()
        if ":" in line and "\t" in line and not line.startswith("Disassembly"):
            lines.append(line)
    return "\n".join(lines)


def get_m2c_output(func):
    """Run decompile_func.py to get m2c output. Raises on failure."""
    result = subprocess.run(
        ["python3", "tools/decompile_func.py", func["address"]],
        capture_output=True, text=True
    )
    if result.returncode != 0:
        raise RuntimeError(
            f"m2c failed for {func['name']}: {result.stderr.strip()[:200]}"
        )
    return result.stdout


def ensure_expected_o(func):
    """Ensure the expected .o file exists. Raises on failure."""
    addr = int(func["address"], 16)
    expected_path = f"expected/build/func/{addr:08x}.o"
    if os.path.exists(expected_path):
        return
    result = subprocess.run(
        ["python3", "tools/extract_func.py", func["address"]],
        capture_output=True, text=True
    )
    if result.returncode != 0:
        raise RuntimeError(
            f"Failed to extract expected .o for {func['name']}: {result.stderr.strip()}"
        )
    if not os.path.exists(expected_path):
        raise RuntimeError(
            f"extract_func.py succeeded but {expected_path} not found"
        )


def get_class_header(class_name):
    if not class_name:
        return None
    for name in [class_name, class_name.replace("::", "_")]:
        path = f"include/{name}.h"
        if os.path.exists(path):
            with open(path, "r") as f:
                return f.read()
    return None


def get_matched_neighbors(functions, func):
    if not func["class_name"]:
        return []
    return [f for f in functions
            if f["class_name"] == func["class_name"]
            and f["match_status"] == "matched"
            and f["address"] != func["address"]][:5]


def determine_source_file(batch):
    first = batch[0]
    if first["class_name"]:
        return f"src/{_safe_class_filename(first['class_name'])}.cpp"
    return "src/free_functions.c"


def get_sched_hint(func):
    """Return a sched zone hint string for this function, or None."""
    addr = int(func["address"], 16)
    if SCHED1_ZONE_START <= addr < SCHED1_ZONE_END:
        return (
            "SCHED HINT: This function is in the confirmed sched=1 zone "
            f"(0x{SCHED1_ZONE_START:06x}-0x{SCHED1_ZONE_END:06x}). "
            "The Makefile should already apply -Xsched=1 for known classes. "
            "If bytes don't match and you suspect a missing sched override, "
            "report failure — a human will update the Makefile."
        )
    if TRANSITION_ZONE_START <= addr < TRANSITION_ZONE_END:
        return (
            "SCHED HINT: This function is in the transition zone "
            f"(0x{TRANSITION_ZONE_START:06x}-0x{TRANSITION_ZONE_END:06x}) where "
            "the sched flag may be either 1 or 2. Try sched=2 first (default). "
            "If bytes don't match, this class may need sched=1 — report failure "
            "with a note and a human will update the Makefile."
        )
    return None


def build_prompt(batch, functions, session_id, variant):
    """Dispatch to a named prompt variant under tools/prompt_variants/.
    Returns (prompt_text, warnings_list). `variant` is required.
    """
    module = importlib.import_module(f"prompt_variants.{variant}")
    return module.build_prompt(batch, functions, session_id)


def parse_session_results(session_id, batch, log_path, variant, backend_name):
    """Read the session's results JSON and produce per-function decisions.

    Phase 1 split: parsing + per-result decision building runs in the
    worker thread without touching the DB. Coordinator applies the
    returned `decisions` (and any `unreported_addrs` reverts) under
    `db_lock`. Logs warnings/events for missing notes + unreported
    entries directly (logging is append-only and thread-safe).

    Returns (claimed_matched, claimed_failed, results, decisions,
             unreported_addrs).

    Raises FileNotFoundError / ValueError on system / schema errors.
    """
    results_path = os.path.join(SESSION_RESULTS_DIR, f"{session_id}.json")

    if not os.path.exists(results_path):
        raise FileNotFoundError(
            f"Session {session_id} did not produce results file at {results_path}. "
            f"This is a system error — the agent failed to write the expected output."
        )

    with open(results_path, "r") as f:
        content = f.read()

    try:
        results = json.loads(content)
    except json.JSONDecodeError as e:
        raise ValueError(
            f"Session {session_id} produced invalid JSON at {results_path}: {e}"
        )

    if not isinstance(results, list):
        raise ValueError(
            f"Session {session_id} results file is not a JSON array: {type(results)}"
        )

    batch_addrs = {f["address"] for f in batch}
    matched = 0
    failed = 0
    reported_addrs = set()
    decisions = {}  # addr -> FunctionDecision
    VALID_STATUSES = {"matched", "failed"}

    for i, entry in enumerate(results):
        addr = entry.get("address")
        status = entry.get("status")

        if not addr or not status:
            raise ValueError(
                f"Session {session_id} result entry {i} missing required keys "
                f"(need 'address' and 'status'): {entry}"
            )

        if status not in VALID_STATUSES:
            raise ValueError(
                f"Session {session_id} result entry {i} has invalid status '{status}' "
                f"(expected 'matched' or 'failed') for address {addr}"
            )

        if addr not in batch_addrs:
            continue

        reported_addrs.add(addr)
        if status == "matched":
            matched += 1
            decisions[addr] = FunctionDecision(
                address=addr, status="matched",
                src_file=entry.get("file"),
                claimed_status="matched",
            )
        else:
            failed += 1
            notes = entry.get("notes", "")
            if not notes:
                log(f"  WARNING: {addr} reported failed with no notes (agent ignored REQUIRED field)")
                if log_path:
                    log_event(log_path, {
                        "event": "missing_failure_notes",
                        "session_id": session_id,
                        "variant": variant,
                        "backend": backend_name,
                        "address": addr,
                    })
            decisions[addr] = FunctionDecision(
                address=addr, status="failed",
                failure_note=(notes or None),
                claimed_status="failed",
                verify_reason="agent_self_reported_failure",
            )

    unreported = []
    for func in batch:
        if func["address"] not in reported_addrs:
            log(f"  WARNING: {func['name']} not reported in session results — reverting to untried")
            if log_path:
                log_event(log_path, {
                    "event": "unreported_function",
                    "session_id": session_id,
                    "variant": variant,
                    "backend": backend_name,
                    "address": func["address"],
                    "name": func["name"],
                })
            unreported.append(func["address"])

    return matched, failed, results, decisions, unreported


def run_one_session(ctx):
    """Worker thread entry point. Runs one session end-to-end.

    Owns: prompt building, agent invocation, result parsing,
    verification, source-quality + extern-C gates, dirty-file
    partitioning, out-of-scope reverts. Does NOT mutate the DB or run
    git commits — those are coordinator-side under `db_lock`. Returns a
    `SessionOutcome` describing what the coordinator should apply.
    """
    backend = ctx.backend
    batch = ctx.batch
    variant = ctx.variant
    session_id = ctx.session_id
    log_path = ctx.log_path

    outcome = SessionOutcome(
        session_id=session_id,
        backend_name=backend.name,
        backend_model=backend.model,
        variant=variant,
        batch=batch,
        classes=ctx.classes,
        worktree=ctx.worktree,
        identity=ctx.identity,
        queue_kind=ctx.queue_kind,
    )

    try:
        for func in batch:
            ensure_expected_o(func)
    except RuntimeError as e:
        outcome.prep_error = str(e)
        return outcome

    prompt, prompt_warnings = build_prompt(
        batch, ctx.functions, session_id, variant=variant)
    for w in prompt_warnings:
        log(f"  {w['type'].upper()}: {w['name']} — {w['error']}")
        log_event(log_path, {
            "event": w["type"],
            "session_id": session_id,
            "variant": variant,
            "backend": backend.name,
            "address": w["address"],
            "name": w["name"],
            "error": w["error"],
        })

    session_start = time.time()
    try:
        success, error_msg, session_usage = run_session(
            backend, prompt, session_id,
            log_fn=lambda ev: log_event(log_path, ev),
            variant=variant,
            timeout=ctx.session_timeout,
            cwd=ctx.worktree,
        )
    except AgentRefused as e:
        outcome.session_duration = time.time() - session_start
        outcome.refused = True
        outcome.refusal_reason = e.reason
        outcome.session_usage = e.session_usage
        return outcome
    outcome.session_usage = session_usage

    outcome.session_duration = time.time() - session_start

    if not success:
        outcome.error_msg = error_msg
        return outcome

    try:
        claimed_matched, claimed_failed, results, decisions, unreported = (
            parse_session_results(session_id, batch, log_path, variant, backend.name)
        )
    except (FileNotFoundError, ValueError) as e:
        outcome.system_error = str(e)
        return outcome

    outcome.success = True
    outcome.claimed_matched = claimed_matched
    outcome.claimed_failed = claimed_failed

    for addr in unreported:
        decisions[addr] = FunctionDecision(address=addr, status="untried")

    addr_to_src = {}
    matched_files = set()
    for entry in results:
        if entry.get("status") == "matched" and entry.get("address") and entry.get("file"):
            addr_to_src[entry["address"]] = entry["file"]
            matched_files.add(entry["file"])

    # Verify each claimed match independently. Reuses the canonical
    # byte_match.check_byte_match path. Decisions are mutated in place.
    for func in batch:
        addr = func["address"]
        d = decisions.get(addr)
        if d is None or d.status != "matched":
            continue
        src_file = addr_to_src.get(addr)
        if not src_file:
            log(f"  VERIFY ERROR: {func['name']} — no src_file in session_results")
            log_event(log_path, {
                "event": "verify_error",
                "session_id": session_id,
                "variant": variant,
                "backend": backend.name,
                "address": addr,
                "name": func["name"],
                "error": "no src_file in session_results",
            })
            d.status = "untried"
            d.src_file = None
            d.verify_reason = "no_src_file_in_session_results"
            continue

        try:
            result = check_byte_match(func, src_file, cwd=ctx.worktree)
        except CompileFailed as e:
            log(f"  VERIFY FAILED: {func['name']} — compile error: {e}")
            log_event(log_path, {
                "event": "verify_failed",
                "session_id": session_id,
                "variant": variant,
                "backend": backend.name,
                "address": addr,
                "name": func["name"],
                "size": func["size"],
                "diff_count": 0,
                "byte_diffs": [],
                "reason": "compile_failed",
            })
            d.status = "failed"
            d.src_file = None
            d.verify_reason = "compile_failed"
            continue
        except RuntimeError as e:
            log(f"  VERIFY ERROR: {func['name']} — {e}")
            log_event(log_path, {
                "event": "verify_error",
                "session_id": session_id,
                "variant": variant,
                "backend": backend.name,
                "address": addr,
                "name": func["name"],
                "error": str(e),
            })
            d.status = "untried"
            d.src_file = None
            d.verify_reason = "verify_tooling_error"
            continue

        if result.ok:
            d.src_file = src_file
            d.symbol_name = result.sym_name
            continue

        diff_preview = ""
        if result.byte_diffs:
            first = result.byte_diffs[0]
            diff_preview = (f" (first diff at {first.get('addr','?')}: "
                            f"compiled={first.get('compiled','?')} "
                            f"expected={first.get('expected','?')}, "
                            f"{result.diff_count} total diffs)")
        log(f"  VERIFY FAILED: {func['name']} — {result.reason}{diff_preview}")
        log_event(log_path, {
            "event": "verify_failed",
            "session_id": session_id,
            "variant": variant,
            "backend": backend.name,
            "address": addr,
            "name": func["name"],
            "size": func["size"],
            "diff_count": result.diff_count,
            "byte_diffs": result.byte_diffs,
            "reason": result.reason,
        })
        d.status = "failed"
        d.src_file = None
        d.verify_reason = result.reason

    matched_funcs = [f for f in batch
                     if decisions.get(f["address"]) is not None
                     and decisions[f["address"]].status == "matched"]

    if matched_funcs and matched_files:
        asm_rejected = validate_source_quality(matched_files)
        if asm_rejected:
            log(f"  ASSEMBLY-ONLY REJECTION: {len(asm_rejected)} files are pure asm with no C/C++")
            for asm_file in asm_rejected:
                log(f"    Rejected: {asm_file}")
                log_event(log_path, {
                    "event": "rejected_assembly_only",
                    "session_id": session_id,
                    "variant": variant,
                    "backend": backend.name,
                    "file": asm_file,
                    "reason": "File contains only __asm__() blocks — no C/C++ training data",
                })
            for func in list(matched_funcs):
                func_files = {e["file"] for e in results
                              if e.get("address") == func["address"]
                              and e.get("file")}
                if func_files and func_files.issubset(set(asm_rejected)):
                    d = decisions[func["address"]]
                    d.status = "failed"
                    d.src_file = None
                    d.symbol_name = None
                    d.verify_reason = "rejected_assembly_only"
                    matched_funcs.remove(func)
                    log(f"    Reverted: {func['name']} → failed (assembly-only source)")
            matched_files -= set(asm_rejected)

    if matched_funcs:
        xc_rejected = reject_extern_c_class_methods(matched_funcs, addr_to_src)
        if xc_rejected:
            log(f"  EXTERN-C CLASS-METHOD REJECTION: "
                f"{len(xc_rejected)} matches lack canonical Class::method form")
            for func, reason in xc_rejected:
                log(f"    Rejected: {func['name']} — {reason}")
                log_event(log_path, {
                    "event": "rejected_extern_c_class_method",
                    "session_id": session_id,
                    "variant": variant,
                    "backend": backend.name,
                    "address": func["address"],
                    "name": func["name"],
                    "reason": reason,
                })
                d = decisions[func["address"]]
                d.status = "failed"
                d.src_file = None
                d.symbol_name = None
                d.failure_note = reason
                d.verify_reason = "rejected_extern_c_class_method"
                d.rejected_extern_c = True
                matched_funcs.remove(func)

    # Phase 1: per-session file ledger replaces _session_dirty_paths().
    # Capture dirty files, partition into in-set / out-of-set, revert
    # out-of-set, reject any matches whose declared src is out-of-set.
    try:
        ledger, out_of_scope = partition_dirty_paths(
            ctx.exact_paths, ctx.sibling_prefixes,
            peer_query=ctx.peer_query, worktree=ctx.worktree)
    except RuntimeError as e:
        log(f"  PARTITION ERROR: {e}")
        log_event(log_path, {
            "event": "partition_error",
            "session_id": session_id,
            "variant": variant,
            "backend": backend.name,
            "error": str(e),
        })
        outcome.system_error = str(e)
        return outcome

    if out_of_scope:
        log(f"  OUT-OF-SCOPE EDITS: reverting {len(out_of_scope)} path(s)")
        for p in sorted(out_of_scope):
            log(f"    {p}")
        log_event(log_path, {
            "event": "out_of_scope_edits",
            "session_id": session_id,
            "variant": variant,
            "backend": backend.name,
            "paths": sorted(out_of_scope),
        })
        revert_paths(out_of_scope, worktree=ctx.worktree)
        for func in list(matched_funcs):
            func_files = {e["file"] for e in results
                          if e.get("address") == func["address"] and e.get("file")}
            if func_files and not func_files.isdisjoint(out_of_scope):
                d = decisions[func["address"]]
                d.status = "failed"
                d.src_file = None
                d.symbol_name = None
                d.failure_note = (
                    "match wrote to out-of-scope path; reverted by Phase 1 ledger"
                )
                d.verify_reason = "out_of_scope_path"
                matched_funcs.remove(func)
        matched_files -= out_of_scope

        # Post-revert re-verification: a match that passed
        # check_byte_match before the out-of-scope revert may now
        # reference symbols whose declarations got reverted (cross-
        # class header dependency, e.g. `cFilePlatform::PollAsync`
        # added to include/cFile.h instead of include/cFilePlatform.h
        # — the revert removes the declaration, the .cpp no longer
        # compiles).
        #
        # Stale-cache hazard: the Makefile's `.cpp.o : .cpp` rule
        # has no header dep tracking, so `make build/src/foo.cpp.o`
        # returns "up to date" if the .cpp mtime is unchanged — even
        # when an included header was just reverted. We must remove
        # the .o explicitly to force a fresh compile that sees the
        # post-revert header state.
        for func in list(matched_funcs):
            d = decisions[func["address"]]
            if d.status != "matched" or not d.src_file:
                continue
            o_path = _src_to_o_path(d.src_file, ctx.worktree)
            if o_path and os.path.exists(o_path):
                try:
                    os.remove(o_path)
                except OSError as e:
                    log(f"  POST-REVERT ERROR: cannot remove {o_path}: {e}")
                    raise
            try:
                result = check_byte_match(func, d.src_file, cwd=ctx.worktree)
            except CompileFailed as e:
                log(f"  POST-REVERT VERIFY FAILED: {func['name']} no longer "
                    f"compiles after out-of-scope revert — "
                    f"{str(e)[:120]}")
                log_event(log_path, {
                    "event": "post_revert_verify_failed",
                    "session_id": session_id,
                    "variant": variant,
                    "backend": backend.name,
                    "address": func["address"],
                    "name": func["name"],
                    "reason": "compile_failed_after_out_of_scope_revert",
                    "error": str(e)[:500],
                })
                d.status = "failed"
                d.src_file = None
                d.symbol_name = None
                d.failure_note = (
                    "match relied on out-of-scope header edits; "
                    "post-revert recompile failed"
                )
                d.verify_reason = "post_revert_compile_failed"
                matched_funcs.remove(func)
                continue
            if not result.ok:
                log(f"  POST-REVERT VERIFY FAILED: {func['name']} bytes "
                    f"diverged after revert ({result.reason})")
                log_event(log_path, {
                    "event": "post_revert_verify_failed",
                    "session_id": session_id,
                    "variant": variant,
                    "backend": backend.name,
                    "address": func["address"],
                    "name": func["name"],
                    "reason": result.reason,
                    "diff_count": result.diff_count,
                })
                d.status = "failed"
                d.src_file = None
                d.symbol_name = None
                d.failure_note = (
                    f"post-revert byte diff: {result.reason}"
                )
                d.verify_reason = "post_revert_byte_mismatch"
                matched_funcs.remove(func)

    outcome.decisions = list(decisions.values())
    outcome.matched_funcs = matched_funcs
    outcome.matched_files = matched_files
    outcome.ledger_paths = ledger
    outcome.out_of_scope_paths = out_of_scope
    return outcome


def git_run(*args, cwd=None):
    """Run a git command serialized through `git_lock`. Returns
    (success, stdout, stderr). stdout is unstripped: porcelain lines
    can begin with a meaningful space, so a global strip would corrupt
    the first line.

    `cwd` targets a specific worktree (Phase 3 shootout). When None
    git uses the orchestrator's CWD — the main worktree, today's
    behavior. We invoke `git -C <cwd>` rather than passing cwd= to
    subprocess.run so the lock is around exactly the same kind of
    invocation regardless of target tree (auditability).
    """
    cmd = [GIT]
    if cwd is not None:
        cmd += ["-C", cwd]
    cmd += list(args)
    with git_lock:
        result = subprocess.run(
            cmd, capture_output=True, text=True
        )
    return result.returncode == 0, result.stdout, result.stderr.strip()


def setup_shootout_worktrees(identities, run_branch):
    """Create one worktree + branch per shootout identity.

    Returns `{identity: worktree_abspath}`. Each worktree is created
    from `run_branch` HEAD so all shootout paths share a parent commit
    with the disjoint slice — cherry-picks downstream apply cleanly.

    Symlinks the shared, gitignored `extern/`, `expected/`, and `logs/`
    directories from the orchestrator CWD into each worktree so the
    agent can use the SDK and dump session_results to a single
    physical location across trees. `build/` is intentionally NOT
    symlinked: each worktree gets its own so concurrent compiles
    don't race on .o file writes.

    On partial failure (some worktrees created, then one fails) we
    tear down the already-created ones before re-raising. Otherwise
    a failed run leaves stale worktrees on disk that block the next
    run's setup with "already exists".
    """
    repo_root = os.getcwd()
    worktrees: dict = {}
    shared_dirs = ("extern", "expected", "logs")
    try:
        _setup_shootout_worktrees_inner(identities, run_branch, repo_root,
                                         worktrees, shared_dirs)
    except BaseException:
        if worktrees:
            log(f"setup_shootout_worktrees: failed mid-build, tearing down "
                f"{len(worktrees)} partial worktree(s)...")
            teardown_shootout_worktrees(worktrees)
        raise
    return worktrees


def _setup_shootout_worktrees_inner(identities, run_branch, repo_root,
                                     worktrees, shared_dirs):
    for ident in identities:
        tag = safe_identity_tag(ident)
        wt_path = os.path.abspath(f"overnight-worktree-{tag}")
        # Sibling, not nested: git can't have both `overnight/<ts>`
        # (a ref) and `overnight/<ts>/<tag>` (a sub-namespace), so we
        # flatten the shootout branch as a sibling of the base run-
        # branch with the same prefix. Operators searching `git branch
        # -l 'overnight/20260425*'` still see all related branches.
        branch = f"{run_branch}-{tag}"
        if os.path.exists(wt_path):
            raise RuntimeError(
                f"setup_shootout_worktrees: {wt_path} already exists. "
                f"Stale worktree from a prior run? Remove with "
                f"`git worktree remove {wt_path}` and retry."
            )
        ok, out, err = git_run("worktree", "add", "-b", branch,
                                wt_path, run_branch)
        if not ok:
            raise RuntimeError(
                f"setup_shootout_worktrees: `git worktree add -b {branch} "
                f"{wt_path} {run_branch}` failed: {err or out}"
            )
        for d in shared_dirs:
            target = os.path.join(repo_root, d)
            link = os.path.join(wt_path, d)
            if not os.path.exists(target):
                # logs/ may not exist yet; create empty so the symlink
                # resolves. extern/ missing is operator misconfig — we
                # raise so it surfaces immediately, not at first session.
                if d == "logs":
                    os.makedirs(target, exist_ok=True)
                else:
                    raise RuntimeError(
                        f"setup_shootout_worktrees: shared dir {target} "
                        f"missing — required for shootout worker to run. "
                        f"This is operator setup, not something the "
                        f"orchestrator should auto-create."
                    )
            if os.path.lexists(link):
                if os.path.isdir(link) and not os.path.islink(link):
                    import shutil
                    shutil.rmtree(link)
                else:
                    os.remove(link)
            os.symlink(target, link)
        # Per-worktree build/ — kept isolated so two backends compiling
        # the same Class.cpp don't race on build/src/Class.cpp.o.
        os.makedirs(os.path.join(wt_path, "build"), exist_ok=True)
        worktrees[ident] = wt_path
        log(f"Shootout worktree: {ident} → {wt_path} (branch {branch})")


def teardown_shootout_worktrees(worktrees):
    """Best-effort worktree removal. Logs but doesn't raise on
    individual failures — at shutdown we want to clean up as much as
    possible even if one tree is in a weird state. Operators can
    finish manually with `git worktree remove --force`.
    """
    for ident, path in (worktrees or {}).items():
        if not os.path.exists(path):
            continue
        ok, out, err = git_run("worktree", "remove", "--force", path)
        if not ok:
            log(f"  worktree remove failed for {ident} ({path}): {err or out}")
        else:
            log(f"  removed worktree: {path}")


@dataclass
class WorkerSlot:
    """One executor lane. Mode A's main pool produces `kind='main'`
    slots that draw from any identity's disjoint queue; shootout adds
    `kind='shootout'` slots, one per identity, each pinned to its own
    worktree.
    """
    kind: str  # "main" | "shootout"
    identity: Optional[str] = None  # required for kind="shootout"
    worktree: Optional[str] = None  # required for kind="shootout"


def create_overnight_branch():
    """Create and checkout a branch for this overnight run.

    Verifies we're on main with a clean tree first.
    Returns the branch name.
    """
    # Verify no uncommitted changes to tracked files (untracked files are fine)
    ok, staged, err = git_run("diff", "--cached", "--name-only")
    if not ok:
        raise RuntimeError(f"git diff --cached failed: {err}")
    ok, unstaged, err = git_run("diff", "--name-only")
    if not ok:
        raise RuntimeError(f"git diff failed: {err}")
    dirty = (staged + "\n" + unstaged).strip()
    if dirty:
        raise RuntimeError(
            f"Working tree has uncommitted changes — commit or stash before running:\n{dirty[:200]}"
        )

    # Verify we're on main
    ok, current, _ = git_run("branch", "--show-current")
    current = current.strip()
    if ok and current != "main":
        raise RuntimeError(
            f"Expected to be on 'main' branch, but on '{current}'. "
            f"Checkout main first: git checkout main"
        )

    branch = f"overnight/{datetime.now().strftime('%Y%m%d-%H%M%S')}"
    ok, _, err = git_run("checkout", "-b", branch)
    if not ok:
        raise RuntimeError(f"Failed to create branch {branch}: {err}")

    log(f"Created branch: {branch}")
    return branch


def _collect_compile_failures(src_paths, cwd=None):
    """Compile each src path via byte_match.compile_src. Returns list of
    (path, error_message) tuples for failures. Skips non-.cpp/.c paths
    and deleted files (no-op for those, not failure).

    `cwd` (Phase 3 shootout): run make inside a worktree. Paths must
    be relative to `cwd` when set (e.g. `src/foo.cpp`, not absolute).
    """
    from byte_match import compile_src, CompileFailed
    failures = []
    for p in src_paths:
        if not p.endswith((".cpp", ".c")):
            continue
        full = os.path.join(cwd, p) if cwd else p
        if not os.path.exists(full):
            continue  # deletion in the change set, not a failure
        try:
            compile_src(p, cwd=cwd)
        except CompileFailed as e:
            failures.append((p, str(e)))
    return failures


def verify_tree_compiles():
    """Compile every src/*.cpp and src/*.c. Raise on any failure.

    Pre-flight gate at run start: we refuse to start sessions from a
    broken tree because any session that touches a class with a broken
    src file either inherits the brokenness or wastes Claude tokens
    debugging it.
    """
    sources = sorted(glob.glob("src/*.cpp") + glob.glob("src/*.c"))
    failures = _collect_compile_failures(sources)
    if failures:
        msg_lines = [
            f"Pre-session build check failed — {len(failures)} src files "
            f"don't compile. Refusing to start sessions from a broken tree.",
            "",
        ]
        for p, err in failures[:10]:
            msg_lines.append(f"  {p}:")
            msg_lines.append(f"    {err[:200]}")
        if len(failures) > 10:
            msg_lines.append(f"  ... and {len(failures) - 10} more")
        raise RuntimeError("\n".join(msg_lines))


def git_commit_batch(session_id, matched_funcs, matched_files, ledger_paths,
                     worktree=None, db_updates=None,
                     backend_name="", backend_model="", matched_at="",
                     variant=""):
    """Commit matched source files and updated functions.json after a batch.

    Phase 1: stages exactly the session's pre-computed `ledger_paths`
    (in-set dirty paths) plus the DB. The old global `_session_dirty_paths`
    sweep is gone — under multi-agent it could absorb other sessions'
    in-flight files into this commit (bug #4 in docs/bugs.md). Compiles
    before staging; refuses to commit on any compile failure. Raises on
    git failure — matched work must be persisted.

    Phase 3 worktree path: when `worktree` is set the DB at the
    *worktree's* config/functions.json is read, mutated by `db_updates`
    (a list of FunctionDecision), and written back before staging.
    The main DB is untouched — shootout decisions are isolated to the
    shootout branches by design and only land in main via
    `tools/ab_promote.py`.
    """
    if not matched_funcs:
        return

    db_path = (os.path.join(worktree, DB_PATH) if worktree else DB_PATH)

    if worktree is not None and db_updates:
        # Mode B/C path: rewrite the worktree's DB so the commit's
        # diff carries the matched_by_* fields the cherry-pick will
        # later re-apply onto main. Same mutator as apply_outcome's
        # in-memory path so the two trees can't drift on schema.
        with open(db_path, "r") as f:
            wt_funcs = json.load(f)
        apply_decisions_to_funcs(
            build_addr_map(wt_funcs), db_updates, session_id,
            backend_name, backend_model, matched_at, variant,
        )
        tmp = db_path + ".tmp"
        with open(tmp, "w") as f:
            json.dump(wt_funcs, f, indent=2)
        os.replace(tmp, db_path)

    files_to_commit = set(matched_files)
    files_to_commit |= set(ledger_paths)
    files_to_commit.add(DB_PATH)

    # Compile-check BEFORE staging. If any src in the set fails, we
    # raise and the operator's tree is left exactly as the session left
    # it — no half-staged half-unstaged mess to untangle.
    compile_check_paths = {p for p in files_to_commit if p != DB_PATH}
    compile_failures = _collect_compile_failures(compile_check_paths,
                                                  cwd=worktree)
    if compile_failures:
        lines = [f"{p}: {e[:120]}" for p, e in compile_failures[:5]]
        raise RuntimeError(
            f"Commit-time build failed for {len(compile_failures)} files. "
            f"Session produced src that doesn't compile cleanly; refusing "
            f"to commit. Tree left dirty for operator inspection. "
            f"Failing files: {'; '.join(lines)}"
        )

    # Stage (deletions included via git add on missing files).
    for path in files_to_commit:
        full = os.path.join(worktree, path) if worktree else path
        if path == DB_PATH:
            pass
        elif path in matched_files and not os.path.exists(full):
            raise RuntimeError(
                f"Matched file not found on disk: {full}. "
                f"Claude reported a match but didn't write the source file."
            )
        ok, _, err = git_run("add", "--", path, cwd=worktree)
        if not ok:
            raise RuntimeError(f"git add failed for {path}: {err}")

    func_names = [f["name"].split("(")[0] for f in matched_funcs]
    msg = f"Match {len(matched_funcs)} functions (session {session_id})\n\n"
    msg += "\n".join(f"  - {name}" for name in func_names)

    # Check if there's actually anything staged before committing
    ok, staged, err = git_run("diff", "--cached", "--name-only", cwd=worktree)
    if not ok:
        raise RuntimeError(f"git diff --cached failed: {err}")
    if not staged.strip():
        log(f"  No changes to commit (source files unchanged from prior batch)")
        return

    ok, out, err = git_run("commit", "-m", msg, cwd=worktree)
    if not ok:
        raise RuntimeError(
            f"git commit failed after matching {len(matched_funcs)} functions: "
            f"{out} {err}"
        )




def reject_extern_c_class_methods(matched_funcs, addr_to_src):
    """Flag class-method matches whose reconstruction is extern-C / safe-name.

    Returns list of (func, reason) for entries where DB has a class_name
    set but the reconstructed src file contains no canonical
    `Class::method(...)` definition. These cases were the predominant
    structural-fidelity bug (see docs/fix_plan.md, Phase 4) — bytes match
    but the source teaches the model wrong abstractions.

    The check is permissive: the canonical form just has to *exist* in
    the file. A file with both canonical and safe-name forms passes.
    """
    import re
    rejected = []
    for func in matched_funcs:
        cls = func.get("class_name")
        if not cls:
            continue
        src_path = addr_to_src.get(func["address"])
        # Upstream verify pass already enforces that every survivor in
        # matched_funcs has a real src file. Assert to fail loudly if
        # that invariant is ever broken — silent skip would let bad
        # entries bypass the gate.
        assert src_path and os.path.exists(src_path), (
            f"reject_extern_c_class_methods: invariant violation — "
            f"matched_funcs entry {func['address']} has no readable "
            f"src_file (addr_to_src={src_path!r})"
        )

        method = (func.get("method_name") or "").split("(", 1)[0]
        if not method:
            raise ValueError(
                f"DB entry {func['address']} has class_name={cls!r} but "
                f"no method_name. Quality gate cannot evaluate. Fix the "
                f"DB before retrying."
            )

        with open(src_path) as f:
            stripped = strip_cpp_comments(f.read())

        canonical = canonical_method_pattern(cls, method) + r"\s*\("
        if re.search(canonical, stripped):
            continue
        rejected.append((func,
                         f"class method but no canonical {cls}::{method}(...) "
                         f"definition found in {src_path}; reconstruction "
                         f"looks extern-C / safe-name (anti-pattern)"))
    return rejected


def validate_source_quality(matched_files):
    """Reject matches that are pure assembly — no C/C++ training data value.

    A source file is rejected if it contains ONLY file-scope __asm__() blocks
    and no real C/C++ function bodies. Small inline __asm__ volatile() inside
    C/C++ functions is fine — the C/C++ surrounding code has training value.

    Returns a list of rejected file paths with reasons.
    """
    import re
    rejected = []
    for src_file in matched_files:
        if not os.path.exists(src_file):
            raise RuntimeError(
                f"validate_source_quality: {src_file} does not exist"
            )
        with open(src_file) as f:
            content = f.read()

        # Strip comments and preprocessor
        stripped = re.sub(r'//.*$', '', content, flags=re.MULTILINE)
        stripped = re.sub(r'/\*.*?\*/', '', stripped, flags=re.DOTALL)
        stripped = re.sub(r'^\s*#.*$', '', stripped, flags=re.MULTILINE)

        # Check for file-scope __asm__( blocks
        has_filescope_asm = bool(re.search(r'^__asm__\s*\(', stripped, re.MULTILINE))
        if not has_filescope_asm:
            continue  # No file-scope asm — fine

        # Count C/C++ statements outside of __asm__() blocks.
        # Skip: struct/class/typedef/extern declarations, blank lines, braces.
        # Count: assignments (=), function calls, control flow keywords,
        # return statements — anything that represents real C/C++ logic.
        lines = stripped.split('\n')
        cpp_statement_count = 0
        in_asm_block = False
        paren_depth = 0
        for line in lines:
            line = line.strip()
            if not line:
                continue
            if re.match(r'^__asm__\s*\(', line):
                in_asm_block = True
                paren_depth = line.count('(') - line.count(')')
                continue
            if in_asm_block:
                paren_depth += line.count('(') - line.count(')')
                if paren_depth <= 0:
                    in_asm_block = False
                continue
            # Skip declarations that aren't executable statements
            if re.match(r'^(struct|class|typedef|extern|enum|union|using)\b', line):
                continue
            if line in ('{', '}', '};', '};'):
                continue
            # Count lines that look like executable C/C++ statements
            if re.match(r'(if|for|while|return|switch|case|do|break|continue|goto)\b', line):
                cpp_statement_count += 1
            elif re.search(r'[=;]', line) and not line.startswith('//'):
                # Assignment, function call with semicolon, variable declaration
                cpp_statement_count += 1

        if cpp_statement_count < 3:
            rejected.append(src_file)

    return rejected


def print_progress(functions, start_time, log_path=None):
    total = sum(1 for f in functions if f["size"] > 0)
    matched = sum(1 for f in functions if f["match_status"] == "matched")
    failed = sum(1 for f in functions if f["match_status"] == "failed")
    untried = sum(1 for f in functions if f["match_status"] == "untried")
    elapsed = datetime.now() - start_time
    elapsed_str = str(elapsed).split(".")[0]

    log(f"Progress: {matched}/{total} matched ({matched*100/total:.1f}%), "
        f"{failed} failed, {untried} untried | elapsed: {elapsed_str}")

    if log_path:
        log_event(log_path, {
            "event": "progress_tick",
            "matched_total": matched,
            "failed_total": failed,
            "untried_total": untried,
            "total": total,
            "elapsed_s": elapsed.total_seconds(),
        })


def main():
    parser = argparse.ArgumentParser(description="Overnight autonomous matching orchestrator")
    parser.add_argument("--hours", type=float, default=8, help="Time limit in hours (default: 8)")
    parser.add_argument("--class", dest="class_name", help="Target specific class")
    parser.add_argument("--name", dest="func_name", help="Target functions matching name")
    parser.add_argument("--obj", help="Target specific .obj file")
    parser.add_argument("--size-min", type=int, help="Minimum function size")
    parser.add_argument("--size-max", type=int, help="Maximum function size")
    parser.add_argument("--limit", type=int, help="Max total functions to attempt")
    parser.add_argument("--batch-size", type=int, default=None,
                        help="Functions per session (default: 2 for --targets, 5 otherwise)")
    parser.add_argument("--targets", type=str,
                        help="Path to targets JSON file (e.g., config/finetune_targets.json). "
                             "Only functions listed in this file will be attempted. "
                             "Uses longer session timeout and smaller batches.")
    parser.add_argument("--dry-run", action="store_true", help="Skip sandbox, for testing")
    parser.add_argument("--variants", type=str, default="base",
                        help="Comma-separated prompt variants to A/B test "
                             "(default: base). Each session picks one at random. "
                             "Variants live in tools/prompt_variants/<name>.py.")
    parser.add_argument("--backend", type=str, default="claude",
                        help="Coding-agent CLI(s) to drive sessions. "
                             "Comma-separated for A/B (e.g. claude,codex); "
                             "each session picks one at random. "
                             f"Available: {', '.join(AVAILABLE_BACKENDS)}.")
    parser.add_argument("--workers", type=int, default=None,
                        help="Number of concurrent worker sessions "
                             "(default: max(1, len(--backend list))). Phase 1 "
                             "of docs/direction/003-multi-agent-ab-architecture.md.")
    parser.add_argument("--shootout", action="store_true",
                        help="Mode B: every backend attempts every function "
                             "in its own worktree, no shared commits. "
                             "Mutually exclusive with --paired-reserve.")
    parser.add_argument("--paired-reserve", type=int, default=0,
                        help="Mode C: reserve N functions (stratified) for "
                             "shootout while the rest runs disjoint. "
                             "Mutually exclusive with --shootout.")
    parser.add_argument("--schedule-seed", type=int, default=42,
                        help="RNG seed for stratified assignment. Same seed "
                             "+ same input pool reproduces the schedule "
                             "exactly — useful for debugging fairness.")

    args = parser.parse_args()

    args.variants = [v.strip() for v in args.variants.split(",") if v.strip()]
    if not args.variants:
        log("ERROR: --variants must specify at least one name")
        sys.exit(1)
    available = sorted(
        os.path.basename(p)[:-3]
        for p in glob.glob(os.path.join("tools", "prompt_variants", "*.py"))
        if os.path.basename(p) not in ("__init__.py", "_common.py")
    )
    for v in args.variants:
        if v not in available:
            log(f"ERROR: variant '{v}' not found. Available: {', '.join(available)}")
            sys.exit(1)
        importlib.import_module(f"prompt_variants.{v}")
    if len(args.variants) > 1:
        log(f"A/B mode: {len(args.variants)} variants ({', '.join(args.variants)}) — "
            f"each session picks one randomly")

    args.backends = list(dict.fromkeys(
        b.strip().lower() for b in args.backend.split(",") if b.strip()
    ))
    if not args.backends:
        log("ERROR: --backend must specify at least one backend")
        sys.exit(1)
    for b in args.backends:
        if b not in AVAILABLE_BACKENDS:
            log(f"ERROR: backend '{b}' not found. Available: {', '.join(AVAILABLE_BACKENDS)}")
            sys.exit(1)
    if len(args.backends) > 1:
        log(f"Backend A/B: {len(args.backends)} backends ({', '.join(args.backends)}) — "
            f"each session picks one randomly")

    if args.workers is None:
        args.workers = max(1, len(args.backends))
    if args.workers < 1:
        log(f"ERROR: --workers must be >= 1 (got {args.workers})")
        sys.exit(1)

    # Mode resolution. Mode A is the default, Mode B is --shootout
    # alone, Mode C is --paired-reserve N (with N > 0). Combining
    # both is a contradiction (paired-reserve already mixes shootout
    # with disjoint).
    if args.shootout and args.paired_reserve > 0:
        log("ERROR: --shootout and --paired-reserve are mutually exclusive")
        sys.exit(1)
    if args.shootout:
        ab_mode = MODE_SHOOTOUT
    elif args.paired_reserve > 0:
        ab_mode = MODE_HYBRID
    else:
        ab_mode = MODE_DISJOINT

    # Load targets file if specified
    targets_list = None
    if args.targets:
        targets_list = load_targets_file(args.targets)
        log(f"Loaded {len(targets_list)} targets from {args.targets}")

    # Set defaults based on mode
    if args.batch_size is None:
        args.batch_size = TARGETS_BATCH_SIZE if targets_list else BATCH_SIZE
    session_timeout = TARGETS_SESSION_TIMEOUT if targets_list else SESSION_TIMEOUT

    backends = {name: get_backend(name, system_append=SYSTEM_PROMPT_APPEND)
                for name in args.backends}

    os.makedirs(LOGS_DIR, exist_ok=True)
    os.makedirs(SESSION_RESULTS_DIR, exist_ok=True)

    log_path = os.path.join(LOGS_DIR, f"match_{datetime.now().strftime('%Y%m%d_%H%M%S')}.jsonl")
    # Symlink logs/match_latest.jsonl so the TUI can tail without knowing the
    # filename. Atomic replace: swap it even if an old run's symlink is stale.
    latest_link = os.path.join(LOGS_DIR, "match_latest.jsonl")
    if os.path.islink(latest_link) or os.path.exists(latest_link):
        os.remove(latest_link)
    os.symlink(os.path.basename(log_path), latest_link)

    set_log_path(log_path)
    log(f"Log: {log_path}")

    # run_id groups attempts.jsonl rows back to one orchestrator
    # invocation — distinct from session_id and the run-branch name.
    run_id = uuid.uuid4().hex[:12]

    functions = load_db()
    addr_index = build_addr_map(functions)
    start_time = datetime.now()
    deadline = start_time + timedelta(hours=args.hours)
    total_attempted = 0
    total_matched = 0

    mode = "targeted" if targets_list else "general"
    backend_summary = ", ".join(
        f"{n}/{backends[n].model}" for n in args.backends
    )
    primary = args.backends[0]
    identities = [identity_key(n, backends[n].model) for n in args.backends]

    log_event(log_path, {
        "event": "run_start",
        "run_id": run_id,
        "hours": args.hours,
        "deadline": deadline.isoformat(),
        "start_time": start_time.isoformat(),
        "variants": args.variants,
        "backend": primary,
        "model": backends[primary].model,
        "backends": [{"name": n, "model": backends[n].model} for n in args.backends],
        "identities": identities,
        "workers": args.workers,
        "batch_size": args.batch_size,
        "session_timeout": session_timeout,
        "mode": mode,
        "ab_mode": ab_mode,
        "paired_reserve_n": args.paired_reserve,
        "schedule_seed": args.schedule_seed,
        "targets_count": len(targets_list) if targets_list else 0,
        "dry_run": args.dry_run,
    })
    total_failed = 0
    total_errors = 0
    consecutive_failures = 0
    consecutive_refusals = 0

    # Create a branch for this run so main stays clean (skip for dry runs)
    if args.dry_run:
        branch = "(dry-run, no branch)"
        log("DRY RUN — no branch created, no Claude sessions, no commits")
    else:
        branch = create_overnight_branch()

    mode_label = f"targeted ({len(targets_list)} targets)" if targets_list else "general pool"
    log(f"Starting overnight run: {args.hours}h time limit, deadline {deadline.strftime('%H:%M')}")
    log(f"Backend: {backend_summary}")
    log(f"Workers: {args.workers}")
    log(f"Mode: {mode_label}, batch_size={args.batch_size}, session_timeout={session_timeout}s")

    # Pre-flight green-build gate — don't sink 8 hours of Claude tokens
    # into sessions whose target classes inherit broken source. See
    # enhancements #8.
    if not args.dry_run:
        log("Pre-flight: verifying the tree compiles cleanly...")
        try:
            verify_tree_compiles()
            log("Pre-flight: OK — all src files compile.")
        except RuntimeError as e:
            log(f"Pre-flight FAILED:\n{e}")
            log_event(log_path, {
                "event": "run_preflight_failed",
                "error": str(e)[:2000],
            })
            sys.exit(1)

    print_progress(functions, start_time, log_path)

    # Build the class → declaring-header map once at run start.
    # compute_allowed_paths uses this to add the actual header
    # where a class's body lives (so an agent matching
    # cFilePlatform::PollAsync can edit include/cFile.h legitimately
    # rather than hitting an out-of-scope revert). Duplicates
    # (mostly templates redeclared in many headers — cHandleT,
    # cMemPool, cTimeValue) are recorded in the log event but not
    # surfaced as a console warning since they're expected noise.
    class_header_map, _dup_classes = scan_class_headers("include")
    log(f"Class header map: {len(class_header_map)} classes mapped to "
        f"declaring header"
        + (f" ({len(_dup_classes)} duplicates noted)" if _dup_classes else ""))
    log_event(log_path, {
        "event": "class_header_map_built",
        "class_count": len(class_header_map),
        "duplicate_count": len(_dup_classes),
        "duplicates": [
            {"class": c, "kept": k, "ignored": i}
            for c, k, i in _dup_classes[:50]
        ],
    })

    # Phase 3 schedule build. The candidate pool we hand to the
    # schedule is the args-filtered untried slice for general-mode
    # runs, or the untried subset of the targets list for targeted
    # runs. Stratified assignment is computed once at run start —
    # not re-balanced as the run progresses (per direction-003 §risks
    # "rate-limit imbalance: weight metrics by attempts attempted").
    if targets_list:
        target_addrs = {t["address"] for t in targets_list}
        candidate_pool = [f for f in functions
                          if f["address"] in target_addrs
                          and f["match_status"] == "untried"]
    else:
        candidate_pool = filter_functions(
            functions,
            status="untried",
            class_name=getattr(args, "class_name", None),
            name=getattr(args, "func_name", None),
            obj=getattr(args, "obj", None),
            size_min=(getattr(args, "size_min", None)
                      if getattr(args, "size_min", None) is not None else 4),
            size_max=getattr(args, "size_max", None),
        )
    matched_classes_set = {f["class_name"] for f in functions
                            if f["match_status"] == "matched"
                            and f.get("class_name")}
    schedule: Schedule = build_schedule(
        candidate_pool, identities, ab_mode,
        paired_reserve_n=args.paired_reserve,
        seed=args.schedule_seed,
        matched_classes=matched_classes_set,
    )
    log(f"Schedule: mode={ab_mode}, "
        f"disjoint={schedule.to_summary_dict()['disjoint_counts']}, "
        f"shootout={schedule.to_summary_dict()['shootout_count']}")
    log_event(log_path, {
        "event": "schedule_built",
        **schedule.to_summary_dict(),
    })

    # Shootout worktrees: created once at run start, torn down in
    # finally. Mode A skips this entirely. Dry-run skips because no
    # branch exists to fork from.
    shootout_worktrees: dict = {}
    if ab_mode in (MODE_SHOOTOUT, MODE_HYBRID) and not args.dry_run:
        shootout_worktrees = setup_shootout_worktrees(identities, branch)

    # Build the slot list. Main slots share the orchestrator CWD (the
    # main run-branch worktree); shootout slots are 1-per-identity
    # and pinned to that identity's worktree.
    slots: list = []
    if ab_mode != MODE_SHOOTOUT:
        for _ in range(args.workers):
            slots.append(WorkerSlot(kind="main"))
    if ab_mode in (MODE_SHOOTOUT, MODE_HYBRID):
        for ident in identities:
            slots.append(WorkerSlot(
                kind="shootout",
                identity=ident,
                worktree=shootout_worktrees.get(ident),
            ))

    if not slots:
        log("ERROR: no worker slots configured (mode/workers/identities mismatch)")
        sys.exit(1)

    log(f"Slots: {len(slots)} total — "
        f"{sum(1 for s in slots if s.kind == 'main')} main, "
        f"{sum(1 for s in slots if s.kind == 'shootout')} shootout")

    held_classes: set = set()
    # `in_flight` maps future → (ctx, slot_idx). Phase 1 just tracked
    # ctx; Phase 3 also needs the slot so completion releases the
    # right slot for re-pick. Mode A's lookup-by-ctx still works
    # because ctx is the second tuple element.
    in_flight: dict = {}
    busy_slots: set = set()
    # `pending_commits[session_id] = {worktree, exact_paths,
    # sibling_prefixes}` for sessions whose commits are queued but
    # not yet executed. peer_query consults this so the next session
    # in the same worktree doesn't classify the to-be-committed
    # ledger as out-of-scope and revert it before the commit thread
    # runs. (A previously-latent race in Phase 1/2: same-tree
    # back-to-back batches across different classes could lose a
    # match if the partition raced ahead of the commit.)
    pending_commits: dict = {}
    pending_lock = threading.Lock()
    next_pick_time = 0.0
    done_picking = False
    # Round-robin starting identity for main-slot picking. Bumped
    # each pick-attempt so two main slots don't both ask claude
    # first (which would empty claude's queue while codex's stays
    # full).
    main_pick_rotation = [0]

    counters = {
        "total_attempted": 0,
        "total_matched": 0,
        "total_failed": 0,
        "total_errors": 0,
        "consecutive_failures": 0,
        "consecutive_refusals": 0,
    }

    def _identity_to_backend(ident: str):
        bname = ident.split("/", 1)[0]
        return backends[bname]

    def _pick_main_slot_batch(slot_idx):
        """Mode A / Mode C main-slot pick. Rotates through identities,
        returns (batch, identity) for the first non-empty disjoint
        queue, or (None, None) when none has eligible work.
        """
        n = len(identities)
        for offset in range(n):
            ident_idx = (main_pick_rotation[0] + offset) % n
            ident = identities[ident_idx]
            allowed = schedule.disjoint_addrs_for(ident)
            if not allowed:
                continue
            if targets_list:
                batch = pick_next_batch_targeted(
                    functions, targets_list, addr_index,
                    batch_size=args.batch_size,
                    excluded_classes=held_classes,
                    allowed_addrs=allowed,
                    class_to_header=class_header_map,
                )
            else:
                batch = pick_next_batch(
                    functions, args, batch_size=args.batch_size,
                    excluded_classes=held_classes,
                    allowed_addrs=allowed,
                    class_to_header=class_header_map,
                )
            if batch:
                main_pick_rotation[0] = (ident_idx + 1) % n
                return batch, ident
        return None, None

    def _pick_shootout_slot_batch(slot):
        """Mode B / Mode C shootout-slot pick. Slot is identity-bound;
        candidates are shootout funcs the identity hasn't yet attempted.
        match_status check is skipped (the shootout DB lives in the
        worktree, not main).
        """
        ident = slot.identity
        allowed = schedule.shootout_remaining(ident)
        if not allowed:
            return None
        if targets_list:
            batch = pick_next_batch_targeted(
                functions, targets_list, addr_index,
                batch_size=args.batch_size,
                excluded_classes=set(),  # shootout slots have no shared class lock
                allowed_addrs=allowed,
                check_status=False,
                class_to_header=class_header_map,
            )
        else:
            batch = pick_next_batch(
                functions, args, batch_size=args.batch_size,
                excluded_classes=set(),
                allowed_addrs=allowed,
                check_status=False,
                class_to_header=class_header_map,
            )
        return batch

    def try_pick_and_submit(executor, slot_idx):
        """Pick one batch for `slot_idx` under db_lock, acquire its
        class locks (main slots only), submit it.

        Returns True if a worker was submitted. False if the slot's
        queue has no eligible work right now (caller decides whether
        that means "permanently done" or "wait for in-flight to
        finish a class").
        """
        slot = slots[slot_idx]
        with db_lock:
            if args.limit and counters["total_attempted"] >= args.limit:
                return False
            if slot.kind == "main":
                batch, picked_ident = _pick_main_slot_batch(slot_idx)
                if not batch:
                    return False
                classes = compute_batch_classes(batch, class_header_map)
                assert not (classes & held_classes)
                held_classes.update(classes)
                set_batch_status(functions, batch, "in_progress", addr_index)
                save_db(functions)
                identity = picked_ident
                queue_kind = "disjoint"
            else:  # shootout
                batch = _pick_shootout_slot_batch(slot)
                if not batch:
                    return False
                classes = compute_batch_classes(batch, class_header_map)
                # Shootout: per-identity attempt set is the lock; main
                # DB stays untouched so no in-progress flip + no
                # save_db.
                schedule.mark_shootout_attempted(
                    slot.identity, [f["address"] for f in batch])
                identity = slot.identity
                queue_kind = "shootout"
            exact_paths, sibling_prefixes = compute_allowed_paths(
                batch, class_to_header=class_header_map)
            counters["total_attempted"] += len(batch)
            busy_slots.add(slot_idx)

        backend = _identity_to_backend(identity)
        session_id = str(uuid.uuid4())[:8]
        selected_variant = random.choice(args.variants)
        kind_tag = "" if slot.kind == "main" else "·shootout"
        tag = f"[{selected_variant}/{backend.name}{kind_tag}]"
        log(f"Session {session_id} {tag}: {len(batch)} functions — "
            f"{', '.join(f['name'].split('(')[0] for f in batch)}")

        log_event(log_path, {
            "event": "session_start",
            "session_id": session_id,
            "variant": selected_variant,
            "backend": backend.name,
            "model": backend.model,
            "identity": identity,
            "queue_kind": queue_kind,
            "worktree": slot.worktree,
            "class_name": batch[0].get("class_name"),
            "functions": [
                {"address": f["address"], "name": f["name"],
                 "size": f["size"], "obj_file": f.get("obj_file"),
                 "class_name": f.get("class_name") or ""}
                for f in batch
            ],
        })

        ctx = WorkContext(
            batch=batch,
            backend=backend,
            variant=selected_variant,
            classes=classes,
            session_id=session_id,
            log_path=log_path,
            session_timeout=session_timeout,
            functions=functions,
            exact_paths=exact_paths,
            sibling_prefixes=sibling_prefixes,
            worktree=slot.worktree,
            identity=identity,
            queue_kind=queue_kind,
        )

        def peer_query(my_ctx=ctx):
            peer_exact = set()
            peer_prefixes = []
            with db_lock:
                for other_pair in in_flight.values():
                    other_ctx, _ = other_pair
                    if other_ctx is my_ctx:
                        continue
                    # Peers in different worktrees don't collide on the
                    # filesystem; only same-worktree peers can step on
                    # each other's still-in-flight ledger.
                    if other_ctx.worktree != my_ctx.worktree:
                        continue
                    peer_exact |= other_ctx.exact_paths
                    peer_prefixes.extend(other_ctx.sibling_prefixes)
            with pending_lock:
                # Queued-commit sessions are out of `in_flight` but
                # their ledger files still sit dirty in the worktree;
                # treat them as peers so we don't revert someone
                # else's pending ledger.
                for entry in pending_commits.values():
                    if entry.get("worktree") != my_ctx.worktree:
                        continue
                    peer_exact |= entry.get("exact_paths") or set()
                    peer_prefixes.extend(entry.get("sibling_prefixes") or ())
            return peer_exact, tuple(peer_prefixes)

        ctx.peer_query = peer_query
        future = executor.submit(run_one_session, ctx)
        in_flight[future] = (ctx, slot_idx)
        return True

    # Dedicated commit thread drains commit_queue FIFO. Workers never
    # hold db_lock while waiting on git, so the next batch can be
    # picked + verified concurrently with the previous batch's commit.
    # The thread MUST keep draining on any exception — its death
    # would silently block shutdown forever via commit_thread.join().
    #
    # Phase 3 fan-in: the queue still serializes through one thread,
    # but each payload carries a worktree path so commits land in the
    # right tree. Single-thread fan-in (vs one thread per worktree)
    # is the chosen strategy because git's index.lock is per-tree,
    # ordering is deterministic, and the work each commit does is
    # fast (a few `git add` + one `git commit`) — parallel commit
    # threads would buy little.
    commit_queue: queue.Queue = queue.Queue()

    def commit_worker():
        while True:
            item = commit_queue.get()
            try:
                if item is None:
                    return
                commit_failed = False
                try:
                    git_commit_batch(
                        item["session_id"],
                        item["matched_funcs"],
                        item["matched_files"],
                        item["ledger_paths"],
                        worktree=item.get("worktree"),
                        db_updates=item.get("db_updates"),
                        backend_name=item.get("backend_name", ""),
                        backend_model=item.get("backend_model", ""),
                        matched_at=item.get("matched_at", ""),
                        variant=item.get("variant", ""),
                    )
                except BaseException as e:
                    commit_failed = True
                    import traceback
                    log(f"  COMMIT THREAD ERROR (matching dropped, "
                        f"reverting worktree): {type(e).__name__}: {e}")
                    log_event(log_path, {
                        "event": "commit_thread_error",
                        "session_id": item.get("session_id"),
                        "worktree": item.get("worktree"),
                        "error_type": type(e).__name__,
                        "error": str(e),
                        "traceback": traceback.format_exc(),
                    })
                    # Revert this session's ledger paths so the next
                    # session in the same worktree doesn't inherit a
                    # half-committed state. Without this, peer_query
                    # would clear from pending_commits below and the
                    # next partition would either revert the matched
                    # files (silent loss) or absorb them into an
                    # unrelated session's ledger (cross-session leak).
                    try:
                        revert_paths(item.get("ledger_paths") or set(),
                                     worktree=item.get("worktree"))
                    except BaseException as re:
                        log(f"  COMMIT-FAILURE CLEANUP ERROR: "
                            f"{type(re).__name__}: {re}")
                        log_event(log_path, {
                            "event": "commit_cleanup_failed",
                            "session_id": item.get("session_id"),
                            "worktree": item.get("worktree"),
                            "error": str(re),
                        })
                with pending_lock:
                    pending_commits.pop(item.get("session_id"), None)
            finally:
                commit_queue.task_done()

    commit_thread = threading.Thread(
        target=commit_worker, name="orch-commit", daemon=False)
    commit_thread.start()

    def _abort(outcome, kind, log_msg, event):
        """Shared error-path: log, emit event, revert in_progress,
        record per-function attempts.jsonl rows, return kind.

        `kind` is also the attempts.jsonl `session_error_kind` value:
        the OUTCOME_* constants are the canonical strings.

        Shootout abort path (outcome.queue_kind == "shootout"): the
        main DB was never flipped, so revert_in_progress is a no-op
        on the actual rows. We still drop the schedule's
        `shootout_done` membership so the function returns to the
        per-identity queue for retry.
        """
        log(log_msg)
        log_event(log_path, event)
        with db_lock:
            if outcome.queue_kind == "disjoint":
                revert_in_progress(functions, outcome.batch, addr_index)
                save_db(functions)
            else:
                schedule.unmark_shootout_attempted(
                    outcome.identity,
                    [f["address"] for f in outcome.batch])
            counters["total_attempted"] -= len(outcome.batch)
        emit_attempts_for_outcome(outcome, run_id, error_kind=kind)
        return kind

    def apply_outcome(outcome, ctx):
        """Apply a worker's outcome to coordinator state. Returns one of
        the OUTCOME_* string constants below (used by the caller to
        update circuit-breaker counters + backoff).
        """
        session_id = outcome.session_id
        backend_name = outcome.backend_name
        variant = outcome.variant
        batch = outcome.batch
        functions_addrs = [f["address"] for f in batch]

        if outcome.prep_error:
            return _abort(outcome, OUTCOME_PREP_ERROR,
                f"Session {session_id} PREP ERROR: {outcome.prep_error}",
                {"event": "prep_error",
                 "session_id": session_id, "variant": variant,
                 "backend": backend_name, "error": outcome.prep_error})

        if outcome.refused:
            return _abort(outcome, OUTCOME_REFUSAL,
                f"Session {session_id} REFUSED ({outcome.session_duration:.0f}s): "
                f"agent refused mid-session ({outcome.refusal_reason})",
                {"event": "session_error",
                 "session_id": session_id, "variant": variant,
                 "backend": backend_name,
                 "error": f"agent refused mid-session ({outcome.refusal_reason})",
                 "functions": functions_addrs,
                 "duration_s": outcome.session_duration,
                 "kind": "refusal",
                 "refusal_reason": outcome.refusal_reason})

        if not outcome.success and outcome.error_msg is not None:
            return _abort(outcome, OUTCOME_AGENT_FAIL,
                f"Session {session_id} FAILED ({outcome.session_duration:.0f}s): "
                f"{outcome.error_msg}",
                {"event": "session_error",
                 "session_id": session_id, "variant": variant,
                 "backend": backend_name, "error": outcome.error_msg,
                 "functions": functions_addrs,
                 "duration_s": outcome.session_duration,
                 "kind": "other"})

        if outcome.system_error:
            return _abort(outcome, OUTCOME_SYSTEM_ERROR,
                f"Session {session_id} SYSTEM ERROR: {outcome.system_error}",
                {"event": "system_error",
                 "session_id": session_id, "variant": variant,
                 "backend": backend_name, "error": outcome.system_error,
                 "functions": functions_addrs,
                 "duration_s": outcome.session_duration})

        log(f"Session {session_id} done ({outcome.session_duration:.0f}s): "
            f"{outcome.claimed_matched} claimed matched, {outcome.claimed_failed} failed (pre-verify)")

        backend_model = outcome.backend_model
        matched_at = _utc_now_iso()
        is_shootout = (outcome.queue_kind == "shootout")

        with db_lock:
            # Disjoint sessions update the in-memory DB so the picker
            # sees the new status. Shootout sessions DO NOT — their
            # DB writes happen inside the worktree, on the shootout
            # branch, via the commit thread. Touching the in-memory
            # DB here would land shootout matches on the main run-
            # branch's DB, defeating the entire isolation guarantee.
            if not is_shootout:
                apply_decisions_to_funcs(
                    addr_index, outcome.decisions, session_id,
                    backend_name, backend_model, matched_at, variant,
                )

            final_matched = 0
            final_failed = 0
            for func in batch:
                if is_shootout:
                    # Use the per-decision verdict directly; the main
                    # DB still says "untried" so addr_index lookup
                    # would mislabel this as untried for the log.
                    d = next((x for x in outcome.decisions
                              if x.address == func["address"]), None)
                    final_status = (d.status if d else "unknown")
                else:
                    target = addr_index.get(func["address"])
                    final_status = target["match_status"] if target else "unknown"
                log_event(log_path, {
                    "event": "function_result",
                    "session_id": session_id,
                    "variant": variant,
                    "backend": backend_name,
                    "model": backend_model,
                    "identity": outcome.identity,
                    "queue_kind": outcome.queue_kind,
                    "address": func["address"],
                    "name": func["name"],
                    "size": func["size"],
                    "status": final_status,
                })
                if final_status == "matched":
                    final_matched += 1
                elif final_status == "failed":
                    final_failed += 1

            log_event(log_path, {
                "event": "session_done",
                "session_id": session_id,
                "variant": variant,
                "backend": backend_name,
                "model": backend_model,
                "identity": outcome.identity,
                "queue_kind": outcome.queue_kind,
                "worktree": outcome.worktree,
                "matched": final_matched,
                "failed": final_failed,
                "claimed_matched": outcome.claimed_matched,
                "claimed_failed": outcome.claimed_failed,
                "duration_s": outcome.session_duration,
            })

            counters["total_matched"] += final_matched
            counters["total_failed"] += final_failed

            if not is_shootout:
                save_db(functions)

        # Outside db_lock: attempts.jsonl has its own lock; commits
        # are async via commit_queue.
        emit_attempts_for_outcome(outcome, run_id, error_kind=None)

        if outcome.matched_funcs:
            with pending_lock:
                pending_commits[session_id] = {
                    "worktree": outcome.worktree,
                    "exact_paths": set(ctx.exact_paths),
                    "sibling_prefixes": tuple(ctx.sibling_prefixes),
                }
            commit_queue.put({
                "session_id": session_id,
                "matched_funcs": outcome.matched_funcs,
                "matched_files": outcome.matched_files,
                "ledger_paths": outcome.ledger_paths,
                "worktree": outcome.worktree,
                "db_updates": (outcome.decisions if is_shootout else None),
                "backend_name": backend_name,
                "backend_model": backend_model,
                "matched_at": matched_at,
                "variant": variant,
            })

        return OUTCOME_SUCCESS

    executor = concurrent.futures.ThreadPoolExecutor(
        max_workers=len(slots),
        thread_name_prefix="orch-worker",
    )
    try:
        while True:
            deadline_hit = datetime.now() >= deadline
            limit_hit = (args.limit is not None
                         and counters["total_attempted"] >= args.limit)
            cb_hit = (counters["consecutive_failures"] >= MAX_CONSECUTIVE_FAILURES
                      or counters["consecutive_refusals"] >= MAX_CONSECUTIVE_REFUSALS)

            can_submit = (not deadline_hit and not limit_hit and not cb_hit
                          and not done_picking
                          and time.time() >= next_pick_time)

            if can_submit:
                progressed = False
                stalled_idle_slots = 0
                idle_slot_indices = [i for i in range(len(slots))
                                      if i not in busy_slots]
                for slot_idx in idle_slot_indices:
                    if try_pick_and_submit(executor, slot_idx):
                        progressed = True
                    else:
                        stalled_idle_slots += 1
                # Done-picking detection: every idle slot was stalled
                # AND nothing's in flight that could release a class
                # lock or fill a queue. (In flight could in principle
                # backfill via class-lock release.)
                if (not progressed and stalled_idle_slots == len(idle_slot_indices)
                        and not in_flight):
                    done_picking = True
                    log("No more untried functions matching criteria. Done.")

            if not in_flight:
                if done_picking or deadline_hit or limit_hit or cb_hit:
                    if limit_hit:
                        log(f"Reached limit of {args.limit} functions. Done.")
                    if cb_hit and counters["consecutive_failures"] >= MAX_CONSECUTIVE_FAILURES:
                        log(f"Too many consecutive failures "
                            f"({counters['consecutive_failures']}). Stopping.")
                    if cb_hit and counters["consecutive_refusals"] >= MAX_CONSECUTIVE_REFUSALS:
                        log(f"Hit {counters['consecutive_refusals']} consecutive refusals — "
                            f"the anti-refusal mitigations aren't holding. Stopping; a "
                            f"human needs to review the prompt/system-prompt-append.")
                    break
                # Idle — likely waiting on backoff timer.
                time.sleep(min(1.0, max(0.1, next_pick_time - time.time())))
                continue

            done, _ = concurrent.futures.wait(
                list(in_flight.keys()),
                timeout=2.0,
                return_when=concurrent.futures.FIRST_COMPLETED,
            )

            if not done:
                continue

            for future in done:
                ctx, slot_idx = in_flight.pop(future)
                busy_slots.discard(slot_idx)
                try:
                    outcome = future.result()
                except BaseException as e:
                    log(f"WORKER EXCEPTION: {ctx.session_id} — {e!r}")
                    log_event(log_path, {
                        "event": "worker_exception",
                        "session_id": ctx.session_id,
                        "variant": ctx.variant,
                        "backend": ctx.backend.name,
                        "identity": ctx.identity,
                        "queue_kind": ctx.queue_kind,
                        "error": repr(e),
                    })
                    with db_lock:
                        if ctx.queue_kind == "disjoint":
                            revert_in_progress(functions, ctx.batch, addr_index)
                            save_db(functions)
                        else:
                            schedule.unmark_shootout_attempted(
                                ctx.identity,
                                [f["address"] for f in ctx.batch])
                        counters["total_attempted"] -= len(ctx.batch)
                    held_classes.difference_update(ctx.classes)
                    counters["total_errors"] += 1
                    counters["consecutive_failures"] += 1
                    if counters["consecutive_failures"] < MAX_CONSECUTIVE_FAILURES:
                        backoff = min(30 * (2 ** counters["consecutive_failures"]), 960)
                        log(f"Backing off {backoff}s before next pick...")
                        next_pick_time = time.time() + backoff
                    continue

                kind = apply_outcome(outcome, ctx)
                # class_locks only matter for the shared (main)
                # worktree. Shootout slots run alone in their own
                # tree, so class_locks were never acquired and there's
                # nothing to release.
                if ctx.queue_kind == "disjoint":
                    held_classes.difference_update(ctx.classes)

                if kind == OUTCOME_REFUSAL:
                    counters["total_errors"] += 1
                    counters["consecutive_refusals"] += 1
                    if counters["consecutive_refusals"] < MAX_CONSECUTIVE_REFUSALS:
                        log(f"Refusal #{counters['consecutive_refusals']} — short pause, continuing.")
                        next_pick_time = max(next_pick_time, time.time() + REFUSAL_BACKOFF_S)
                elif kind in (OUTCOME_PREP_ERROR, OUTCOME_AGENT_FAIL, OUTCOME_SYSTEM_ERROR):
                    counters["total_errors"] += 1
                    counters["consecutive_failures"] += 1
                    if counters["consecutive_failures"] < MAX_CONSECUTIVE_FAILURES:
                        backoff = min(30 * (2 ** counters["consecutive_failures"]), 960)
                        log(f"Backing off {backoff}s before next pick...")
                        next_pick_time = max(next_pick_time, time.time() + backoff)
                elif kind == OUTCOME_SUCCESS:
                    counters["consecutive_failures"] = 0
                    counters["consecutive_refusals"] = 0
                else:
                    raise RuntimeError(
                        f"apply_outcome returned unknown kind {kind!r}"
                    )

            print_progress(functions, start_time, log_path)
    finally:
        # Order matters: workers finish first so the last apply_outcome
        # calls reach commit_queue, then drain. Reverse order would
        # silently drop commits queued during shutdown. Worktree
        # teardown happens last, after commits have flushed —
        # `git worktree remove` on a tree with uncommitted changes
        # would lose work otherwise.
        executor.shutdown(wait=True)
        commit_queue.put(None)
        commit_thread.join()
        if shootout_worktrees:
            log("Tearing down shootout worktrees...")
            teardown_shootout_worktrees(shootout_worktrees)

    total_attempted = counters["total_attempted"]
    total_matched = counters["total_matched"]
    total_failed = counters["total_failed"]
    total_errors = counters["total_errors"]

    # Final summary
    elapsed = datetime.now() - start_time
    log("")
    log("=== OVERNIGHT RUN COMPLETE ===")
    log(f"Branch: {branch}")
    log(f"Duration: {str(elapsed).split('.')[0]}")
    log(f"Attempted: {total_attempted} functions")
    log(f"Matched: {total_matched}")
    log(f"Failed: {total_failed}")
    log(f"System errors: {total_errors}")
    print_progress(functions, start_time, log_path)
    log(f"Full log: {log_path}")
    log(f"")
    log(f"To review: git log {branch} --oneline")
    log(f"To merge:  git checkout main && git merge {branch}")

    log_event(log_path, {
        "event": "run_done",
        "branch": branch,
        "duration_s": elapsed.total_seconds(),
        "attempted": total_attempted,
        "matched": total_matched,
        "failed": total_failed,
        "errors": total_errors,
    })


if __name__ == "__main__":
    main()
