# SNC Decompilation Model

A fine-tuned small model that serves as a specialized tool for the main Claude agent, providing SNC-specific codegen pattern knowledge that no existing model has.

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

**Primary: Qwen3-Coder-30B-A3B** (MoE, 30B total / 3B active)
- Purpose-built code model with 262K context
- QLoRA fine-tuning uses ~17.5GB — fits on 40GB Mac Mini with 22GB headroom
- MLX-native 4-bit quantized versions already on HuggingFace
- Unsloth supports QLoRA fine-tuning

**Fallback: Gemma 4 31B Dense**
- Better fine-tuning characteristics (dense > MoE for adapter training)
- 31B dense at 4-bit ≈ 20GB, fits on 40GB Mac Mini
- Unsloth has explicit Gemma 4 fine-tuning support
- LiveCodeBench 80%, strong code understanding

**Pre-training base: LLM4Decompile weights** (if compatible)
- Start from LLM4Decompile-Ref checkpoint if architecturally compatible
- This gives general decompilation knowledge; we specialize on SNC

## Synthetic Data Generation

Our unique advantage: **we have the exact production compiler and can generate unlimited verified training pairs.**

### Pipeline
```
tools/generate_training_data.py
  1. Generate random C functions (varying patterns: loops, branches, struct access,
     casts, VFPU intrinsics, switch statements, bool params, etc.)
  2. Compile each: wibo pspsnc.exe -c -O2 -G0 -Iextern/include
  3. Disassemble: mipsel-linux-gnu-objdump -d (+ VFPU fix via rabbitizer)
  4. Output: JSONL of {"assembly": "...", "source": "...", "pattern_tags": [...]}
```

### Why synthetic data works here
- **Full verifiability**: every pair is ground truth (identical compiler, flags, architecture)
- **No distribution mismatch**: synthetic pairs use the actual production compiler with actual flags
- **Controllable coverage**: we can specifically generate patterns we struggle with (branch-likely, bool casts, struct returns, VFPU, switch statements)
- **Unlimited volume**: compiler runs in seconds, can generate 10K+ pairs overnight

### Real data augmentation
- Every overnight match adds another verified (asm, C) pair from the real binary
- These are the highest-value training examples — real game code patterns
- Target: 500+ real pairs before first fine-tuning (currently at 546 matches)

### Target dataset
- **Phase 1**: 5,000 synthetic SNC pairs + 500 real matched pairs → first fine-tune
- **Phase 2**: 20,000 synthetic + 1,000 real → refined model
- **Phase 3**: Self-improving loop (sc2dec approach) — model decompiles, we verify, successful pairs become training data

## Training Plan

### Hardware
- 40GB Mac Mini (Apple Silicon)
- MLX for native Metal acceleration
- QLoRA: 4-bit base + low-rank adapters

### Approach
1. **Download base model**: `mlx_lm.convert` Qwen3-Coder-30B-A3B to MLX 4-bit format
2. **Prepare training data**: JSONL format matching model's chat template
3. **QLoRA fine-tune**: `mlx_lm.lora --model ./mlx_model --data ./training_data --train`
4. **Evaluate**: test on held-out SNC pairs + failed overnight functions
5. **Iterate**: add more synthetic patterns for failure modes, re-train

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

## Open Questions

1. **MoE fine-tuning stability**: Qwen3-Coder-30B-A3B is MoE — QLoRA on MoE models is newer and potentially less stable than on dense models. May need to use Gemma 4 31B Dense instead.
2. **Cross-architecture transfer**: Will pre-training on x86 decompilation data (LLM4Decompile/Decompile-Bench) transfer usefully to MIPS/Allegrex? The Refined approach (Ghidra pseudo-C) should transfer better than raw asm.
3. **VFPU patterns**: Can the model learn VFPU intrinsics from synthetic data? Need to generate sufficient VFPU training pairs.
4. **Evaluation metrics**: Re-executability (standard in literature) vs byte-exact match (our requirement). Byte-exact is stricter — model may score well on re-executability but poorly on byte-exact.

## References

- [LLM4Decompile](https://arxiv.org/abs/2403.05286) — arxiv 2403.05286
- [LLM4Decompile GitHub](https://github.com/albertan017/LLM4Decompile)
- [Nova/Nova+](https://arxiv.org/abs/2311.13721v3) — ICLR 2025
- [SLaDe](https://arxiv.org/abs/2305.12520) — CGO 2024
- [Decompile-Bench](https://arxiv.org/abs/2505.12668) — NeurIPS 2025
- [sc2dec](https://aclanthology.org/2024.findings-emnlp.385/) — EMNLP 2024
- [Qwen3-Coder](https://qwenlm.github.io/blog/qwen3-coder/) — Qwen team
- [Gemma 4](https://blog.google/innovation-and-ai/technology/developers-tools/gemma-4/) — Google
- [Unsloth Gemma 4 fine-tuning](https://unsloth.ai/docs/models/gemma-4/train)
- [MLX fine-tuning guide](https://github.com/ml-explore/mlx-examples/blob/main/lora/README.md)
