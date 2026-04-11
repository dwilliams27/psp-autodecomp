# 004: Pre-Fine-Tuning Matching Targets

**Date:** 2026-04-10
**Status:** Active

## Decision

Before generating synthetic training data or fine-tuning the SNC decompilation model, we must match 44 strategically selected functions to close coverage gaps in our compiler flag knowledge and training data diversity.

## Why

Our current 649 matched functions are **86% trivial stubs** (<=32B). This creates two problems:

1. **Flag confidence gaps.** We've verified that `-Xsched`, `-Xmopt`, and `-Xxopt` are the only flags that affect codegen (tested 43 flag variations on stress-test code). But the verification was done on small functions where most flags are irrelevant. We have zero matches above 512B — the range where loop unrolling, forward code motion, and cross-iteration hoisting could reveal unknown pragma-based flag overrides.

2. **Training data bias.** A model trained on 649 examples averaging 27B will learn to produce trivial return stubs, not real game code. We need matches across diverse code patterns (loops, switches, serialization, float math, constructors, factory methods) to generate representative synthetic training data.

## Target Selection

44 functions selected across 3 priority tiers. Full list in `config/finetune_targets.json`.

### Tier 1: Flag Verification (18 critical)

| Category | Count | What it resolves |
|---|---|---|
| eAll_psp.obj transition zone probes | 10 | Exact address where sched switches from 2 to 1 |
| eCollisionConstraint / eInputKeyboard verification | 3 | Confirm sched=1 with non-trivial functions (currently 0 non-trivial matches) |
| mAll_psp.obj (completely unmatched) | 5 | Verify assumed sched=2 for the math library |

### Tier 2: Large Function Coverage (15 high)

| Category | Count | What it resolves |
|---|---|---|
| 500-640B functions across obj files | 6 | Whether -Xunroll or -Xfcm pragma overrides exist |
| sched=1 zone at scale (264-552B) | 4 | sched=1 codegen for complex loops/physics |
| Algorithmic loops (CRC, bitstream) | 2 | Whether bitwise loops need special flags |
| eAll chunk 2 transition zone | 3 | Second sched boundary (addresses 0x1e7000-0x213000) |

### Tier 3: Training Data Diversity (11 medium)

Functions representing distinct code patterns the model needs to learn:
- Loop/iteration (eAudioFader::Update, gcExpressionList::Evaluate)
- Switch/dispatch (gcDoReturn::Evaluate, gcValCaseRange::Evaluate)
- Read/Write serialization (gcDoReturn::Write, gcDoReturn::Read)
- Bool/flag manipulation (eCollisionPair::BodiesSleeping, gcEntity::GetSoundFrequencyOffset)
- Float-heavy math (mBasis::SetQuat, mSphere::CalcEmbeddedContact)
- Network (nwConnection::UpdatePing)

## Selection Criteria

All targets were chosen for:
1. **Spread across address ranges** — especially the 0x040000-0x06e000 transition zone
2. **Size 60-640B** — complex enough for flags to matter, feasible to match in single sessions
3. **Leaf functions preferred** — 25 of 44 are leaf (0 callees), minimizing dependency chains
4. **Classes with existing header knowledge** — reduces decompilation difficulty
5. **Diverse code patterns** — loops, switches, serialization, math, constructors, factories

## Expected Outcomes

After matching these 44 functions:
- All flag confidence levels should reach **90%+** (up from 60-80% for transition zones)
- We'll have matches across **all 8 obj files** (currently mAll_psp.obj has zero)
- Size distribution extends to **640B** (up from current max of ~300B among non-trivial matches)
- Training data covers **8 distinct code pattern categories** (up from mostly stubs)
- If any unknown flag requirements exist, they'll surface during this targeted matching

## Execution

- 40 of 44 targets need `.bin` extraction first (`python3 tools/extract_func.py`)
- Estimated effort: 2-3 overnight runs with the orchestrator targeting `config/finetune_targets.json`
- After completion, update `docs/decisions/003-compiler-flags.md` confidence levels

## Relationship to Fine-Tuning

See `docs/direction/002-snc-decompilation-model.md` for the full pipeline. This matching phase is a prerequisite — it ensures:
1. Synthetic data generator uses correct compiler flags for all code regions
2. Real matched pairs in the training set include diverse code patterns
3. No flag surprises during model evaluation that would require retraining
