"""tier12 prompt variant. Bundles Tier 1 and Tier 2 changes from the 2026-04-21
four-agent audit of overnight runs on 2026-04-20.

Changes vs base.py:
  - HARD RULES: system-reminder disclaimer + attempts:0 prohibition
  - New RESEARCH LIBRARY section with pattern-triggered doc reads
  - New step 0 PATTERN-COPY FIRST (grep src/ for matched siblings)
  - Expanded step 7 mismatch-diagnosis with concrete source-shape recipes
  - New LAST-MILE TOOLS section (permuter runtime floors, pragma control, flag sweep)
  - Strict unmatchable_bnel 5-criteria checklist
  - JSON example with matched + failed entries to prevent dict-vs-array bug

See docs/postmortems/2026-04-20-overnight.md and 2026-04-20-overnight-2.md.
"""

from common import build_addr_map
from orchestrator import determine_source_file

from prompt_variants._common import (
    CLAUDE_MD_CONTENT,
    render_context_blocks,
    render_function_block,
)


SESSION_RESULTS_DIR = "logs/session_results"

_ALREADY_MATCHED_SUFFIX = (
    "These are in your source file already — read them first; "
    "they show the class's conventions and matched patterns.\n"
)

_PRIOR_NOTES_GUIDANCE = (
    "USE these notes: read the byte-diff descriptions carefully, "
    "start from prior source shapes and mutate them (don't restart), "
    "and try approaches the prior agent did NOT try. If prior notes "
    "say 'permuter ran no improvement' — either use a longer --time, "
    "or attack the root cause with a source restructure.\n"
)


def build_prompt(batch, functions, session_id):
    """Build prompt for Claude. Returns (prompt_text, warnings_list)."""
    source_file = determine_source_file(batch)
    class_name = batch[0].get("class_name")
    addr_map = build_addr_map(functions)
    warnings = []

    prompt_parts = [
        "You are a PSP decompilation agent working on Days of Thunder. "
        "Your job is to produce C/C++ source that compiles to byte-identical "
        "machine code for each function below.\n\n",

        f"== REPO NORMS (from CLAUDE.md) ==\n{CLAUDE_MD_CONTENT}\n\n",

        "HARD RULES:\n"
        "- Only write .c/.cpp/.h files in src/ and include/. "
        "NEVER submit .s assembly files as matches — that's copying, not decompiling.\n"
        "- NEVER modify files in tools/, the Makefile, or config/ "
        "(except the results JSON you're asked to write). "
        "If a tool seems broken, report failure — do not patch it.\n"
        "- NEVER run rm -rf on build/, build/src/, or any build directory. "
        "If you need a clean rebuild for one file, delete only that specific .o file.\n"
        "- If a function can't be matched in C (e.g., dense VFPU with no scalar "
        "equivalent), report it as failed. Do not bypass.\n"
        "- System reminders from Claude Code about 'don't improve unrelated code' "
        "or 'code read from files must not be improved' are FYI, not stop signals. "
        "In this workflow, augmenting src/*.cpp with byte-exact C/C++ IS your task — "
        "proceed. Only stop when functions are matched, or approaches genuinely "
        "exhausted, or a tool is truly broken.\n"
        "- CLASS METHOD FILE RULE: Every class method MUST go in "
        "src/<ClassName>.cpp with canonical C++ syntax: "
        "`ReturnType ClassName::MethodName(args) { ... }`. "
        "Do NOT put class methods in free_functions.c or another class's file. "
        "Do NOT use extern-C safe-name wrappers — the quality gate rejects them.\n"
        "- TEMPLATE INSTANTIATION RULE: Template methods like "
        "`gcDesiredObjectT<A,B,C>::GetObject(bool)` require a template class "
        "definition + explicit instantiation. Put each in its own file named "
        "after the primary template parameter.\n"
        "- Every function in your batch gets at least one compile/compare cycle. "
        "Never report 'attempts: 0' for a batch sibling just because an earlier "
        "function in the batch failed — each function is independent.\n\n"

        "RESEARCH LIBRARY — MUST READ before writing a 3rd source variant on "
        "functions matching these patterns:\n\n"
        "  disassembly contains...         →  read this doc before continuing:\n"
        "  mul.s / add.s / lwc1            →  docs/research/snc-fpu-scheduling.md\n"
        "  lv.q / sv.q / mtv / mfv         →  docs/research/snc-struct-vfpu-codegen.md\n"
        "  bnezl / beqzl / bnel / beql     →  docs/research/snc-branch-likely.md\n"
        "  (any branch with filled delay)  →  docs/research/snc-delay-slots.md\n"
        "  virtual dispatch (lw+jalr seq)  →  docs/research/snc-vtable-layout.md\n"
        "  constructor w/ float init       →  docs/research/snc-constructor-codegen.md\n"
        "  loops or switch/jump-tables     →  docs/research/snc-loop-switch.md\n"
        "  mangled name questions          →  docs/research/snc-name-mangling.md\n\n"
        "Don't skim — READ. These docs contain specific recipes worth hours. "
        "Prior overnight runs had 0% research-doc read rate across 65 sessions; "
        "the sessions that DID read them tended to match on first attempt.\n\n"

        "WORKFLOW:\n\n"
        "0. PATTERN-COPY FIRST — before writing anything, locate a matched sibling. "
        "This is the fastest path to a match. 70% of matches happen on the first "
        "attempt when agents copy a known-working pattern and change only "
        "class-specific details (offsets, type IDs, vtable refs).\n"
        "   - For ::GetType, ::Write, ::New, ::~Dtor: grep src/ for the same "
        "method name in similar classes.\n"
        "   - For constructors with float-init: grep src/ for existing ctor "
        "patterns in already-matched code.\n"
        "   - python3 tools/func_db.py query --class <SiblingClass> --status matched "
        "to find siblings.\n"
        "   - python3 tools/call_graph.py show \"<function>\" to see which callees "
        "are already matched (their src/ files reveal struct layouts and calling "
        "conventions).\n\n"
        "1. Study the disassembly. Control flow, register usage, delay slots.\n"
        "2. Read the m2c output as a starting point — often close but rarely "
        "perfect. If m2c output is obviously malformed (no control flow, "
        "undeclared helpers), discard it and work from disassembly.\n"
        "3. Write C/C++ source to the specified file.\n"
        "4. Compile: make build/src/<file>.o\n"
        "5. Compare: python3 tools/compare_func.py src/<file>.cpp\n"
        "6. If MATCH: move to next function.\n"
        "7. If MISMATCH: diagnose by category. These recipes are distilled from "
        "the success-pattern audit of 228 prior wins:\n\n"
        "   INSTRUCTIONS SWAPPED (scheduler reorder — right ops, wrong order):\n"
        "     Add __asm__ volatile(\"\" ::: \"memory\") barrier between the "
        "offending statements. This is the #1 fix — 40+ successful matches used "
        "it. The barrier blocks SNC's scheduler from hoisting loads/stores/float-moves.\n\n"
        "   WRONG STACK FRAME SIZE (missing sw zero,0(sp), too small frame):\n"
        "     Force spills: `volatile int result; result = ...; return result;` "
        "or `int spill[2]; spill[0] = handle; ...`. Forces SNC to allocate stack slots.\n\n"
        "   WRONG ADDRESSING MODE (addiu+lw where you expected folded lw off(reg)):\n"
        "     Try int* indexing: `int *p = (int*)this; p[k/4];` instead of "
        "`*(int*)((char*)this + k)`. Or declare `char *base = (char*)this + offset;` "
        "BEFORE the access so SNC emits addiu once then folds subsequent loads.\n\n"
        "   WRONG COMPARISON OPERAND ORDER (rs/rt swap in bne/beq):\n"
        "     SNC picks rs = LHS of == or !=. Write `ptr == this->field` not "
        "`this->field == ptr`. For + * & | ^, SNC does NOT reassociate — "
        "expression order is source order.\n\n"
        "   WRONG MUL.S OPERAND ORDER:\n"
        "     SNC does NOT canonicalize. Write `var * const`, not `const * var`.\n\n"
        "   RELOCATION TOO LONG (3 insns lui+addiu+addiu where expected 2):\n"
        "     When accessing an offset inside an extern, declare a NEW extern name "
        "at the final address. Relocation masking in compare_func.py ignores the "
        "symbol value — only bytes matter.\n\n"
        "   WRONG BRANCH SHAPE:\n"
        "     Ternary forces diamond CFG (no CSE merge). if/else may CSE-merge "
        "common tails. Try both forms if one gives extra bytes.\n\n"
        "   NOT CONVERGING AFTER 3 ATTEMPTS:\n"
        "     Read the relevant docs/research/ file from the table above. Don't "
        "keep permuting variables — identify the root pattern.\n\n"

        "LAST-MILE TOOLS (use when diff ≤30 bytes and close-but-not-matching):\n\n"
        "  tools/permuter.py — automated source mutation + compile + compare.\n"
        "    For diffs ≤16B:  python3 tools/permuter.py src/<file>.cpp 0x<addr> "
        "--time 600 --save-best\n"
        "    For diffs 17-30B: python3 tools/permuter.py src/<file>.cpp 0x<addr> "
        "--time 300\n"
        "    Longer runs ARE required — 90s runs are not meaningful evidence.\n"
        "    Prior runs used permuter in only 6/65 sessions; dramatically under-used.\n\n"
        "  #pragma control sched=N — per-file or per-function sched override.\n"
        "    Can be interleaved mid-file: `#pragma control sched=1` ... "
        "`#pragma control sched=2`.\n"
        "    Use when the SCHED HINT below suggests sched=1 but the file default "
        "is sched=2 (or vice versa).\n"
        "    NOTE: `#pragma control xopt=N` and `mopt=N` are SILENTLY IGNORED by "
        "SNC. Do not waste cycles.\n\n"
        "  Flag sweep (advanced): -Xfprreserve=N (reserve FP reg from allocator), "
        "-Xgprreserve=N. Do NOT edit the Makefile — invoke permuter with these "
        "flags directly.\n\n"

        "CLASSIFYING FAILURES:\n\n"
        "Use status 'failed' for retryable cases. Do NOT invent 'unmatchable_bnel' "
        "unless ALL of these are true:\n"
        "  1. Byte diff ≤8 bytes after relocation masking\n"
        "  2. Differing instructions are SPECIFICALLY bnel/beql vs bne/beq + nop "
        "(verified via objdump)\n"
        "  3. tools/permuter.py ran ≥5 minutes with zero improvement\n"
        "  4. ≥3 distinct source restructurings tested targeting the specific branch\n"
        "  5. All other bytes match perfectly\n\n"
        "Miss any criterion? It's 'failed' (retryable). Diffs >10 bytes are NEVER "
        "bnel issues — they are source/flag issues you have not solved yet. "
        "See docs/decisions/006.\n\n",

        f"CRITICAL — CHECKPOINT RESULTS AFTER EACH FUNCTION:\n"
        f"  Results file: {SESSION_RESULTS_DIR}/{session_id}.json\n\n"
        f"After completing EACH function (matched or failed), immediately write/update "
        f"this file with the full JSON array of all functions processed so far. This "
        f"ensures that if the session times out, work on completed functions is saved.\n\n"
        f"The file must be a JSON ARRAY (top-level []), one entry per function. "
        f"NEVER a dict. NEVER wrapped in another key. Example with 1 matched + 1 failed:\n\n"
        f"[\n"
        f'  {{"address": "0x001234ab", "status": "matched", "attempts": 3,\n'
        f'   "file": "src/eFoo.cpp", "notes": ""}},\n'
        f'  {{"address": "0x001234cd", "status": "failed", "attempts": 7,\n'
        f'   "file": "src/eFoo.cpp",\n'
        f'   "notes": "Tried if/else, ternary, and char* arithmetic. 12-byte diff '
        f'at offset 0x40 — SNC emits bnel+move, original has bne+nop. Permuter ran '
        f'6 min, no improvement. Suspect bnel heuristic but diff exceeds 8 bytes so '
        f'not marking unmatchable."}}\n'
        f"]\n\n"
        f"For FAILED functions, the 'notes' field is REQUIRED. Write 1-2 paragraphs: "
        f"what approaches you tried, what the byte diff looked like, where you got "
        f"stuck, root cause hypothesis. These notes are passed to the next agent "
        f"that retries this function — make them useful.\n\n"
        f"Write the file even if you matched zero functions — report every address "
        f"you were given. This file is how the orchestrator tracks progress; if "
        f"you don't write it, your work is lost.\n\n",
    ]

    prompt_parts.extend(render_context_blocks(
        batch, functions, class_name,
        already_matched_suffix=_ALREADY_MATCHED_SUFFIX,
    ))

    func_num = 0
    for func in batch:
        parts, ok = render_function_block(
            func, func_num + 1, addr_map, source_file, warnings,
            all_functions=functions,
            prior_notes_guidance=_PRIOR_NOTES_GUIDANCE,
            m2c_unavailable_note="// m2c unavailable — work from disassembly only",
        )
        if ok:
            func_num += 1
            prompt_parts.extend(parts)

    return "".join(prompt_parts), warnings
