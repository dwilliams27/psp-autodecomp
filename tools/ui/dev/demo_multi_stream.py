#!/usr/bin/env python3
"""Multi-agent demo event stream for TUI development.

Emits interleaved events from 2 concurrent Codex sessions at different
effort levels to exercise the Phase 4 tiled layout. Run with:

    python3 tools/ui/dev/demo_multi_stream.py /tmp/multi.jsonl
    python3 tools/ui/app.py --log /tmp/multi.jsonl --replay
"""

import json
import sys
import time
import uuid
from datetime import datetime, timedelta


def emit(path, event):
    event.setdefault("timestamp", datetime.now().isoformat())
    with open(path, "a") as f:
        f.write(json.dumps(event) + "\n")


def main():
    if len(sys.argv) < 2:
        print(f"usage: {sys.argv[0]} <output.jsonl>")
        sys.exit(1)

    path = sys.argv[1]
    open(path, "w").close()  # truncate

    now = datetime.now()
    s1 = uuid.uuid4().hex[:8]
    s2 = uuid.uuid4().hex[:8]
    s3 = uuid.uuid4().hex[:8]  # reuse session after s1 finishes

    # -- run_start --
    emit(path, {
        "event": "run_start",
        "hours": 2.0,
        "start_time": now.isoformat(),
        "deadline": (now + timedelta(hours=2)).isoformat(),
        "variants": ["base"],
        "batch_size": 5,
        "session_timeout": 1800,
        "backend": "codex",
        "model": "gpt-5.5",
        "effort": "low",
        "identity": "codex/gpt-5.5/low",
        "backends": [
            {"name": "codex", "model": "gpt-5.5", "effort": "low",
             "identity": "codex/gpt-5.5/low"},
            {"name": "codex", "model": "gpt-5.5", "effort": "high",
             "identity": "codex/gpt-5.5/high"},
        ],
        "identities": ["codex/gpt-5.5/low", "codex/gpt-5.5/high"],
        "workers": 2,
        "ab_mode": "hybrid",
        "mode": "general",
    })
    time.sleep(0.3)

    emit(path, {"event": "progress_tick", "matched_total": 1300,
                "failed_total": 420, "untried_total": 7300,
                "elapsed_s": 0})
    time.sleep(0.3)

    # -- Session 1 (codex low) starts --
    emit(path, {
        "event": "session_start",
        "session_id": s1, "variant": "base",
        "backend": "codex", "model": "gpt-5.5", "effort": "low",
        "identity": "codex/gpt-5.5/low",
        "queue_kind": "disjoint",
        "class_name": "eShape",
        "functions": [
            {"address": "0x0002ba38", "name": "eShape::Collide(const eMultiSphereShape *)", "size": 36},
            {"address": "0x0002bac8", "name": "eShape::Collide(const eCompoundShape *)", "size": 40},
            {"address": "0x0002bb50", "name": "eShape::Collide(const eMeshShape *)", "size": 48},
        ],
    })
    time.sleep(0.5)

    # -- Session 2 (codex high) starts --
    emit(path, {
        "event": "session_start",
        "session_id": s2, "variant": "base",
        "backend": "codex", "model": "gpt-5.5", "effort": "high",
        "identity": "codex/gpt-5.5/high",
        "queue_kind": "disjoint",
        "class_name": "gcConstant",
        "functions": [
            {"address": "0x0034e4cc", "name": "gcConstant::gcConstant(cBase *)", "size": 56},
            {"address": "0x0034e510", "name": "gcConstant::GetName(char *) const", "size": 32},
            {"address": "0x0034e540", "name": "gcConstant::Write(cWriteBlock &) const", "size": 44},
        ],
    })
    time.sleep(0.3)

    # -- Interleaved agent events --
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "base",
                "backend": "codex",
                "kind": "text", "text": "I'll start by reading the existing eShape source to check for sibling patterns."})
    time.sleep(0.2)
    emit(path, {"event": "agent_event", "session_id": s2, "variant": "base",
                "backend": "codex",
                "kind": "text", "text": "Looking at gcConstant class — should be straightforward constructors."})
    time.sleep(0.3)

    emit(path, {"event": "agent_event", "session_id": s1, "variant": "base",
                "backend": "codex",
                "kind": "tool_use", "tool": "Read", "text": "read src/eShape.cpp"})
    time.sleep(0.2)
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "base",
                "backend": "codex",
                "kind": "tool_result", "text": "\u2192 14 lines, existing Collide stubs"})
    time.sleep(0.3)

    emit(path, {"event": "agent_event", "session_id": s2, "variant": "base",
                "backend": "codex",
                "kind": "tool_use", "tool": "Bash",
                "text": "$ python3 tools/func_db.py query --class gcConstant --status matched"})
    time.sleep(0.3)
    emit(path, {"event": "agent_event", "session_id": s2, "variant": "base",
                "backend": "codex",
                "kind": "tool_result", "text": "\u2192 2 matched siblings found"})
    time.sleep(0.2)

    emit(path, {"event": "agent_event", "session_id": s1, "variant": "base",
                "backend": "codex",
                "kind": "thinking",
                "text": "The Collide methods are thin wrappers that just return 0. I can match all three by adding the same pattern."})
    time.sleep(0.4)

    emit(path, {"event": "agent_event", "session_id": s1, "variant": "base",
                "backend": "codex",
                "kind": "tool_use", "tool": "Bash",
                "text": "$ python3 tools/compare_func.py src/eShape.cpp 0x0002ba38"})
    time.sleep(0.3)
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "base",
                "backend": "codex",
                "kind": "tool_result", "text": "\u2192 MATCH eShape::Collide (36B)"})
    time.sleep(0.2)

    emit(path, {"event": "function_result", "session_id": s1, "variant": "base",
                "backend": "codex", "identity": "codex/gpt-5.5/low",
                "address": "0x0002ba38",
                "name": "eShape::Collide(const eMultiSphereShape *)", "size": 36,
                "status": "matched"})
    time.sleep(0.3)

    emit(path, {"event": "agent_event", "session_id": s2, "variant": "base",
                "backend": "codex",
                "kind": "tool_use", "tool": "Bash",
                "text": "$ python3 tools/compare_func.py src/gcConstant.cpp 0x0034e4cc"})
    time.sleep(0.4)
    emit(path, {"event": "agent_event", "session_id": s2, "variant": "base",
                "backend": "codex",
                "kind": "tool_result", "text": "\u2192 MATCH gcConstant::gcConstant (56B)"})
    time.sleep(0.2)

    emit(path, {"event": "function_result", "session_id": s2, "variant": "base",
                "backend": "codex", "identity": "codex/gpt-5.5/high",
                "address": "0x0034e4cc",
                "name": "gcConstant::gcConstant(cBase *)", "size": 56,
                "status": "matched"})
    time.sleep(0.3)

    # More interleaved work
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "base",
                "backend": "codex",
                "kind": "tool_use", "tool": "Bash",
                "text": "$ python3 tools/compare_func.py src/eShape.cpp 0x0002bac8"})
    time.sleep(0.3)
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "base",
                "backend": "codex",
                "kind": "tool_result", "text": "\u2192 MATCH eShape::Collide (40B)"})

    emit(path, {"event": "function_result", "session_id": s1, "variant": "base",
                "backend": "codex", "identity": "codex/gpt-5.5/low",
                "address": "0x0002bac8",
                "name": "eShape::Collide(const eCompoundShape *)", "size": 40,
                "status": "matched"})
    time.sleep(0.3)

    emit(path, {"event": "agent_event", "session_id": s2, "variant": "base",
                "backend": "codex",
                "kind": "tool_use", "tool": "Bash",
                "text": "$ python3 tools/compare_func.py src/gcConstant.cpp 0x0034e510"})
    time.sleep(0.3)
    emit(path, {"event": "agent_event", "session_id": s2, "variant": "base",
                "backend": "codex",
                "kind": "tool_result", "text": "\u2192 MISMATCH 4/32 bytes"})
    time.sleep(0.2)

    emit(path, {"event": "agent_event", "session_id": s2, "variant": "base",
                "backend": "codex",
                "kind": "thinking", "text": "Off by 4 bytes — likely a const qualifier issue. Let me try adding const to the return."})
    time.sleep(0.5)

    # Session 1 finishes third function
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "base",
                "backend": "codex",
                "kind": "tool_use", "tool": "Bash",
                "text": "$ python3 tools/compare_func.py src/eShape.cpp 0x0002bb50"})
    time.sleep(0.3)
    emit(path, {"event": "agent_event", "session_id": s1, "variant": "base",
                "backend": "codex",
                "kind": "tool_result", "text": "\u2192 MISMATCH 2/48 bytes (bnel divergence)"})
    time.sleep(0.2)
    emit(path, {"event": "function_result", "session_id": s1, "variant": "base",
                "backend": "codex", "identity": "codex/gpt-5.5/low",
                "address": "0x0002bb50",
                "name": "eShape::Collide(const eMeshShape *)", "size": 48,
                "status": "failed"})
    time.sleep(0.3)

    # Session 1 done
    emit(path, {"event": "session_done", "session_id": s1, "variant": "base",
                "backend": "codex", "model": "gpt-5.5", "effort": "low",
                "identity": "codex/gpt-5.5/low",
                "matched": 2, "failed": 1, "duration_s": 180.0})
    time.sleep(0.5)

    # Session 2 finishes remaining
    emit(path, {"event": "agent_event", "session_id": s2, "variant": "base",
                "backend": "codex",
                "kind": "tool_use", "tool": "Bash",
                "text": "$ python3 tools/compare_func.py src/gcConstant.cpp 0x0034e510"})
    time.sleep(0.3)
    emit(path, {"event": "agent_event", "session_id": s2, "variant": "base",
                "backend": "codex",
                "kind": "tool_result", "text": "\u2192 MATCH gcConstant::GetName (32B)"})

    emit(path, {"event": "function_result", "session_id": s2, "variant": "base",
                "backend": "codex", "identity": "codex/gpt-5.5/high",
                "address": "0x0034e510",
                "name": "gcConstant::GetName(char *) const", "size": 32,
                "status": "matched"})
    time.sleep(0.3)

    emit(path, {"event": "agent_event", "session_id": s2, "variant": "base",
                "backend": "codex",
                "kind": "tool_use", "tool": "Bash",
                "text": "$ python3 tools/compare_func.py src/gcConstant.cpp 0x0034e540"})
    time.sleep(0.3)
    emit(path, {"event": "agent_event", "session_id": s2, "variant": "base",
                "backend": "codex",
                "kind": "tool_result", "text": "\u2192 MATCH gcConstant::Write (44B)"})

    emit(path, {"event": "function_result", "session_id": s2, "variant": "base",
                "backend": "codex", "identity": "codex/gpt-5.5/high",
                "address": "0x0034e540",
                "name": "gcConstant::Write(cWriteBlock &) const", "size": 44,
                "status": "matched"})
    time.sleep(0.3)

    # Session 2 done
    emit(path, {"event": "session_done", "session_id": s2, "variant": "base",
                "backend": "codex", "model": "gpt-5.5",
                "identity": "codex/gpt-5.5/high",
                "matched": 3, "failed": 0, "duration_s": 240.0})
    time.sleep(0.5)

    # -- Slot reuse: new session on slot 0 (codex low) --
    emit(path, {
        "event": "session_start",
        "session_id": s3, "variant": "base",
        "backend": "codex", "model": "gpt-5.5", "effort": "low",
        "identity": "codex/gpt-5.5/low",
        "queue_kind": "disjoint",
        "class_name": "eInput",
        "functions": [
            {"address": "0x00045000", "name": "eInput::Reset(void)", "size": 28},
            {"address": "0x00045020", "name": "eInput::ResetIdleTime(void)", "size": 24},
        ],
    })
    time.sleep(0.3)

    emit(path, {"event": "agent_event", "session_id": s3, "variant": "base",
                "backend": "codex",
                "kind": "text", "text": "Both are trivial zero-out functions. Quick matches expected."})
    time.sleep(0.5)

    emit(path, {"event": "agent_event", "session_id": s3, "variant": "base",
                "backend": "codex",
                "kind": "tool_use", "tool": "Bash",
                "text": "$ python3 tools/compare_func.py src/eInput.cpp 0x00045000"})
    time.sleep(0.3)
    emit(path, {"event": "agent_event", "session_id": s3, "variant": "base",
                "backend": "codex",
                "kind": "tool_result", "text": "\u2192 MATCH eInput::Reset (28B)"})

    emit(path, {"event": "function_result", "session_id": s3, "variant": "base",
                "backend": "codex", "identity": "codex/gpt-5.5/low",
                "address": "0x00045000",
                "name": "eInput::Reset(void)", "size": 28,
                "status": "matched"})
    time.sleep(0.3)

    emit(path, {"event": "agent_event", "session_id": s3, "variant": "base",
                "backend": "codex",
                "kind": "tool_use", "tool": "Bash",
                "text": "$ python3 tools/compare_func.py src/eInput.cpp 0x00045020"})
    time.sleep(0.3)
    emit(path, {"event": "agent_event", "session_id": s3, "variant": "base",
                "backend": "codex",
                "kind": "tool_result", "text": "\u2192 MATCH eInput::ResetIdleTime (24B)"})

    emit(path, {"event": "function_result", "session_id": s3, "variant": "base",
                "backend": "codex", "identity": "codex/gpt-5.5/low",
                "address": "0x00045020",
                "name": "eInput::ResetIdleTime(void)", "size": 24,
                "status": "matched"})
    time.sleep(0.3)

    emit(path, {"event": "session_done", "session_id": s3, "variant": "base",
                "backend": "codex", "model": "gpt-5.5", "effort": "low",
                "identity": "codex/gpt-5.5/low",
                "matched": 2, "failed": 0, "duration_s": 60.0})
    time.sleep(0.5)

    emit(path, {"event": "progress_tick", "matched_total": 1307,
                "failed_total": 421, "untried_total": 7293,
                "elapsed_s": 300})
    time.sleep(0.5)

    emit(path, {"event": "run_done", "branch": "overnight/demo",
                "duration_s": 300, "attempted": 8, "matched": 7, "failed": 1,
                "errors": 0})

    print(f"wrote {path}")


if __name__ == "__main__":
    main()
