# Battle Packet: `cFactory::DeleteMarkedForClean(unsigned int, bool)`

## Target

- Address: `0x0000bc90`
- Size: `180` bytes
- Status: `failed`
- Object: `cAll_psp.obj`
- Class: `cFactory`
- Method family: `DeleteMarkedForClean`
- Leaf: `False`

## Queue Metadata

- score: `174`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 7B; codegen-specific diagnosis; manageable size`
- failure_action: `quarantine`
- near_miss_bytes: `7`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `7`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x0000b648` `untried` 196B `cFactory::CleanGroups(unsigned int, bool)`
- `0x0000bc90` `failed` 180B `cFactory::DeleteMarkedForClean(unsigned int, bool)`

Callers:
- `0x0000b5f8` `matched` 80B `cFactory::Clean(bool)`
- `0x0000bc90` `failed` 180B `cFactory::DeleteMarkedForClean(unsigned int, bool)`

## Placement

- Canonical source: `src/cFactory.cpp`
- Header(s): `include/cFactory.h`
- Allowed source(s): `src/cFactory.cpp`
- Split-TU prefix: `src/cFactory_*.cpp`

## Class Header: `include/cFactory.h`

Not found.

## Matched Same-Class Neighbors

- `0x0000ab4c` 76B `cFactory::Write(cFile &) const` — `src/cFactory.cpp`
- `0x0000ab98` 188B `cFactory::Read(cFile &, cMemPool *)` — `src/cFactory.cpp`
- `0x0000ac54` 152B `cFactory::Load(void)` — `src/cFactory.cpp`
- `0x0000b114` 296B `cFactory::CreateObject(const cType *, const cGUID &, bool, unsigned int, bool)` — `src/cFactory.cpp`
- `0x0000b23c` 160B `cFactory::CopyObject(const cObject *, const cGUID &)` — `src/cFactory.cpp`

## Prior Failure Notes

### Note 1

- session `287a7542`; src `src/cFactory.cpp`; snapshot `logs/failure_snapshots/match_20260505_232432/287a7542/0000bc90__agent_self_reported_failure__src_cFactory.cpp`

cFactory::DeleteMarkedForClean(unsigned int, bool) — 7 byte diff remains, all 1-byte register-encoding differences. Function structure matches expected exactly EXCEPT for s2/s3 saved-register swap in the loop: expected uses counter→s3 (HIGHER) and pointer→s2 (LOWER); mine uses counter→s2 (LOWER) and pointer→s3 (HIGHER). Diffs are in: li (counter init), slt×2 (count comparisons), move (pointer init from v0), lw (pointer deref), addiu×2 (counter increment, pointer +=4). All other instructions match including prologue/epilogue, vtable dispatch (offset 176), recursive call to DeleteMarkedForClean(child, param, false), and CleanGroups(0, flag) tail call.

Experiments tried: (1) variable declaration order swaps (i before/after groups); (2) declaring i with initial 0 vs uninitialized; (3) for-loop vs while-loop vs if+do-while; (4) putting count init before vs after dispatch entry computation. None changed the s2/s3 assignment. Permuter ran 150s, 1575 candidates, zero improvement.

The contrast with sibling cFactory::DeleteGroups (matched) is striking: DG has only 2 saved-reg loop variables (s0=counter, s1=ptr — counter LOWER), while DMFC has 5 saved regs total (s0=arg1, s1=this, s4=bool, s2/s3=loop vars). With s4 already taken before s2/s3 are needed, SNC apparently allocates the loop vars in a different order than for DG. The choice (counter→s3 vs counter→s2) appears compiler-deterministic with no source-level lever. May need to try entirely different loop structures (pointer-comparison loop, manually-decremented counter, recursion replaced with goto-based iteration) but those would be significant departures from clean C.


## Disassembly

```asm
bc90:	27bdffe0 	addiu	sp,sp,-32
bc94:	afb10008 	sw	s1,8(sp)
bc98:	00808825 	move	s1,a0
bc9c:	8e240004 	lw	a0,4(s1)
bca0:	afb00004 	sw	s0,4(sp)
bca4:	00a08025 	move	s0,a1
bca8:	afa00000 	sw	zero,0(sp)
bcac:	248400b0 	addiu	a0,a0,176
bcb0:	afb40014 	sw	s4,20(sp)
bcb4:	30d400ff 	andi	s4,a2,0xff
bcb8:	84850000 	lh	a1,0(a0)
bcbc:	8c860004 	lw	a2,4(a0)
bcc0:	02252021 	addu	a0,s1,a1
bcc4:	afb2000c 	sw	s2,12(sp)
bcc8:	afb30010 	sw	s3,16(sp)
bccc:	afbf0018 	sw	ra,24(sp)
bcd0:	00c0f809 	jalr	a2
bcd4:	03a02825 	move	a1,sp
bcd8:	8fa40000 	lw	a0,0(sp)
bcdc:	34130000 	li	s3,0x0
bce0:	0264202a 	slt	a0,s3,a0
bce4:	1080000b 	beqz	a0,0xbd14
bce8:	00409025 	move	s2,v0
bcec:	8e440000 	lw	a0,0(s2)
bcf0:	10800003 	beqz	a0,0xbd00
bcf4:	02002825 	move	a1,s0
bcf8:	0c002f24 	jal	0xbc90
bcfc:	00003025 	move	a2,zero
bd00:	8fa40000 	lw	a0,0(sp)
bd04:	26730001 	addiu	s3,s3,1
bd08:	0264202a 	slt	a0,s3,a0
bd0c:	1480fff7 	bnez	a0,0xbcec
bd10:	26520004 	addiu	s2,s2,4
bd14:	02202025 	move	a0,s1
bd18:	00002825 	move	a1,zero
bd1c:	0c002d92 	jal	0xb648
bd20:	02803025 	move	a2,s4
bd24:	8fb00004 	lw	s0,4(sp)
bd28:	8fb10008 	lw	s1,8(sp)
bd2c:	8fb2000c 	lw	s2,12(sp)
bd30:	8fb30010 	lw	s3,16(sp)
bd34:	8fb40014 	lw	s4,20(sp)
bd38:	8fbf0018 	lw	ra,24(sp)
bd3c:	03e00008 	jr	ra
bd40:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: cFactory::DeleteMarkedForClean(unsigned int, bool)
// Address: 0x0000bc90, Size: 180B
// Obj: cAll_psp.obj

? cFactory__CleanGroups_unsignedint_bool(void *, ?, s32); /* extern */

void cFactory__DeleteMarkedForClean_unsignedint_bool(void *arg0, ? arg1, s32 arg2) {
    s32 var_s3;
    void **(*temp_a2)(void *, void *, ?);
    void **var_s2;
    void *temp_a0;
    void *temp_a0_2;

    temp_a0 = arg0->unk4;
    temp_a2 = (temp_a0 + 0xB0)->unk4;
    var_s3 = 0;
    var_s2 = temp_a2(arg0 + temp_a0->unkB0, sp, temp_a2);
    if (subroutine_arg0 > 0) {
        do {
            temp_a0_2 = *var_s2;
            if (temp_a0_2 != NULL) {
                cFactory__DeleteMarkedForClean_unsignedint_bool(temp_a0_2, arg1, 0);
            }
            var_s3 += 1;
            var_s2 += 4;
        } while (var_s3 < subroutine_arg0);
    }
    cFactory__CleanGroups_unsignedint_bool(arg0, 0, arg2 & 0xFF);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
