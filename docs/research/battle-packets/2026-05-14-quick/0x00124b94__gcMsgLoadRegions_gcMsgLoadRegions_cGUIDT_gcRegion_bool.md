# Battle Packet: `gcMsgLoadRegions::gcMsgLoadRegions(cGUIDT<gcRegion> *, bool)`

## Target

- Address: `0x00124b94`
- Size: `152` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcMsgLoadRegions`
- Method family: `Constructor`
- Leaf: `False`

## Queue Metadata

- score: `154`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 10B; correct function size; codegen-specific diagnosis; Constructor retry family; manageable size`
- failure_action: `retry`
- near_miss_bytes: `10`

## Failure Classification

- Primary: `retryable_nearmiss`
- Action: `retry`
- Confidence: `medium`
- Tags: `retryable_nearmiss`
- Near miss: `10`
- Summary: Small byte diff with no stronger compiler-blocker signal.

## Graph Context

Callees:
- `0x001b9c64` `untried` 68B `__vec_new`

Callers:
- `0x0016b438` `untried` 1436B `gcGameSettings::SaveGameRestore(void)`

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

- `0x00124c2c` 228B `gcMsgLoadRegions::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const` — `src/gcMsgLoadRegions.cpp`
- `0x0026484c` 76B `gcMsgLoadRegions::GetType(void) const` — `src/gcMsgRequestLoadedState.cpp`
- `0x00264898` 96B `gcMsgLoadRegions::New(nwMsgBuffer &) static` — `src/gcMsgLoadRegions.cpp`

## Prior Failure Notes

### Note 1

- session `f6cbf877`

Implemented the constructor in C++ and reduced it to the correct 152-byte size. After masking the __vec_new relocation, the best version is a 10-byte diff: the generated body matches the target except SNC assigns the bool flag to s1 and the regions pointer to s2, while the target uses regions in s1 and flag in s2. This changes the initial andi/move pair, the mFlag store source register, and the source-base register in the copy-loop addu; the loop body itself is otherwise aligned (li a0, addiu a1, lw/lw, sw/sw, i++ and pointer increments).

Tried direct pointer walking, byte-offset loops, indexed array expressions, explicit a/b temporaries, declaration reordering, local register-variable forms, sched=1 as a diagnostic, and a 300-second tools/permuter.py run. The permuter improved baseline from 26 to 24 bytes before manual indexed/destination-first restructuring reduced it to 10 bytes, but no tested C++ shape swapped only the saved-register assignment without disturbing the matched loop or prologue.


## Disassembly

```asm
124b94:	27bdfff0 	addiu	sp,sp,-16
124b98:	afb00000 	sw	s0,0(sp)
124b9c:	00808025 	move	s0,a0
124ba0:	3c040039 	lui	a0,0x39
124ba4:	24849070 	addiu	a0,a0,-28560
124ba8:	afb10004 	sw	s1,4(sp)
124bac:	afb20008 	sw	s2,8(sp)
124bb0:	00a08825 	move	s1,a1
124bb4:	30d200ff 	andi	s2,a2,0xff
124bb8:	ae040000 	sw	a0,0(s0)
124bbc:	3c070024 	lui	a3,0x24
124bc0:	26040004 	addiu	a0,s0,4
124bc4:	34050002 	li	a1,0x2
124bc8:	34060008 	li	a2,0x8
124bcc:	afbf000c 	sw	ra,12(sp)
124bd0:	0c06e719 	jal	0x1b9c64
124bd4:	24e75578 	addiu	a3,a3,21880
124bd8:	34040000 	li	a0,0x0
124bdc:	a2120014 	sb	s2,20(s0)
124be0:	34060000 	li	a2,0x0
124be4:	26050004 	addiu	a1,s0,4
124be8:	02242021 	addu	a0,s1,a0
124bec:	8c870000 	lw	a3,0(a0)
124bf0:	8c880004 	lw	t0,4(a0)
124bf4:	aca70000 	sw	a3,0(a1)
124bf8:	aca80004 	sw	t0,4(a1)
124bfc:	24c60001 	addiu	a2,a2,1
124c00:	24a50008 	addiu	a1,a1,8
124c04:	28c70002 	slti	a3,a2,2
124c08:	14e0fff8 	bnez	a3,0x124bec
124c0c:	24840008 	addiu	a0,a0,8
124c10:	02001025 	move	v0,s0
124c14:	8fb00000 	lw	s0,0(sp)
124c18:	8fb10004 	lw	s1,4(sp)
124c1c:	8fb20008 	lw	s2,8(sp)
124c20:	8fbf000c 	lw	ra,12(sp)
124c24:	03e00008 	jr	ra
124c28:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcMsgLoadRegions::gcMsgLoadRegions(cGUIDT<gcRegion> *, bool)
// Address: 0x00124b94, Size: 152B
// Obj: gcAll_psp.obj

? __vec_new(void *, ?, ?, ?);                       /* extern */

void *gcMsgLoadRegions__gcMsgLoadRegions_cGUIDT_gcRegion_ptr_bool(void *arg0, void *arg1, s32 arg2) {
    s32 var_a2;
    void *var_a0;
    void *var_a1;

    arg0->unk0 = 0x389070;
    __vec_new(arg0 + 4, 2, 8, 0x245578);
    arg0->unk14 = (s8) (arg2 & 0xFF);
    var_a2 = 0;
    var_a1 = arg0 + 4;
    var_a0 = arg1;
    do {
        var_a1->unk0 = (s32) var_a0->unk0;
        var_a1->unk4 = (s32) var_a0->unk4;
        var_a2 += 1;
        var_a1 += 8;
        var_a0 += 8;
    } while (var_a2 < 2);
    return arg0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
