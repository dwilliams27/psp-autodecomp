# Battle Packet: `eTexture::GetFullTexCoords(mVec4 *) const`

## Target

- Address: `0x001e1134`
- Size: `52` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eTexture`
- Method family: `GetFullTexCoords`
- Leaf: `True`

## Queue Metadata

- score: `163`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 8B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `constant_cse_codegen`
- Near miss: `8`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Placement

- Canonical source: `src/eTexture.cpp`
- Header(s): `include/eTexture.h`
- Allowed source(s): `src/eTexture.cpp`
- Split-TU prefix: `src/eTexture_*.cpp`

## Class Header: `include/eTexture.h`

Not found.

## Matched Same-Class Neighbors

- `0x0002bcd8` 136B `eTexture::Write(cFile &) const` — `src/eTexture.cpp`
- `0x0002bd60` 288B `eTexture::Read(cFile &, cMemPool *)` — `src/eTexture.cpp`
- `0x0002be80` 80B `eTexture::eTexture(cBase *)` — `src/eTexture.cpp`
- `0x001e1010` 292B `eTexture::GetType(void) const` — `src/eTexture.cpp`

## Prior Failure Notes

### Note 1

- session `ea37cfd0`

Tried a new SNC native-VFPU route not covered by the prior notes: declare a v4sf_t temp, use a `=v` inline-asm output, and finish with `*(v4sf_t *)out = temp` so the final `sv.q C120,0($a1)` is compiler-generated and lands in the `jr $ra` delay slot. This works for delay-slot placement. A C/asm hybrid using `mtc1`/`mfc1` operands compiles to the right final `jr`/`sv.q`, but it injects a leading `vzero.q C120` because SNC considers the vector temp uninitialized unless the `%0` vector operand appears in a real asm instruction. Referencing `%0` as `mtv ..., C120` suppresses the `vzero.q` in SNC's .s output, but GAS rejects `mtv $r,C120` as an illegal VFPU register; using comments or empty asm does not count as a real reference, and using `vmov.q %0,%0` adds an extra instruction. A fixed-register variant can produce the exact target 13-instruction body plus the compiler-owned delay-slot store, but still has the one extra leading `vzero.q`.

Also retested the prior recommendation around `__builtin_ftovs` and lane-wise VS writes. Direct `((vs_t *)out)[i] = __builtin_ftovs(...)` generates scalar VFPU stores (`sv.s`/encoded scalar stores), not the target `mtv` fan-out followed by one `sv.q`. Writing lanes into a union/local and then copying forces a stack frame plus `lv.q`. This is not a branch-likely issue. The remaining blocker is specifically SNC/GAS inline-asm operand syntax: the only discovered way to both suppress `vzero.q` and let SNC emit the return-delay `sv.q` requires an asm operand spelling (`mtv ..., %0` -> `C120`) that the assembler cannot assemble. I removed the non-matching eTexture implementation rather than leaving a known-bad symbol or submitting a pure assembly wrapper.

### Note 2

- session `1e199480`

eTexture::GetFullTexCoords(mVec4*) — writes the constant vector {0.0, 0.0, 1.0, 1.0} to *out. The original codegen is 13 instructions (52 bytes) that fabricate 0.0 in $f12 via mtc1 $zero, fabricate 1.0 in $f13 via lui+mtc1, fan out to four GPRs via mfc1 (a0,a3,a2,t0 — explicitly avoiding $a1 which holds the output pointer), push them into S120..S123 via mtv, and finish with `jr $ra` with `sv.q C120, 0($a1)` in the delay slot.

Attempts tried:
1) Pure inline-asm replica of the sequence (lui/mtc1/mtc1/4×mfc1/4×mtv/sv.q). SNC wraps asm blocks with `.set reorder` but mipsel-linux-gnu-as does NOT move the inline `sv.q` into the following compiler-emitted `jr $ra` delay slot — it inserts a nop. Result: 56 bytes with an extra nop, 40/56 bytes diff.
2) Explicit `jr $ra; sv.q` (and a variant with trailing nop) inside the asm block. SNC still emits its own `j $ra; nop` epilogue after the asm, so we end up with two returns and 64+ bytes. No way to suppress the epilogue: SNC does not implement `__builtin_unreachable` (error 0020: identifier undefined) and has no `noreturn` attribute.
3) Native v4sf_t (`typedef int v4sf_t __attribute__((mode(V4SF)))`) with a compound-literal initializer {0.0f,0.0f,1.0f,1.0f}. SNC rejects this at the second element: `error 0067: expected a "}"`. Per docs/decisions/007, SNC's V4SF is transport-only — it accepts copy/assignment between v4sf_t lvalues but NOT aggregate-initializer lists. Same error for `static const struct{v4sf_t v;} k = {{0,0,0x3f800000,0x3f800000}};`.
4) Static-const struct copy via v4sf_t member — blocked by the same initializer restriction; a workable kFull constant cannot be built in SNC C/C++ source.

Root cause: to match byte-for-byte we need SNC to generate the exact mtc1/mfc1/mtv/sv.q construction pattern with `sv.q` scheduled into the `jr $ra` delay slot. That pattern is compiler-owned scheduling; `.set reorder` in GAS does not synthesize it for `sv.q`. SNC itself will only emit it when it chooses to materialize a V4SF constant inline — and SNC refuses every syntactic form of v4sf_t constant we can write. This is NOT a bnel divergence (the diffs are not 1–8 bytes of bnel/beql) — it is a genuine "source can't express the codegen" case.

Recommendation for the next agent: investigate SNC intrinsics such as `__builtin_ftovs` to see if a per-lane assignment to a v4sf_t via `((vs_t*)&v)[i] = __builtin_ftovs(c)` triggers the mtc1/mfc1/mtv path (docs/decisions/007 names the builtin but does not document lane-wise writes). If that does not work, this function is a candidate for `unmatchable` pending a future SNC feature discovery, but NOT `unmatchable_bnel` — the bnel criteria in CLAUDE.md (≤8-byte diff on bnel vs bne+nop) do not apply. Per CLAUDE.md I refused to submit an inline-asm-wrapped match (pure asm is banned and the orchestrator rejects it).


## Disassembly

```asm
1e1134:	3c063f80 	lui	a2,0x3f80
1e1138:	44806000 	mtc1	zero,$f12
1e113c:	44866800 	mtc1	a2,$f13
1e1140:	44046000 	mfc1	a0,$f12
1e1144:	44076000 	mfc1	a3,$f12
1e1148:	44066800 	mfc1	a2,$f13
1e114c:	44086800 	mfc1	t0,$f13
1e1150:	48e40006 	mtv         $a0, S120
1e1154:	48e70026 	mtv         $a3, S121
1e1158:	48e60046 	mtv         $a2, S122
1e115c:	48e80066 	mtv         $t0, S123
1e1160:	03e00008 	jr	ra
1e1164:	f8a60000 	sv.q        C120, 0x0($a1)
```

## m2c Starting Point

```c
// Decompiled: eTexture::GetFullTexCoords(mVec4 *) const
// Address: 0x001e1134, Size: 52B
// Obj: eAll_psp.obj

void eTexture__GetFullTexCoords_mVec4ptrconst(void) {
    __asm__ volatile("mtv $a0, S120");
    __asm__ volatile("mtv $a3, S121");
    __asm__ volatile("mtv $a2, S122");
    __asm__ volatile("mtv $t0, S123");
    __asm__ volatile("sv.q C120, ($a1)");
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
