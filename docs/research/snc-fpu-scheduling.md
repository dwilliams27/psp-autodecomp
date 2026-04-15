# SNC FPU Scheduling Rules

Reverse-engineered from SNC compilation experiments (2026-04-14, 50 test cases).

## Core Rules

1. **Source expression order controls scheduling priority.** If the binary shows `mul(bw,ax)` before `mul(bx,aw)`, the C++ must compute `bw*ax` first. SNC preserves source declaration order when there are no dependency constraints.

2. **SNC never emits `madd.s`.** All multiply-accumulate patterns become separate `mul.s` + `add.s`. Every FPU multiply creates a scheduling window that SNC fills with independent work.

3. **Minimum 2-cycle FPU latency model.** SNC places at least 1 independent instruction between an FPU producer and its consumer when possible. It does NOT insert NOPs for FPU stalls — hardware interlocks handle them.

4. **Load interleaving — "pipeline priming."** SNC loads 4 values (2 pairs) before starting computation, then interleaves remaining loads with computes:
   ```
   lwc1 f12, 0(a0)     # pair 1a
   lwc1 f13, 0(a1)     # pair 1b
   lwc1 f14, 4(a0)     # pair 2a (priming)
   lwc1 f15, 4(a1)     # pair 2b (priming)
   mul.s f12, f12, f13  # compute pair 1
   lwc1 f16, 8(a0)     # pair 3a (keep priming)
   mul.s f14, f14, f15  # compute pair 2
   ```

5. **Float constants loaded via GPR, not from .rodata:**
   - `0.0f`: `mtc1 $zero, $f12`
   - `1.0f`: `lui $a0, 0x3f80` / `mtc1 $a0, $f12`
   - Complex constants: `lui` + `ori` + `mtc1`
   - SNC uses `$a0-$a3` for constant loading

6. **c.lt.s → bc1f has mandatory 1-cycle delay.** SNC always inserts at least a NOP between compare and branch. When independent FPU work is available, it fills the gap.

## sched=1 vs sched=2

- Tiny functions (<5 FPU ops): identical output
- Medium functions (6+ muls): sched=2 reorders more aggressively, starts multiplies earlier
- Both produce same-size code but different instruction ordering in the middle

## Negation Patterns

- `-(a*b)` → `mul.s` + `neg.s` (separate)
- `a*b - c*d` → two `mul.s` + `sub.s` (folded into subtraction)
- `a*b + -(c*d)` → NOT folded: `mul.s` / `neg.s` / `add.s`
- Source expression structure matters: `a*b - c*d` ≠ `a*b + -(c*d)` in codegen

## FPU Register Allocation

- `$f12-$f19`: Primary working registers (caller-saved)
- `$f0-$f3`: Secondary working registers
- `$f20-$f31`: Callee-saved, used only under register pressure
- Register allocation follows source declaration order

## Struct Return Pattern

- Functions returning small structs via pointer: `mfc1 $a1, $f_result` / `sw $a1, offset($a0)` (NOT `swc1`)
- Functions writing to `this->member` directly: uses `swc1`
- The `mfc1+sw` vs `swc1` difference depends on whether the write is through a return pointer or a direct member access

## volatile Effect

`volatile float *` forces SNC to serialize: compute → store → compute → store. Without volatile, SNC starts all computations then stores all results.

Memory barriers (`__asm__ volatile("" ::: "memory")`) do NOT affect pure FPU register-to-register scheduling, only memory operations.
