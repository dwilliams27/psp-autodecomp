# Enhancements

Ideas for improvements and future directions. Ordered roughly by impact.

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

## 4. Pretty overnight console output

Currently `tools/orchestrator.py` and `tools/run_overnight.sh` log plain text to stdout — timestamps and event messages all in one undifferentiated stream. Hard to scan a multi-hour log for what mattered.

**Status (2026-04-21)**: design WIP in `tools/launcher_demos/`. Final chosen layout is `v11_dashboard_with_helix.py` — figlet `autodecomp` header + run-status panel (time left / progress bar / this-run deltas / current variant / current function on the left, braille-rendered single-coil fixed-ends assembly-helix in the middle, variant A/B scoreboard with mini-bars on the right), orchestrator + agent log panels below, recent-outcomes panel at the bottom with size-proportional bars. Dynamically resizes with terminal width. Needs real wiring: today it plays a scripted demo timeline; production needs a tailer that reads `logs/match_*.jsonl` + the live Claude transcript. Other demo variants (v1-v10) are rejected iterations kept as reference.

Changes to make:
- **ANSI color** the per-event lines: green for `matched`, red for `failed`, yellow for `verify_failed` / `session_error` / `system_error`, dim/grey for routine events (batch start, function claim).
- **Highlight match counts** in summary lines (e.g., "session N produced **3 matches**, 2 failed" with the numbers in bold/colored).
- **Live progress bar** for the run as a whole — sessions completed / sessions remaining / time elapsed vs. budget.
- **Per-session summary block** when a session completes, with a 2-3 line digest (functions, match rate, notable diffs) instead of the raw JSONL spam.
- **TTY detection**: only emit color when stdout is a terminal (preserve plain JSONL piping behavior, since `match_*.jsonl` is also written to disk).

Constraints:
- Must not change the JSONL log file format (other tools / post-mortem skill parse it).
- No new dependencies — stick to stdlib `os.isatty()` + bare ANSI escape codes.
- The `run_overnight.sh` wrapper passes stdout through to console; pretty output should work end-to-end without piping through `tee` etc.

Impact: zero on match rate, but big on operator UX during long unattended runs and when reviewing console output post-hoc.

---

## 5. Overnight-run observability gaps

The first deep prompt-audit (2026-04-21) surfaced data-access gaps that made session analysis harder than it should be. Fix these so future audits and A/B experiment analysis go faster.

### Orchestrator logging additions
- **`function_start` / `function_end` events** in `match_*.jsonl` with timestamps, so per-function duration is computable. Currently only session-level timing exists; median session is 1799s but we can't tell if that was 1 function at 25 min + 4 starved, or balanced 6 min each.
- **Timeout-hit tagging**: when a session reaches `SESSION_TIMEOUT`, explicitly emit a `session_timeout` event. Every session hitting 1799s (1s below the 1800s cap) is a red flag that agents never finish naturally, but it's not flagged anywhere today.
- **Per-session summary file** at `logs/session_summary/{sid}.json` written at session end with extracted stats: tool counts (bash/read/write/edit/grep/glob), compare_func iterations per function, research-doc reads, permuter invocations, system-reminder counts, time-to-first-match, time-to-last-match. Auditing a run becomes reading ~30 small structured JSONs instead of parsing ~130 MB of raw transcripts.

### Transcript access
- **`sandbox_setup.sh` should set group-readable permissions** on `/Users/autodecomp/.claude/projects/-Users-dwilliams-proj-psp-autodecomp/`, either via `setfacl` or by putting `autodecomp` and `dwilliams` in a shared group with `g+r` inheritance. Currently every new overnight run writes owner-only files and manual `sudo chmod -R a+r` is needed before every audit.
- **Explicit sid→transcript-UUID index**: orchestrator should emit `logs/session_transcripts.jsonl` at session start with `{sid, transcript_uuid, started_at}`. Today sid→transcript mapping requires grepping inside transcript content, which is brittle (sid can appear in subagent spawns, need to disambiguate by file size).

### Why later, not now
None of these block the A/B testing work or the prompt overhaul — they just make next-round audits ~5× faster. Schedule after we have the A/B harness running and a couple of prompt-change runs to analyze.

---

## 6. ASM Bigram/Trigram RAG for Function Matching

Index matched (successfully decompiled) functions by the bigrams and trigrams of their assembly instructions. Use ColBERT-style per-token embeddings so that retrieval captures fine-grained instruction-sequence similarity rather than collapsing an entire function into a single vector.

**Why this matters:** Different games — and different compilers/optimization levels — produce recurring idioms at the instruction level. A traditional single-vector embedding loses the structural detail that makes two functions "feel" similar to a human reverser (e.g., a specific prologue pattern, a particular loop-unrolling style, a characteristic VFPU sequence). Per-token embeddings preserve that granularity.

**How it helps over time:** As the system matches more functions across more games, the index accumulates compiler- and engine-specific patterns. The system effectively learns the quirks of the toolchain and engine it's working on — SNC's register allocation habits, CodeWarrior's stack frame layout, ViciousEngine's common inline helpers — making retrieval increasingly precise as the corpus grows. A new function that stumps the agent on first sight may closely resemble something already solved in a different game built with the same compiler.

**Key design questions:**
- Granularity of "tokens" — individual instructions, instruction+operand pairs, or basic-block-level chunks?
- How to handle variable register names and immediates (normalize vs. preserve)?
- Index partitioning strategy — per-game, per-compiler, or global with metadata filters?
