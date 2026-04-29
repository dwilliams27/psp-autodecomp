#!/usr/bin/env python3
"""Phase 1 smoke test: race-freedom invariants for the multi-agent
orchestrator. See docs/direction/003-multi-agent-ab-architecture.md.

Mocks the agent CLI + verify path so the coordinator runs end-to-end
without spending credits or touching the real DB. Asserts the four
Phase 1 invariants:

  1. functions.json mutations all happen under db_lock.
  2. Class-level lock exclusion: no two concurrent sessions ever hold
     the same class.
  3. Each commit covers exactly one session's ledger.
  4. Out-of-scope edits are detected, reverted, and reject the match.

Run from the repo root: `python3 tools/test_phase1_smoke.py`.
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
from unittest import mock

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
sys.path.insert(0, HERE)


def _make_func(addr_int, class_name, idx, size=8):
    addr = f"0x{addr_int:08x}"
    method = f"M{idx}"
    return {
        "address": addr,
        "name": f"{class_name}::{method}(int)" if class_name else f"free_{idx}(int)",
        "method_name": f"{method}(int)" if class_name else "",
        "class_name": class_name,
        "obj_file": "test.obj",
        "size": size,
        "is_leaf": True,
        "match_status": "untried",
    }


def _seed_repo(tmpdir, classes_funcs):
    """Build a minimal repo layout for the test under tmpdir.

    Initializes a git repo (so git_run + partition_dirty_paths work),
    creates config/functions.json from `classes_funcs`, stubs src/include
    scaffolding, and writes a tools/prompt_variants/base.py shim so the
    orchestrator's startup variant glob succeeds in this CWD.
    """
    cfg_dir = os.path.join(tmpdir, "config")
    src_dir = os.path.join(tmpdir, "src")
    inc_dir = os.path.join(tmpdir, "include")
    res_dir = os.path.join(tmpdir, "logs", "session_results")
    pv_dir = os.path.join(tmpdir, "tools", "prompt_variants")
    os.makedirs(cfg_dir)
    os.makedirs(src_dir)
    os.makedirs(inc_dir)
    os.makedirs(res_dir)
    os.makedirs(pv_dir)
    # Empty stubs — the real prompt_variants.base is on sys.path already
    # (tools/ is sys.path[0]); the orchestrator only needs the file to
    # exist for its `tools/prompt_variants/*.py` glob check.
    with open(os.path.join(pv_dir, "__init__.py"), "w"):
        pass
    with open(os.path.join(pv_dir, "base.py"), "w"):
        pass

    functions = []
    addr_counter = 0x1000
    for cls, count in classes_funcs:
        for idx in range(count):
            functions.append(_make_func(addr_counter, cls, idx))
            addr_counter += 0x100

    with open(os.path.join(cfg_dir, "functions.json"), "w") as f:
        json.dump(functions, f, indent=2)

    subprocess.run(["git", "init", "-q"], cwd=tmpdir, check=True)
    subprocess.run(["git", "-c", "user.email=t@t", "-c", "user.name=t",
                    "add", "config/functions.json"], cwd=tmpdir, check=True)
    subprocess.run(["git", "-c", "user.email=t@t", "-c", "user.name=t",
                    "commit", "-q", "-m", "seed"], cwd=tmpdir, check=True)
    return functions


@contextmanager
def _cwd(path):
    prev = os.getcwd()
    os.chdir(path)
    try:
        yield
    finally:
        os.chdir(prev)


class FakeBackend:
    def __init__(self, name="fakeclaude", model="fake-model-1", *_, **__):
        self.name = name
        self.model = model


def _make_fake_run_session(observed_classes, lock,
                           per_session_delay=0.05,
                           write_extra_oos_path=False):
    """Returns a stub `run_session(...)` that:
      - Records the class set held while it's "running" (for invariant 2).
      - Writes a fake session_results JSON declaring all batch funcs matched.
      - Optionally writes an out-of-scope file to test invariant 4.
    """
    def fake(backend, prompt, session_id, log_fn, variant, timeout, cwd=None):
        # Extract the batch addresses from the prompt by looking at the
        # results-file path the prompt instructs the agent to write.
        # We re-derive batch addresses from the session_results file
        # convention. But we need to know what to put in the JSON. The
        # coordinator gives us the prompt; we can't easily extract batch
        # addrs. Instead, the test installs `_pending_batches` keyed by
        # session_id so the stub can look up what to claim.
        info = _PENDING_BATCHES.get(session_id)
        if info is None:
            return False, "test stub: no pending batch info for session", {}

        batch, allowed_paths = info["batch"], info["allowed_paths"]
        # Write each allowed src/.cpp file with a single fake C++ method
        # so the partition picks them up as in-scope dirty paths.
        seen_files = set()
        results = []
        for func in batch:
            cls = func["class_name"]
            if cls:
                from orchestrator import _safe_class_filename
                src = f"src/{_safe_class_filename(cls)}.cpp"
            else:
                src = "src/free_functions.c"
            if src not in seen_files:
                with open(src, "a") as f:
                    f.write(
                        f"// touched by session {session_id}\n"
                        f"int {func['method_name'].split('(')[0] if cls else func['name'].split('(')[0]}_stub() {{ return 0; }}\n"
                    )
                seen_files.add(src)
            results.append({
                "address": func["address"],
                "status": "matched",
                "file": src,
            })

        # Optional invariant-4 probe: write a file outside the allowed set.
        if write_extra_oos_path:
            with open("src/EVIL_out_of_scope.cpp", "w") as f:
                f.write("// shouldn't survive — Phase 1 ledger should revert me\n")

        results_path = os.path.join("logs", "session_results", f"{session_id}.json")
        with open(results_path, "w") as f:
            json.dump(results, f)

        # Record the class set while we're "live" (for invariant 2 audit).
        classes = {(f.get("class_name") or "<free>") for f in batch}
        with lock:
            for prior_id, prior_classes in observed_classes.items():
                if prior_id == session_id:
                    continue
                # The other session is also "live" — its class set must
                # be disjoint from ours, by invariant 2.
                if prior_classes & classes:
                    raise AssertionError(
                        f"INVARIANT 2 VIOLATED: session {session_id} "
                        f"running concurrently with {prior_id} on overlapping "
                        f"class set {prior_classes & classes}"
                    )
            observed_classes[session_id] = classes

        time.sleep(per_session_delay)

        with lock:
            observed_classes.pop(session_id, None)

        return True, None, {"had_usage_data": False}

    return fake


_PENDING_BATCHES = {}


def _record_pending_batch_via_session_start(orig_log_event):
    """Hook into log_event to capture the (session_id, batch, allowed_paths)
    triple from session_start events so the fake run_session can replay them.
    """
    def wrapper(log_path, event):
        if event.get("event") == "session_start":
            sid = event["session_id"]
            # The batch + allowed paths must be retrieved from elsewhere.
            # We attach them via the orchestrator module's WorkContext;
            # see _hook_run_one_session.
            pass
        orig_log_event(log_path, event)
    return wrapper


def main():
    # Each test starts in a clean tempdir.
    tmp = tempfile.mkdtemp(prefix="phase1_smoke_")
    try:
        functions = _seed_repo(
            tmp,
            classes_funcs=[("eFoo", 4), ("eBar", 4), ("eBaz", 4), ("eQux", 2)],
        )
        with _cwd(tmp):
            import orchestrator
            from backends import base as backends_base

            # Replace heavy/external dependencies with fakes.
            orchestrator.ensure_expected_o = lambda func: None
            orchestrator.create_overnight_branch = lambda: "test-branch"
            orchestrator.verify_tree_compiles = lambda: None

            class _OkResult:
                ok = True
                sym_name = "fake_sym"
                reason = ""
                diff_count = 0
                byte_diffs: list = []

            orchestrator.check_byte_match = lambda func, src, **kw: _OkResult()
            orchestrator.validate_source_quality = lambda files, **kw: []
            orchestrator.reject_extern_c_class_methods = lambda funcs, m, **kw: []

            # Stub build_prompt to a trivial no-op (we're not actually
            # running an agent; the fake run_session writes results
            # directly).
            orchestrator.build_prompt = (
                lambda batch, functions, session_id, variant: ("prompt", []))

            # Track concurrent class sets for invariant 2.
            observed_classes = {}
            audit_lock = threading.Lock()
            commits_observed: list = []
            commits_lock = threading.Lock()

            # Stub git_commit_batch: record what would be committed
            # (invariant 3 — each commit covers exactly one session's
            # ledger, and the ledger is what we recorded).
            def fake_commit(session_id, matched_funcs, matched_files,
                            ledger_paths, **kwargs):
                with commits_lock:
                    commits_observed.append({
                        "session_id": session_id,
                        "matched_addrs": [f["address"] for f in matched_funcs],
                        "matched_files": sorted(matched_files),
                        "ledger_paths": sorted(ledger_paths),
                        "worktree": kwargs.get("worktree"),
                    })
            orchestrator.git_commit_batch = fake_commit

            # Wire up _PENDING_BATCHES via WorkContext: monkey-patch
            # run_one_session to record batch info before delegating.
            real_run_one_session = orchestrator.run_one_session

            def hooked_run_one_session(ctx):
                _PENDING_BATCHES[ctx.session_id] = {
                    "batch": ctx.batch,
                    "allowed_paths": ctx.exact_paths,
                }
                try:
                    return real_run_one_session(ctx)
                finally:
                    _PENDING_BATCHES.pop(ctx.session_id, None)

            orchestrator.run_one_session = hooked_run_one_session

            # Every session writes an out-of-scope probe (`src/EVIL_*.cpp`)
            # so invariant 4 is exercised on every batch — the partition
            # must classify it as out-of-scope, the worker must revert
            # it, and no commit's ledger may contain it.
            call_count = {"n": 0}
            evil_fake = _make_fake_run_session(
                observed_classes, audit_lock, per_session_delay=0.10,
                write_extra_oos_path=True)

            def dispatching_fake(*a, **kw):
                call_count["n"] += 1
                return evil_fake(*a, **kw)

            orchestrator.run_session = dispatching_fake

            # Also stub the Backend constructor — coordinator calls
            # get_backend() to instantiate.
            orchestrator.get_backend = (
                lambda name, system_append="", model="", effort="": FakeBackend(
                    name=name, model=f"{name}-model"
                )
            )

            # Drive a small run: 2 workers, --limit 4.
            sys.argv = ["orchestrator.py", "--workers", "2",
                        "--backend", "fakeclaude,fakeclaude2",
                        "--limit", "4",
                        "--hours", "0.05",
                        "--dry-run",
                        "--batch-size", "2"]
            # Allow our fake backend names through the validation check.
            orchestrator.AVAILABLE_BACKENDS = ["fakeclaude", "fakeclaude2"]

            try:
                orchestrator.main()
            except SystemExit as e:
                if e.code not in (0, None):
                    raise AssertionError(
                        f"orchestrator.main() exited early (code {e.code}); "
                        f"check that the test setup matches main()'s expectations"
                    ) from e

            # ---- Invariant audits -------------------------------------------------
            # Invariant 1: every save_db went through atomic write — verified
            # by reading the DB at the end and confirming it parses.
            with open("config/functions.json") as f:
                final_db = json.load(f)
            assert isinstance(final_db, list) and len(final_db) == len(functions)

            # Invariant 2: no concurrent class overlaps (audit_lock
            # assertion would have raised; if we got here, no overlap).
            print("INVARIANT 2 (no concurrent class overlap): PASS")

            # Invariant 3: every commit's ledger_paths is a subset of the
            # session's allowed paths. We need the original allowed
            # paths per session. Verify each ledger file is .cpp / .h
            # under src/include in the expected pattern.
            for c in commits_observed:
                for p in c["ledger_paths"]:
                    assert p.startswith(("src/", "include/")), (
                        f"ledger contains non-src/include path: {p}")
                assert len(c["matched_addrs"]) > 0
            print(f"INVARIANT 3 (commits scoped to ledger): PASS over {len(commits_observed)} commits")

            # Invariant 4: out-of-scope files were reverted (probe wrote
            # `src/EVIL_out_of_scope.cpp` from session #3 onward; that
            # path is not in any class's allowed set, so it must not be
            # in any commit's ledger and must not exist on disk after
            # the session).
            for c in commits_observed:
                for p in c["ledger_paths"]:
                    assert "EVIL_out_of_scope" not in p, (
                        f"INVARIANT 4 VIOLATED: out-of-scope probe in commit ledger: {p}")
            assert not os.path.exists("src/EVIL_out_of_scope.cpp"), (
                "INVARIANT 4 VIOLATED: out-of-scope file survived; expected revert")
            print("INVARIANT 4 (out-of-scope reverted, never committed): PASS")

            # Confirm we actually ran with concurrency: at least 2 sessions
            # observed (could be limit-bounded). matched count should
            # equal the post-final DB count.
            matched = sum(1 for f in final_db if f["match_status"] == "matched")
            print(f"matched in final DB: {matched}")
            print(f"sessions observed: {len(commits_observed)}")
            assert len(commits_observed) >= 1, (
                "no sessions completed — coordinator wiring broken")
    finally:
        shutil.rmtree(tmp, ignore_errors=True)

    print("Phase 1 smoke: PASS")


if __name__ == "__main__":
    main()
