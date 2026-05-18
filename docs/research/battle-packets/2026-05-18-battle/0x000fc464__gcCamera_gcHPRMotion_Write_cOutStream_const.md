# Battle Packet: `gcCamera::gcHPRMotion::Write(cOutStream &) const`

## Target

- Address: `0x000fc464`
- Size: `228` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcCamera::gcHPRMotion`
- Method family: `Write`
- Leaf: `False`

## Queue Metadata

- score: `155`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 27B; codegen-specific diagnosis; Write retry family; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `27`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `27`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00006454` `failed` 412B `cOutStream::Write(float, bool)`
- `0x000fba84` `failed` 92B `gcCamera::gcMotionLimits::Write(cOutStream &) const`

Callers:
- `0x000fca74` `failed` 780B `gcCamera::gcEntityFollowState::Write(cOutStream &) const`
- `0x000fd6ac` `failed` 596B `gcCamera::gcState::Write(cOutStream &) const`

## Placement

- Canonical source: `src/gcCamera_gcHPRMotion.cpp`
- Header(s): `include/gcCamera_gcHPRMotion.h`
- Allowed source(s): `src/gcCamera_gcHPRMotion.cpp`
- Split-TU prefix: `src/gcCamera_gcHPRMotion_*.cpp`

## Compiler Guidance

Write template: preserve cWriteBlock/cOutStream construction, field write order, and End() placement. SNC is sensitive to temporary bool/cast shapes and to independent store ordering.

## Class Header: `include/gcCamera::gcHPRMotion.h`

Not found.

## Matched Method Exemplars

### Exemplar 1: `gcMsgLoadRegions::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const`

- Address: `0x00124c2c`
- Size: `228` bytes
- Source: `src/gcMsgLoadRegions.cpp`

```cpp
void gcMsgLoadRegions::Write(cOutStream &stream, nwSocketHandle,
                             const nwAddress &, nwConnectionHandle) const {
    int i = 0;
    const cGUIDT<gcRegion> *region = &mRegions[0];
    do {
        region->Write(stream);
        i++;
        region++;
    } while (i < 2);

    int value = mFlag & 0xFF;
    int bitPos = stream.mBitPos;
    int bit = bitPos & 7;
    unsigned char *out = stream.mData + (bitPos >> 3);
    int nextBit = bitPos + 1;
    unsigned char overflow = stream.mOverflow;
    stream.mBitPos = nextBit;

    if (overflow == 0) {
        if (stream.mCapacity < ((stream.mBitPos + 7) >> 3)) {
            stream.mOverflow = 1;
            overflow = stream.mOverflow;
        }
    }

    unsigned int writable = overflow < 1U;

    if ((writable & 0xFF) != 0) {
        unsigned char old = *out;
        int mask = 1 << bit;
        *out = (old & ~mask) | ((value != 0) << bit);
    }
}
```

### Exemplar 2: `gcTimer::Write(cFile &) const`

- Address: `0x0012c1b0`
- Size: `228` bytes
- Source: `src/gcTimer.cpp`

```cpp
void gcTimer::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cObject::Write(file);
    wb.Write(*(const unsigned int *)((const char *)this + 0xB4));
    wb.Write(*(const int *)((const char *)this + 0xBC));

    TypeMethod *slot0 = (TypeMethod *)((char *)*(void **)((char *)this + 0x48) + 0x28);
    cBase *base0 = (cBase *)((char *)this + 0x44);
    slot0->fn((cBase *)((char *)base0 + slot0->offset), wb._file);

    TypeMethod *slot1 = (TypeMethod *)((char *)*(void **)((char *)this + 0x64) + 0x28);
    cBase *base1 = (cBase *)((char *)this + 0x60);
    slot1->fn((cBase *)((char *)base1 + slot1->offset), wb._file);

    TypeMethod *slot2 = (TypeMethod *)((char *)*(void **)((char *)this + 0x80) + 0x28);
    cBase *base2 = (cBase *)((char *)this + 0x7C);
    slot2->fn((cBase *)((char *)base2 + slot2->offset), wb._file);

    TypeMethod *slot3 = (TypeMethod *)((char *)*(void **)((char *)this + 0x9C) + 0x28);
    cBase *base3 = (cBase *)((char *)this + 0x98);
    slot3->fn((cBase *)((char *)base3 + slot3->offset), wb._file);

    wb.End();
}
```

### Exemplar 3: `gcMsgAttachEntity::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const`

- Address: `0x001356f4`
- Size: `228` bytes
- Source: `src/gcMsgAttachEntity.cpp`

```cpp
void gcMsgAttachEntity::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    mHandle1.Write(s);
    mHandle2.Write(s);
    bool sign = true;
    int n = ((int)mShort << 16) >> 16;
    ((cOutStreamRef *)&s)->Write(n, 9, sign);
    ((cOutStreamBit *)&s)->WriteBit(mFlag);
}
```

### Exemplar 4: `gcMsgRequestPlayerId::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const`

- Address: `0x001336a4`
- Size: `224` bytes
- Source: `src/gcMsgRequestPlayerId.cpp`

```cpp
void gcMsgRequestPlayerId::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    ((cOutStreamRef *)&s)->Write(mNum, 5, true);
    int len = cStrLength((const char *)&mName[0]) & 0xFFFF;
    {
        unsigned int len_arg = (unsigned int)(len & 0xFFFF);
        bool sign2 = true;
        ((cOutStreamRef *)&s)->Write(len_arg, 0x10, sign2);
    }
    for (int i = 0; i < len; i++) {
        bool sign = false;
        signed char v = mName[i];
        int iv = v;
        ((cOutStreamRef *)&s)->Write((signed char)iv, 8, sign);
    }
    ((cOutStreamRef *)&s)->Write(mF, true);
    ((cOutStreamRef *)&s)->Write((unsigned int)mU, 0x20, true);
}
```


## Prior Failure Notes

### Note 1

- session `17cb0e3a`; src `src/gcCamera_gcHPRMotion.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/17cb0e3a/000fc464__agent_self_reported_failure__src_gcCamera_gcHPRMotion.cpp`

27-byte mismatch in 228B function after permuter ran. Function structure (3 sequential float-write loops + gcMotionLimits::Write) is correct. Best version uses three block-local `bool sign = true` blocks and three function-scope pointers `p1`, `p2`, `p3` declared (after permuter shuffle) as p2 first, then p3, p1. This produces: this=s0, stream=s1 (correct), but pointers in s2/s3/s4 instead of target's s4/s5/s6. Sign and counter end up scattered across loops since they're block-local.

The function-scope-everything alternative (decl order p3,p2,p1,i,sign) produces: pointers in s4/s5/s6 (correct) but sign=s3, counter=s2 (target wants sign=s2, counter=s3) AND this/stream get swapped to s1/s0. So there's a fundamental tradeoff between the two divergent allocator decisions.

Root cause: SNC's register allocator has TWO independent decisions diverging from target. (1) When sign+counter are FUNCTION-scope, counter has more uses than sign so it gets the LOWER s-num — but target has them inverted. (2) Whether this/stream end up in s0/s1 vs s1/s0 depends on whether locals are block- or function-scoped.

Need: this=s0, stream=s1, sign=s2, counter=s3, p1=s4, p2=s5, p3=s6. Tried block-local sign+counter, function-scope sign+counter, hybrids, do-while vs for, decl order permutations, `bool sign = true` initialized at decl, and 90s of permuter (29→27). Permuter improvement was structural (declaration reorder), not the s-num swap that's needed.

Next retry idea: structural change — maybe an inlined helper `inline void writeAxis(cOutStream &s, const float *p)` taking the pointer as parameter. The inlined parameter `p` might allocate differently; sign as param could land in s2.

But also: the prior agent (5 attempts) and this attempt (3 attempts) both converged on the same wall. This may need a -Xmopt or -Xxopt flag tweak rather than source rewrite, OR may be a candidate for unmatchable classification once it can be shown that the diff is purely register-renaming (no semantic divergence).

### Note 2

- session `57559f4f`

I created a dedicated `src/gcCamera_gcHPRMotion.cpp` and iterated through several source shapes for `gcCamera::gcHPRMotion::Write(cOutStream &) const`: a straight three-loop pointer walk, `register`-qualified locals, empty-asm pinning for the sign/counter/pointer locals, and local-order reshuffles intended to coerce SNC’s saved-register allocation. I also briefly tried hosting the method next to `gcScalarMotion`, but moved it into its own translation unit to keep the iteration stable and inspectable.

Those attempts converged on the correct high-level structure every time: three 3-iteration float write loops starting at offsets `0x00`, `0x10`, and `0x20`, followed by `gcMotionLimits::Write` at `0x30`. The remaining mismatch is saved-register assignment rather than control flow. The best build still keeps `true`/counter/pointers in `s6/s5/s4/s3/s2` and stores `this`/stream in `s1/s0`, while the target wants `s2/s3/s4/s5/s6` and `s0/s1`. That changes the prologue/epilogue and every loop-body operand even though the instruction pattern is otherwise the same. The next retry should keep working in `src/gcCamera_gcHPRMotion.cpp` and focus specifically on SNC register-pressure shaping for `sign`, the loop counter, and the `this`/stream aliases.


## Disassembly

```asm
fc464:	27bdffe0 	addiu	sp,sp,-32
fc468:	afb00000 	sw	s0,0(sp)
fc46c:	afb10004 	sw	s1,4(sp)
fc470:	afb20008 	sw	s2,8(sp)
fc474:	afb3000c 	sw	s3,12(sp)
fc478:	afb40010 	sw	s4,16(sp)
fc47c:	afb50014 	sw	s5,20(sp)
fc480:	afb60018 	sw	s6,24(sp)
fc484:	34120001 	li	s2,0x1
fc488:	34130000 	li	s3,0x0
fc48c:	0080a025 	move	s4,a0
fc490:	24950010 	addiu	s5,a0,16
fc494:	24960020 	addiu	s6,a0,32
fc498:	00808025 	move	s0,a0
fc49c:	00a08825 	move	s1,a1
fc4a0:	afbf001c 	sw	ra,28(sp)
fc4a4:	c68c0000 	lwc1	$f12,0(s4)
fc4a8:	02202025 	move	a0,s1
fc4ac:	0c001915 	jal	0x6454
fc4b0:	02402825 	move	a1,s2
fc4b4:	26730001 	addiu	s3,s3,1
fc4b8:	2a640003 	slti	a0,s3,3
fc4bc:	1480fff9 	bnez	a0,0xfc4a4
fc4c0:	26940004 	addiu	s4,s4,4
fc4c4:	34120001 	li	s2,0x1
fc4c8:	34130000 	li	s3,0x0
fc4cc:	c6ac0000 	lwc1	$f12,0(s5)
fc4d0:	02202025 	move	a0,s1
fc4d4:	0c001915 	jal	0x6454
fc4d8:	02402825 	move	a1,s2
fc4dc:	26730001 	addiu	s3,s3,1
fc4e0:	2a640003 	slti	a0,s3,3
fc4e4:	1480fff9 	bnez	a0,0xfc4cc
fc4e8:	26b50004 	addiu	s5,s5,4
fc4ec:	34120001 	li	s2,0x1
fc4f0:	34130000 	li	s3,0x0
fc4f4:	c6cc0000 	lwc1	$f12,0(s6)
fc4f8:	02202025 	move	a0,s1
fc4fc:	0c001915 	jal	0x6454
fc500:	02402825 	move	a1,s2
fc504:	26730001 	addiu	s3,s3,1
fc508:	2a640003 	slti	a0,s3,3
fc50c:	1480fff9 	bnez	a0,0xfc4f4
fc510:	26d60004 	addiu	s6,s6,4
fc514:	26040030 	addiu	a0,s0,48
fc518:	0c03eea1 	jal	0xfba84
fc51c:	02202825 	move	a1,s1
fc520:	8fb00000 	lw	s0,0(sp)
fc524:	8fb10004 	lw	s1,4(sp)
fc528:	8fb20008 	lw	s2,8(sp)
fc52c:	8fb3000c 	lw	s3,12(sp)
fc530:	8fb40010 	lw	s4,16(sp)
fc534:	8fb50014 	lw	s5,20(sp)
fc538:	8fb60018 	lw	s6,24(sp)
fc53c:	8fbf001c 	lw	ra,28(sp)
fc540:	03e00008 	jr	ra
fc544:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcCamera::gcHPRMotion::Write(cOutStream &) const
// Address: 0x000fc464, Size: 228B
// Obj: gcAll_psp.obj

? cOutStream__Write_float_bool(f32, ?, ?);          /* extern */
? gcCamera__gcMotionLimits__Write_cOutStreamrefconst(void *, ?); /* extern */

void gcCamera__gcHPRMotion__Write_cOutStreamrefconst(f32 *arg0, ? arg1) {
    f32 *var_s4;
    f32 *var_s5;
    f32 *var_s6;
    s32 var_s3;
    s32 var_s3_2;
    s32 var_s3_3;

    var_s3 = 0;
    var_s4 = arg0;
    var_s5 = arg0 + 0x10;
    var_s6 = arg0 + 0x20;
    do {
        cOutStream__Write_float_bool(*var_s4, arg1, 1);
        var_s3 += 1;
        var_s4 += 4;
    } while (var_s3 < 3);
    var_s3_2 = 0;
    do {
        cOutStream__Write_float_bool(*var_s5, arg1, 1);
        var_s3_2 += 1;
        var_s5 += 4;
    } while (var_s3_2 < 3);
    var_s3_3 = 0;
    do {
        cOutStream__Write_float_bool(*var_s6, arg1, 1);
        var_s3_3 += 1;
        var_s6 += 4;
    } while (var_s3_3 < 3);
    gcCamera__gcMotionLimits__Write_cOutStreamrefconst(arg0 + 0x30, arg1);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
