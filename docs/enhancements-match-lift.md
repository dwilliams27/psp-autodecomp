# Match Lift Effort

Post-analysis of overnight runs 20260426-015829, 20260426-215603, 20260427-104523. All items here aim to increase match rate and speed beyond horizontal scaling.

---

## Items

| # | Item | Category | Status | Est. Matches Unlocked |
|---|------|----------|--------|----------------------|
| ML1 | AST-driven permuter mutations | Permuter | **Deprioritized** | ~10-20 (cheaper alternatives first) |
| ML2 | Binary patch pspcor.exe prologue scheduler | Compiler | Scoping | ~100+ |
| ML3 | Fix template instantiation verification | Tooling | **DONE** | ~355 unblocked |
| ML4 | Audit for adjacent verification gaps | Tooling | **DONE** | N/A (preventive) |
| ML5 | Destructor exemplars in agent prompt | Prompt | **DONE** | Prevents ~7/run waste |
| ML6 | Exclude unmatchable intrinsics/SDK functions | Orchestrator | Scoped | Saves ~30+ wasted attempts |
| ML7 | 128-256B function category research | Research | **DONE** | ~493 projected |
| ML8 | Chained permuter mutations (2-3 per candidate) | Permuter | Scoped | ~10-30 |
| ML9 | Matched function corpus as agent exemplars | Prompt | Scoped | +5-15% first-attempt rate |
| ML10 | Cross-class method template library | Prompt | Scoped | ~50-100 |
| ML11 | Static analysis pre-pass for prompt enrichment | Prompt | **Deprioritized** | <1% overall improvement |
| ML12 | Diff-guided agent retries | Orchestrator | Idea | ~10-30 |

---

## ML1. AST-driven permuter mutations

**Status: Deprioritized.** Analysis shows cheaper alternatives capture 70-80% of the value.

**What it is:** Replace the regex-based mutation engine in `tools/mutations.py` with a pycparser/libclang AST-driven approach for more robust source transformations.

**Where AST genuinely helps (regex cannot):**
- **N-way statement permutation**: The current system can only swap adjacent pairs. SNC emits stores in source order, so reordering 5-7 independent field assignments requires trying all permutations — impossible with pair-swaps. This is the strongest argument.
- **Control flow restructuring**: if/else to ternary, early-return to single-return, while to do-while. SNC codegen is highly sensitive to these per `snc-loop-switch.md`. Regex fundamentally cannot do this.
- **Variable inlining/extraction**: Finding single-use variables and inlining them, or extracting repeated subexpressions. Requires understanding scope.
- **Condition inversion**: Swapping if/else branches with the negated condition.

**Where regex is fine (AST is overkill):**
- Memory barrier insertion/removal (string insertion)
- Signedness toggling (lexical substitution)
- Cast insertion/removal (text-level)
- Operand reordering for simple expressions
- Multiply decomposition

**Why deprioritized:**
1. The codebase is simpler than general C — mostly 10-50 line functions, repetitive patterns. Elaborate AST transforms rarely apply because the decompilation agent already handles control flow.
2. pycparser can't handle C++ (classes, namespaces, templates, `extern "C"`). libclang would work but is a heavy dependency requiring matching SNC's C++ dialect.
3. The #1 AST-only transform (N-way permutation) can be approximated without a full AST — the existing `_is_simple_statement` and `_brace_depth_map` can identify independent statements and try random permutations. See ML8.
4. Compile-and-discard is cheap (~50% compile success rate is fine at 10-20 candidates/sec).

**If we do build it:**
- Testable: yes. Property-based tests (random C, mutate, verify it parses), known-transform pairs, round-trip tests. The C subset is narrow enough for high-confidence testing.
- Spec: each mutation takes an AST node, returns a new node or None. Blacklist zones: `__asm__` blocks, `#pragma`, string literals. Test matrix: K mutation types x M known-good pairs + N random round-trips.
- Effort: 3-5 days for a working pipeline (mainly fighting pycparser/libclang integration with SNC's dialect).

**Recommended instead:** ML8 (chained mutations + N-way permutation) captures most value at 5% of the cost.

---

## ML2. Binary patch pspcor.exe prologue scheduler

**Status: Scoping.** This is the single highest-ROI research investment.

**The problem:** 48 confirmed failures at 188B (all `Read(cFile &, cMemPool *)` in sched=2 zone) plus an estimated 50+ failures at other sizes — all caused by the same prologue scheduling divergence. The original compiler interleaves `sw s3,32(sp)` and `li s3,1` into the prologue before a `jal cReadBlock::ctor`; our SNC defers both until after. The function body matches perfectly — only the prologue is wrong.

**What's been tried and failed:**
- All 18 `-Xmopt`/`-Xxopt` flag combinations
- `__asm__` placement before/after cReadBlock (either forces `li s3,1` early but de-interleaves saves, or defers it)
- `#pragma sched=0/1/2` (all produce the wrong ordering for sched=2 functions)
- `register asm("$s3") = 1` / `volatile int result = 1` (wrong codegen)
- Permuter: 9,600+ candidates across multiple sessions, zero improvement

**Why the 4 matching Read functions work:** They're in the sched=1 zone. Sched=1 produces a different (non-interleaved) prologue that happens to match the original.

**Approach:** Same methodology as decision 011 (bnel patch). `docs/decisions/010-pspcor-experiments.md` already mapped `del_slot.c` (the delay-slot filler). The prologue scheduling likely lives in the register allocator or instruction scheduler pass. A surgical binary patch to the scheduling heuristic for callee-save write + immediate-load ordering could fix ~100+ functions at once.

**Research needed:**
1. Map the prologue scheduler in pspcor.exe — which pass decides when to emit `sw sN,off(sp)` relative to argument setup?
2. Compare the scheduling decision for sched=1 vs sched=2 — what parameter controls interleaving?
3. Find the specific branch/comparison that decides "defer `li s3,1` until after jal" vs "emit before jal"
4. Design a patch that changes that decision to match the original compiler

**Alternative approach:** Post-compilation binary rewrite tool. The transformation is mechanical: 5 specific instructions reordered deterministically. A tool could detect this pattern in our compiled .o and patch it. Unlike bnel (which changes instruction encoding), this is pure reordering.

**Files:** `docs/decisions/010-pspcor-experiments.md`, `docs/decisions/011-bnel-compiler-patch-design.md` (for methodology)

---

## ML3. Fix template instantiation verification

**Status: DONE.**

**Root cause:** SNC places template class member function instantiations in `.gnu.linkonce.t.<symbol>` sections, not `.text`. Three interlocking failures:
1. `nm_text_symbols()` filtered for `t == "T"` only, ignoring `W` (weak) symbols
2. `extract_text_section()` used `objcopy -j .text`, missing linkonce sections
3. `get_text_relocations()` only parsed `.rel.text`, missing `.rel.gnu.linkonce.t.*`

**Scope:** 355 template class functions in the database, 0 ever matched. 18+ wasted agent sessions on `gcDesiredObjectT<...>::GetObject` alone across recent runs.

**Fix applied:**
- `tools/common.py`: New `get_all_code_relocations()` parses all `.rel.text` and `.rel.gnu.linkonce.t.*` sections. `get_text_relocations()` delegates to it.
- `tools/byte_match.py`: New `extract_section()` (generic section extractor), `symbols_with_bytes_and_relocs()` handles both T and W symbols with per-symbol relocations. `check_byte_match()` uses the new function.
- `tools/compare_func.py`: Updated to use `symbols_with_bytes_and_relocs()`.
- `tools/permuter.py`: Uses `byte_match.extract_section()` and `byte_match.nm_symbols()` instead of duplicated subprocess calls.

**Verified:** Existing smoke tests pass. `gcDesiredObjectT` functions now verify correctly.

---

## ML4. Audit for adjacent verification gaps

**Status: DONE.** Investigated alongside ML3.

**Additional gaps found (none are currently blocking):**
- **Nested class functions** (e.g., `eAudioPlatform::eStreamThread::ThreadFunc`): The heuristic fails because `class_name` contains `::`, but exact `mangled_symbol` match rescues these. The 1 existing matched nested class function verified correctly.
- **152 functions with corrupted `class_name`** due to `extract_class_method()` splitting on `::` inside parameter lists. Cosmetic — all have `mangled_symbol` set and verify correctly.
- **Operator table** (`byte_match._OP_CODES`): Hand-maintained ~30 operators. Missing operators return `NO_NAMED_SYMBOL` loudly. Not blocking.
- **Nested-class name-check gap**: Documented in `docs/enhancements.md` item 7. The within-file byte-match scope makes name matching unnecessary when only one candidate exists per .o. Not worth fixing until ambiguity fires.

**No new blocking gaps found.**

---

## ML5. Destructor exemplars in agent prompt

**Status: DONE.**

**Problem:** 7 destructor functions across 2 runs were rejected by the `rejected_extern_c_class_method` quality gate — agents wrote `extern "C"` wrappers instead of proper `ClassName::~ClassName()` syntax.

**Fix:** Added explicit destructor syntax example to the CLASS METHOD FILE RULE in all three prompt variants (`base.py`, `tier12.py`, `tier12b.py`): "DESTRUCTORS: use `ClassName::~ClassName() { ... }` — never `extern \"C\" void __0o...` wrappers."

The exemplar system already normalizes destructors to `~dtor` for cross-class matching (via `_extract_method_name`), so matched destructor source is already injected as exemplars when available.

---

## ML6. Exclude unmatchable intrinsics/SDK functions

**Status: Scoped.**

**Why these are unmatchable (by category):**

### Mid-function labels (NOT real functions)
Many "functions" in `gMain_psp.obj` are **labels within larger routines** where debug symbols exported internal labels that splat treated as function boundaries. Assembly proves code flows directly into these labels without any function prologue. Confirmed mid-function labels: `__snmain`, `__default_terminate`, `_array_pointer_not_from_vec_new`, `eh_free_memory`, `_i2b`, `setjmp`, `longjmp`, all 17 SDK wrapper functions (`sceSasInit`, `sceWaveAudioSetFormat`, etc.).

These are **fundamentally unmatchable** — you cannot write a C function that compiles to bytes starting at an arbitrary mid-function point.

### SNC softfloat library
`__make_dp`, `__make_fp`, etc. at 0x001c094c-0x001c27a8 use non-standard register conventions (`$t0`/`$t1` for arguments). Many are also mid-function labels. Cannot be expressed in C.

### Dead epilogue SNC version divergence
`__abort_execution`, `_pure_error_`, `__call_terminate` have unreachable epilogues after infinite loops that our SNC version elides via dead code elimination. The original preserved them. Thoroughly confirmed by multiple agents.

### Zero-size import stubs (206 functions)
0x0036bxxx entries are PSP firmware import table entries (NIDs), resolved at load time. No code to match.

**Recommended exclusion list:**
- All 206 zero-size import stubs
- All 17 SDK wrapper mid-function labels
- All confirmed mid-function labels in gMain_psp.obj (~7+)
- All softfloat library functions with non-standard conventions (~20)
- Functions with confirmed dead-epilogue SNC version divergence (~5)

**Do NOT exclude:** Standard libc/math functions that are already matched (40 in gMain_psp.obj) — these prove some CRT code IS matchable. Game-code functions with dead-epilogue issues should remain `failed` for retry.

**Implementation:** Add a new status `unmatchable_crt` or similar. Update `pick_next_batch()` to filter them out. Requires identifying all affected addresses.

---

## ML7. 128-256B function category research

**Status: DONE.** Complete taxonomy below.

**Total: 2,164 functions** (252 matched, 198 failed, 1,711 untried, 3 unmatchable)

### High opportunity categories (prioritize these)

| Category | Matched | Failed | Untried | Match Rate | Projected Yield |
|----------|---------|--------|---------|-----------|----------------|
| **New** (factory allocation) | 86 | 6 | 154 | **93%** | ~144 |
| **Write** (serialization) | 28 | 8 | 186 | **78%** | ~145 |
| **GetType** (RTTI registration) | 39 | 9 | 168 | **81%** | ~136 |
| **AssignCopy** (deep copy) | 23 | 3 | 77 | **88%** | ~68 |
| **PlatformRead** | 7 | 0 | 12 | **100%** | ~12 |
| **VisitReferences** | 6 | 0 | 10 | **100%** | ~10 |

**Combined: 585 untried functions with ~493 projected matches** from high-yield categories alone.

### Research needed categories

| Category | Matched | Failed | Untried | Match Rate | Issue |
|----------|---------|--------|---------|-----------|-------|
| **Read** | 12 | 83 | 197 | **13%** | Prologue scheduling divergence (see ML2). gcAll_psp.obj Read functions have **0% match rate**. |
| **Destructor** | 7 | 29 | 167 | **19%** | Complex virtual dispatch chains, operator delete with pool lookup. Deeper inheritance = harder. |
| **Getter (Get*)** | 11 | 15 | 179 | **42%** | Mixed bag — physics/geometry getters involve FPU math, string operations, loops. |
| **Constructor** | 8 | 3 | 46 | **73%** | Decent rate but small sample. |

### Likely blocked categories

| Category | Matched | Failed | Untried | Match Rate | Issue |
|----------|---------|--------|---------|-----------|-------|
| **Collide** | 0 | 9 | 27 | **0%** | All in transition zone (0x040000-0x06e000) with unknown sched. Virtual dispatch trampolines. |
| **Free functions** | 0 | 0 | 65 | N/A | 48 are gMain_psp.obj CRT functions. 14 are cAll_psp.obj utilities (untested). |

### Why the match rate drops from 95% (64-128B) to 56% (128-256B)

Three systematic blockers:
1. **Read prologue scheduling** (83 failures, 42% of all failures in this range) — ML2
2. **Destructor chain complexity** (29 failures, 15%) — virtual dispatch, operator delete
3. **Transition zone sched ambiguity** (30+ failures) — neither sched=1 nor sched=2 consistently matches

**Action:** Queue the 585 high-yield untried functions for the next overnight runs. They should match at 80%+ without any tooling changes.

---

## ML8. Chained permuter mutations

**Status: Scoped.**

Two improvements that capture most of ML1's value at 5% of the cost:

### N-way block permutation (~2 hours to implement)
Find contiguous runs of 3-7 simple statements at the same brace depth. Pick a random permutation (not just an adjacent swap). This directly addresses SNC's store-ordering sensitivity — the highest-value mutation the current system lacks. Uses existing `_is_simple_statement` and `_brace_depth_map` infrastructure.

### Multi-mutation candidates (~1 hour to implement)
Currently `mutate()` applies exactly one random mutation per candidate. Allow 2-3 mutations in sequence (with lower probability, e.g., 30% chance of 2nd mutation, 10% of 3rd). This lets the permuter reach states requiring multiple coordinated changes (e.g., reorder declarations AND swap statements).

**Combined effect:** 12x expansion of search neighborhood for N-way, plus combinatorial coverage from chaining. These should be the next permuter improvement before considering ML1.

**Files:** `tools/mutations.py` (`mutate()`, new `permute_block()` function), `tools/permuter.py` (mutation count parameter)

---

## ML9. Matched function corpus as agent exemplars

**Status: Scoped.**

**Clarification:** m2c is a non-intelligent CLI tool (rule-based decompiler). It cannot consume training data or learn from examples. The matched function corpus should be fed to the **agents** (Claude/Codex), not m2c. m2c's output is just a starting point the agent rewrites.

The S1 enhancement (already DONE in `docs/enhancements.md`) already injects one matched exemplar per function via `get_method_exemplar()`. The improvement is selecting *better* and *more* exemplars.

**Design for improved exemplar selection:**

1. **Multiple exemplars per function (top 3 by size similarity):** Currently limited to 1. Showing 3 gives the agent more structural variety to learn from. ~600 extra tokens per function.

2. **Exemplar quality ranking:** Prefer exemplars from the same obj file (same compilation unit = same flags). Then same sched zone. Then closest byte size.

3. **Method archetype templates:** For each method type (Read, Write, New, GetType, AssignCopy, destructor, constructor), extract the canonical source pattern from the 3 best matches. Store as static templates in `config/method_templates/`. Inject the relevant template even when no size-matched exemplar exists.

4. **Failure-note-aware selection:** If a function has prior failure notes mentioning a specific issue (e.g., "scheduling"), prefer exemplars from classes that had similar issues and were eventually matched.

**Files:** `tools/orchestrator.py` (`get_method_exemplar`), `tools/prompt_variants/_common.py` (`render_function_block`), new `config/method_templates/` directory

---

## ML10. Cross-class method template library

**Status: Scoped.** Extension of ML9.

Build a library of canonical method patterns from matched functions. For each method archetype:
- Extract the structure (prologue, field access pattern, control flow, epilogue)
- Parameterize by class name, field offsets, type IDs
- Store as reusable templates

**Method types to template (from ML7 data):**
- `New(cMemPool *, cBase *)` — 93% match rate, AllocRec pattern
- `Write(cFile &) const` — 78% match rate, cWriteBlock + field serialization
- `GetType(void)` — 81% match rate, cType::InitializeType chain
- `AssignCopy(cBase const *)` — 88% match rate, dynamic_cast + field copy
- `Read(cFile &, cMemPool *)` — 13% but sched=1 variants match, document both
- `PlatformRead(cFile &)` — 100% match rate, simple delegation pattern
- `VisitReferences(...)` — 100% match rate, fn(this, flags, member_ptr) chain

**Implementation:** Script that reads matched source files, groups by method name, extracts common structure, generates template files. Orchestrator injects the relevant template when no close exemplar exists.

---

## ML11. Static analysis pre-pass for prompt enrichment

**Status: Deprioritized.** Data does not support high value.

**Evidence from transcript analysis (336 sessions across 4 runs):**
- Only 11.6% of sessions read any research doc (39/336)
- Research doc reads consume 0.48% of tool calls — negligible time
- Sessions that read docs had **68.7% match rate** vs **84.6% for those that didn't** — but this is a confounding variable (harder functions trigger both doc reading and lower match rates)
- The match rate gap between "encountered codegen quirk" vs "didn't" sessions is only **4 percentage points**
- Zero failure notes mention scheduling, FPU, delay slots, or any research-doc topic
- Failures are dominated by Read methods (structural complexity), not codegen quirks that docs address

**Cheaper alternative:** Just inject all 7 codegen docs (17.3 KB, +16% prompt) unconditionally into every prompt. No static analysis needed. The context cost is trivial for a 1M-context model. One-line change in `_common.py`.

**Reconsider when:** Moving to larger functions (>256B) where FPU-heavy, VFPU, and vtable dispatch become common. The current pool is dominated by small serialization methods.

---

## ML12. Diff-guided agent retries

**Status: Idea.**

When a function fails with a small byte diff, parse the diff to identify the specific codegen issue and include that diagnosis in the retry prompt.

**Example:** "Your last attempt had 12 bytes wrong: instructions at offset 0x18-0x24 have `move s1,a1` before `sw s3,32(sp)` but the target has them reversed — this is a declaration-order issue, try reordering your variable declarations."

**How it would work:**
1. On failure, `compare_func.py` already reports the byte diff
2. New tool: `tools/diagnose_diff.py` — takes a compiled .o and expected bytes, runs objdump on both, aligns instruction sequences, classifies the divergence type (scheduling, register allocation, wrong instruction, missing/extra instruction)
3. Orchestrator injects the diagnosis into the retry prompt's `failure_notes`

**Classification categories:**
- **Scheduling**: Same instructions, different order → "try reordering declarations/statements"
- **Register allocation**: Same instructions, different registers → "try reordering variable declarations"
- **Wrong instruction**: Different opcode → "the expression structure is wrong"
- **Size mismatch**: Different function size → "the control flow structure is wrong"

**Estimated impact:** 10-30 additional matches from functions that currently fail on retry because agents repeat the same approaches without understanding why they failed.

**Files:** New `tools/diagnose_diff.py`, updates to `tools/orchestrator.py` (failure handling)
