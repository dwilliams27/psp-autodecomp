# Battle Packet: `cOutStream::Write(int, int, bool)`

## Target

- Address: `0x0000616c`
- Size: `372` bytes
- Status: `failed`
- Object: `cAll_psp.obj`
- Class: `cOutStream`
- Method family: `Write`
- Leaf: `False`

## Queue Metadata

- score: `131`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 48B; codegen-specific diagnosis; Write retry family; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `48`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `needs_layout`
- Near miss: `48`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000068c4` `failed` 296B `cOutStream::WriteBits(const void *, int)`

Callers:
- `0x000065f0` `failed` 504B `cOutStream::Write(float, int, int, bool)`
- `0x00008cf4` `failed` 152B `cName::Write(cOutStream &) const`
- `0x0000d418` `matched` 44B `cTimeValue::Write(cOutStream &) const`
- `0x000ec27c` `matched` 152B `gcCinematicInstance::Write(cOutStream &)`
- `0x000f3b50` `untried` 756B `gcMap::Replicate(gcReplicationVisitor *)`
- `0x000f5db8` `untried` 656B `gcMap::WriteDynamicallyLoadedObjects(cOutStream &)`
- `0x000f612c` `untried` 196B `gcMap::WriteDynamicallyLoadedObjectsToLoad(cOutStream &)`
- `0x000f655c` `untried` 2120B `gcMap::MemCardReplicateStreamedCinematic(gcReplicationVisitor &)`
- `0x000fca74` `failed` 780B `gcCamera::gcEntityFollowState::Write(cOutStream &) const`
- `0x000fd6ac` `failed` 596B `gcCamera::gcState::Write(cOutStream &) const`
- `0x00100d64` `untried` 312B `gcProfile::WriteVariables(cOutStream &, const cArray<cArray<float> > &) static`
- `0x0010141c` `untried` 400B `gcProfile::WriteStrings(cOutStream &, const cArray<cArray<cStrW> > &) static`
- `0x00101718` `untried` 564B `gcProfile::WriteTables(cOutStream &, const cArray<cHandleT<gcTableTemplate> > &) static`
- `0x00118d48` `untried` 576B `gcEntity::Post(bool, gcEntity::gcDesiredStateMachine, const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &, const gcEventParams &, const gcEventStackData &)`
- `0x001206b8` `untried` 1508B `gcPlayer::Replicate(gcReplicationVisitor *)`
- `0x0012bd14` `failed` 436B `gcTableTemplate::Write(cOutStream &) const`
- `0x001336a4` `matched` 224B `gcMsgRequestPlayerId::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const`
- `0x00133940` `matched` 212B `gcMsgAssignPlayerId::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const`
- `0x00133bd0` `failed` 52B `gcMsgConnected::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const`
- `0x00133c54` `failed` 468B `gcMsgEntityMoveTo::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const`
- `0x00135558` `matched` 108B `gcMsgAssignEntityControl::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const`
- `0x001356f4` `matched` 228B `gcMsgAttachEntity::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const`
- `0x001421c8` `untried` 1324B `gcBipedController::WriteMovement(cOutStream &, bool) const`
- `0x00162c0c` `untried` 3448B `gcRegionBase::Replicate(gcReplicationVisitor *)`

## Placement

- Canonical source: `src/cOutStream.cpp`
- Header(s): `include/cOutStream.h`
- Allowed source(s): `src/cOutStream.cpp`
- Split-TU prefix: `src/cOutStream_*.cpp`

## Compiler Guidance

Write template: preserve cWriteBlock/cOutStream construction, field write order, and End() placement. SNC is sensitive to temporary bool/cast shapes and to independent store ordering.

## Class Header: `include/cOutStream.h`

```cpp
#pragma once

// cOutStream: bitwise output stream (24 bytes, 0x18)
// Confirmed from cOutStream::Write (0x000065f0) and cOutStream::WriteBits (0x000068c4):
//   offset 0x00: lw — pointer to data buffer (unsigned char *)
//   offset 0x04: lw — capacity in bytes
//   offset 0x08: lw/sw — current bit position
//   offset 0x0C: lw/sw — CRC accumulator
// Confirmed from cOutStream::GetCRC (0x00006118):
//   offset 0x0C: CRC value
//   offset 0x10: lbu — dirty flag (CRC needs recalc)
//   offset 0x14: lw — saved bit position for CRC range
// Confirmed from cOutStream::WriteRaw (0x000067e8):
//   offset 0x12: lbu/sb — overflow flag
struct cOutStream {
    unsigned char *mData;    // offset 0x00 — data buffer
    int mCapacity;           // offset 0x04 — buffer size in bytes
    int mBitPos;             // offset 0x08 — current write position in bits
    int mCRC;                // offset 0x0C — CRC accumulator
    unsigned char mDirty;    // offset 0x10 — CRC dirty flag
    char _pad11;             // offset 0x11
    unsigned char mOverflow; // offset 0x12 — overflow flag
    char _pad13;             // offset 0x13
    int mCRCBitPos;          // offset 0x14 — bit position for CRC calc

    int GetCRC(void) const;
    void Write(int, int, bool);
    void Write(unsigned int, int, bool);
    void Write(float, bool);
    void Write(float, int, int, bool);
    void WriteRaw(int, const void *);
    void WriteBits(const void *, int);
};
```

## Matched Method Exemplars

### Exemplar 1: `cWriteBlock::Write(int, const int *)`

- Address: `0x00005930`
- Size: `208` bytes
- Source: `src/cWriteBlock.cpp`

```cpp
void cWriteBlock::Write(bool data) {
    unsigned char tmp = (unsigned int)data;
    cFileSystem::Write(mFile->mHandle, &tmp, 1);
}
```

### Exemplar 2: `cConfigPSP::Write(cFile &) const`

- Address: `0x000087b4`
- Size: `188` bytes
- Source: `src/cConfigPSP.cpp`

```cpp
// cConfigPSP::Write(cFile &) const @ 0x000087b4
void cConfigPSP::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    cConfigBase::Write(file);
    wb.Write(8);
    wb.Write(10);
    int i = 0;
    const char *name = (const char *)this + 0x244;
    do {
        wb.Write(10, name);
        i += 1;
        name += 10;
    } while (i < 8);
    wb.Write(8, (const int *)((const char *)this + 0x294));
    wb.Write(*(const int *)((const char *)this + 0x2B4));
    wb.Write(false);
    wb.End();
}
```

### Exemplar 3: `cWriteBlock::Write(int, const short *)`

- Address: `0x00005a1c`
- Size: `172` bytes
- Source: `src/cWriteBlock.cpp`

```cpp
void cWriteBlock::Write(bool data) {
    unsigned char tmp = (unsigned int)data;
    cFileSystem::Write(mFile->mHandle, &tmp, 1);
}
```

### Exemplar 4: `cGroup::Write(cFile &) const`

- Address: `0x0000c18c`
- Size: `160` bytes
- Source: `src/cGroup.cpp`

```cpp
void cGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    cGroupNode *node = mList;
    if (node != 0) {
        do {
            int write = node->flags & 4;
            write = write != 0;
            write = (unsigned char)write;
            if (write != 0) {
                wb.Write(true);
                node->handle.Write(wb);
            }
            cGroupNode *nextNode = 0;
            cGroupNode *next = node->next;
            if (next != mList) {
                nextNode = next;
            }
            node = nextNode;
        } while (node != 0);
    }
    wb.Write(false);
    wb.End();
}
```


## Prior Failure Notes

### Note 1

- session `b81b3ed1`; src `src/cOutStream.cpp`; snapshot `logs/failure_snapshots/match_20260506_235212/b81b3ed1/0000616c__agent_self_reported_failure__src_cOutStream.cpp`

Implemented cOutStream::Write(int, int, bool) as real C++ with the expected stack-addressable value fallback, sign && numBits >= 3 fast path, sentinel detection for value 0/1, one inlined sentinel-bit write, optional second inlined bit write for value == 1, and fallback to WriteBits(&value, numBits). Manual tuning changed the first bit-write temporaries to expose mBitPos, mData, mOverflow, the byte pointer, and the bool-normalized bit value in the target order; it reduced the prior-session 145-byte best to a 48-byte mismatch, and the project permuter (`python3 tools/permuter.py src/cOutStream.cpp 0x0000616c --time 300 --save-best`) improved that to 25 bytes and saved the current closest source.

The remaining diff is concentrated in the first fast-path setup and bit store. Target loads mBitPos into t0, computes the value==1 xori in a3, loads mData after the sentinel OR, computes bit in a3 and byte index in t3, then uses t1 for the normalized sentinel bit. The closest source still assigns mBitPos to a3, uses t0 for the xori/base pointer path, swaps some mData/sentinel setup order, and carries those register choices into the first sllv/sltu sequence. The second inlined bit write is essentially structurally aligned after the saved best. This looks like SNC register-allocation/scheduling context drift in a no-callee-save leaf-ish method rather than a high-level logic problem; sched=1 was tested and reduced part of the body diff but broke the prologue save order, so it is not a match.

### Note 2

- session `af7ab555`

Implemented `cOutStream::Write(int, int, bool)` as real C++ with the expected fast path: `sign && numBits >= 3`, sentinel detection for `value == 0 || value == 1`, one manual bit write for the sentinel flag, optional second bit for `value == 1`, and fallback to `WriteBits(&value, numBits)`. I tried several source shapes for the overflow check and bit write: direct `if (!mOverflow)` form, explicit `overflow` / `canWrite` temporaries, `unsigned char` normalization for the sentinel byte, different local declaration orders (`bit`, `bytePos`, `newBitPos`, pointer order), and empty memory barriers to influence scheduling. I also ran `python3 tools/permuter.py src/cOutStream.cpp 0x0000616c --time 180 --save-best`, which improved the best diff from 161 differing bytes to 145 but did not get materially closer after 180 seconds.

The remaining problem is not the high-level logic; the generated code is structurally close but SNC still emits the wrong register allocation and instruction ordering in the first fast-path block. The hardest misses are around the first overflow check and sentinel-bit write: our build still schedules `mBitPos` / `mOverflow` / pointer setup differently from the target and keeps a different live-register layout through the `andi/sltiu/beqz` sequence. The second fast-path block got closer and produces the expected branch-likely shape, but function 1 still needs a source form that preserves the target’s exact first-block ordering.


## Disassembly

```asm
616c:	27bdfff0 	addiu	sp,sp,-16
6170:	afa50000 	sw	a1,0(sp)
6174:	30e700ff 	andi	a3,a3,0xff
6178:	afbf0004 	sw	ra,4(sp)
617c:	10e00052 	beqz	a3,0x62c8
6180:	00c02825 	move	a1,a2
6184:	28a60003 	slti	a2,a1,3
6188:	14c00050 	bnez	a2,0x62cc
618c:	00a03025 	move	a2,a1
6190:	8fa60000 	lw	a2,0(sp)
6194:	8c880008 	lw	t0,8(a0)
6198:	38c70001 	xori	a3,a2,0x1
619c:	2cca0001 	sltiu	t2,a2,1
61a0:	2ce60001 	sltiu	a2,a3,1
61a4:	01465025 	or	t2,t2,a2
61a8:	8c860000 	lw	a2,0(a0)
61ac:	31070007 	andi	a3,t0,0x7
61b0:	000858c3 	sra	t3,t0,0x3
61b4:	25020001 	addiu	v0,t0,1
61b8:	00cb4021 	addu	t0,a2,t3
61bc:	90860012 	lbu	a2,18(a0)
61c0:	314900ff 	andi	t1,t2,0xff
61c4:	14c0000b 	bnez	a2,0x61f4
61c8:	ac820008 	sw	v0,8(a0)
61cc:	8c8b0008 	lw	t3,8(a0)
61d0:	8c820004 	lw	v0,4(a0)
61d4:	256b0007 	addiu	t3,t3,7
61d8:	000b58c3 	sra	t3,t3,0x3
61dc:	004b582a 	slt	t3,v0,t3
61e0:	11600005 	beqz	t3,0x61f8
61e4:	2cc60001 	sltiu	a2,a2,1
61e8:	34060001 	li	a2,0x1
61ec:	a0860012 	sb	a2,18(a0)
61f0:	90860012 	lbu	a2,18(a0)
61f4:	2cc60001 	sltiu	a2,a2,1
61f8:	30c600ff 	andi	a2,a2,0xff
61fc:	10c00009 	beqz	a2,0x6224
6200:	34060001 	li	a2,0x1
6204:	910b0000 	lbu	t3,0(t0)
6208:	00e63004 	sllv	a2,a2,a3
620c:	00c03027 	nor	a2,a2,zero
6210:	0009482b 	sltu	t1,zero,t1
6214:	01663024 	and	a2,t3,a2
6218:	00e93804 	sllv	a3,t1,a3
621c:	00c73025 	or	a2,a2,a3
6220:	a1060000 	sb	a2,0(t0)
6224:	11400029 	beqz	t2,0x62cc
6228:	00a03025 	move	a2,a1
622c:	8fa50000 	lw	a1,0(sp)
6230:	8c860008 	lw	a2,8(a0)
6234:	38a50001 	xori	a1,a1,0x1
6238:	2ca50001 	sltiu	a1,a1,1
623c:	30a800ff 	andi	t0,a1,0xff
6240:	8c850000 	lw	a1,0(a0)
6244:	30c70007 	andi	a3,a2,0x7
6248:	000648c3 	sra	t1,a2,0x3
624c:	24ca0001 	addiu	t2,a2,1
6250:	90860012 	lbu	a2,18(a0)
6254:	310800ff 	andi	t0,t0,0xff
6258:	00a92821 	addu	a1,a1,t1
625c:	14c0000b 	bnez	a2,0x628c
6260:	ac8a0008 	sw	t2,8(a0)
6264:	8c890008 	lw	t1,8(a0)
6268:	8c8a0004 	lw	t2,4(a0)
626c:	25290007 	addiu	t1,t1,7
6270:	000948c3 	sra	t1,t1,0x3
6274:	0149482a 	slt	t1,t2,t1
6278:	51200005 	beqzl	t1,0x6290
627c:	2cc40001 	sltiu	a0,a2,1
6280:	34060001 	li	a2,0x1
6284:	a0860012 	sb	a2,18(a0)
6288:	90860012 	lbu	a2,18(a0)
628c:	2cc40001 	sltiu	a0,a2,1
6290:	308400ff 	andi	a0,a0,0xff
6294:	1080000f 	beqz	a0,0x62d4
6298:	34040001 	li	a0,0x1
629c:	90a60000 	lbu	a2,0(a1)
62a0:	00e42004 	sllv	a0,a0,a3
62a4:	00802027 	nor	a0,a0,zero
62a8:	0008402b 	sltu	t0,zero,t0
62ac:	00c42024 	and	a0,a2,a0
62b0:	00e83004 	sllv	a2,t0,a3
62b4:	00862025 	or	a0,a0,a2
62b8:	a0a40000 	sb	a0,0(a1)
62bc:	8fbf0004 	lw	ra,4(sp)
62c0:	03e00008 	jr	ra
62c4:	27bd0010 	addiu	sp,sp,16
62c8:	00a03025 	move	a2,a1
62cc:	0c001a31 	jal	0x68c4
62d0:	03a02825 	move	a1,sp
62d4:	8fbf0004 	lw	ra,4(sp)
62d8:	03e00008 	jr	ra
62dc:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: cOutStream::Write(int, int, bool)
// Address: 0x0000616c, Size: 372B
// Obj: cAll_psp.obj

? cOutStream__WriteBits_constvoidptr_int(void *, s32, s32); /* extern */

void cOutStream__Write_int_int_bool(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_a2_2;
    s32 temp_a2_3;
    s32 temp_a3;
    s32 temp_t0;
    s32 temp_t2;
    s32 var_a2;
    s32 var_a3;
    u8 *temp_a1;
    u8 *temp_t0_2;
    u8 temp_a2;

    var_a3 = arg3 & 0xFF;
    if (var_a3 != 0) {
        if (arg2 >= 3) {
            temp_t0 = arg0->unk8;
            temp_t2 = (subroutine_arg0 == 0) | (subroutine_arg0 == 1);
            var_a3 = temp_t0 & 7;
            temp_t0_2 = arg0->unk0 + (temp_t0 >> 3);
            temp_a2 = arg0->unk12;
            arg0->unk8 = (s32) (temp_t0 + 1);
            if (temp_a2 == 0) {
                var_a2 = temp_a2 == 0;
                if (arg0->unk4 < ((s32) (arg0->unk8 + 7) >> 3)) {
                    arg0->unk12 = 1U;
                    goto block_5;
                }
            } else {
block_5:
                var_a2 = arg0->unk12 == 0;
            }
            if (var_a2 & 0xFF) {
                temp_a2_2 = *temp_t0_2 & ~(1 << var_a3);
                var_a3 = ((temp_t2 & 0xFF) != 0) << var_a3;
                *temp_t0_2 = temp_a2_2 | var_a3;
            }
            if (temp_t2 != 0) {
                temp_a2_3 = arg0->unk8;
                temp_a3 = temp_a2_3 & 7;
                temp_a1 = arg0->unk0 + (temp_a2_3 >> 3);
                arg0->unk8 = (s32) (temp_a2_3 + 1);
                if ((arg0->unk12 == 0) && (arg0->unk4 < ((s32) (arg0->unk8 + 7) >> 3))) {
                    arg0->unk12 = 1U;
                }
                if ((arg0->unk12 == 0) & 0xFF) {
                    *temp_a1 = (*temp_a1 & ~(1 << temp_a3)) | ((((subroutine_arg0 == 1) & 0xFF & 0xFF) != 0) << temp_a3);
                }
            } else {
                goto block_15;
            }
        } else {
            goto block_15;
        }
    } else {
block_15:
        cOutStream__WriteBits_constvoidptr_int(sp, arg2, var_a3);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
