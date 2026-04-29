# Enhancements

Ideas for improvements and future directions. Ordered roughly by impact.

---

## Speed Enhancements (2026-04-26)

Identified after deep analysis of overnight run throughput. Goal: 2-3 week completion for remaining ~7,500 functions. Mode C (worktrees, 4 workers) already proved 5x throughput (93-109 m/hr vs 20 m/hr Mode A). These stack on top.

| # | Enhancement | Priority | Status | Est. Impact |
|---|------------|----------|--------|-------------|
| S1 | Inject matched source exemplars in prompt | P1 | **DONE** | +30-50% first-attempt hit rate |
| S2 | Template-first batch scheduling | P1 | **DONE** | Clears 1,250 easy funcs fast |
| S3 | Mid-session checkpoint writes | P2 | **DONE** | Saves ~15% wasted compute |
| S4 | Reduce default batch size to 3 | P2 | **DONE** | Less timeout risk |
| S5 | Embed CLAUDE.md in agent prompt | P3 | **DONE** | Minor per-session savings |

### S1. Inject matched source exemplars in prompt

**Problem**: Agent prompt lists matched siblings (name + size) but not their source code. Agent wastes 3-5 tool calls to grep/cat and find them. Cross-class exemplars (same method, different class) are not surfaced at all.

**Solution**: In `render_context_blocks()` (`tools/prompt_variants/_common.py`), for each target function's method signature, find the closest-size matched exemplar across all classes and inline its source. ~200 extra tokens per function.

**Data**: 1,250 functions have exact (signature + byte size) matches in `src/`. 2,672 more have same-method-different-size templates.

**Files**: `tools/prompt_variants/_common.py`, `tools/orchestrator.py` (new `get_method_exemplar`)

**Implementation (2026-04-26)**: Added `_extract_method_name()`, `_extract_function_source()`, and `get_method_exemplar()` to orchestrator.py. `render_function_block()` in `_common.py` now accepts `all_functions` param and injects a `MATCHED EXEMPLAR` block per function. All three variants updated to pass `all_functions=functions`.

### S2. Template-first batch scheduling

**Problem**: Orchestrator picks batches semi-randomly by class/size tiers, missing the opportunity to prioritize functions with known-working templates.

**Solution**: Add tier -1 to `pick_next_batch()` priority: functions whose (method_name, byte_size) pair already has a matched exemplar. These 1,250 functions are near-guaranteed copy-paste-rename jobs.

**Files**: `tools/orchestrator.py` — `pick_next_batch()` priority_key

**Implementation (2026-04-26)**: Added `matched_method_sizes` set built from all matched functions. New tier -1 in `priority_key()` fires when `(method_name, byte_size)` already exists in the matched set. Destructors normalized to `~dtor` for cross-class matching.

### S3. Mid-session checkpoint writes

**Problem**: Session timeout (1800s) loses ALL work — no results file written. Two timeouts this run = 1 hour wasted.

**Solution**: Change prompt to instruct agents to write partial results after EACH function, not only at the end.

**Files**: `tools/prompt_variants/base.py`, `tier12.py`, `tier12b.py`

**Implementation (2026-04-26)**: Changed results instruction header from "When done with ALL functions" to "CHECKPOINT RESULTS AFTER EACH FUNCTION" in all three variants. Also modified orchestrator's timeout handler to attempt checkpoint recovery: when a session times out AND a results file exists, it falls through to normal result parsing instead of returning immediately.

### S4. Reduce default batch size to 3

**Problem**: batch=5 means one hard function drags the session. 17min median session, timeouts waste all 5 functions.

**Solution**: `BATCH_SIZE = 5` → `BATCH_SIZE = 3`. Input caching ~100%, prompt overhead negligible.

**Files**: `tools/orchestrator.py` line 188

**Implementation (2026-04-26)**: Changed `BATCH_SIZE = 5` → `BATCH_SIZE = 3`. `TARGETS_BATCH_SIZE = 2` unchanged.

### S5. Embed CLAUDE.md in agent prompt

**Problem**: Every session the agent re-reads CLAUDE.md via a tool call.

**Solution**: Read CLAUDE.md at prompt build time and inline its content. Agent no longer needs to tool-call for it.

**Files**: `tools/prompt_variants/_common.py`, all variant files

**Implementation (2026-04-26)**: Added `_load_claude_md()` and `CLAUDE_MD_CONTENT` constant to `_common.py` (reads CLAUDE.md once at import time). All three variants import it and inject as `== REPO NORMS (from CLAUDE.md) ==` block. Falls back to the old "Read CLAUDE.md" instruction if the file is missing.

---

## 0. Session Notes (carry progress between attempts)

When a function times out or fails, the next session starts from scratch with zero knowledge of what was tried. A per-function notes file would let agents build on previous work:

- After each session, orchestrator saves agent observations to `notes/<address>.md`
- Content: what approaches were tried, which bytes diverged, what the diff looked like
- Next session's prompt includes: "Previous attempts tried X, Y, Z. The instruction at offset 0x14 was wrong because..."
- Orchestrator manages the notes lifecycle (create on first attempt, append on retry, archive on match)

This is the single highest-impact improvement for hard functions that need multiple attempts. Currently, a function that takes 25 minutes and times out loses ALL progress — the next session rediscovers the same dead ends.

---

## 1. SNC-Specific Fine-Tuned Model (synthetic data approach)

We have the SNC compiler and can generate unlimited (C source, SNC assembly) training pairs:
1. Write thousands of random C functions with varying patterns
2. Compile with `wibo pspsnc.exe -c -O2 -G0`
3. Disassemble output → (C, asm) pairs

No one else has a corpus of "SNC at -O2 on Allegrex" codegen patterns. This would be the only dataset in the world for this compiler.

**Use as tool, not replacement for Claude:** Fine-tune a 7B model (CodeLlama/deepseek-coder) via QLoRA as an "SNC pattern oracle" that Claude queries:
- "What C expression produces `andi $a1, $a1, 0xff; andi $a1, $a1, 0xff`?" → "SNC double-masks for `(unsigned char)` cast on bool"
- "What C control flow produces `bnezl` (branch-likely)?" → "SNC emits branch-likely for `do-while` with compound condition at -O2"

**Feasibility:** 40GB Mac Mini can QLoRA a 7B model (~16-20GB). Training: hours to a day. The synthetic data generation is the key innovation — we can produce 10K+ pairs covering every codegen pattern.

**Prerequisite:** More matched functions first. The synthetic pairs validate the model, but it needs enough real matched pairs to verify it's learning the right patterns. Target: build after ~1,000 matched functions.

---

## 2. Revisit m2c as the initial decompiler

m2c is likely the weakest link in the matching pipeline. Most agent time on near-miss functions goes to restructuring m2c's generic output into SNC-idiomatic C. m2c was built for generic MIPS (N64/PS1 target era) and has no knowledge of SNC's quirks: branch-likely patterns, VFPU intrinsics, SNC's specific register allocation preferences, struct-access idioms, etc.

Options to explore:
1. **Fine-tuned SNC decompilation model** (see above / `docs/direction/002-snc-decompilation-model.md`). Long-term answer: replace m2c with a model trained on SNC output patterns.
2. **m2c patches for SNC idioms** — fork `extern/m2c` (already on the `psp-vfpu-passthrough` branch) and add SNC-specific pattern recognition. Lower ceiling than #1 but faster to implement.
3. **Ghidra pseudo-C as an alternative input** — per LLM4Decompile research, refining Ghidra output produces better results than refining raw decompiler output. Could replace or supplement m2c in the pipeline.
4. **Hybrid**: use multiple decompilers (m2c + Ghidra + future SNC model) and have the agent choose the best starting point.

Impact if solved: directly raises the overall match rate ceiling, reduces wasted agent time, and improves training data quality (better-structured C in training pairs).

Not blocked by anything. Can be worked on in parallel with other tasks.

---

## 3. Binary-patch pspcor.exe to fix bnel heuristic divergence

Surgical patch to `pspcor.exe` that adds a sixth check to `del_slot_can_fill` at VA `0x43afbc`, rejecting branch-likely promotion when the delay-slot candidate is a scheduling-filler move with no register overlap with the branch's compare registers. Replicates the original SNC's heuristic per Experiment F's rules.

**Expected impact**: 2-7 additional matches among the failed-40 (confirmed: `gcStateVTableEntry::Set`, `cOutStream::WriteBits`). Low immediate ROI but eliminates a known failure class going forward.

**Not blocking anything.** Defer until source-quality improvements (items #1, #2) plateau and bnel-divergence cases become a meaningful fraction of remaining failures.

Full design + reverse-engineering context + patch bytes in `docs/decisions/011-bnel-compiler-patch-design.md`.

---

## 4. Overnight-run observability: remaining gaps

The live TUI (`tools/ui/app.py`, shipped 2026-04-21) closed the biggest observability gaps — orchestrator now emits `run_start`/`run_done`, `session_start`/`session_done`, `progress_tick`, `orch_note` (every `log()` call teed to jsonl), and `agent_event` entries parsed from Claude's stream-json (tool_use/tool_result/text/thinking). Live-viewing a run is solved; post-hoc audits can now read the jsonl instead of chasing transcripts.

What's still missing for deeper post-hoc analysis:

- **Per-function timing**: `function_start` / `function_end` events so you can tell whether a session ran "1 function for 25 min + 4 starved" vs "balanced 6 min each". The current `agent_event` stream gives rough timing via tool_use timestamps but no explicit start/end bracket per function.
- **Per-session summary file** at `logs/session_summary/<sid>.json` with extracted stats (tool counts, compare_func iterations per function, research-doc reads, permuter invocations, time-to-first-match). The raw jsonl has everything needed to compute these; it's a post-processing step that would make A/B comparison queries one-line.
- **Transcript access perms**: `/Users/autodecomp/.claude/projects/...` is still mode 600. The live stream covers ~90% of what you'd want to see, but direct transcript access is needed for internal Claude state that stream-json doesn't expose (e.g., permission-denial events). Fix via `setfacl` or shared-group setup in `sandbox_setup.sh`.
- **sid → transcript-UUID index**: orchestrator could emit `logs/session_transcripts.jsonl` at each session start with `{sid, transcript_uuid, started_at}`. Today the mapping requires grepping inside transcript content, which is brittle when a sid also appears in subagent spawns.

Schedule after the next few overnight runs surface which of these is actually blocking analysis — `agent_event` may cover more ground than originally estimated.

---

## 5. TUI: additional screens

`tools/ui/` has a screen registry (`App.register` / `App.switch_to`) already plumbed; today only `running` is registered. Candidate next screens:

- **Config**: interactive setup for `--hours`, `--variants`, `--targets`, `--size-max` instead of CLI args. Would let non-CLI users start a run via `python3 tools/ui/app.py --config` and tune parameters live before hitting "start".
- **Historical / replay**: scrub through a past `match_*.jsonl` file with playback speed control. Useful for onboarding and for re-viewing incidents.
- **Postmortem**: compare two runs side-by-side (variant rates, failure categories, session timings). Complements the post-mortem markdown docs with a live view.

Adding screen switching (number keys, Ctrl-Tab) requires a keyboard-input story. Rich's `Live` doesn't own stdin; options are (a) a stdin-reader thread, or (b) migrate to Textual. Keyboard nav is deferred until the second screen is actually wanted — no point choosing the input mechanism in advance.

---

## 6. ASM Bigram/Trigram RAG for Function Matching

Index matched (successfully decompiled) functions by the bigrams and trigrams of their assembly instructions. Use ColBERT-style per-token embeddings so that retrieval captures fine-grained instruction-sequence similarity rather than collapsing an entire function into a single vector.

**Why this matters:** Different games — and different compilers/optimization levels — produce recurring idioms at the instruction level. A traditional single-vector embedding loses the structural detail that makes two functions "feel" similar to a human reverser (e.g., a specific prologue pattern, a particular loop-unrolling style, a characteristic VFPU sequence). Per-token embeddings preserve that granularity.

**How it helps over time:** As the system matches more functions across more games, the index accumulates compiler- and engine-specific patterns. The system effectively learns the quirks of the toolchain and engine it's working on — SNC's register allocation habits, CodeWarrior's stack frame layout, ViciousEngine's common inline helpers — making retrieval increasingly precise as the corpus grows. A new function that stumps the agent on first sight may closely resemble something already solved in a different game built with the same compiler.

**Key design questions:**
- Granularity of "tokens" — individual instructions, instruction+operand pairs, or basic-block-level chunks?
- How to handle variable register names and immediates (normalize vs. preserve)?
- Index partitioning strategy — per-game, per-compiler, or global with metadata filters?

---

## 7. Verification pipeline follow-ups

The 2026-04-21 verification-pipeline cleanup (commits `59c5b41` / `cfee3dd` / `1a106e1`) consolidated the three match-checking tools onto a single `tools/byte_match.check_byte_match`. A few items were flagged during that work but deferred so we could get back to matching. None are blocking, all are worth closing before the next major refactor:

- **`record_match_provenance()` helper.** Both `tools/orchestrator.py` (post-verify) and `tools/compare_func.py` (`update_matched`) write `src_file` + `symbol_name` onto the DB entry, but each can write a different path shape (`src/foo.cpp` from the orchestrator via session_results; whatever the agent typed from compare_func's CLI). `tools/verify_matches.py` tolerates both via `os.path.exists`, but a single helper in `byte_match.py` that normalizes (e.g., always repo-relative, always `src/...`) would eliminate the latent drift. Small diff — 20 lines plus two call-site updates.

- **Migrate `tools/permuter.py` off its own compile/nm/objcopy helpers.** Per the pre-commit-review, permuter has its own `compile_source` (via direct WIBO+SNC call, not `make`), its own `extract_text_section` / `get_symbol_bytes` / `score_bytes`. Semantically similar to `byte_match`'s helpers but bypasses `make` for throughput. Two options: (a) add a `compile_src(..., bypass_make=True)` variant in `byte_match` that drops into the raw compiler call permuter needs; (b) keep permuter separate but share the post-compile nm/objcopy/compare helpers. Either way drops ~100 lines of permuter and gives it the same relocation-masking + reason-code semantics as the main path.

- **`record_match_provenance()` inverse — backfill older DB entries from `logs/session_results/*.json`.** The migration (`tools/migrations/backfill_match_schema.py`) runs successfully today but flipped 11 entries to `failed` for lack of any session_results record (these are pre-session-results matches). Those might still be recoverable by scanning `asm/` or splat output. Low priority.

- **`tools/func_db.py` query subcommand should surface `src_file` / `symbol_name`.** Today it prints status but not provenance, so agents can't easily answer "which src file contains the already-matched `cFoo::Bar`?" without grep'ing session_results.

- **Nested-class name-check gap (mitigated, not eliminated).** `byte_match.sym_encodes_func` uses SNC's non-nested mangling rule (`<class><methodLen><method>` with 1-3 char gap). For nested classes SNC uses the `5`-prefix scope form (see `docs/research/snc-name-mangling.md` §5.1). The current checker passes nested-class matches through *because the within-file byte-match scope makes name matching unnecessary when only one candidate exists per .o*. That's robust in practice but masks a real gap: if a .o ever contains byte-identical nested-class overloads we'd pick the wrong one. A proper forward-mangler in `byte_match` (~200 lines Python) closes this. Not worth it until we see the ambiguity fire.

- **Operator table is hand-maintained.** `byte_match._OP_CODES` covers ~30 operators from the mangling doc's §6.1. Any operator we miss returns `NO_NAMED_SYMBOL`, which is loud — but also destructive under `--fix`. Audit any newly-matched operator before running `--fix`.

---

## 8. Build-must-stay-green invariant at commit time

On 2026-04-21 we discovered 10 src files committed into the repo in a broken-compile state, ~23 DB entries left stranded with bogus provenance, and a verification pipeline that had been silently masking the damage for weeks. Root-cause analysis surfaced three distinct ways this drift gets in:

1. **Per-function verify ≠ per-file verify.** Today's `git_commit_batch` commits whole src files, but `check_byte_match` only verifies the specific functions the session claimed. An agent that correctly decompiles `cFile::AddDependency` and simultaneously breaks `cFile::Close` gets both committed — only the former went through byte verification.

2. **Header drift invalidates downstream src silently.** Session A matches `cFile.cpp` against a header that has `cFileSystem::Read`. Session B later trims that header without touching `cFile.cpp`. No agent ever sees the mismatch because no one recompiles `cFile.cpp` after the header edit.

3. **Header changes commit separately from dependent src.** Agents sometimes modify `include/*.h` during matching without the orchestrator's commit flow treating the header edits as part of the match. Downstream breakage is invisible until someone tries the next session on an affected class.

Prevention ideas, any of which close a subset of the drift:

- **`git_commit_batch` should compile every src file it's about to commit, end-to-end, not just the files it claims matches in.** A single `make build/src/<f>.o` per touched src before staging. On failure, don't commit — emit a loud error event and leave the session's work unstaged for operator review. Cost: ~30s per session. Closes case 1.

- **Pre-session build check: refuse to start a session if the current tree doesn't fully compile.** The orchestrator could keep a `last_known_green` commit sha and compare against it before picking a batch. If any file in the current tree fails to compile, block new sessions until it's fixed (or auto-revert to `last_known_green`). Closes cases 1, 2, 3 prospectively.

- **Periodic full-build audit.** `tools/verify_matches.py` already re-compiles all matched src as part of the audit. Run it as a cron (or every N sessions) and alert on any new compile failure. Closes cases 2, 3 retroactively.

- **Header-change reachability check.** When an agent writes `include/*.h`, the orchestrator could re-compile every src file that `#include`s that header. Catches case 3 at the moment of damage. Most expensive (could be 30-100 src files per header touch); defer until cheaper options prove insufficient.

- **Session-level src-file locking.** If session A is currently working on `cFile.cpp`, session B (potentially modifying headers `cFile` depends on) can't run concurrently. Prevents one class of race. Low value solo; only worthwhile as part of a broader concurrency model.

Recommend: ship the first two (commit-time compile check + pre-session green-build gate) together — they close the most common drift paths for ~1-2 hours of work. Ship the periodic audit as a cron once the others are in so we have a safety net for anything the gates missed.

## 9. Retire `SYSTEM_PROMPT_APPEND` once upstream safety classifier stabilizes

The orchestrator currently injects a project-specific framing (`tools/orchestrator.py:SYSTEM_PROMPT_APPEND`) explaining that the inline asm, mangled symbols, and hex constants in src/ and include/ are legitimate decompilation artifacts, not malware. Without it Claude's safety classifier misfires mid-session and refuses to edit src/ files, citing "malware analysis" — which doesn't apply.

**Status as of 2026-04-24**: the framing is doing its job. Looking at `logs/match_20260422_122826.jsonl` and similar, the agent receives a stream of upstream system-reminders flagging the content as malware-adjacent, then correctly identifies them as misfires and continues the matching task. No refusals land; the orchestrator's `MAX_CONSECUTIVE_REFUSALS` circuit-breaker hasn't tripped in any recent run.

But the agent's narrative output is now ~30+ "acknowledging the misfiring reminder, continuing" lines per session. That's noise — visible in the `agent activity` panel of the TUI — and a tax on the prompt context. Suspicion is that the current Claude model (Opus 4.7) is over-tuned on this content class; it's plausible Anthropic ships a fix that re-tunes the classifier.

**Action**: keep the framing for now. Periodically (e.g., every Claude major release) try a smoke run with `SYSTEM_PROMPT_APPEND = ""` on a small batch of src/ that contains inline asm, mangled symbols, and hex constants. If the agent doesn't refuse and the narrative panel doesn't fill with classifier-acknowledgments, drop the framing entirely. Until then it's a free win even if noisy.

The Codex backend doesn't have this issue at all (different classifier, different misfire surface) — useful negative control.

## 10. Confirm Claude usage-limit normalization

The backend layer now has generic rate-limit detection for agent text, tool-result text, raw lines, and stderr. That should catch Claude too **if** the Claude CLI surfaces account/usage limits with ordinary phrases like "usage limit", "rate limit", "too many requests", "quota exceeded", or `429`.

Gap: we only validated the concrete reset-time behavior against Codex logs (`try again at 2:35 PM`). We do not yet have a captured Claude usage-limit stream shape. Claude may emit limits as a `result` subtype, a structured error field, or stderr text that our parser does not preserve fully.

Action: the next time Claude hits a limit, save the surrounding `logs/match_*.jsonl` rows and the CLI stderr tail, then add a unit fixture that proves `AgentRateLimited` fires with a parsed retry time or a conservative fallback pause. This is hardening, not a blocker for Codex-only runs.
