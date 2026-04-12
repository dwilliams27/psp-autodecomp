# 005a: SNC Codegen Experiment Results

**Date:** 2026-04-12
**Purpose:** Raw experiment data backing the permuter mutation design in 005-snc-permuter.md

These experiments were run with `pspsnc.exe` v1.2.7503.0 at `-O2 -G0 -Xsched=2`. Each pair was compiled and disassembled to determine whether the transformation changes output bytes.

## Experiments That CHANGE Output

### 1. Variable declaration order controls register assignment
```c
// A: puts x+1 in v0 first
int a = x + 1; int b = y + 2; return a + b;
// B: puts y+2 in v0 first
int b = y + 2; int a = x + 1; return a + b;
```
**Result: Different bytes.** Declaration order = register order.

### 2. Operand order in commutative ops preserved
```c
// A: computes a*b first
return (a * b) + (c * d);
// B: computes c*d first
return (c * d) + (a * b);
```
**Result: Different bytes.** SNC does NOT reassociate commutative operands.

### 3. Expression associativity in chains
```c
// A: left-to-right
a + b + c        // addu a0,a1 then addu v0,a2
// B: right-grouped
a + (b + c)      // addu a1,a2 then addu v0,a0
// C: mixed
(a + c) + b      // addu a0,a2 then addu v0,a1
```
**Result: All three produce different bytes.**

### 4. Store order preserved
```c
// A:
*p = a; *q = b;
// B:
*q = b; *p = a;
```
**Result: Different bytes.** Stores emitted in source order.

### 5. Compound condition order preserved
```c
// A: checks a first
a > 0 && b > 0   // blez a0
// B: checks b first
b > 0 && a > 0   // blez a1
```
**Result: Different bytes.**

### 6. Function call evaluation is left-to-right
```c
// SNC evaluates left-to-right (NOT right-to-left like some compilers)
sink(f1(), f2())  // calls f1 first, saves to s0
```
**Result: Different bytes when call order changes.**

### 7. Struct member access order preserved
```c
s->a + s->b + s->c  // loads a,b,c in order
s->c + s->b + s->a  // loads c,b,a in order
```
**Result: Different bytes.**

### 8. Early return vs single-return variable
```c
// A: multiple returns
if (x > 0) return 1; if (x < 0) return -1; return 0;
// B: single return variable
int r = 0; if (x > 0) r = 1; else if (x < 0) r = -1; return r;
```
**Result: Different bytes, different structure.** B uses `bltzl` (branch-likely), is more compact.

### 9. Multiply by constant decomposition
```c
x * 3       // addu v0,a0,a0; addu v0,a0,v0
(x << 1) + x // sll v0,a0,1; addu v0,v0,a0
x + x + x    // addu v0,a0,a0; addu v0,v0,a0 (different final add operands from x*3)
```
**Result: All three produce different bytes.**

### 10. Float compound assignment swaps operands
```c
return a + b;           // add.s $f0,$f12,$f13
float t = a; t += b; return t;  // add.s $f0,$f13,$f12  (SWAPPED)
```
**Result: Different bytes.** Operands swapped.

### 11. Int-to-float cast placement
```c
(float)a + (float)b   // two cvt.s.w then add.s
(float)(a + b)         // one addu then one cvt.s.w
```
**Result: Completely different code.**

### 12. Signed vs unsigned (sub-word)
- `char` load: `lb` (sign-extend). `unsigned char`: `lbu` (zero-extend). **Different.**
- `int` vs `unsigned int` load: both `lw`. **Same.**
- Signed compare: `slt`. Unsigned: `sltu`. **Different.**
- Signed shift: `sra`. Unsigned: `srl`. **Different.**
- Signed div by constant 4: complex 5-instruction rounding. Unsigned: single `srl`. **Very different.**

### 13. Short type causes sign-extension overhead
`short` parameters get `sll`/`sra` pairs. `int` does not. **Different.**

### 14. Switch vs if-chain
Switch with 3 cases: binary-search branch tree (`bgtz`, `slti`, `bnez`).
If-chain: sequential `beq`/`bne` with `li` per constant. **Completely different.**

### 15. Bitfield extraction order preserved
```c
(x >> 4) & 0xF   // sra then andi
(x & 0xF0) >> 4  // andi then sra
```
**Result: Different bytes.** Source operation order preserved.

### 16. `__asm__ volatile("" ::: "memory")` barriers
Prevents scheduler from hoisting instructions across the barrier. Without: scheduler moves `lui` earlier to fill load-use delay slot. With: instructions stay in source order. **Different bytes when scheduler would reorder.**

### 17. Sched=1 vs sched=2
Multiple independent load-use chains: sched=2 batches loads together then interleaves; sched=1 alternates load/use pairs. Also affects prologue register save placement. **Different bytes on non-trivial functions.**

### 18. Comparison-to-zero branch encodings
- `x == 0` → `bnez` (skip body)
- `x != 0` → `beqz` (skip body)
- `x > 0` → `blez`
- `x >= 0` → `bltz`
- `x < 0` → `bgez`
- `x <= 0` → `bgtz`
- `x > 5` → `slti a0,6` then `bnez`
- `x == 1` → `li a1,1` then `bne`

## Experiments That Are NO-OPS

### If/else vs ternary → IDENTICAL
```c
if (x) r = a; else r = b; return r;
return x ? a : b;
```
Both produce `bnezl` branch-likely pattern. Same bytes.

### Temp variable vs inline → IDENTICAL
```c
int t = *p; return t + 1;
return *p + 1;
```
Optimizer eliminates the temporary. Same bytes.

### For vs while → IDENTICAL
```c
for (i = 0; i < n; i++) { ... }
int i = 0; while (i < n) { ...; i++; }
```
SNC normalizes both to same form. Same bytes.

### Pre/post increment → IDENTICAL
`*p++`, `*p = i; p++`, `*p = i; p = p + 1` — all same bytes in loops.

### Compound assignment vs explicit → IDENTICAL
`*p += x` and `*p = *p + x` and `int t = *p; t += x; *p = t` — all same bytes.

### Constant expressions → IDENTICAL
`256`, `1 << 8`, `int x = 256; return x;` — constant folding works. Same bytes.

### Pointer cast forms → IDENTICAL
`((Derived*)p)->y`, `*(int*)((char*)p + 4)` — both produce `lw v0,4(a0)`. Same bytes.

### Array index vs pointer arithmetic → IDENTICAL
`p[i]`, `*(p + i)`, `*(int*)((char*)p + i*4)` — all same bytes.

### Volatile has no scheduling effect
`volatile int *` loads produce same scheduling as normal pointer loads.

### SNC does NOT perform tail call optimization
At any optimization level (-O2 through -O5), `return other(x+1)` always generates jal + frame save/restore.
