# SNC Constructor Codegen

Reverse-engineered from SNC compilation experiments (2026-04-14, 168 tool uses).

## The mtc1/mfc1/mtv/sv.q Pattern — SOLVED

The pattern:
```asm
mtc1 $zero, $f12     // float 0.0
mfc1 $a0, $f12       // to GPR
mfc1 $a2, $f12       // to GPR (separate for each field)
mfc1 $a3, $f12       // to GPR
mtv  $a0, S120       // GPR to VFPU
mtv  $a2, S121
mtv  $a3, S122
sv.q C120, offset($ptr)  // quad store
```

### Recipe to Reproduce

Use **separate `__asm__ volatile` statements** for each `mfc1`, alternating with C float computations, then ONE asm block for `mtv+sv.q`:

```cpp
float z = 0.0f;
int a, b, c;
__asm__ volatile("mfc1 %0, %1" : "=r"(a) : "f"(z));
__asm__ volatile("mfc1 %0, %1" : "=r"(b) : "f"(z));
__asm__ volatile("mfc1 %0, %1" : "=r"(c) : "f"(z));
__asm__ volatile(
    "mtv %0, S120\n"
    "mtv %1, S121\n"
    "mtv %2, S122\n"
    "sv.q C120, offset(%3)\n"
    :: "r"(a), "r"(b), "r"(c), "r"(ptr)
);
```

**Key insight:** Each `mfc1` must be a SEPARATE `__asm__ volatile` statement so SNC allocates a different GPR for each. If they're in one asm block with three outputs, SNC may reuse the same register.

### Why This Pattern Exists

The original compiler generated this when constructors initialize struct members with float constants and the struct is 16-byte aligned. SNC groups 3-4 consecutive float stores into a VFPU quad store for efficiency.

The `mtc1 → mfc1` roundtrip converts float values to their integer bit representation in GPR. The `mtv → sv.q` path then stores them as a VFPU quad.

## Member Initializer List vs Body Assignment

- **Initializer list** (`S() : a(0), b(0), c(0) {}`): SNC may reorder member initialization
- **Body assignment** (`S() { a=0; b=0; c=0; }`): SNC preserves source order
- For matching, body assignment gives more control over instruction ordering

## Float Constant Grouping

SNC groups consecutive float stores to aligned addresses into VFPU quad stores when:
1. The target address is 16-byte aligned
2. The stores are to consecutive offsets (0, 4, 8, 12 within a 16-byte block)
3. The function uses `-Xvfpumatrix=N -Xvfpuscalar=N` flags

Stores that DON'T form complete groups remain as individual `swc1`.

## __vec_new Pattern

Constructor calls to `__vec_new` (array element construction) use this pattern:
```cpp
extern void *__vec_new(void *array, int count, int size, void (*ctor)(void *));
__vec_new((char*)this + OFFSET, COUNT, ELEMENT_SIZE, element_ctor_func);
```

The function pointer is passed as the 4th argument (`$a3`), loaded via `lui+addiu` for relocated addresses.
