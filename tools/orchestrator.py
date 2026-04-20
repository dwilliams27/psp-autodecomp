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
import json
import os
import subprocess
import sys
import time
import uuid
from datetime import datetime, timedelta

from common import (DB_PATH, EBOOT_PATH, OBJDUMP, OBJCOPY, NM, CLAUDE,
                    CLAUDE_MODEL, TEXT_FILE_OFFSET,
                    load_db, save_db, filter_functions, build_addr_map,
                    fix_vfpu_disassembly,
                    get_text_relocations, mask_relocation_bytes)

BATCH_SIZE = 5
TARGETS_BATCH_SIZE = 2
MAX_CONSECUTIVE_FAILURES = 5
SESSION_TIMEOUT = 1800
TARGETS_SESSION_TIMEOUT = 5400  # 1.5 hours for larger finetune targets

# Address ranges for sched zone hints in prompts
SCHED1_ZONE_START = 0x06e000
SCHED1_ZONE_END = 0x0bab28
TRANSITION_ZONE_START = 0x040000
TRANSITION_ZONE_END = 0x06e000
LOGS_DIR = "logs"
SESSION_RESULTS_DIR = "logs/session_results"
GIT = "git"


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


def pick_next_batch_targeted(functions, targets, addr_index, batch_size):
    """Select the next batch from a specific targets list.

    Only picks targets that are untried in the database.
    Priority order matches the targets file order (critical first).
    Warns about target addresses not found in the database.
    """
    candidates = []
    for t in targets:
        addr = t["address"]
        func = addr_index.get(addr)
        if not func:
            log(f"  WARNING: target {addr} ({t.get('name', '?')}) not in database — skipping")
            continue
        if func["match_status"] == "untried":
            candidates.append(func)

    if not candidates:
        return []

    return _group_and_fill_batch(candidates, batch_size)


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
        class_safe = first["class_name"].replace("::", "_")
        # Remove any chars that are problematic in filenames
        class_safe = "".join(c for c in class_safe if c.isalnum() or c in "_-")
        return f"src/{class_safe}.cpp"
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


def build_prompt(batch, functions, session_id):
    """Build prompt for Claude. Returns (prompt_text, warnings_list)."""
    source_file = determine_source_file(batch)
    class_name = batch[0].get("class_name")
    header_content = get_class_header(class_name)
    addr_map = build_addr_map(functions)
    warnings = []

    prompt_parts = [
        "You are a PSP decompilation agent working on Days of Thunder. "
        "Your job is to produce C/C++ source that compiles to byte-identical "
        "machine code for each function below.\n\n"
        "Read CLAUDE.md for repo norms and the matching workflow.\n\n"

        "HARD RULES:\n"
        "- Only write .c/.cpp/.h files in src/ and include/. "
        "NEVER submit .s assembly files as matches — that's copying, not decompiling.\n"
        "- NEVER modify files in tools/, the Makefile, or config/ "
        "(except the results JSON you're asked to write). "
        "If a tool seems broken, report failure — do not patch it.\n"
        "- If a function can't be matched in C (e.g., dense VFPU with no scalar "
        "equivalent), report it as failed. Do not bypass.\n\n"

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
        f'"file": "src/...", "notes": "..."}}]\n\n'
        f"For FAILED functions, the 'notes' field is REQUIRED. Write 1-2 paragraphs: "
        f"what approaches you tried, what the byte diff looked like, where you got "
        f"stuck, and what you think the root cause is. These notes are passed to the "
        f"next agent that retries this function — make them useful.\n\n"
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
            warnings.append({"type": "disasm_failed", "address": func["address"],
                             "name": func["name"], "error": str(e)})
            continue

        try:
            m2c = get_m2c_output(func)
        except RuntimeError as e:
            warnings.append({"type": "m2c_failed", "address": func["address"],
                             "name": func["name"], "error": str(e)})
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
        prompt_parts.append("\n")

        sched_hint = get_sched_hint(func)
        if sched_hint:
            prompt_parts.append(f"\n{sched_hint}\n")

        prior_notes = func.get("failure_notes", [])
        if prior_notes:
            prompt_parts.append(
                f"\nPRIOR ATTEMPTS ({len(prior_notes)} failed):\n"
            )
            for note in prior_notes:
                prompt_parts.append(
                    f"  Session {note['session']}: {note['notes']}\n"
                )
            prompt_parts.append(
                "Use these notes to avoid repeating the same approaches. "
                "Try something different.\n"
            )

        prompt_parts.append(f"\nDisassembly:\n{disasm}\n\n")
        prompt_parts.append(f"m2c output:\n{m2c}\n\n")
        prompt_parts.append(f"Write to: {source_file}\n\n")

    return "".join(prompt_parts), warnings


def run_claude_session(prompt, session_id, timeout=SESSION_TIMEOUT):
    """Invoke Claude Code in headless mode. Returns (success, error_msg)."""
    cmd = [
        CLAUDE, "-p", prompt,
        "--model", CLAUDE_MODEL,
        "--dangerously-skip-permissions",
        "--output-format", "json",
        "--verbose",
    ]

    try:
        result = subprocess.run(
            cmd, capture_output=True, text=True, timeout=timeout
        )
    except subprocess.TimeoutExpired:
        return False, f"session timed out after {timeout}s"

    if result.returncode != 0:
        stderr = result.stderr.strip()[:200] if result.stderr else "unknown error"
        return False, f"claude exited {result.returncode}: {stderr}"

    return True, None


def process_session_results(session_id, batch, functions, log_path=None):
    """Read the results file written by Claude and update the database.

    Also reverts any batch functions not mentioned in results back to untried.
    Returns (matched_count, failed_count, results_list) or raises on error.
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
                notes = entry.get("notes", "")
                if not notes:
                    log(f"  WARNING: {addr} reported failed with no notes (agent ignored REQUIRED field)")
                    if log_path:
                        log_event(log_path, {
                            "event": "missing_failure_notes",
                            "session_id": session_id,
                            "address": addr,
                        })
                if notes:
                    if "failure_notes" not in addr_index[addr]:
                        addr_index[addr]["failure_notes"] = []
                    addr_index[addr]["failure_notes"].append({
                        "session": session_id,
                        "notes": notes,
                    })
                    # Keep only the most recent 5 to bound prompt/DB size
                    addr_index[addr]["failure_notes"] = addr_index[addr]["failure_notes"][-5:]

    # Log and revert any batch functions NOT mentioned in results
    for func in batch:
        if func["address"] not in reported_addrs:
            target = addr_index.get(func["address"])
            if target and target["match_status"] == "in_progress":
                log(f"  WARNING: {func['name']} not reported in session results — reverting to untried")
                if log_path:
                    log_event(log_path, {
                        "event": "unreported_function",
                        "session_id": session_id,
                        "address": func["address"],
                        "name": func["name"],
                    })
                target["match_status"] = "untried"

    return matched, failed, results


def git_run(*args):
    """Run a git command. Returns (success, stdout, stderr)."""
    result = subprocess.run(
        [GIT] + list(args),
        capture_output=True, text=True
    )
    return result.returncode == 0, result.stdout.strip(), result.stderr.strip()


def create_overnight_branch():
    """Create and checkout a branch for this overnight run.

    Verifies we're on main with a clean tree first.
    Returns the branch name.
    """
    # Verify no uncommitted changes to tracked files (untracked files are fine)
    ok, staged, _ = git_run("diff", "--cached", "--name-only")
    ok2, unstaged, _ = git_run("diff", "--name-only")
    dirty = (staged + "\n" + unstaged).strip()
    if dirty:
        raise RuntimeError(
            f"Working tree has uncommitted changes — commit or stash before running:\n{dirty[:200]}"
        )

    # Verify we're on main
    ok, current, _ = git_run("branch", "--show-current")
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


def git_commit_batch(session_id, matched_funcs, matched_files):
    """Commit matched source files and updated functions.json after a batch.

    Raises on any git failure — matched work must be persisted.
    """
    if not matched_funcs:
        return

    files_to_commit = set(matched_files)

    # Include any .h files that were created/modified for matched classes
    for f in matched_funcs:
        if f.get("class_name"):
            header = f"include/{f['class_name'].replace('::', '_')}.h"
            if os.path.exists(header):
                files_to_commit.add(header)

    files_to_commit.add(DB_PATH)

    # Stage all files — fail loudly if any are missing or git add fails
    for path in files_to_commit:
        if path == DB_PATH:
            # DB always exists
            pass
        elif not os.path.exists(path):
            raise RuntimeError(
                f"Matched file not found on disk: {path}. "
                f"Claude reported a match but didn't write the source file."
            )
        ok, _, err = git_run("add", "--", path)
        if not ok:
            raise RuntimeError(f"git add failed for {path}: {err}")

    func_names = [f["name"].split("(")[0] for f in matched_funcs]
    msg = f"Match {len(matched_funcs)} functions (session {session_id})\n\n"
    msg += "\n".join(f"  - {name}" for name in func_names)

    # Check if there's actually anything staged before committing
    ok, staged, _ = git_run("diff", "--cached", "--name-only")
    if not staged.strip():
        log(f"  No changes to commit (source files unchanged from prior batch)")
        return

    ok, out, err = git_run("commit", "-m", msg)
    if not ok:
        raise RuntimeError(
            f"git commit failed after matching {len(matched_funcs)} functions: "
            f"{out} {err}"
        )


def verify_match(func, matched_files):
    """Verify a claimed match by comparing compiled bytes against original.

    Uses nm to find the exact symbol in the compiled .o, extracts its bytes,
    applies relocation masking, and compares against the original EBOOT.

    Returns (True, None) if byte-exact match confirmed.
    Returns (False, diff_summary) if bytes genuinely don't match, where
    diff_summary is a list of dicts describing each mismatched word.
    Raises RuntimeError on system/tooling errors.
    """
    import tempfile

    addr = int(func["address"], 16)
    size = func["size"]

    # Read expected bytes from original binary
    with open(EBOOT_PATH, "rb") as f:
        f.seek(addr + TEXT_FILE_OFFSET)
        expected = f.read(size)

    if len(expected) != size:
        raise RuntimeError(
            f"Short read from EBOOT: wanted {size} bytes at offset "
            f"{addr + TEXT_FILE_OFFSET:#x}, got {len(expected)}"
        )

    best_diff = None  # track the closest mismatch for logging

    for src_file in matched_files:
        if not os.path.exists(src_file):
            raise RuntimeError(f"Source file {src_file} does not exist")

        o_path = src_file.replace("src/", "build/src/") + ".o"
        r = subprocess.run(["make", o_path], capture_output=True, text=True)
        if r.returncode != 0:
            raise RuntimeError(f"Compilation failed for {src_file}: {r.stderr.strip()[:200]}")
        if not os.path.exists(o_path):
            raise RuntimeError(f"make succeeded but {o_path} not found")

        # Get symbol offsets via nm
        nm_result = subprocess.run([NM, o_path], capture_output=True, text=True)
        if nm_result.returncode != 0:
            raise RuntimeError(f"nm failed on {o_path}: {nm_result.stderr.strip()}")

        symbols = []
        for line in nm_result.stdout.strip().split("\n"):
            parts = line.split()
            if len(parts) == 3 and parts[1] == "T":
                symbols.append((int(parts[0], 16), parts[2]))
        symbols.sort()

        if not symbols:
            continue  # This .o has no text symbols — try next source file

        # Extract raw .text bytes
        with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as tmp:
            tmp_path = tmp.name
        try:
            r = subprocess.run(
                [OBJCOPY, "-O", "binary", "-j", ".text", o_path, tmp_path],
                capture_output=True, text=True
            )
            if r.returncode != 0:
                raise RuntimeError(f"objcopy failed on {o_path}: {r.stderr.strip()}")
            with open(tmp_path, "rb") as f:
                text_bytes = f.read()
        finally:
            if os.path.exists(tmp_path):
                os.unlink(tmp_path)

        # Get relocations
        relocations = get_text_relocations(o_path)

        # Check each symbol that matches the expected size
        for i, (sym_offset, sym_name) in enumerate(symbols):
            sym_end = symbols[i + 1][0] if i + 1 < len(symbols) else len(text_bytes)
            sym_size = sym_end - sym_offset

            if sym_size < size:
                continue

            chunk = text_bytes[sym_offset:sym_offset + size]
            compiled_masked = chunk
            expected_masked = expected

            # Filter and adjust relocations for this symbol
            func_relocs = [(off - sym_offset, rtype) for off, rtype in relocations
                           if sym_offset <= off < sym_offset + size]
            if func_relocs:
                compiled_masked = mask_relocation_bytes(chunk, func_relocs)
                expected_masked = mask_relocation_bytes(expected, func_relocs)

            if compiled_masked == expected_masked:
                return True, None

            # Build diff summary for the closest match
            diffs = build_byte_diff(compiled_masked, expected_masked, addr)
            if best_diff is None or len(diffs) < len(best_diff):
                best_diff = diffs

    return False, best_diff


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


def build_byte_diff(compiled, expected, base_addr):
    """Build a summary of mismatched 4-byte words between compiled and expected.

    Returns a list of dicts, each describing one mismatched word:
      {"offset": int, "addr": "0x...", "compiled": "0x...", "expected": "0x..."}
    Capped at 8 entries to keep logs concise.
    """
    diffs = []
    total_mismatches = 0
    length = min(len(compiled), len(expected))
    for off in range(0, length - 3, 4):
        c_word = compiled[off:off + 4]
        e_word = expected[off:off + 4]
        if c_word != e_word:
            total_mismatches += 1
            if len(diffs) < 8:
                diffs.append({
                    "offset": off,
                    "addr": f"0x{base_addr + off:08x}",
                    "compiled": f"0x{int.from_bytes(c_word, 'little'):08x}",
                    "expected": f"0x{int.from_bytes(e_word, 'little'):08x}",
                })

    # Note size mismatch if applicable
    if len(compiled) != len(expected):
        diffs.append({
            "offset": -1,
            "addr": "size_mismatch",
            "compiled": f"{len(compiled)}B",
            "expected": f"{len(expected)}B",
        })

    # Mark truncation so log readers know the diff is incomplete
    if total_mismatches > 8:
        diffs.append({
            "offset": -2,
            "addr": "truncated",
            "compiled": f"{total_mismatches} total mismatched words",
            "expected": "only first 8 shown",
        })

    return diffs


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
    parser.add_argument("--batch-size", type=int, default=None,
                        help="Functions per session (default: 2 for --targets, 5 otherwise)")
    parser.add_argument("--targets", type=str,
                        help="Path to targets JSON file (e.g., config/finetune_targets.json). "
                             "Only functions listed in this file will be attempted. "
                             "Uses longer session timeout and smaller batches.")
    parser.add_argument("--dry-run", action="store_true", help="Skip sandbox, for testing")

    args = parser.parse_args()

    # Load targets file if specified
    targets_list = None
    if args.targets:
        targets_list = load_targets_file(args.targets)
        log(f"Loaded {len(targets_list)} targets from {args.targets}")

    # Set defaults based on mode
    if args.batch_size is None:
        args.batch_size = TARGETS_BATCH_SIZE if targets_list else BATCH_SIZE
    session_timeout = TARGETS_SESSION_TIMEOUT if targets_list else SESSION_TIMEOUT

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

    # Create a branch for this run so main stays clean (skip for dry runs)
    if args.dry_run:
        branch = "(dry-run, no branch)"
        log("DRY RUN — no branch created, no Claude sessions, no commits")
    else:
        branch = create_overnight_branch()

    mode_label = f"targeted ({len(targets_list)} targets)" if targets_list else "general pool"
    log(f"Starting overnight run: {args.hours}h time limit, deadline {deadline.strftime('%H:%M')}")
    log(f"Mode: {mode_label}, batch_size={args.batch_size}, session_timeout={session_timeout}s")
    print_progress(functions, start_time)

    while datetime.now() < deadline:
        if targets_list:
            batch = pick_next_batch_targeted(functions, targets_list, addr_index,
                                             batch_size=args.batch_size)
        else:
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
        prompt, prompt_warnings = build_prompt(batch, functions, session_id)
        for w in prompt_warnings:
            log(f"  {w['type'].upper()}: {w['name']} — {w['error']}")
            log_event(log_path, {
                "event": w["type"],
                "session_id": session_id,
                "address": w["address"],
                "name": w["name"],
                "error": w["error"],
            })
        session_start = time.time()
        success, error_msg = run_claude_session(prompt, session_id, timeout=session_timeout)
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
            matched, failed, session_results = process_session_results(session_id, batch, functions, log_path)
            total_matched += matched
            total_failed += failed
            total_attempted += len(batch)
            consecutive_failures = 0

            log(f"Session {session_id} done ({session_duration:.0f}s): "
                f"{matched} matched, {failed} failed")

            matched_funcs = []
            matched_files = set()
            for func in batch:
                target = addr_index.get(func["address"])
                status = target["match_status"] if target else "unknown"
                log_event(log_path, {
                    "event": "function_result",
                    "session_id": session_id,
                    "address": func["address"],
                    "name": func["name"],
                    "size": func["size"],
                    "status": status,
                })
                if status == "matched":
                    matched_funcs.append(func)

            # Collect file paths from results
            for entry in session_results:
                if entry.get("status") == "matched" and entry.get("file"):
                    matched_files.add(entry["file"])

            # Orchestrator verification: independently verify each claimed match
            # by comparing compiled bytes against original (with relocation masking).
            # No name matching needed — pure byte comparison.
            if matched_funcs:
                reverted = 0
                for func in list(matched_funcs):
                    try:
                        verified, diff_summary = verify_match(func, matched_files)
                    except RuntimeError as e:
                        # System/tooling error — not a genuine mismatch
                        log(f"  VERIFY ERROR: {func['name']} — {e}")
                        log_event(log_path, {
                            "event": "verify_error",
                            "session_id": session_id,
                            "address": func["address"],
                            "name": func["name"],
                            "error": str(e),
                        })
                        # Revert to untried (system error, deserves retry)
                        target = addr_index.get(func["address"])
                        if target:
                            target["match_status"] = "untried"
                        matched_funcs.remove(func)
                        reverted += 1
                        total_matched -= 1
                        continue

                    if not verified:
                        diff_count = len(diff_summary) if diff_summary else 0
                        diff_preview = ""
                        if diff_summary:
                            first = diff_summary[0]
                            diff_preview = (f" (first diff at {first['addr']}: "
                                          f"compiled={first['compiled']} "
                                          f"expected={first['expected']}, "
                                          f"{diff_count} total diffs)")
                        log(f"  VERIFY FAILED: {func['name']} — bytes don't match{diff_preview}")
                        log_event(log_path, {
                            "event": "verify_failed",
                            "session_id": session_id,
                            "address": func["address"],
                            "name": func["name"],
                            "size": func["size"],
                            "diff_count": diff_count,
                            "byte_diffs": diff_summary or [],
                        })
                        target = addr_index.get(func["address"])
                        if target:
                            target["match_status"] = "failed"
                        matched_funcs.remove(func)
                        reverted += 1
                        total_matched -= 1
                        total_failed += 1
                if reverted > 0:
                    log(f"  Orchestrator verification rejected {reverted} matches")

            # Source quality gate: reject pure-assembly matches
            if matched_funcs and matched_files:
                asm_rejected = validate_source_quality(matched_files)
                if asm_rejected:
                    log(f"  ASSEMBLY-ONLY REJECTION: {len(asm_rejected)} files are pure asm with no C/C++")
                    for asm_file in asm_rejected:
                        log(f"    Rejected: {asm_file}")
                        log_event(log_path, {
                            "event": "rejected_assembly_only",
                            "session_id": session_id,
                            "file": asm_file,
                            "reason": "File contains only __asm__() blocks — no C/C++ training data",
                        })
                    # Revert match status for functions whose ONLY source is a rejected file
                    for func in list(matched_funcs):
                        func_files = {e["file"] for e in session_results
                                      if e.get("address") == func["address"]
                                      and e.get("file")}
                        if func_files and func_files.issubset(set(asm_rejected)):
                            target = addr_index.get(func["address"])
                            if target:
                                target["match_status"] = "failed"
                            matched_funcs.remove(func)
                            total_matched -= 1
                            total_failed += 1
                            log(f"    Reverted: {func['name']} → failed (assembly-only source)")
                    # Remove rejected files from commit set
                    matched_files -= set(asm_rejected)

            # Auto-commit matched work
            # Git errors are loud but non-fatal — matched work is saved in
            # functions.json and source files on disk regardless of git state
            if matched_funcs:
                save_db(functions)
                try:
                    git_commit_batch(session_id, matched_funcs, matched_files)
                except RuntimeError as e:
                    log(f"  GIT COMMIT ERROR (matching still saved): {e}")
                    log_event(log_path, {
                        "event": "git_error",
                        "session_id": session_id,
                        "error": str(e),
                    })
                    total_errors += 1

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
    log(f"Branch: {branch}")
    log(f"Duration: {str(elapsed).split('.')[0]}")
    log(f"Attempted: {total_attempted} functions")
    log(f"Matched: {total_matched}")
    log(f"Failed: {total_failed}")
    log(f"System errors: {total_errors}")
    print_progress(functions, start_time)
    log(f"Full log: {log_path}")
    log(f"")
    log(f"To review: git log {branch} --oneline")
    log(f"To merge:  git checkout main && git merge {branch}")


if __name__ == "__main__":
    main()
