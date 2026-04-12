# 005: SNC Permuter — Last-Mile Matching Tool

**Date:** 2026-04-12
**Status:** Approved, not yet implemented

## Decision

Build a custom SNC-aware source permuter (`tools/permuter.py`) that mechanically closes last-mile byte gaps by systematically mutating C source and comparing compiled output against EBOOT bytes. Designed for near-miss functions where agents get the structure right but can't nail the exact instruction ordering or register allocation.

## Why

25/44 finetune targets matched across 3 overnight runs. The remaining failures include 5 near-misses where agents identified exact byte diffs (97.7% match, 4B short, 8B short, etc.) but couldn't close the gap manually after multiple attempts. Root causes are mechanical — instruction scheduling swaps, register allocation differences, branch-likely optimizations — not algorithmic misunderstandings.

No existing permuter works with SNC. decomp-permuter targets GCC/IDO and produces fundamentally different codegen. We need our own.

## SNC Codegen Sensitivities (experimentally verified)

**Transformations that change output (the search space):**
1. Expression operand order — `a+b` vs `b+a` (SNC does NOT reassociate)
2. Expression association — `a+b+c` vs `a+(b+c)` vs `(a+c)+b` (all different bytes)
3. Statement order — stores/assignments emitted in source order
4. Variable declaration order — directly controls register numbering
5. Compound condition order — `a && b` checks `a` first; `b && a` checks `b` first
6. Early return vs single-return variable — completely different branch structure
7. Memory barriers — `__asm__ volatile("" ::: "memory")` prevents scheduling across
8. Signed/unsigned types — affects loads (lb vs lbu), shifts (sra vs srl), compares (slt vs sltu)
9. Cast placement — `(float)a + (float)b` vs `(float)(a + b)` (different code)
10. Switch vs if-chain — different branch structure
11. Compiler flags — `-Xmopt=0`, `-Xsched=1/2` (small set, easily enumerated)

**Transformations that are NO-OPS (skip these):**
- Ternary vs if/else (identical under SNC)
- Temp variable vs inline (identical for simple cases)
- For vs while loops (identical)
- Pre/post increment (identical)
- Pointer arithmetic forms (identical)
- Constant folding forms (identical)

## Near-Miss Root Causes

| Pattern | Count | Example | Fix |
|---|---|---|---|
| Instruction reordering | 24 insns | eTextureMap::Read — 2 insns swapped | Statement reorder, asm barriers |
| Register allocation | 14 insns | gcDoChangeState::New — v4 vs v5 | Declaration reorder, intermediate vars |
| Branch-likely transform | 3-4 funcs | gcExpressionList::Evaluate — bne vs bnel | Control flow restructure, -Xmopt=0 |
| CSE/constant merge | 1 func | eDynamicGeom ctor — -1 loads merged | -Xmopt=0, expression variation |

## Design

### Architecture

```
Source C file → Mutation Engine → N parallel workers (compile+compare) → Track best
```

Two files: `tools/permuter.py` (main tool, ~400 lines) and `tools/mutations.py` (transformation engine, ~300 lines). No external dependencies beyond existing toolchain (SNC, wibo, objdump, EBOOT).

### Mutation Engine

Priority-ordered mutations based on experimentally verified SNC sensitivities:

**High value (always try):** swap adjacent statements, reorder operands, reassociate expressions, reorder declarations, insert/remove asm memory barriers.

**Medium value (try often):** toggle signed/unsigned types, split/merge expressions into temp variables, restructure control flow (if/else ↔ goto ↔ early return), swap condition order in compound booleans.

**Low value (try occasionally):** compiler flag variations (-Xmopt=0, -Xsched=1/2), cast insertion/removal, multiply decomposition (x*3 ↔ (x<<1)+x ↔ x+x+x).

Each mutation uses lightweight C tokenization (not full AST), applies one random transformation, returns modified source string.

### Scoring

Pure byte diff count against EBOOT — NOT asm-differ's penalty system. Rationale: asm-differ's tiered penalties (stack=1, regalloc=1, reorder=10, insert=20) guide humans toward impactful manual changes. For automated search, raw byte diff count directly measures what we care about, skips the objdump disassembly step, and is ~10x faster to compute.

### Parallelism

`multiprocessing.Pool` with N workers (default: CPU count). Each worker compiles with SNC (~0.3-0.5s per candidate), extracts bytes, scores. Expected throughput on M-series Mac: ~10-20 candidates/sec for small functions, ~4-8/sec for large ones.

### CLI

```bash
python3 tools/permuter.py src/eTextureMap.cpp 0x0007ae64 --time 600 --workers 8 --save-best
```

### Orchestrator Integration

Overnight agents can invoke the permuter as a final step when within ~10 bytes of matching:

```python
subprocess.run(['python3', 'tools/permuter.py', source_file, func_address,
                '--time', '120', '--save-best'])
```

Agent does creative work (control flow, algorithm, struct layout). Permuter handles mechanical last-mile (operand ordering, register allocation, scheduling).

## What This Doesn't Solve

- VFPU interleaving (eDynamicGeom ctor, mBasis::SetQuat) — needs native VFPU assembler support
- Fundamentally wrong algorithms — agent must get the structure right first
- Missing struct definitions — agent must figure out field layouts

The permuter is a **force multiplier for agents that are already close**, not a replacement for decompilation skill.

## Verification Plan

1. Each mutation type produces valid C that compiles
2. Known-matching source scores 0, known-near-miss scores >0
3. End-to-end: run on eTextureMap::Read (97.7% match, 6 bytes off) — should find match within minutes
4. Performance: confirm ≥10 candidates/sec on M-series Mac
5. Parallel scaling: N workers gives ~N× throughput
