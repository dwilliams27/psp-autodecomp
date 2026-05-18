# Battle Packet: `eBroadphase::ClearAccumulatedImpulses(eCollisionPair *)`

## Target

- Address: `0x000372d8`
- Size: `164` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eBroadphase`
- Method family: `ClearAccumulatedImpulses`
- Leaf: `False`

## Queue Metadata

- score: `148`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 10B; codegen-specific diagnosis; manageable size`
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
- `0x00034130` `matched` 76B `eCollisionPair::RemoveContacts(void)`
- `0x001ad668` `matched` 52B `memset`

Callers:
- `0x00036b40` `untried` 1012B `eBroadphase::MakeDeterministic(void)`

## Placement

- Canonical source: `src/eBroadphase.cpp`
- Header(s): `include/eBroadphase.h`
- Allowed source(s): `src/eBroadphase.cpp`
- Split-TU prefix: `src/eBroadphase_*.cpp`

## Class Header: `include/eBroadphase.h`

Not found.

## Matched Same-Class Neighbors

- `0x00035c64` 68B `eBroadphase::UpdateRigidBody(eRigidBodyState *)` — `src/eSweptSphereCollisionHandler.cpp`
- `0x00035ca8` 64B `eBroadphase::MarkPairsUnvisited(eRigidBodyState *)` — `src/eBroadphase.cpp`
- `0x00036adc` 100B `eBroadphase::UpdatePairSleepState(eCollisionPair *)` — `src/eBroadphase.cpp`

## Prior Failure Notes

### Note 1

- session `cadcd6fa`

The existing C++ remains the best form: it has the same outer circular active-pair loop, memset of the 0x10 accumulated impulse block, contact-list impulse zeroing loop, RemoveContacts call, flag clear, and bnel active-list traversal. The final diff is still 10/164 bytes, caused by saved-register allocation in the loop: expected uses s1 for the -3 flag mask and s2 for the contact cursor, while SNC keeps producing s2 for the mask and s1 for the contact cursor.

Retried declaration-order changes and explicit local register annotations; they either preserved the same 10-byte s1/s2 swap or worsened the function to 24 bytes different. Ran tools/permuter.py for 314 seconds (7968 generated, 4564 compiled) with --save-best; it found zero improvement over the 10-byte baseline. This is not a bnel/unmatchable classification because the mismatch is register allocation, not branch-likely divergence.

### Note 2

- session `9a0ed17e`

ClearAccumulatedImpulses is close at 10/164 bytes different with the same loop shape, memset, contact impulse zeroing loop, RemoveContacts call, flag clear, and circular active-list traversal. The remaining mismatch is saved-register allocation inside the loop: original uses s1 for the -3 mask and s2 for the contact cursor, while the best C++ shape here uses s2 for the mask and s1 for the contact cursor. I tried changing declaration order, hoisting the contact variable, using register variables, and varying the first/current pair temporaries; these either preserved the swapped s1/s2 allocation or made the outer loop allocation worse.


## Disassembly

```asm
372d8:	27bdffe0 	addiu	sp,sp,-32
372dc:	e7b40000 	swc1	$f20,0(sp)
372e0:	afb00004 	sw	s0,4(sp)
372e4:	afb10008 	sw	s1,8(sp)
372e8:	afb2000c 	sw	s2,12(sp)
372ec:	afb30010 	sw	s3,16(sp)
372f0:	afbf0014 	sw	ra,20(sp)
372f4:	10a00019 	beqz	a1,0x3735c
372f8:	00a08025 	move	s0,a1
372fc:	02009825 	move	s3,s0
37300:	4480a000 	mtc1	zero,$f20
37304:	2411fffd 	li	s1,-3
37308:	8e720048 	lw	s2,72(s3)
3730c:	2664004c 	addiu	a0,s3,76
37310:	00002825 	move	a1,zero
37314:	0c06b59a 	jal	0x1ad668
37318:	34060010 	li	a2,0x10
3731c:	12400006 	beqz	s2,0x37338
37320:	00000000 	nop
37324:	8e440000 	lw	a0,0(s2)
37328:	e6540004 	swc1	$f20,4(s2)
3732c:	00809025 	move	s2,a0
37330:	5640fffd 	bnezl	s2,0x37328
37334:	8e440000 	lw	a0,0(s2)
37338:	0c00d04c 	jal	0x34130
3733c:	02602025 	move	a0,s3
37340:	96640040 	lhu	a0,64(s3)
37344:	8e650020 	lw	a1,32(s3)
37348:	00912024 	and	a0,a0,s1
3734c:	a6640040 	sh	a0,64(s3)
37350:	00a09825 	move	s3,a1
37354:	5670ffed 	bnel	s3,s0,0x3730c
37358:	8e720048 	lw	s2,72(s3)
3735c:	c7b40000 	lwc1	$f20,0(sp)
37360:	8fb00004 	lw	s0,4(sp)
37364:	8fb10008 	lw	s1,8(sp)
37368:	8fb2000c 	lw	s2,12(sp)
3736c:	8fb30010 	lw	s3,16(sp)
37370:	8fbf0014 	lw	ra,20(sp)
37374:	03e00008 	jr	ra
37378:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: eBroadphase::ClearAccumulatedImpulses(eCollisionPair *)
// Address: 0x000372d8, Size: 164B
// Obj: eAll_psp.obj

? eCollisionPair__RemoveContacts_void(void *);      /* extern */
? memset(void *, ?, ?);                             /* extern */

void eBroadphase__ClearAccumulatedImpulses_eCollisionPairptr(void *arg1) {
    void *var_s2;
    void *var_s3;

    if (arg1 != NULL) {
        var_s3 = arg1;
        do {
            var_s2 = var_s3->unk48;
            memset(var_s3 + 0x4C, 0, 0x10);
            if (var_s2 != NULL) {
                do {
                    var_s2->unk4 = 0.0f;
                    var_s2 = var_s2->unk0;
                } while (var_s2 != NULL);
            }
            eCollisionPair__RemoveContacts_void(var_s3);
            var_s3->unk40 = (u16) (var_s3->unk40 & ~2);
            var_s3 = var_s3->unk20;
        } while (var_s3 != arg1);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
