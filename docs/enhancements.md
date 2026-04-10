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

## 2. ASM Bigram/Trigram RAG for Function Matching

Index matched (successfully decompiled) functions by the bigrams and trigrams of their assembly instructions. Use ColBERT-style per-token embeddings so that retrieval captures fine-grained instruction-sequence similarity rather than collapsing an entire function into a single vector.

**Why this matters:** Different games — and different compilers/optimization levels — produce recurring idioms at the instruction level. A traditional single-vector embedding loses the structural detail that makes two functions "feel" similar to a human reverser (e.g., a specific prologue pattern, a particular loop-unrolling style, a characteristic VFPU sequence). Per-token embeddings preserve that granularity.

**How it helps over time:** As the system matches more functions across more games, the index accumulates compiler- and engine-specific patterns. The system effectively learns the quirks of the toolchain and engine it's working on — SNC's register allocation habits, CodeWarrior's stack frame layout, ViciousEngine's common inline helpers — making retrieval increasingly precise as the corpus grows. A new function that stumps the agent on first sight may closely resemble something already solved in a different game built with the same compiler.

**Key design questions:**
- Granularity of "tokens" — individual instructions, instruction+operand pairs, or basic-block-level chunks?
- How to handle variable register names and immediates (normalize vs. preserve)?
- Index partitioning strategy — per-game, per-compiler, or global with metadata filters?
