# SNC Struct Copy and VFPU Auto-Vectorization

Reverse-engineered from SNC compilation experiments (2026-04-14).

## When Does SNC Use VFPU for Struct Copies?

SNC auto-vectorizes struct copies (using `lv.q`/`sv.q`) when ALL of these are true:
1. The struct contains a `SceULong128` (`unsigned __attribute__((mode(TI)))`) or `v4sf_t` (`int __attribute__((mode(V4SF)))`) member
2. The copy is a whole-struct copy (`*dst = *src` or equivalent)
3. The `-Xvfpumatrix=N -Xvfpuscalar=N` flags are set

**Structs with only `float` members do NOT get VFPU copies** — they use `swc1` (scalar FPU store) or `sw` (word store).

## Struct Definitions and Their Copy Codegen

| Struct Type | Copy Pattern |
|---|---|
| `struct { SceULong128 qw; }` | `lv.q` + `sv.q` (VFPU quad) |
| `struct { v4sf_t v; }` | `lv.q` + `sv.q` (VFPU quad) |
| `struct { float x,y,z,w; }` | 4× `swc1` or 4× `sw` (scalar) |
| `struct { float x,y,z,w; } __attribute__((aligned(16)))` | Still scalar — alignment alone doesn't trigger VFPU |
| `union { v4sf_t v; struct { float x,y,z,w; }; }` | `lv.q` + `sv.q` for copy (V4SF member triggers it) |

## The mtc1/mfc1/mtv/sv.q Construction Pattern

This pattern appears in constructors that initialize aligned struct members with float constants:
```asm
mtc1 $zero, $f12     // create float 0.0
mfc1 $a0, $f12       // FPU → GPR
mfc1 $a2, $f12       // FPU → GPR (separate register per field)
mfc1 $a3, $f12       // FPU → GPR
mtv  $a0, S120       // GPR → VFPU
mtv  $a2, S121
mtv  $a3, S122
sv.q C120, offset(ptr)  // VFPU quad store
```

### How to Reproduce in C++
See `docs/research/snc-constructor-codegen.md` for the recipe. Key: use separate `__asm__ volatile("mfc1")` statements for each GPR, then one block for `mtv+sv.q`.

### What DOESN'T Produce This Pattern
- Individual float member stores → `swc1` (scalar FPU store)
- `vzero.q` builtin → too efficient (3 instructions vs 9)
- `memset` → not available in SNC's header path
- Struct copy from zero global → `lv.q` from global + `sv.q` (different pattern)

## VFPU Register Allocation

With `-Xvfpumatrix=1 -Xvfpuscalar=8`:
- Matrix 0 (vt 0-3, columns C000-C030): reserved for V16SF user code
- Scalars 16-23 (vt 16-23): reserved for scalar user code  
- First auto-temp: vt=24 (column C120) — this is why all auto-copies use C120
- Additional auto-temps: C130 (vt=28), C200 (vt=32), etc.

## Inline Asm and VFPU Stores

**Inline asm `sv.q` CANNOT fill delay slots.** Only compiler-generated `sv.q` from V4SF/SceULong128 assignments can be scheduled into `jr $ra` delay slots. See `docs/research/snc-delay-slots.md`.

This means functions where the original binary has `sv.q` in a delay slot MUST use compiler-generated copies, not inline asm.
