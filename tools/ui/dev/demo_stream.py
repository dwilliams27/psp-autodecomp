#!/usr/bin/env python3
"""Write a scripted sequence of realistic orchestrator events to a jsonl
file with simulated timing, so the TUI can be exercised end-to-end
without actually invoking Claude.

Usage:
    # terminal 1: start the streamer
    python3 tools/ui/dev/demo_stream.py logs/demo.jsonl

    # terminal 2: point the TUI at the same file
    python3 tools/ui/app.py --log logs/demo.jsonl --no-backfill

The stream hits every panel: run_start, session_start, agent_event
(text/thinking/tool_use/tool_result), verify_failed, function_result
(post-verify), session_done, progress_tick, orch_note, and finally
run_done. About 90 seconds end-to-end so you can watch all four panels
light up without waiting through a real overnight.
"""

import argparse
import json
import os
import sys
import time
from datetime import datetime, timedelta


def emit(path, event):
    event["timestamp"] = datetime.now().isoformat()
    with open(path, "a") as f:
        f.write(json.dumps(event) + "\n")


def run(path):
    # Truncate if the file already exists so each run is a fresh stream.
    with open(path, "w"):
        pass

    now = datetime.now()

    # --- run starts -------------------------------------------------
    emit(path, {
        "event": "run_start",
        "hours": 1.0,
        "start_time": now.isoformat(),
        "deadline": (now + timedelta(hours=1)).isoformat(),
        "variants": ["base", "tier12b"],
        "batch_size": 5,
        "session_timeout": 1800,
        "mode": "general",
        "targets_count": 0,
        "dry_run": False,
    })
    time.sleep(0.5)

    emit(path, {"event": "orch_note",
                "text": "Starting overnight run: 1.0h time limit, deadline 22:00"})
    emit(path, {"event": "orch_note",
                "text": "Mode: general pool, batch_size=5, session_timeout=1800s"})
    emit(path, {"event": "progress_tick",
                "matched_total": 1208, "failed_total": 256,
                "untried_total": 7778, "total": 9242,
                "elapsed_s": 0.0})
    time.sleep(1.0)

    # --- session 1: clean 4/5 match with one verify rejection --------
    s1 = "11111111"
    emit(path, {"event": "orch_note",
                "text": f"Session {s1} [tier12b]: 5 functions — eFoo::A, eFoo::B, eFoo::C, eFoo::D, eFoo::E"})
    emit(path, {
        "event": "session_start",
        "session_id": s1,
        "variant": "tier12b",
        "class_name": "eFoo",
        "functions": [
            {"address": "0x00012000", "name": "eFoo::A(int)", "size": 32, "obj_file": "eAll_psp.obj"},
            {"address": "0x00012020", "name": "eFoo::B(void)", "size": 48, "obj_file": "eAll_psp.obj"},
            {"address": "0x00012050", "name": "eFoo::C(const mVec3 &)", "size": 96, "obj_file": "eAll_psp.obj"},
            {"address": "0x000120b0", "name": "eFoo::D(cFile &) const", "size": 64, "obj_file": "eAll_psp.obj"},
            {"address": "0x000120f0", "name": "eFoo::~eFoo(void)", "size": 128, "obj_file": "eAll_psp.obj"},
        ],
    })
    time.sleep(0.8)

    # agent_event stream for one function
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "tier12b",
                "kind": "text", "text": "I'll start by querying the class's matched siblings."})
    time.sleep(0.3)
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "tier12b",
                "kind": "tool_use", "tool": "Bash",
                "text": "$ python3 tools/func_db.py query --class eFoo --status matched"})
    time.sleep(0.4)
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "tier12b",
                "kind": "tool_result", "text": "5 matched siblings found"})
    time.sleep(0.3)
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "tier12b",
                "kind": "tool_use", "tool": "Read", "text": "read src/eFoo.cpp"})
    time.sleep(0.3)
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "tier12b",
                "kind": "thinking",
                "text": "The pattern matches eBar::A. I'll copy the template and adjust the type ID."})
    time.sleep(0.5)
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "tier12b",
                "kind": "tool_use", "tool": "Bash",
                "text": "$ python3 tools/compare_func.py src/eFoo.cpp 0x00012000"})
    time.sleep(0.5)
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "tier12b",
                "kind": "tool_result", "text": "→ MATCH eFoo::A (32B)"})
    time.sleep(0.4)
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "tier12b",
                "kind": "tool_use", "tool": "Bash",
                "text": "$ python3 tools/compare_func.py src/eFoo.cpp 0x000120f0"})
    time.sleep(0.3)
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "tier12b",
                "kind": "tool_result", "text": "→ MISMATCH 8/128 bytes (trailing b . nop)"})
    time.sleep(0.3)
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "tier12b",
                "kind": "tool_use", "tool": "Bash",
                "text": "$ python3 tools/permuter.py src/eFoo.cpp 0x000120f0 --time 60"})
    time.sleep(0.4)
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "tier12b",
                "kind": "tool_result",
                "text": "permuter: 432 candidates; best still 8/128 diff"})
    time.sleep(0.3)
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "tier12b",
                "kind": "tool_result",
                "text": "→ FAILED eFoo::~eFoo — trailing b.trap unresolvable"})
    time.sleep(0.5)

    # verify rejects one of the four claimed matches
    emit(path, {"event": "verify_failed", "session_id": s1, "variant": "tier12b",
                "address": "0x000120b0", "name": "eFoo::D(cFile &) const",
                "size": 64, "diff_count": 3, "byte_diffs": []})
    time.sleep(0.3)
    emit(path, {"event": "orch_note",
                "text": "  Orchestrator verification rejected 1 matches"})
    time.sleep(0.3)

    # authoritative function_result (post-verify)
    for addr, name, size, status in [
        ("0x00012000", "eFoo::A(int)", 32, "matched"),
        ("0x00012020", "eFoo::B(void)", 48, "matched"),
        ("0x00012050", "eFoo::C(const mVec3 &)", 96, "matched"),
        ("0x000120b0", "eFoo::D(cFile &) const", 64, "failed"),
        ("0x000120f0", "eFoo::~eFoo(void)", 128, "failed"),
    ]:
        emit(path, {"event": "function_result", "session_id": s1, "variant": "tier12b",
                    "address": addr, "name": name, "size": size, "status": status})
        time.sleep(0.2)

    emit(path, {"event": "session_done", "session_id": s1, "variant": "tier12b",
                "matched": 3, "failed": 2,
                "claimed_matched": 4, "claimed_failed": 1,
                "duration_s": 280})
    time.sleep(1.0)

    emit(path, {"event": "progress_tick",
                "matched_total": 1211, "failed_total": 258,
                "untried_total": 7773, "total": 9242, "elapsed_s": 280})
    time.sleep(1.0)

    # --- session 2: base variant, all 5 matched cleanly --------------
    s2 = "22222222"
    emit(path, {
        "event": "session_start",
        "session_id": s2,
        "variant": "base",
        "class_name": "gcBaz",
        "functions": [
            {"address": "0x00020000", "name": "gcBaz::GetType(void) const", "size": 28,
             "obj_file": "gcAll_psp.obj"},
            {"address": "0x00020020", "name": "gcBaz::Write(cFile &) const", "size": 40,
             "obj_file": "gcAll_psp.obj"},
            {"address": "0x00020050", "name": "gcBaz::New(cMemPool *, cBase *) static",
             "size": 72, "obj_file": "gcAll_psp.obj"},
        ],
    })
    time.sleep(0.5)
    emit(path, {"event": "agent_event", "session_id": s2, "variant": "base",
                "kind": "text", "text": "Trivial pattern — copying from gcFoo template."})
    time.sleep(0.3)
    for addr, name, size in [
        ("0x00020000", "gcBaz::GetType(void) const", 28),
        ("0x00020020", "gcBaz::Write(cFile &) const", 40),
        ("0x00020050", "gcBaz::New(cMemPool *, cBase *) static", 72),
    ]:
        emit(path, {"event": "agent_event", "session_id": s2, "variant": "base",
                    "kind": "tool_use", "tool": "Bash",
                    "text": f"$ python3 tools/compare_func.py src/gcBaz.cpp {addr}"})
        time.sleep(0.3)
        emit(path, {"event": "agent_event", "session_id": s2, "variant": "base",
                    "kind": "tool_result", "text": f"→ MATCH {name.split('(')[0]} ({size}B)"})
        time.sleep(0.2)
        emit(path, {"event": "function_result", "session_id": s2, "variant": "base",
                    "address": addr, "name": name, "size": size, "status": "matched"})
        time.sleep(0.3)
    emit(path, {"event": "session_done", "session_id": s2, "variant": "base",
                "matched": 3, "failed": 0,
                "claimed_matched": 3, "claimed_failed": 0,
                "duration_s": 140})
    time.sleep(0.8)

    # A malformed line for good measure (tests log_tailer's visible-skip behavior)
    with open(path, "a") as f:
        f.write("{not valid json\n")
    time.sleep(0.5)

    emit(path, {"event": "progress_tick",
                "matched_total": 1214, "failed_total": 258,
                "untried_total": 7770, "total": 9242, "elapsed_s": 420})
    time.sleep(1.0)

    # --- run done --------------------------------------------------
    emit(path, {
        "event": "run_done",
        "branch": "overnight/20260421-demo",
        "duration_s": 420,
        "attempted": 8,
        "matched": 6,
        "failed": 2,
        "errors": 0,
    })
    print(f"demo stream complete: {path}")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("path", help="jsonl file to write the demo stream to")
    args = parser.parse_args()
    run(args.path)


if __name__ == "__main__":
    main()
