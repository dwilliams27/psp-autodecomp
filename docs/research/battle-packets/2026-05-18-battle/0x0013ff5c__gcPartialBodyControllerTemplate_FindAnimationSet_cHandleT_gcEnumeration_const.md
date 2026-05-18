# Battle Packet: `gcPartialBodyControllerTemplate::FindAnimationSet(cHandleT<gcEnumeration>) const`

## Target

- Address: `0x0013ff5c`
- Size: `112` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcPartialBodyControllerTemplate`
- Method family: `FindAnimationSet`
- Leaf: `True`

## Queue Metadata

- score: `154`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 4B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `4`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `4`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context


Callers:
- `0x0013ffcc` `untried` 264B `gcPartialBodyControllerTemplate::GetAnimation(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &) const`

## Placement

- Canonical source: `src/gcPartialBodyControllerTemplate.cpp`
- Header(s): `include/gcPartialBodyControllerTemplate.h`
- Allowed source(s): `src/gcPartialBodyControllerTemplate.cpp`
- Split-TU prefix: `src/gcPartialBodyControllerTemplate_*.cpp`

## Class Header: `include/gcPartialBodyControllerTemplate.h`

Not found.

## Matched Same-Class Neighbors

- `0x0013fbe8` 112B `gcPartialBodyControllerTemplate::Write(cFile &) const` — `src/gcPartialBodyControllerTemplate.cpp`
- `0x0013fd40` 72B `gcPartialBodyControllerTemplate::gcPartialBodyControllerTemplate(cBase *)` — `src/gcPartialBodyControllerTemplate.cpp`
- `0x0013fe38` 64B `gcPartialBodyControllerTemplate::Reset(cMemPool *, bool)` — `src/gcPartialBodyControllerTemplate_Reset.cpp`
- `0x0013fe78` 228B `gcPartialBodyControllerTemplate::GetInstanceType(void) const` — `src/gcPartialBodyControllerTemplate.cpp`
- `0x002a5580` 124B `gcPartialBodyControllerTemplate::New(cMemPool *, cBase *) static` — `src/gcPartialBodyControllerTemplate.cpp`

## Prior Failure Notes

### Note 1

- session `5dee6267`

The best C++ body remains the prior near-match: `int i = 0; void *data = mArr1.mData; int offset = 0;` with `if (*eh == h) return i;`. It emits the expected loop body, including the addiu+lw split for the +8 handle load and the xor/sltiu/andi bool conversion. The only residual diff is still the prologue scheduler swap: compiled output has `li a1,0` before `lw a0,0(sp)`, while expected has `lw a0,0(sp)` before `li a1,0`.

Additional shapes tested this session included direct `mIndex` comparison with explicit `& 0xff`, target handle/index locals before and after offset initialization, explicit register bindings, empty asm dependency barriers, a local cHandleT copy, `h == *eh`, and a named `IsEqual` helper. These either returned to the same 4-byte scheduler diff or changed register allocation/control-flow and made the function worse. The root cause still appears to be a narrow SNC scheduling heuristic difference around the saved by-value handle load versus immediate zeroing of the offset register.

### Note 2

- session `8fadd97b`

FindAnimationSet — got to 4 bytes diff (108B compiled vs 112B expected initially). Body matches perfectly: addiu+lw split for entry+8 dereference, xor/sltiu/andi 0xff bool conversion (operator== returning bool on cHandleT<T>). Remaining diff is a scheduler permutation in prologue: my output emits 'li a1,0; lw a0,0(sp)' but expected has 'lw a0,0(sp); li a1,0' (4 bytes from instruction swap). The earlier prologue swap (sw a1 vs lw a2,20(a0)) was fixed by reordering local declarations to 'int i = 0; void *data = mArr1.mData; int offset = 0;'. The remaining lw a0/li a1 swap was NOT fixable with: permuter (5min, 5300+ candidates, 0 improvements), value vs ref operator== params, local handle copy, for-loop instead of while(1), data load inside loop, sched=0, or local hIdx capture (which lost the addiu split). Suspected root cause: SNC scheduler heuristic prioritizes li (immediate) before lw (load) when used register has earlier dependency; expected source must use a structural pattern (perhaps a member-fn comparison helper or an explicit cHandleT<T>::IsEqual method) that re-prioritizes the load. Try: a non-inline operator== declaration, a SNC-specific intrinsic, or restructuring so that a0 (h.mIndex) is used earlier in the IR than a1 (offset).


## Disassembly

```asm
13ff5c:	27bdfff0 	addiu	sp,sp,-16
13ff60:	afa50000 	sw	a1,0(sp)
13ff64:	8c860014 	lw	a2,20(a0)
13ff68:	34020000 	li	v0,0x0
13ff6c:	8fa40000 	lw	a0,0(sp)
13ff70:	34050000 	li	a1,0x0
13ff74:	34070000 	li	a3,0x0
13ff78:	54c00001 	bnezl	a2,0x13ff80
13ff7c:	8cc7fffc 	lw	a3,-4(a2)
13ff80:	0047382a 	slt	a3,v0,a3
13ff84:	10e0000e 	beqz	a3,0x13ffc0
13ff88:	00c53821 	addu	a3,a2,a1
13ff8c:	8ce70000 	lw	a3,0(a3)
13ff90:	24e70008 	addiu	a3,a3,8
13ff94:	8ce70000 	lw	a3,0(a3)
13ff98:	00e43826 	xor	a3,a3,a0
13ff9c:	2ce70001 	sltiu	a3,a3,1
13ffa0:	30e700ff 	andi	a3,a3,0xff
13ffa4:	14e00004 	bnez	a3,0x13ffb8
13ffa8:	00000000 	nop
13ffac:	24420001 	addiu	v0,v0,1
13ffb0:	1000fff0 	b	0x13ff74
13ffb4:	24a50004 	addiu	a1,a1,4
13ffb8:	03e00008 	jr	ra
13ffbc:	27bd0010 	addiu	sp,sp,16
13ffc0:	2402ffff 	li	v0,-1
13ffc4:	03e00008 	jr	ra
13ffc8:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcPartialBodyControllerTemplate::FindAnimationSet(cHandleT<gcEnumeration>) const
// Address: 0x0013ff5c, Size: 112B
// Obj: gcAll_psp.obj

s32 gcPartialBodyControllerTemplate__FindAnimationSet_cHandleT_gcEnumeration_const(void *arg0, s32 arg1) {
    s32 sp0;
    s32 var_a1;
    s32 var_a3;
    s32 var_v0;
    void *temp_a2;

    sp0 = arg1;
    temp_a2 = arg0->unk14;
    var_v0 = 0;
    var_a1 = 0;
loop_1:
    var_a3 = 0;
    if (temp_a2 != NULL) {
        var_a3 = temp_a2->unk-4;
    }
    if (var_v0 < var_a3) {
        if (!(((*(temp_a2 + var_a1))->unk8 == sp0) & 0xFF)) {
            var_v0 += 1;
            var_a1 += 4;
            goto loop_1;
        }
        return var_v0;
    }
    return -1;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
