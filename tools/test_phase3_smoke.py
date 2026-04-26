#!/usr/bin/env python3
"""Phase 3 smoke test: A/B framework end-to-end.

Asserts the four Phase 3 deliverables hold in a mocked run:

  (a) Schedule fan-out: Mode B builds a shootout queue containing
      every input function, and `disjoint_addrs[ident]` for each
      identity is empty (Mode B does not fill disjoint queues).
  (b) Shootout commits land on per-backend branches, never on the
      base run-branch (orchestrator wires worktrees → identity
      branches; commit thread routes by worktree).
  (c) ab_report.py produces the expected McNemar table and is
      idempotent (running it twice on the same input is byte-equal).
  (d) ab_promote.py --auto-prefer claude cherry-picks claude's
      matched commits onto the base run-branch and updates the DB
      without duplicate matched_by_* writes.

Plus the standalone schedule fairness unit test (no full-run mock):
each (backend, model) identity gets approximately the same tier ×
size × class-context distribution.

Run from the repo root: `python3 tools/test_phase3_smoke.py`.
"""

from __future__ import annotations

import json
import os
import shutil
import subprocess
import sys
import tempfile
import threading
import time
from contextlib import contextmanager

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
sys.path.insert(0, HERE)


def _make_func(addr_int, class_name, idx, size=8, is_leaf=True):
    addr = f"0x{addr_int:08x}"
    method = f"M{idx}"
    return {
        "address": addr,
        "name": f"{class_name}::{method}(int)" if class_name else f"free_{idx}(int)",
        "method_name": f"{method}(int)" if class_name else "",
        "class_name": class_name,
        "obj_file": "test.obj",
        "size": size,
        "is_leaf": is_leaf,
        "match_status": "untried",
    }


def _seed_repo(tmpdir, classes_funcs):
    cfg_dir = os.path.join(tmpdir, "config")
    src_dir = os.path.join(tmpdir, "src")
    inc_dir = os.path.join(tmpdir, "include")
    res_dir = os.path.join(tmpdir, "logs", "session_results")
    pv_dir = os.path.join(tmpdir, "tools", "prompt_variants")
    extern_dir = os.path.join(tmpdir, "extern")
    expected_dir = os.path.join(tmpdir, "expected")
    for d in (cfg_dir, src_dir, inc_dir, res_dir, pv_dir, extern_dir, expected_dir):
        os.makedirs(d)
    open(os.path.join(pv_dir, "__init__.py"), "w").close()
    open(os.path.join(pv_dir, "base.py"), "w").close()

    functions = []
    addr_counter = 0x1000
    for cls, count, size in classes_funcs:
        for idx in range(count):
            functions.append(_make_func(addr_counter, cls, idx, size=size))
            addr_counter += 0x100

    with open(os.path.join(cfg_dir, "functions.json"), "w") as f:
        json.dump(functions, f, indent=2)

    subprocess.run(["git", "init", "-q", "-b", "main"], cwd=tmpdir, check=True)
    subprocess.run(["git", "config", "user.email", "t@t"], cwd=tmpdir, check=True)
    subprocess.run(["git", "config", "user.name", "t"], cwd=tmpdir, check=True)
    subprocess.run(["git", "add", "config/functions.json"],
                    cwd=tmpdir, check=True)
    subprocess.run(["git", "commit", "-q", "-m", "seed"],
                    cwd=tmpdir, check=True)
    return functions


@contextmanager
def _cwd(path):
    prev = os.getcwd()
    os.chdir(path)
    try:
        yield
    finally:
        os.chdir(prev)


def test_schedule_fairness():
    """Stratified assignment fairness: each identity sees roughly the
    same tier × size × class-context distribution.

    We construct a balanced pool (60 funcs across 5 classes × 12 sizes)
    and assert that the per-identity strata counts are within ±1 of
    each other for every stratum that has >= MIN_STRATUM_SIZE in the
    merged-strata view. Strict ±1 is what stratified round-robin
    guarantees: the rotation start is randomized but the assignment
    cadence is exact.
    """
    from ab_schedule import (build_schedule, MODE_DISJOINT,
                              _stratum_key, _build_strata)

    funcs = []
    addr = 0x1000
    for cls_idx in range(5):
        cls = f"C{cls_idx}"
        for i in range(12):
            funcs.append({
                "address": f"0x{addr:08x}",
                "size": 8 + (i % 6) * 16,
                "is_leaf": (i % 2) == 0,
                "class_name": cls,
                "match_status": "untried",
            })
            addr += 0x10

    matched_classes = {"C0"}
    identities = ["claude/x", "codex/y"]
    sched = build_schedule(funcs, identities, MODE_DISJOINT,
                            seed=12345, matched_classes=matched_classes)

    # Each identity should have ~half the functions.
    counts = {ident: len(sched.disjoint[ident]) for ident in identities}
    total = sum(counts.values())
    assert total == len(funcs), (counts, total)
    diff = abs(counts[identities[0]] - counts[identities[1]])
    assert diff <= 5, f"identity counts too skewed: {counts}"

    # Within each stratum that has >= 4 funcs, identity counts must
    # differ by at most 1 (round-robin guarantee).
    by_stratum_per_ident = {}
    for ident in identities:
        s = {}
        for f in sched.disjoint[ident]:
            k = _stratum_key(f, matched_classes)
            s[k] = s.get(k, 0) + 1
        by_stratum_per_ident[ident] = s

    merged = _build_strata(funcs, matched_classes)
    for k, lst in merged.items():
        if len(lst) < 4:
            continue
        a = by_stratum_per_ident[identities[0]].get(k, 0)
        b = by_stratum_per_ident[identities[1]].get(k, 0)
        assert abs(a - b) <= 1, (
            f"stratum {k} unbalanced: claude={a}, codex={b} (total {len(lst)})")
    print(f"schedule fairness: OK ({counts}, "
          f"{sum(1 for k, v in merged.items() if len(v) >= 4)} strata audited)")


def test_holdout_size_exact():
    """Mode C: paired_reserve_n is *exactly* honored — operator asked
    for N, the schedule must hold out N (not N-1 from rounding loss).
    """
    from ab_schedule import build_schedule, MODE_HYBRID

    funcs = [
        {"address": f"0x{i:08x}", "size": 8 + i % 32,
         "is_leaf": (i % 3) == 0, "class_name": f"C{i % 7}",
         "match_status": "untried"}
        for i in range(120)
    ]
    sched = build_schedule(funcs, ["claude/x", "codex/y"], MODE_HYBRID,
                            paired_reserve_n=37, seed=7)
    assert len(sched.shootout) == 37, len(sched.shootout)
    total_disjoint = sum(len(v) for v in sched.disjoint.values())
    assert len(sched.shootout) + total_disjoint == 120
    print("holdout size exact: OK")


class FakeBackend:
    def __init__(self, name, model):
        self.name = name
        self.model = model


def _make_shootout_run_session(matched_addrs_by_identity, lock):
    """Stub run_session that:
      - writes per-backend src files inside `cwd` (= worktree for
        shootout slots, orchestrator tree otherwise)
      - declares matches per `matched_addrs_by_identity[ident]`
      - emits a session_usage event with synthetic token counts
    """
    def fake(backend, prompt, session_id, log_fn, variant, timeout, cwd=None):
        info = _PENDING.get(session_id)
        assert info is not None
        batch = info["batch"]
        ident = info["identity"]

        usage_payload = {
            "event": "session_usage",
            "session_id": session_id,
            "variant": variant,
            "backend": backend.name,
            "model": backend.model,
            "input_tokens": 1000,
            "output_tokens": 200,
            "cached_tokens": 50,
            "cost_usd": 0.01,
            "had_usage_data": True,
            "n_turns": 1,
        }
        log_fn(usage_payload)

        results = []
        seen = set()
        wins = matched_addrs_by_identity.get(ident, set())
        base_dir = cwd if cwd else os.getcwd()
        for func in batch:
            cls = func["class_name"]
            from orchestrator import _safe_class_filename
            rel_src = (f"src/{_safe_class_filename(cls)}.cpp"
                       if cls else "src/free_functions.c")
            full_src = os.path.join(base_dir, rel_src)
            if rel_src not in seen:
                os.makedirs(os.path.dirname(full_src), exist_ok=True)
                with open(full_src, "a") as f:
                    f.write(
                        f"// {ident} {session_id}\n"
                        f"int {func['method_name'].split('(')[0] if cls else func['name'].split('(')[0]}_stub() {{ return 0; }}\n"
                    )
                seen.add(rel_src)
            status = "matched" if func["address"] in wins else "failed"
            results.append({
                "address": func["address"],
                "status": status,
                "file": rel_src,
                "notes": "" if status == "matched" else "stub failure",
            })

        # session_results dir is symlinked across worktrees so always
        # lands in one place — write via the orchestrator's tree to
        # match the orchestrator's read path in parse_session_results.
        results_path = os.path.join("logs", "session_results", f"{session_id}.json")
        with open(results_path, "w") as f:
            json.dump(results, f)

        return True, None, dict(usage_payload)
    return fake


_PENDING: dict = {}


def test_phase3_end_to_end():
    """Mode B run with two backends across the same 6-function pool.
    Asserts (a)–(d) end-to-end."""
    tmp = tempfile.mkdtemp(prefix="phase3_smoke_")
    try:
        functions = _seed_repo(
            tmp,
            classes_funcs=[
                ("eAlpha", 2, 12),
                ("eBeta", 2, 16),
                ("eGamma", 2, 24),
            ],
        )

        with _cwd(tmp):
            import orchestrator
            from ab_schedule import MODE_SHOOTOUT
            import importlib

            orchestrator.ensure_expected_o = lambda func: None
            orchestrator.verify_tree_compiles = lambda: None

            class _OkResult:
                ok = True
                sym_name = "fake_sym"
                reason = ""
                diff_count = 0
                byte_diffs: list = []

            orchestrator.check_byte_match = lambda func, src, **kw: _OkResult()
            orchestrator.validate_source_quality = lambda files: []
            orchestrator.reject_extern_c_class_methods = lambda funcs, m: []
            # Compile-check is shell-out to SNC; no compiler in this
            # test env, so short-circuit to "no failures".
            orchestrator._collect_compile_failures = lambda paths, **kw: []
            orchestrator.build_prompt = (
                lambda batch, functions, session_id, variant: ("prompt", []))

            # Choose the matchers per-identity — claude wins on
            # addresses 0x1000, 0x1100, 0x1200 (eAlpha + eBeta partial);
            # codex wins on 0x1100, 0x1300 (overlap on 0x1100, codex
            # alone on 0x1300). This exercises the McNemar table:
            # both=1, only-claude=2, only-codex=1.
            matched_by_ident = {
                "claudefake/c-1": {"0x00001000", "0x00001100", "0x00001200"},
                "codexfake/c-2": {"0x00001100", "0x00001300"},
            }

            real_run_one_session = orchestrator.run_one_session

            def hooked_run_one_session(ctx):
                _PENDING[ctx.session_id] = {
                    "batch": ctx.batch,
                    "identity": ctx.identity,
                }
                try:
                    return real_run_one_session(ctx)
                finally:
                    _PENDING.pop(ctx.session_id, None)

            orchestrator.run_one_session = hooked_run_one_session

            commit_observed: list = []
            commit_lock = threading.Lock()
            real_commit = orchestrator.git_commit_batch

            def tracking_commit(session_id, matched_funcs, matched_files,
                                ledger_paths, **kwargs):
                # Defer to the real commit so per-backend branches
                # actually advance — that's what (b) and (d) need.
                real_commit(session_id, matched_funcs, matched_files,
                            ledger_paths, **kwargs)
                # Capture branch + sha NOW: teardown removes the
                # worktree before main() returns, so post-run lookup
                # would hit a missing dir. We record the branch the
                # commit landed on and its sha so audit (b) doesn't
                # need the worktree to still exist.
                wt = kwargs.get("worktree")
                branch = None
                sha = None
                if wt:
                    rc = subprocess.run(
                        ["git", "-C", wt, "rev-parse", "HEAD"],
                        capture_output=True, text=True)
                    if rc.returncode == 0:
                        sha = rc.stdout.strip()
                    rc = subprocess.run(
                        ["git", "-C", wt, "branch", "--show-current"],
                        capture_output=True, text=True)
                    if rc.returncode == 0:
                        branch = rc.stdout.strip()
                with commit_lock:
                    commit_observed.append({
                        "session_id": session_id,
                        "matched_addrs": [f["address"] for f in matched_funcs],
                        "ledger_paths": sorted(ledger_paths),
                        "worktree": wt,
                        "branch": branch,
                        "sha": sha,
                        "db_updates_count": (len(kwargs["db_updates"])
                                              if kwargs.get("db_updates")
                                              else 0),
                    })

            orchestrator.git_commit_batch = tracking_commit
            orchestrator.run_session = _make_shootout_run_session(
                matched_by_ident, threading.Lock())

            def fake_get_backend(name, system_append="", model=""):
                # Distinguish the two by name suffix so identity_key
                # produces two distinct identities.
                return {
                    "claudefake": FakeBackend("claudefake", "c-1"),
                    "codexfake": FakeBackend("codexfake", "c-2"),
                }[name]
            orchestrator.get_backend = fake_get_backend

            # Real overnight branch creation. The fixture is on `main`,
            # so create_overnight_branch checks out a fresh branch as
            # in production.

            sys.argv = [
                "orchestrator.py",
                "--workers", "1",
                "--backend", "claudefake,codexfake",
                "--shootout",
                "--limit", "100",
                "--hours", "0.05",
                "--batch-size", "1",
            ]
            orchestrator.AVAILABLE_BACKENDS = ["claudefake", "codexfake"]

            try:
                orchestrator.main()
            except SystemExit as e:
                if e.code not in (0, None):
                    raise AssertionError(
                        f"orchestrator.main() exited early (code {e.code})"
                    ) from e

            # ---- Audits ----------------------------------------------------------
            # (a) Schedule fan-out: log shows mode=B with shootout count
            log_files = [p for p in os.listdir("logs")
                          if p.startswith("match_") and p.endswith(".jsonl")
                          and not os.path.islink(os.path.join("logs", p))]
            assert log_files, "no match log produced"
            schedule_events = []
            for lf in log_files:
                with open(os.path.join("logs", lf)) as f:
                    for line in f:
                        line = line.strip()
                        if not line:
                            continue
                        try:
                            ev = json.loads(line)
                        except json.JSONDecodeError:
                            continue
                        if ev.get("event") == "schedule_built":
                            schedule_events.append(ev)
            assert schedule_events, "no schedule_built event"
            ev = schedule_events[-1]
            assert ev["mode"] == "B"
            assert ev["shootout_count"] == 6, ev
            for ident, n in ev["disjoint_counts"].items():
                assert n == 0, ev
            print(f"(a) schedule fan-out (Mode B, shootout={ev['shootout_count']}): PASS")

            # (b) shootout commits live only on per-backend branches.
            commits_per_branch: dict = {}
            for c in commit_observed:
                wt = c["worktree"]
                assert wt is not None, c
                tag = os.path.basename(wt).replace("overnight-worktree-", "")
                commits_per_branch.setdefault(tag, []).append(c)
            assert len(commits_per_branch) == 2, commits_per_branch
            # Branches were captured during the commit hook; assert
            # each is the expected per-identity sibling of the base
            # run-branch.
            for tag, commits in commits_per_branch.items():
                for c in commits:
                    assert c["branch"], c
                    assert c["branch"].endswith(tag), (c["branch"], tag)
            # Main run-branch should NOT have any of these commits
            # (shootout isolation invariant). The orchestrator's
            # `branch` is `overnight/<ts>` — find it.
            rc = subprocess.run(["git", "branch", "--list", "overnight/*"],
                                 capture_output=True, text=True, cwd=tmp)
            assert rc.returncode == 0
            all_branches = [l.strip().lstrip("* ").strip()
                              for l in rc.stdout.splitlines() if l.strip()]
            # Base has fewest dashes in the suffix after `overnight/`
            # (YYYYMMDD-HHMMSS = 1 dash). Shootout siblings add more.
            base = min(all_branches,
                        key=lambda b: b[len("overnight/"):].count("-"))
            assert base.count("-") == 1, all_branches
            rc = subprocess.run(["git", "log", "--format=%s", base],
                                 capture_output=True, text=True, cwd=tmp)
            assert rc.returncode == 0, rc.stderr
            main_subjects = rc.stdout.strip().splitlines()
            shootout_subjects = {f"Match 1 functions (session {c['session_id']})"
                                  for c in commit_observed}
            leaked = [s for s in main_subjects if s in shootout_subjects]
            assert not leaked, f"shootout commits leaked to main run-branch: {leaked}"
            print(f"(b) shootout commits on per-identity branches "
                  f"({len(commit_observed)} commits, "
                  f"{len(commits_per_branch)} branches, none on main): PASS")

            # (c) ab_report McNemar + idempotent
            attempts_path = os.path.join(tmp, "logs", "attempts.jsonl")
            assert os.path.exists(attempts_path), "attempts.jsonl missing"

            import ab_report
            from pathlib import Path
            attempts = ab_report.load_attempts(Path(attempts_path))
            pairs = ab_report.shootout_pairs(attempts)
            ident_a, ident_b = sorted({
                f"{r['backend']}/{r['model']}" for r in attempts})[:2]
            both, a_only, b_only, neither = ab_report._mcnemar_table(
                pairs, ident_a, ident_b)
            # Expected from matched_by_ident dict above (after sort):
            # ident_a = claudefake/c-1, ident_b = codexfake/c-2
            # claude wins: 1000, 1100, 1200 -> 3
            # codex wins:  1100, 1300       -> 2
            # both:        1100             -> 1
            # only_claude: 1000, 1200       -> 2
            # only_codex:  1300             -> 1
            # neither:     1200..no, wait: addrs covered =
            #   0x1000 0x1100 0x1200 0x1300 0x1400 0x1500
            #   neither = 0x1400, 0x1500 → 2
            assert both == 1, (both, a_only, b_only, neither)
            assert a_only == 2, (both, a_only, b_only, neither)
            assert b_only == 1, (both, a_only, b_only, neither)
            assert neither == 2, (both, a_only, b_only, neither)
            print(f"(c) McNemar table both={both}, "
                  f"only_a={a_only}, only_b={b_only}, neither={neither}: PASS")

            # Idempotency: render twice, byte-equal.
            per_id1 = ab_report.per_identity_summary(attempts)
            strat1 = ab_report.stratified_breakdown(attempts)
            text1 = ab_report.render_text(per_id1, strat1, pairs,
                                           sorted({f"{r['backend']}/{r['model']}" for r in attempts}))
            per_id2 = ab_report.per_identity_summary(attempts)
            strat2 = ab_report.stratified_breakdown(attempts)
            text2 = ab_report.render_text(per_id2, strat2, pairs,
                                           sorted({f"{r['backend']}/{r['model']}" for r in attempts}))
            assert text1 == text2, "ab_report not idempotent"
            print(f"(c) ab_report idempotent over {len(text1)} chars: PASS")

            # (d) ab_promote.py --auto-prefer claudefake cherry-picks
            # claude's commits onto the main run-branch and updates DB.
            # We can't shell out to ab_promote (it's a CLI), so call
            # plan() and simulate cherry-pick by invoking git directly.
            import importlib
            ab_promote = importlib.import_module("ab_promote")

            # `base` was already resolved in audit (b). Discover the
            # shootout branches relative to it.
            shootout = ab_promote.discover_shootout_branches(base)
            assert len(shootout) == 2, shootout

            # Plan + execute --auto-prefer claudefake.
            actions = ab_promote.plan(
                attempts,
                identities=[ident_a, ident_b],
                shootout_branches=shootout,
                prefer="claudefake",
            )
            promoted = [a for a in actions if a.get("winner_identity")]
            # Expected: every matched address gets a winner.
            # Both matched 0x1100 → claude (--prefer claudefake).
            # Claude alone: 0x1000, 0x1200.
            # Codex alone:  0x1300.
            # Total 4 promotions.
            assert len(promoted) == 4, [a for a in actions]
            claude_wins = sum(1 for a in promoted
                               if a["winner_identity"].startswith("claudefake"))
            codex_wins = sum(1 for a in promoted
                              if a["winner_identity"].startswith("codexfake"))
            assert claude_wins == 3, (claude_wins, codex_wins)
            assert codex_wins == 1, (claude_wins, codex_wins)
            print(f"(d) ab_promote plan: {claude_wins} claude + "
                  f"{codex_wins} codex (prefer=claudefake): PASS")

            # Cherry-pick on the base run-branch and verify DB updates.
            # Important: cherry-pick in commit-chronological order
            # within an identity so each commit's parent state lines
            # up with what's on main. ab_promote.plan returns actions
            # in addr order, which happens to match chronological
            # order for a single identity here, but we sort
            # explicitly for clarity.
            subprocess.run(["git", "checkout", base], cwd=tmp,
                            check=True, capture_output=True)
            claude_actions = [a for a in promoted
                               if a["winner_identity"].startswith("claudefake")]
            codex_actions = [a for a in promoted
                              if a["winner_identity"].startswith("codexfake")]
            assert len(claude_actions) == 3 and len(codex_actions) == 1, promoted

            for a in claude_actions:
                ok, err = ab_promote.cherry_pick_one(
                    a["winner_sha"], a["winner_branch"], dry_run=False)
                if not ok:
                    raise AssertionError(
                        f"claude cherry-pick {a['winner_sha']} unexpectedly "
                        f"failed: {err}")
            print(f"(d) cherry-picked {len(claude_actions)} claude commits "
                  f"onto {base}: PASS")

            # Verify claude's 3 matches landed in main DB with
            # matched_by_* attribution and no duplicates.
            with open(os.path.join(tmp, "config", "functions.json")) as f:
                final_db = json.load(f)
            matched_db = {f["address"]: f for f in final_db
                           if f["match_status"] == "matched"}
            expected_claude = {"0x00001000", "0x00001100", "0x00001200"}
            assert set(matched_db.keys()) == expected_claude, (
                matched_db.keys(), expected_claude)
            # Tied address 0x00001100 went to claude per --auto-prefer.
            assert matched_db["0x00001100"]["matched_by_backend"] == "claudefake"
            for addr, f in matched_db.items():
                # No duplicate matched_by_* writes (each row attributed
                # exactly once).
                assert "matched_by_backend" in f
                assert "matched_by_session_id" in f
                assert "matched_by_model" in f
                assert "matched_at" in f
            print(f"(d) main DB has {len(matched_db)} claude matches with "
                  f"single matched_by_* attributions: PASS")

            # ab_promote conflict path: codex's eBeta.cpp adds collide
            # with claude's already-promoted eBeta.cpp ("both added").
            # cherry_pick_one must (i) return ok=False, (ii) leave a
            # clean tree (cherry-pick aborted).
            codex_action = codex_actions[0]
            ok, err = ab_promote.cherry_pick_one(
                codex_action["winner_sha"], codex_action["winner_branch"],
                dry_run=False)
            assert not ok, "expected conflict on codex eBeta.cpp"
            assert "conflicted" in err.lower(), err
            rc = subprocess.run(["git", "status", "--porcelain"],
                                 cwd=tmp, capture_output=True, text=True)
            # Untracked logs/ + tools/ from the smoke test scaffolding
            # are fine; assert there are NO unmerged paths or staged
            # changes.
            for line in rc.stdout.splitlines():
                xy = line[:2]
                # "??" = untracked (ok), anything else (M., A., U., etc.)
                # would mean the abort failed.
                assert xy.strip() == "??" or xy == "??", (
                    f"cherry-pick abort left dirty path: {line}")
            print(f"(d) ab_promote conflict path: aborted cleanly with "
                  f"clear error message: PASS")

            # (e) Worktree teardown invariant: after the orchestrator
            # exits, `git worktree list` should show no shootout trees.
            rc = subprocess.run(["git", "worktree", "list"],
                                 cwd=tmp, capture_output=True, text=True)
            assert rc.returncode == 0
            wt_lines = [l for l in rc.stdout.splitlines()
                         if "overnight-worktree-" in l]
            assert not wt_lines, (
                f"worktrees not cleaned up: {wt_lines}")
            print("(e) worktrees torn down at run end: PASS")

    finally:
        shutil.rmtree(tmp, ignore_errors=True)


def main():
    test_schedule_fairness()
    test_holdout_size_exact()
    test_phase3_end_to_end()
    print("Phase 3 smoke: PASS")


if __name__ == "__main__":
    main()
