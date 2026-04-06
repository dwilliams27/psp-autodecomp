# Enhancements

Ideas for improvements and future directions.

---

## 1. ASM Bigram/Trigram RAG for Function Matching

Index matched (successfully decompiled) functions by the bigrams and trigrams of their assembly instructions. Use ColBERT-style per-token embeddings so that retrieval captures fine-grained instruction-sequence similarity rather than collapsing an entire function into a single vector.

**Why this matters:** Different games — and different compilers/optimization levels — produce recurring idioms at the instruction level. A traditional single-vector embedding loses the structural detail that makes two functions "feel" similar to a human reverser (e.g., a specific prologue pattern, a particular loop-unrolling style, a characteristic VFPU sequence). Per-token embeddings preserve that granularity.

**How it helps over time:** As the system matches more functions across more games, the index accumulates compiler- and engine-specific patterns. The system effectively learns the quirks of the toolchain and engine it's working on — SNC's register allocation habits, CodeWarrior's stack frame layout, ViciousEngine's common inline helpers — making retrieval increasingly precise as the corpus grows. A new function that stumps the agent on first sight may closely resemble something already solved in a different game built with the same compiler.

**Key design questions:**
- Granularity of "tokens" — individual instructions, instruction+operand pairs, or basic-block-level chunks?
- How to handle variable register names and immediates (normalize vs. preserve)?
- Index partitioning strategy — per-game, per-compiler, or global with metadata filters?
