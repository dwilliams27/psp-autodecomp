# SNC Decompilation Model

A fine-tuned small model that serves as a specialized tool for the main Claude agent, providing SNC-specific codegen pattern knowledge that no existing model has.

## 2026-04-29 Reassessment

Fine-tuning is still credible, but it should be a bounded research lane rather than the next production blocker. The current matching frontier has moved from "we need more basic examples" to "we need category-specific leverage, compiler/tooling fixes, and better retry guidance." A local fine-tuned model can help there, but only if it is integrated as a narrow specialist with objective evaluation.

Recommendation:

- Do **not** pause match-lift work, compiler research, or targeted matching campaigns for fine-tuning.
- Do build a small evaluation harness before training anything serious.
- Treat the fine-tuned model as a local SNC codegen assistant, not as a replacement for Codex/Claude agents.
- Continue only if it improves measured production metrics: byte-match rate, near-miss retry rate, tokens per match, cost per matched byte, or local top-K candidate success.

Most promising uses:

1. **Pattern oracle.** Given a short SNC/Allegrex instruction sequence, suggest the likely C idiom, source ordering, cast pattern, or temporary shape that emits it.
2. **Diff-guided retry helper.** Given target asm, generated asm, current C, and a byte-diff summary, suggest source edits for common mismatch classes.
3. **Local candidate generator.** Generate several cheap C variants for a small/medium function or narrow mismatch, then let the existing compiler/verifier choose winners.
4. **Exemplar/template assistant.** Pick or summarize matched corpus exemplars for a target method family without bloating the main agent prompt.

Less promising uses:

- Full replacement for the agentic matching loop.
- Blind decompilation of large functions without compile/verify feedback.
- Fixing true compiler divergences such as the known `Read(cFile &, cMemPool *)` prologue scheduler issue.
- Recovering missing class layouts without normal type/header work.

The core research advantage remains strong: this project has the exact production compiler and a byte verifier. That means every model output can be compiled, scored, and folded back into the dataset. The risk is not "fine-tuning cannot work"; the risk is spending time training a general decompiler when the near-term bottleneck is narrower and more mechanical.

## Why

Claude is excellent at reasoning about code but has no knowledge of SNC's specific compilation patterns. Generic decompilation models (LLM4Decompile, etc.) are trained on x86 GCC/Clang output. No one has published an SNC-specific or MIPS/Allegrex-specific model. We'd be building the first.

The main bottleneck in overnight matching is the agent writing C that compiles to the *wrong instruction scheduling* — logic is correct but bytes don't match. A model trained on thousands of SNC (C → asm) pairs would know: "this C pattern produces these exact SNC instructions."

## Architecture

The model is a **tool for Claude, not a replacement**. Claude calls it like:
- "What C expression makes SNC emit `andi $a1, $a1, 0xff; andi $a1, $a1, 0xff`?"
- "Given this 8-instruction sequence, what's the likely C?"
- "Does SNC emit `bnezl` (branch-likely) for this loop pattern at -O2?"

Two possible deployment modes:
1. **Ghidra-refine mode** (like LLM4Decompile-Ref): Ghidra pseudo-C → correct SNC-matching C
2. **Pattern oracle mode**: small assembly snippet → C expression

## State of the Art (April 2026)

Key papers and projects:

- **LLM4Decompile** (arxiv 2403.05286): The pioneer. Ref-22B achieves 64.9% re-executability refining Ghidra output. Trained on 7.2M GCC x86 pairs. Their "Refined" approach (refine Ghidra pseudo-C rather than translate raw asm) gets better results with 10x less data.
- **Nova/Nova+** (ICLR 2025): Hierarchical attention + contrastive learning on optimization levels. Teaches the model that O0/O1/O2/O3 are semantically equivalent.
- **SLaDe** (CGO 2024): Small 200M encoder-decoder, 60% IO accuracy on x86-O0. Custom tokenizer for assembly.
- **RevEng.AI** (2024): 1.1B model beats LLM4Decompile-6.7B. Dataset quality > model size.
- **Decompile-Bench** (NeurIPS 2025): 2M binary-source pairs, current gold-standard dataset.
- **sc2dec** (EMNLP 2024): Self-constructed context — recompile LLM output, use successful pairs as training data. Directly applicable to our pipeline.

**No existing model handles MIPS, Allegrex, or SNC.** All are x86/ARM with GCC/Clang.

## Target Model

Current recommendation: do not lock the target model before the evaluation harness exists. The first experiment should answer whether a local model can improve narrow SNC tasks, not whether it can beat frontier hosted models at full decompilation.

**Candidate: Gemma 4 E4B**
- Fastest practical local iteration target.
- Good first model for harness, dataset, and integration tests.
- Long context is useful for compact asm + C + diff examples.
- Low enough cost to train multiple adapters while refining task format.

**Candidate: Gemma 4 31B Dense**
- Better quality target if the E4B experiment shows signal.
- Dense architecture is simpler for adapter training than MoE.
- Fits the "specialist oracle" role better than full autonomous matching.
- More expensive/slower to train and serve; should be gated by E4B evidence.

**Candidate: Gemma 4 26B A4B MoE**
- Attractive inference speed/quality tradeoff.
- Adapter training may be more finicky than dense models.
- Consider after the harness exists, especially for inference-only oracle experiments.

**Candidate: Qwen3-Coder-30B-A3B** (MoE, 30B total / 3B active)
- Purpose-built code model with 262K context
- QLoRA fine-tuning uses ~17.5GB — fits on 40GB Mac Mini with 22GB headroom
- MLX-native 4-bit quantized versions already on HuggingFace
- Unsloth supports QLoRA fine-tuning

Gemma 4 status as of April 2026:
- Hugging Face lists four Gemma 4 model sizes: E2B, E4B, 31B dense, and 26B A4B MoE.
- The E2B/E4B variants have 128K context; 31B and 26B A4B have 256K context.
- The ecosystem supports local inference through MLX, llama.cpp, mistral.rs, and Transformers, plus fine-tuning with TRL and Unsloth Studio.
- The HF Gemma 4 launch notes report strong coding/reasoning benchmarks for 31B and 26B A4B, but this does not directly predict byte-exact SNC usefulness.

**Pre-training base: LLM4Decompile weights** (if compatible)
- Start from LLM4Decompile-Ref checkpoint if architecturally compatible
- This gives general decompilation knowledge; we specialize on SNC

## Compiler Flag Confidence (as of 2026-04-10)

Comprehensive audit of compiler flags completed. See `docs/decisions/003-compiler-flags.md` for full evidence.

### What is verified

| Claim | Evidence | Confidence |
|---|---|---|
| Optimization level is -O2 | Compiled 5 files at -O0 through -O5; -O0/-O1 produce larger code, -O2/-O3/-O4/-O5 identical. Opt agent tested 10 more. | **99%** |
| Default flags: `-c -O2 -G0 -Xsched=2` | SNC preset table extracted from binary confirms sched=2 at -O2. 641/641 matched functions compile correctly. | **99%** |
| sched=1 for eTextureMap, eBumpOffsetMap, eDynamicMeshMorphTarget | Prologue bytes in binary match sched=1 output, not sched=2. 10 non-trivial functions verified. | **95%** |
| sched=1 for eCollisionConstraint, eInputKeyboard | Assigned based on address range in eAll_psp.obj. Zero non-trivial (>=60B) matches to verify byte-level difference. | **70%** — needs larger function matches |
| Only 3 flags affect codegen: sched, mopt, xopt | 43 flag variations tested on stress-test code (loops, floats, switches, structs, aliasing). 26 flags produce identical bytes at all levels. | **90%** — tested on 2 synthetic + 5 real functions |
| No `-Xunroll` pragma overrides | 641 matches without hitting one. But zero matches above 512B where unroll would matter most. | **80%** — needs large function coverage |
| eAll_psp.obj sched boundary at ~0x040000-0x06e000 | 21 classifiable prologues confirm sched=2 early, sched=1 late. 726 functions in transition zone, only 40 matched (all trivial). | **60%** — transition zone uncharted |

### Coverage gaps (blocks before fine-tuning)

Current matches are **heavily biased toward trivial code**: avg matched function is 27B, 86% of matches are <=32B. The functions where flags matter (>=60B) are only 1% matched (84/8,265).

| Gap | Functions available | Matched | What it would resolve |
|---|---|---|---|
| eAll_psp.obj transition zone | 592 (>=60B) | ~0 non-trivial | Exact sched boundary, new sched=1 classes |
| 200-512B functions (any obj) | 4,317 | 28 (0.6%) | Whether unroll/fcm/constp pragmas exist |
| 513B+ functions | 1,713 | 0 (0%) | Unknown unknowns in complex code |
| mAll_psp.obj | 58 | 0 | Verify sched=2 assumption |
| eCollisionConstraint (>=60B) | 2 | 0 | Verify sched=1 |
| eInputKeyboard (>=60B) | 1 | 0 | Verify sched=1 |

### Pre-fine-tuning matching targets

~50 strategic matches needed (2-3 overnight runs). See `config/finetune_targets.json` when generated. Matching these would raise all confidence levels to 90%+ before generating training data.

## Synthetic Data Generation

Our unique advantage: **we have the exact production compiler and can generate unlimited verified training pairs.**

### Pipeline
```
tools/generate_training_data.py
  1. Generate random C functions (varying patterns: loops, branches, struct access,
     casts, VFPU intrinsics, switch statements, bool params, etc.)
  2. Compile each TWICE with both scheduling modes:
       wibo pspsnc.exe -c -O2 -G0 -Xsched=2 -Iextern/include  (most code)
       wibo pspsnc.exe -c -O2 -G0 -Xsched=1 -Iextern/include  (some engine classes)
     NOTE: The binary has multiple .obj units with different -Xsched flags.
     Most code (gc/c/nw prefix + most e-prefix) uses sched=2. Within
     eAll_psp.obj, some classes use sched=1 (eTextureMap, eBumpOffsetMap,
     eDynamicMeshMorphTarget, eCollisionConstraint, eInputKeyboard).
     See docs/decisions/003-compiler-flags.md for the full analysis.
     Each training pair must be tagged with its sched flag.
  3. Disassemble: mipsel-linux-gnu-objdump -d (+ VFPU fix via rabbitizer)
  4. Output: JSONL of {"assembly": "...", "source": "...", "sched": 1|2, "pattern_tags": [...]}
```

### Why synthetic data works here
- **Full verifiability**: every pair is ground truth (identical compiler, flags, architecture)
- **No distribution mismatch**: synthetic pairs use the actual production compiler with actual flags
- **Controllable coverage**: we can specifically generate patterns we struggle with (branch-likely, bool casts, struct returns, VFPU, switch statements)
- **Unlimited volume**: compiler runs in seconds, can generate 10K+ pairs overnight

### Real data augmentation
- Every overnight match adds another verified (asm, C) pair from the real binary
- These are the highest-value training examples — real game code patterns
- As of 2026-04-29, the corpus is around 3,055 matched functions and 300KB of matched code.
- The corpus is no longer just tiny stubs, but it is still biased toward small functions: matched average size is about 98B, while untried average size is about 579B.
- **Critical gap**: the byte-heavy 257B+ and 1KB+ pools remain underrepresented. Training data must include complex functions and mid-game categories, not only wrappers.
- High-value real examples for training now include `Write`, `GetType`, `New`, `AssignCopy`, constructors/destructors, `VisitReferences`, and failed near-misses with known byte-diff diagnoses.

### Target dataset
- **Phase 0**: Evaluation harness only. Build held-out tasks from real matched functions and known near-misses before training.
- **Phase 1**: 2,000-5,000 synthetic SNC pairs + 500-1,000 real matched pairs for pattern-oracle and diff-retry tasks.
- **Phase 2**: 10,000-20,000 synthetic pairs + 1,500+ real matched pairs, weighted toward real method families and current failure modes.
- **Phase 3**: Self-improving loop (sc2dec approach) — model proposes candidates, the compiler/verifier scores them, successful pairs become training data.
- **Do not train only on final matched source.** Include failed/near-miss attempts with labeled corrections where possible; those are exactly the cases where a local oracle could save hosted-agent time.

## Training Plan

### Hardware
- 40GB Mac Mini (Apple Silicon)
- MLX for native Metal acceleration
- QLoRA: 4-bit base + low-rank adapters

### Approach
0. **Build evaluation harness first**:
   - held-out real matched functions by category and size
   - known failed near-misses with byte-diff labels
   - metrics for compile success, exact byte match, top-K candidate match, and diff improvement
1. **Download base model**: start with Gemma 4 E4B or another small local model for fast iteration; promote to 31B dense only if the harness shows signal.
2. **Prepare training data**: JSONL format matching model's chat template, with separate task formats for oracle, diff-retry, and source-refine modes.
3. **QLoRA/LoRA fine-tune** using MLX/TRL/Unsloth depending on the chosen model and hardware.
4. **Evaluate**: test on held-out SNC pairs, recent failed functions, and category-specific near-miss cases.
5. **Integrate only if useful**: wire into the orchestrator as an optional local tool, not as a mandatory dependency.
6. **Iterate**: add synthetic patterns for failure modes that the harness proves are learnable.

### Training data format
```json
{"messages": [
  {"role": "system", "content": "You are an SNC decompilation expert. Given MIPS assembly from the PSP Allegrex CPU compiled with SNC at -O2, produce the C source code."},
  {"role": "user", "content": "<assembly>"},
  {"role": "assistant", "content": "<c_source>"}
]}
```

### Estimated timeline
- Synthetic data generation script: 1 day
- Generate 5,000 pairs: overnight
- QLoRA fine-tuning: 2-4 hours on Mac Mini
- Evaluation + iteration: 1-2 days
- Integration into orchestrator: 1 day

## Integration with Matching Pipeline

### As diff-guided retry helper
Best first integration target:

```
target asm + generated asm + current C + mismatch summary
  -> local model suggests 3-10 source edits or variants
  -> compiler/verifier scores each variant
  -> successful candidate is kept, failures are logged as training data
```

This is narrower than full decompilation and directly targets the current failure shape: correct logic but wrong source ordering, temporaries, casts, branch form, or register allocation.

### As m2c replacement
Replace or supplement m2c in `tools/decompile_func.py`:
```python
# Current: m2c (generic MIPS decompiler, no SNC knowledge)
# New: SNC model (trained on SNC output patterns)
#   Falls back to m2c if model is unavailable
```

### As pattern oracle
Add to orchestrator prompt:
```
Before writing C, consult the SNC pattern oracle:
  python3 tools/snc_oracle.py "<instruction_sequence>"
This will tell you what C pattern SNC typically generates for
these instructions.
```

### As Ghidra output refiner
```
1. Run Ghidra headless on function → pseudo-C
2. Feed pseudo-C to fine-tuned model → SNC-matching C
3. Compile and verify
```

This should be evaluated later than pattern-oracle/diff-retry mode. It is more ambitious and may duplicate what frontier agents already do well.

## Open Questions

1. **MoE fine-tuning stability**: Qwen3-Coder-30B-A3B is MoE — QLoRA on MoE models is newer and potentially less stable than on dense models. May need to use Gemma 4 31B Dense instead.
2. **Cross-architecture transfer**: Will pre-training on x86 decompilation data (LLM4Decompile/Decompile-Bench) transfer usefully to MIPS/Allegrex? The Refined approach (Ghidra pseudo-C) should transfer better than raw asm.
3. **VFPU patterns**: Can the model learn VFPU intrinsics from synthetic data? Need to generate sufficient VFPU training pairs.
4. **Evaluation metrics**: Re-executability (standard in literature) vs byte-exact match (our requirement). Byte-exact is stricter — model may score well on re-executability but poorly on byte-exact.
5. **Prompt and context shape**: Does the local model work better on raw asm, m2c output, source+diff, or exemplar-conditioned tasks?
6. **Synthetic-to-real transfer**: Which synthetic patterns transfer to real game code, and which overfit to toy functions?
7. **Economic value**: Does the model reduce hosted model tokens/cost enough to justify training and integration time?
8. **Local throughput**: Can local inference generate enough candidate variants per minute to matter inside the compile/verify loop?

## References

- [LLM4Decompile](https://arxiv.org/abs/2403.05286) — arxiv 2403.05286
- [LLM4Decompile GitHub](https://github.com/albertan017/LLM4Decompile)
- [Nova/Nova+](https://arxiv.org/abs/2311.13721v3) — ICLR 2025
- [SLaDe](https://arxiv.org/abs/2305.12520) — CGO 2024
- [Decompile-Bench](https://arxiv.org/abs/2505.12668) — NeurIPS 2025
- [sc2dec](https://aclanthology.org/2024.findings-emnlp.385/) — EMNLP 2024
- [Qwen3-Coder](https://qwenlm.github.io/blog/qwen3-coder/) — Qwen team
- [Gemma 4](https://blog.google/innovation-and-ai/technology/developers-tools/gemma-4/) — Google
- [Gemma 4 launch and model details](https://huggingface.co/blog/gemma4) — Hugging Face, 2026-04-02
- [Gemma 4 E4B model card](https://huggingface.co/google/gemma-4-E4B-it) — Hugging Face / Google DeepMind
- [Gemma 4 TRL fine-tuning on Vertex AI](https://huggingface.co/docs/google-cloud/examples/vertex-ai-notebooks-fine-tune-gemma-4) — Hugging Face
- [Unsloth Gemma 4 fine-tuning](https://unsloth.ai/docs/models/gemma-4/train)
- [MLX fine-tuning guide](https://github.com/ml-explore/mlx-examples/blob/main/lora/README.md)
