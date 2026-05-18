# Battle Packet: `gcMsgLoadRegions::gcMsgLoadRegions(int, int, bool)`

## Target

- Address: `0x00124a70`
- Size: `292` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcMsgLoadRegions`
- Method family: `Constructor`
- Leaf: `False`

## Queue Metadata

- score: `155`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 10B; codegen-specific diagnosis; Constructor retry family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `10`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `10`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x001b9c64` `untried` 68B `__vec_new`

Callers:
- `0x002dec64` `untried` 592B `gcDoLoadRegionSet::Evaluate(void) const`

## Placement

- Canonical source: `src/gcMsgLoadRegions.cpp`
- Header(s): `include/gcMsgLoadRegions.h`
- Allowed source(s): `src/gcMsgLoadRegions.cpp`
- Split-TU prefix: `src/gcMsgLoadRegions_*.cpp`

## Compiler Guidance

Constructor template: base constructor call order, vtable store, field initialization order, and vec_new/helper calls drive the bytes. Prefer already matched constructors from the same family.

## Class Header: `include/gcMsgLoadRegions.h`

Not found.

## Matched Same-Class Neighbors

- `0x00124b94` 152B `gcMsgLoadRegions::gcMsgLoadRegions(cGUIDT<gcRegion> *, bool)` — `src/gcMsgLoadRegions.cpp`
- `0x00124c2c` 228B `gcMsgLoadRegions::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const` — `src/gcMsgLoadRegions.cpp`
- `0x0026484c` 76B `gcMsgLoadRegions::GetType(void) const` — `src/gcMsgRequestLoadedState.cpp`
- `0x00264898` 96B `gcMsgLoadRegions::New(nwMsgBuffer &) static` — `src/gcMsgLoadRegions.cpp`

## Prior Failure Notes

### Note 1

- session `a23450bd`

Added the canonical C++ gcMsgLoadRegions::gcMsgLoadRegions(int, int, bool) overload and modeled the constructor as real C++: vtable store, __vec_new for the two cGUIDT<gcRegion> slots, flag store, global map lookup at 0x37D7FC, bounds-checked group and entry vector indexing, and a two-GUID copy from entry+8 into this+4. Manual restructuring got the control flow and most scheduling aligned; explicit success-path gotos produced the target bnez-on-valid bounds checks, and precomputing the source pointer put the entry+8 add into the null-check delay slot. New/Write remain matched after the edit.

Best manual version reached a 20-byte mismatch caused by scratch-register allocation in the first vector-length check and final copy loop. A 300-second tools/permuter.py run generated 9,768 candidates, compiled 5,755, and saved an improved 10-byte mismatch. The saved best still does not match; it changes local allocation enough to introduce s3/s4 usage and leaves remaining differences in prologue/copy-loop scheduling and register choices. This is not a bnel issue.


## Disassembly

```asm
124a70:	27bdffe0 	addiu	sp,sp,-32
124a74:	afb00000 	sw	s0,0(sp)
124a78:	00808025 	move	s0,a0
124a7c:	3c040039 	lui	a0,0x39
124a80:	24849070 	addiu	a0,a0,-28560
124a84:	afb10004 	sw	s1,4(sp)
124a88:	afb20008 	sw	s2,8(sp)
124a8c:	afb3000c 	sw	s3,12(sp)
124a90:	30f300ff 	andi	s3,a3,0xff
124a94:	00c09025 	move	s2,a2
124a98:	00a08825 	move	s1,a1
124a9c:	ae040000 	sw	a0,0(s0)
124aa0:	3c070024 	lui	a3,0x24
124aa4:	26040004 	addiu	a0,s0,4
124aa8:	34050002 	li	a1,0x2
124aac:	34060008 	li	a2,0x8
124ab0:	afbf0010 	sw	ra,16(sp)
124ab4:	0c06e719 	jal	0x1b9c64
124ab8:	24e75578 	addiu	a3,a3,21880
124abc:	a2130014 	sb	s3,20(s0)
124ac0:	3c040038 	lui	a0,0x38
124ac4:	8c84d7fc 	lw	a0,-10244(a0)
124ac8:	1080001d 	beqz	a0,0x124b40
124acc:	34050000 	li	a1,0x0
124ad0:	06200008 	bltz	s1,0x124af4
124ad4:	00000000 	nop
124ad8:	8c850398 	lw	a1,920(a0)
124adc:	34040000 	li	a0,0x0
124ae0:	54a00001 	bnezl	a1,0x124ae8
124ae4:	8ca4fffc 	lw	a0,-4(a1)
124ae8:	0224202a 	slt	a0,s1,a0
124aec:	14800003 	bnez	a0,0x124afc
124af0:	00112080 	sll	a0,s1,0x2
124af4:	10000003 	b	0x124b04
124af8:	34110000 	li	s1,0x0
124afc:	00a42021 	addu	a0,a1,a0
124b00:	8c910000 	lw	s1,0(a0)
124b04:	1220000e 	beqz	s1,0x124b40
124b08:	34050000 	li	a1,0x0
124b0c:	06400008 	bltz	s2,0x124b30
124b10:	00000000 	nop
124b14:	8e310020 	lw	s1,32(s1)
124b18:	34040000 	li	a0,0x0
124b1c:	56200001 	bnezl	s1,0x124b24
124b20:	8e24fffc 	lw	a0,-4(s1)
124b24:	0244202a 	slt	a0,s2,a0
124b28:	14800003 	bnez	a0,0x124b38
124b2c:	00122080 	sll	a0,s2,0x2
124b30:	10000003 	b	0x124b40
124b34:	34050000 	li	a1,0x0
124b38:	02242021 	addu	a0,s1,a0
124b3c:	8c850000 	lw	a1,0(a0)
124b40:	10a0000c 	beqz	a1,0x124b74
124b44:	24a60008 	addiu	a2,a1,8
124b48:	34040000 	li	a0,0x0
124b4c:	26050004 	addiu	a1,s0,4
124b50:	8cc70000 	lw	a3,0(a2)
124b54:	8cc80004 	lw	t0,4(a2)
124b58:	aca70000 	sw	a3,0(a1)
124b5c:	aca80004 	sw	t0,4(a1)
124b60:	24840001 	addiu	a0,a0,1
124b64:	24a50008 	addiu	a1,a1,8
124b68:	28870002 	slti	a3,a0,2
124b6c:	14e0fff8 	bnez	a3,0x124b50
124b70:	24c60008 	addiu	a2,a2,8
124b74:	02001025 	move	v0,s0
124b78:	8fb00000 	lw	s0,0(sp)
124b7c:	8fb10004 	lw	s1,4(sp)
124b80:	8fb20008 	lw	s2,8(sp)
124b84:	8fb3000c 	lw	s3,12(sp)
124b88:	8fbf0010 	lw	ra,16(sp)
124b8c:	03e00008 	jr	ra
124b90:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcMsgLoadRegions::gcMsgLoadRegions(int, int, bool)
// Address: 0x00124a70, Size: 292B
// Obj: gcAll_psp.obj

? __vec_new(void *, ?, ?, ?);                       /* extern */

void *gcMsgLoadRegions__gcMsgLoadRegions_int_int_bool(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a1;
    void *temp_a0;
    void *temp_a1;
    void *temp_s1;
    void *var_a1_2;
    void *var_a2;
    void *var_s1;

    arg0->unk0 = 0x389070;
    __vec_new(arg0 + 4, 2, 8, 0x245578);
    arg0->unk14 = (s8) (arg3 & 0xFF);
    temp_a0 = *(void **)0x37D7FC;
    var_a1 = 0;
    if (temp_a0 != NULL) {
        if (arg1 >= 0) {
            temp_a1 = temp_a0->unk398;
            var_a0 = 0;
            if (temp_a1 != NULL) {
                var_a0 = temp_a1->unk-4;
            }
            if (arg1 >= var_a0) {
                goto block_5;
            }
            var_s1 = *(temp_a1 + (arg1 * 4));
        } else {
block_5:
            var_s1 = NULL;
        }
        var_a1 = 0;
        if (var_s1 != NULL) {
            if (arg2 >= 0) {
                temp_s1 = var_s1->unk20;
                var_a0_2 = 0;
                if (temp_s1 != NULL) {
                    var_a0_2 = temp_s1->unk-4;
                }
                if (arg2 >= var_a0_2) {
                    goto block_12;
                }
                var_a1 = *(temp_s1 + (arg2 * 4));
            } else {
block_12:
                var_a1 = 0;
            }
        }
    }
    var_a2 = var_a1 + 8;
    if (var_a1 != 0) {
        var_a0_3 = 0;
        var_a1_2 = arg0 + 4;
        do {
            var_a1_2->unk0 = (s32) var_a2->unk0;
            var_a1_2->unk4 = (s32) var_a2->unk4;
            var_a0_3 += 1;
            var_a1_2 += 8;
            var_a2 += 8;
        } while (var_a0_3 < 2);
    }
    return arg0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
