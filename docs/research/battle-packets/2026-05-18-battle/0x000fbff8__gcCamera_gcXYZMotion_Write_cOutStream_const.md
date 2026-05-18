# Battle Packet: `gcCamera::gcXYZMotion::Write(cOutStream &) const`

## Target

- Address: `0x000fbff8`
- Size: `228` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcCamera::gcXYZMotion`
- Method family: `Write`
- Leaf: `False`

## Queue Metadata

- score: `170`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 16B; correct function size; codegen-specific diagnosis; Write retry family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `16`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `16`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00006454` `failed` 412B `cOutStream::Write(float, bool)`
- `0x000fba84` `failed` 92B `gcCamera::gcMotionLimits::Write(cOutStream &) const`

Callers:
- `0x000f8ad8` `failed` 560B `gcCamera::Write(cOutStream &) const`

## Placement

- Canonical source: `src/gcCamera_gcXYZMotion.cpp`
- Header(s): `include/gcCamera_gcXYZMotion.h`
- Allowed source(s): `src/gcCamera_gcXYZMotion.cpp`
- Split-TU prefix: `src/gcCamera_gcXYZMotion_*.cpp`

## Compiler Guidance

Write template: preserve cWriteBlock/cOutStream construction, field write order, and End() placement. SNC is sensitive to temporary bool/cast shapes and to independent store ordering.

## Class Header: `include/gcCamera::gcXYZMotion.h`

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

- session `71f03ff2`

Recreated the missing canonical source file and improved the prior 37-byte register-allocation mismatch to a stable 16/228-byte mismatch. The current source uses three C++ pointer loops and normal cOutStream::Write(float,bool) calls. All loop bodies, call sites, epilogue, and second/third loop reset order match; the remaining bytes are only the independent initial setup order. Built output initializes z/y/x/i/sign/out/self as addiu/addiu/move/li/li/move/move, while the target schedules sign/i/x/y/z/self/out as li/li/move/addiu/addiu/move/move with the same registers.

Tried explicit local register bindings, bool/int sign variants, empty-asm live-set ordering, extern-C callee shims, and small initializer asm blocks. The asm-block variants either introduced bool normalization, changed relocation names/register allocation, or regressed to 47+ bytes. Ran tools/permuter.py for 300 seconds with --save-best: 7,536 candidates generated, 3,981 compiled, zero improvements; best stayed at 16 bytes.

### Note 2

- session `2f24cd53`

Implemented `gcCamera::gcXYZMotion::Write(cOutStream &) const` as three 3-float write loops followed by `gcMotionLimits::Write`, using a new canonical sibling file `src/gcCamera_gcXYZMotion.cpp`. I tried three source shapes: a straightforward pointer-loop version, a pressure-increased variant with extra live temporaries / barriers, and an explicit-register variant. The best result is the current plain version: it compiles to the correct 228-byte size and the loop/control-flow structure matches the target, but `compare_func.py` still reports `MISMATCH: 37/228 bytes differ`.

Disassembly of the best build shows SNC collapsing the live ranges differently from the target. The original keeps `this`, `out`, and all three vector pointers in `s0/s1/s4/s5/s6` with `s2=1` and `s3` as the shared loop counter; our best build still reuses fewer saved registers and recomputes or reassigns some loop state, so the byte diff looks like register-allocation/live-range divergence rather than a semantic mismatch. Next retry should keep attacking pointer lifetime and counter/sign placement around the three loops.


## Disassembly

```asm
fbff8:	27bdffe0 	addiu	sp,sp,-32
fbffc:	afb00000 	sw	s0,0(sp)
fc000:	afb10004 	sw	s1,4(sp)
fc004:	afb20008 	sw	s2,8(sp)
fc008:	afb3000c 	sw	s3,12(sp)
fc00c:	afb40010 	sw	s4,16(sp)
fc010:	afb50014 	sw	s5,20(sp)
fc014:	afb60018 	sw	s6,24(sp)
fc018:	34120001 	li	s2,0x1
fc01c:	34130000 	li	s3,0x0
fc020:	0080a025 	move	s4,a0
fc024:	24950010 	addiu	s5,a0,16
fc028:	24960020 	addiu	s6,a0,32
fc02c:	00808025 	move	s0,a0
fc030:	00a08825 	move	s1,a1
fc034:	afbf001c 	sw	ra,28(sp)
fc038:	c68c0000 	lwc1	$f12,0(s4)
fc03c:	02202025 	move	a0,s1
fc040:	0c001915 	jal	0x6454
fc044:	02402825 	move	a1,s2
fc048:	26730001 	addiu	s3,s3,1
fc04c:	2a640003 	slti	a0,s3,3
fc050:	1480fff9 	bnez	a0,0xfc038
fc054:	26940004 	addiu	s4,s4,4
fc058:	34120001 	li	s2,0x1
fc05c:	34130000 	li	s3,0x0
fc060:	c6ac0000 	lwc1	$f12,0(s5)
fc064:	02202025 	move	a0,s1
fc068:	0c001915 	jal	0x6454
fc06c:	02402825 	move	a1,s2
fc070:	26730001 	addiu	s3,s3,1
fc074:	2a640003 	slti	a0,s3,3
fc078:	1480fff9 	bnez	a0,0xfc060
fc07c:	26b50004 	addiu	s5,s5,4
fc080:	34120001 	li	s2,0x1
fc084:	34130000 	li	s3,0x0
fc088:	c6cc0000 	lwc1	$f12,0(s6)
fc08c:	02202025 	move	a0,s1
fc090:	0c001915 	jal	0x6454
fc094:	02402825 	move	a1,s2
fc098:	26730001 	addiu	s3,s3,1
fc09c:	2a640003 	slti	a0,s3,3
fc0a0:	1480fff9 	bnez	a0,0xfc088
fc0a4:	26d60004 	addiu	s6,s6,4
fc0a8:	26040030 	addiu	a0,s0,48
fc0ac:	0c03eea1 	jal	0xfba84
fc0b0:	02202825 	move	a1,s1
fc0b4:	8fb00000 	lw	s0,0(sp)
fc0b8:	8fb10004 	lw	s1,4(sp)
fc0bc:	8fb20008 	lw	s2,8(sp)
fc0c0:	8fb3000c 	lw	s3,12(sp)
fc0c4:	8fb40010 	lw	s4,16(sp)
fc0c8:	8fb50014 	lw	s5,20(sp)
fc0cc:	8fb60018 	lw	s6,24(sp)
fc0d0:	8fbf001c 	lw	ra,28(sp)
fc0d4:	03e00008 	jr	ra
fc0d8:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcCamera::gcXYZMotion::Write(cOutStream &) const
// Address: 0x000fbff8, Size: 228B
// Obj: gcAll_psp.obj

? cOutStream__Write_float_bool(f32, ?, ?);          /* extern */
? gcCamera__gcMotionLimits__Write_cOutStreamrefconst(void *, ?); /* extern */

void gcCamera__gcXYZMotion__Write_cOutStreamrefconst(f32 *arg0, ? arg1) {
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
