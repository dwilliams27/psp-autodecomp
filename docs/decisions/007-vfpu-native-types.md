# 007: SNC Native VFPU Type System — Unlocking VFPU Scheduling

**Date:** 2026-04-12
**Status:** Verified

## Discovery

SNC pspsnc 1.2.7503.0 has a fully functional native VFPU type system that generates `lv.q`/`sv.q` instructions from C pointer operations and schedules them with regular MIPS code (including delay-slot placement). It has been inaccessible because the Sony PSP SDK headers (`psptypes.h`) declare the types with `float` as the base type, which SNC rejects. **The correct base type is `int`.**

Wrong (from psptypes.h):
```c
float v4sf __attribute__((mode(V4SF)));     // error 1090: mode incompatible with "float"
```

Correct:
```c
typedef int v4sf_t __attribute__((mode(V4SF)));     // quad vector (4 VFPU regs, 16 bytes)
typedef int v16sf_t __attribute__((mode(V16SF)));   // 4x4 matrix (16 VFPU regs, 64 bytes)
typedef int vs_t __attribute__((mode(VS)));          // scalar (1 VFPU reg, 4 bytes)
```

Required flags:
- `-Xvfpumatrix=N` (N=1..4) — allocate N matrix banks (16 regs each) for V16SF
- `-Xvfpuscalar=N` (N=1..124) — allocate N scalar regs for VS
- Constraint: `matrix * 16 + scalar <= 124` (128 regs total, 4 reserved)

## What SNC Generates

**Quad vector copy** (V4SF):
```c
void store_quad(v4sf_t *dst, v4sf_t *src) { *dst = *src; }
```
→ `lv.q` + `sv.q` (one load, one store), with `sv.q` placed in `jr ra` delay slot.

**Matrix copy** (V16SF):
```c
void store_matrix(v16sf_t *dst, v16sf_t *src) { *dst = *src; }
```
→ 4× `lv.q` + 4× `sv.q`, with the last `sv.q` in the `jr ra` delay slot.

**Matrix + function call scheduling** (the eCamera pattern):
```c
void test(void *self, v16sf_t *src) {
    *(v16sf_t *)((char *)self + 0xE0) = *src;
    do_reset(self);
}
```
→ SNC interleaves `sw ra, 0(sp)` between `sv.q` stores and places the final `sv.q` **in the `jal` delay slot**. This is the exact scheduling pattern the EBOOT exhibits.

## Builtins

Two scalar builtins for converting between C floats and VFPU scalar registers:
- `__builtin_ftovs(float)` → VS type (generates `mfc1` + `mtv`)
- `__builtin_vstof(VS)` → float (generates `mfv` + `mtc1`)

## Impact

| Metric | Before | After |
|--------|--------|-------|
| VFPU functions in EBOOT | 855 | 855 |
| VFPU functions matchable | ~18 (inline asm only, no scheduling) | Potentially all 855 |
| Scheduling capability | None (asm blocks are opaque) | Full (native lv.q/sv.q with delay slot fill) |
| Delay-slot VFPU stores | Impossible | Works natively |

## Limitations and Open Questions

1. **vmidt.q (identity matrix)**: No known C-level way to generate this. May still need inline asm for initial VFPU setup, with V16SF types for the subsequent stores.
2. **VFPU arithmetic** (vadd.q, vmul.q, vdot.q, etc.): Unclear whether SNC auto-vectorizes C arithmetic on V4SF types or if these need inline asm. Needs testing.
3. **Register allocation**: SNC chooses which VFPU matrix banks to use. The EBOOT may use specific registers that don't match SNC's allocation choices. Needs per-function verification.
4. **Makefile integration**: Need to add `-Xvfpumatrix=N` flags for source files using these types.

## Method

Systematic experimentation:
1. Discovered `__builtin_ftovs`/`__builtin_vstof` via `strings` analysis of pspsnc.exe
2. Found `-Xvfpumatrix=N` from compiler error message "use of MF mode with no matrix regs allocated"
3. Discovered `int` base type works by testing all C base types against `mode(VS)` — `float` and `double` fail, `int`/`char`/`short`/`long`/`unsigned` all succeed
4. Confirmed V4SF and V16SF work with `int` base + `-Xvfpumatrix` flag
5. Verified scheduling by testing matrix store + function call pattern — SNC placed `sv.q` in `jal` delay slot
