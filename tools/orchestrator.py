#!/usr/bin/env python3
"""Overnight autonomous matching orchestrator.

Picks functions from the database, prepares context, invokes Claude Code
in headless mode to write and match C source, and tracks results.

Usage:
    python3 tools/orchestrator.py --hours 8                  # 8-hour run
    python3 tools/orchestrator.py --hours 2 --size-max 8     # trivial only
    python3 tools/orchestrator.py --dry-run --limit 3        # test run
    python3 tools/orchestrator.py --hours 8 --class eWorld   # target class
"""

import argparse
import json
import os
import subprocess
import sys
import time
import uuid
from datetime import datetime, timedelta

from common import (DB_PATH, EBOOT_PATH, OBJDUMP,
                    load_db, save_db, filter_functions, build_addr_map)

BATCH_SIZE = 5
MAX_CONSECUTIVE_FAILURES = 5
SESSION_TIMEOUT = 1800
LOGS_DIR = "logs"
SESSION_RESULTS_DIR = "logs/session_results"


def log(msg):
    ts = datetime.now().strftime("%H:%M:%S")
    print(f"[{ts}] {msg}", flush=True)


def log_event(log_path, event):
    event["timestamp"] = datetime.now().isoformat()
    with open(log_path, "a") as f:
        f.write(json.dumps(event) + "\n")


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


def pick_next_batch(functions, args, batch_size=BATCH_SIZE):
    """Select the next batch of functions to attempt.

    Priority:
    1. Untried ≤8 byte functions (trivial, highest success rate)
    2. Untried leaf functions ≤64 bytes
    3. Untried functions in classes with existing matches (context available)
    4. Untried functions by ascending size
    """
    candidates = filter_functions(
        functions,
        status="untried",
        class_name=getattr(args, 'class_name', None),
        name=getattr(args, 'func_name', None),
        obj=getattr(args, 'obj', None),
        size_min=getattr(args, 'size_min', None) if getattr(args, 'size_min', None) is not None else 4,
        size_max=getattr(args, 'size_max', None),
    )

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

    # Group by class when possible for shared context
    batch = []
    if candidates[0].get("class_name"):
        target_class = candidates[0]["class_name"]
        batch = [f for f in candidates if f["class_name"] == target_class][:batch_size]

    # Fill remaining slots from other candidates
    if len(batch) < batch_size:
        batch_addrs = {f["address"] for f in batch}
        for c in candidates:
            if c["address"] not in batch_addrs:
                batch.append(c)
                if len(batch) >= batch_size:
                    break

    return batch


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

    lines = []
    for line in result.stdout.split("\n"):
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
        class_safe = first["class_name"].replace("::", "_")
        return f"src/{class_safe}.cpp"
    return "src/free_functions.c"


def build_prompt(batch, functions, session_id):
    source_file = determine_source_file(batch)
    class_name = batch[0].get("class_name")
    header_content = get_class_header(class_name)
    addr_map = build_addr_map(functions)

    prompt_parts = [
        "You are a PSP decompilation agent working on Days of Thunder. "
        "Your job is to produce C/C++ source that compiles to byte-identical "
        "machine code for each function below.\n\n"
        "Read CLAUDE.md for repo norms and the matching workflow.\n\n"
        "For each function:\n"
        "1. Study the disassembly carefully. Understand the control flow, "
        "register usage, and delay slot filling.\n"
        "2. Read the m2c output as a starting point — it's often close but "
        "rarely perfect.\n"
        "3. Write C/C++ source to the specified file.\n"
        "4. Compile: make build/src/<file>.o\n"
        "5. Compare: python3 tools/compare_func.py src/<file>.cpp\n"
        "6. If MATCH: great, move to next function.\n"
        "7. If MISMATCH: this is where the real work begins. Analyze the "
        "byte diff:\n"
        "   - Different register allocation? Try reordering variable "
        "declarations, splitting/merging expressions.\n"
        "   - Different branch structure? Try if/else vs ternary vs switch. "
        "Try inverting conditions.\n"
        "   - Different instruction scheduling? Try changing expression "
        "evaluation order, adding/removing casts.\n"
        "   - SNC at -O2 fills branch delay slots and sometimes generates "
        "redundant masks (andi x,x,0xff twice for bool casts). Work WITH "
        "the compiler's patterns, not against them.\n"
        "   - Keep iterating. Try radically different approaches if "
        "incremental changes aren't working.\n"
        "   - Only report failure after you've genuinely exhausted your ideas.\n\n",

        f"CRITICAL: When done with ALL functions, write results to this exact file:\n"
        f"  {SESSION_RESULTS_DIR}/{session_id}.json\n\n"
        f"The file must contain a JSON array, one entry per function:\n"
        f'[{{"address": "0x...", "status": "matched|failed", "attempts": N, '
        f'"file": "src/..."}}]\n\n'
        f"This file is how the orchestrator tracks your progress. "
        f"If you don't write it, your work is lost.\n\n",
    ]

    if header_content:
        prompt_parts.append(
            f"== CLASS HEADER: include/{class_name}.h ==\n"
            f"```\n{header_content}\n```\n\n"
        )

    if class_name:
        neighbors = get_matched_neighbors(functions, batch[0])
        if neighbors:
            prompt_parts.append(f"== ALREADY MATCHED in {class_name} ==\n")
            for n in neighbors:
                prompt_parts.append(f"  {n['name']} ({n['size']}B) — matched\n")
            prompt_parts.append("\n")

    func_num = 0
    for func in batch:
        addr = int(func["address"], 16)
        try:
            disasm = disassemble_function(addr, func["size"])
        except RuntimeError as e:
            log(f"  Skipping {func['name']}: disassembly failed — {e}")
            continue

        try:
            m2c = get_m2c_output(func)
        except RuntimeError as e:
            log(f"  WARNING: m2c failed for {func['name']}: {e}")
            m2c = "// m2c unavailable — see orchestrator log"

        func_num += 1

        callees = func.get("callees", [])
        callee_names = []
        for c_addr in callees:
            target = addr_map.get(c_addr)
            callee_names.append(target["name"] if target else f"unknown@{c_addr}")

        prompt_parts.append(f"== FUNCTION {func_num}: {func['name']} ==\n")
        prompt_parts.append(f"Address: {func['address']}, Size: {func['size']} bytes, "
                           f"Obj: {func['obj_file']}\n")
        prompt_parts.append(f"Leaf: {func.get('is_leaf', 'unknown')}")
        if callee_names:
            prompt_parts.append(f", Calls: {', '.join(callee_names)}")
        prompt_parts.append(f"\n\nDisassembly:\n{disasm}\n\n")
        prompt_parts.append(f"m2c output:\n{m2c}\n\n")
        prompt_parts.append(f"Write to: {source_file}\n\n")

    return "".join(prompt_parts)


def run_claude_session(prompt, session_id):
    """Invoke Claude Code in headless mode. Returns (success, error_msg)."""
    cmd = [
        "claude", "-p", prompt,
        "--dangerously-skip-permissions",
        "--output-format", "json",
        "--verbose",
    ]

    try:
        result = subprocess.run(
            cmd, capture_output=True, text=True, timeout=SESSION_TIMEOUT
        )
    except subprocess.TimeoutExpired:
        return False, "session timed out"

    if result.returncode != 0:
        stderr = result.stderr.strip()[:200] if result.stderr else "unknown error"
        return False, f"claude exited {result.returncode}: {stderr}"

    return True, None


def process_session_results(session_id, batch, functions):
    """Read the results file written by Claude and update the database.

    Also reverts any batch functions not mentioned in results back to untried.
    Returns (matched_count, failed_count) or raises on error.
    """
    results_path = os.path.join(SESSION_RESULTS_DIR, f"{session_id}.json")

    if not os.path.exists(results_path):
        raise FileNotFoundError(
            f"Session {session_id} did not produce results file at {results_path}. "
            f"This is a system error — Claude failed to write the expected output."
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

    addr_index = build_addr_map(functions)
    matched = 0
    failed = 0
    reported_addrs = set()
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

        if addr in addr_index:
            reported_addrs.add(addr)
            addr_index[addr]["match_status"] = status
            if status == "matched":
                matched += 1
            else:
                failed += 1

    # Log and revert any batch functions NOT mentioned in results
    for func in batch:
        if func["address"] not in reported_addrs:
            target = addr_index.get(func["address"])
            if target and target["match_status"] == "in_progress":
                log(f"  WARNING: {func['name']} not reported in session results — reverting to untried")
                target["match_status"] = "untried"

    return matched, failed


def print_progress(functions, start_time):
    total = sum(1 for f in functions if f["size"] > 0)
    matched = sum(1 for f in functions if f["match_status"] == "matched")
    failed = sum(1 for f in functions if f["match_status"] == "failed")
    untried = sum(1 for f in functions if f["match_status"] == "untried")
    elapsed = datetime.now() - start_time
    elapsed_str = str(elapsed).split(".")[0]

    log(f"Progress: {matched}/{total} matched ({matched*100/total:.1f}%), "
        f"{failed} failed, {untried} untried | elapsed: {elapsed_str}")


def main():
    parser = argparse.ArgumentParser(description="Overnight autonomous matching orchestrator")
    parser.add_argument("--hours", type=float, default=8, help="Time limit in hours (default: 8)")
    parser.add_argument("--class", dest="class_name", help="Target specific class")
    parser.add_argument("--name", dest="func_name", help="Target functions matching name")
    parser.add_argument("--obj", help="Target specific .obj file")
    parser.add_argument("--size-min", type=int, help="Minimum function size")
    parser.add_argument("--size-max", type=int, help="Maximum function size")
    parser.add_argument("--limit", type=int, help="Max total functions to attempt")
    parser.add_argument("--batch-size", type=int, default=BATCH_SIZE, help="Functions per session")
    parser.add_argument("--dry-run", action="store_true", help="Skip sandbox, for testing")

    args = parser.parse_args()

    os.makedirs(LOGS_DIR, exist_ok=True)
    os.makedirs(SESSION_RESULTS_DIR, exist_ok=True)

    log_path = os.path.join(LOGS_DIR, f"match_{datetime.now().strftime('%Y%m%d_%H%M%S')}.jsonl")
    log(f"Log: {log_path}")

    functions = load_db()
    addr_index = build_addr_map(functions)
    start_time = datetime.now()
    deadline = start_time + timedelta(hours=args.hours)
    total_attempted = 0
    total_matched = 0
    total_failed = 0
    total_errors = 0
    consecutive_failures = 0

    log(f"Starting overnight run: {args.hours}h time limit, deadline {deadline.strftime('%H:%M')}")
    print_progress(functions, start_time)

    while datetime.now() < deadline:
        batch = pick_next_batch(functions, args, batch_size=args.batch_size)
        if not batch:
            log("No more untried functions matching criteria. Done.")
            break

        if args.limit and total_attempted >= args.limit:
            log(f"Reached limit of {args.limit} functions. Done.")
            break

        session_id = str(uuid.uuid4())[:8]
        log(f"Session {session_id}: {len(batch)} functions — "
            f"{', '.join(f['name'].split('(')[0] for f in batch)}")

        # Pre-generate expected .o files
        try:
            for func in batch:
                ensure_expected_o(func)
        except RuntimeError as e:
            log(f"Session {session_id} PREP ERROR: {e}")
            log_event(log_path, {
                "event": "prep_error",
                "session_id": session_id,
                "error": str(e),
            })
            total_errors += 1
            consecutive_failures += 1
            if consecutive_failures >= MAX_CONSECUTIVE_FAILURES:
                log(f"Too many consecutive failures ({consecutive_failures}). Stopping.")
                break
            continue

        # Mark as in_progress
        set_batch_status(functions, batch, "in_progress", addr_index)
        save_db(functions)

        # Build prompt and run Claude
        prompt = build_prompt(batch, functions, session_id)
        session_start = time.time()
        success, error_msg = run_claude_session(prompt, session_id)
        session_duration = time.time() - session_start

        if not success:
            log(f"Session {session_id} FAILED ({session_duration:.0f}s): {error_msg}")
            log_event(log_path, {
                "event": "session_error",
                "session_id": session_id,
                "error": error_msg,
                "functions": [f["address"] for f in batch],
                "duration_s": session_duration,
            })

            revert_in_progress(functions, batch, addr_index)
            save_db(functions)

            total_errors += 1
            consecutive_failures += 1
            if consecutive_failures >= MAX_CONSECUTIVE_FAILURES:
                log(f"Too many consecutive failures ({consecutive_failures}). Stopping.")
                break
            backoff = min(30 * (2 ** consecutive_failures), 960)
            log(f"Backing off {backoff}s...")
            time.sleep(backoff)
            continue

        # Process results
        try:
            matched, failed = process_session_results(session_id, batch, functions)
            total_matched += matched
            total_failed += failed
            total_attempted += len(batch)
            consecutive_failures = 0

            log(f"Session {session_id} done ({session_duration:.0f}s): "
                f"{matched} matched, {failed} failed")

            for func in batch:
                target = addr_index.get(func["address"])
                log_event(log_path, {
                    "event": "function_result",
                    "session_id": session_id,
                    "address": func["address"],
                    "name": func["name"],
                    "size": func["size"],
                    "status": target["match_status"] if target else "unknown",
                })

        except (FileNotFoundError, ValueError) as e:
            log(f"Session {session_id} SYSTEM ERROR: {e}")
            log_event(log_path, {
                "event": "system_error",
                "session_id": session_id,
                "error": str(e),
                "functions": [f["address"] for f in batch],
                "duration_s": session_duration,
            })

            revert_in_progress(functions, batch, addr_index)

            total_errors += 1
            consecutive_failures += 1
            if consecutive_failures >= MAX_CONSECUTIVE_FAILURES:
                log(f"Too many consecutive failures ({consecutive_failures}). Stopping.")
                break
            backoff = min(30 * (2 ** consecutive_failures), 960)
            log(f"Backing off {backoff}s...")
            time.sleep(backoff)

        save_db(functions)
        print_progress(functions, start_time)

    # Final summary
    elapsed = datetime.now() - start_time
    log("")
    log("=== OVERNIGHT RUN COMPLETE ===")
    log(f"Duration: {str(elapsed).split('.')[0]}")
    log(f"Attempted: {total_attempted} functions")
    log(f"Matched: {total_matched}")
    log(f"Failed: {total_failed}")
    log(f"System errors: {total_errors}")
    print_progress(functions, start_time)
    log(f"Full log: {log_path}")


if __name__ == "__main__":
    main()
