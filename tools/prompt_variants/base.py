"""Baseline prompt variant. Reference against which new variants are measured.

Do not modify this file — the prompt text below is the current-production prompt.
Changes go in new variant files (e.g., tier12.py) so both can run side-by-side.
"""

from common import build_addr_map
from orchestrator import determine_source_file

from prompt_variants._common import render_context_blocks, render_function_block


SESSION_RESULTS_DIR = "logs/session_results"


def build_prompt(batch, functions, session_id):
    """Build prompt for Claude. Returns (prompt_text, warnings_list)."""
    source_file = determine_source_file(batch)
    class_name = batch[0].get("class_name")
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

    prompt_parts.extend(render_context_blocks(batch, functions, class_name))

    func_num = 0
    for func in batch:
        parts, ok = render_function_block(
            func, func_num + 1, addr_map, source_file, warnings
        )
        if ok:
            func_num += 1
            prompt_parts.extend(parts)

    return "".join(prompt_parts), warnings
