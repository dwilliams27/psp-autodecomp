# Battle Packet: `gcEntity::Write(cFile &) const`

## Target

- Address: `0x00113acc`
- Size: `644` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcEntity`
- Method family: `Write`
- Leaf: `False`

## Queue Metadata

- score: `167`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 12B; codegen-specific diagnosis; Write retry family`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `12`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `needs_layout`
- Near miss: `12`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00005458` `failed` 136B `cWriteBlock::cWriteBlock(cFile &, unsigned int)`
- `0x00005510` `matched` 44B `cWriteBlock::Write(char)`
- `0x0000553c` `matched` 44B `cWriteBlock::Write(unsigned char)`
- `0x00005690` `matched` 116B `cWriteBlock::Write(int)`
- `0x00005704` `matched` 116B `cWriteBlock::Write(unsigned int)`
- `0x00005b3c` `failed` 212B `cWriteBlock::Write(int, const float *)`
- `0x00005c90` `matched` 112B `cWriteBlock::End(void)`
- `0x00007244` `matched` 148B `cBaseArray::Write(cWriteBlock &) const`
- `0x00009030` `failed` 664B `cHandle::Write(cWriteBlock &) const`
- `0x00009b30` `matched` 92B `cObject::Write(cFile &) const`

## Placement

- Canonical source: `src/gcEntity.cpp`
- Header(s): `include/gcEntity.h`
- Allowed source(s): `src/gcEntity.cpp`, `src/gcEntity_Write.cpp`
- Split-TU prefix: `src/gcEntity_*.cpp`

## Compiler Guidance

Write template: preserve cWriteBlock/cOutStream construction, field write order, and End() placement. SNC is sensitive to temporary bool/cast shapes and to independent store ordering.

## Class Header: `include/gcEntity.h`

```cpp
#ifndef GCENTITY_H
#define GCENTITY_H

#include "cObject.h"
#include "mVec3.h"

class cBase;
class cMemPool;
class cType;
class gcPlayer;

class gcEntity : public cObject {
public:
    gcEntity(cBase *);
    gcEntity &operator=(const gcEntity &);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    static void OnFullscreenCinematicStarting(void);
    gcPlayer *GetPlayer(void) const;
    void SetInitialControllerIndex(int);
    void SetVelocity(const mVec3 &);
    void *GetSubGeomController(int);
    void GetVelocity(mVec3 *) const;
    void SetPreviousPrimaryController(bool);
    void SetPrimaryController(int, bool);
    float GetSoundFrequencyOffset(int) const;
    float GetSoundVolumeOffset(int) const;
};

#endif
```

## Matched Same-Class Neighbors

- `0x00114300` 316B `gcEntity::gcEntity(cBase *)` — `src/gcEntity.cpp`
- `0x00117498` 28B `gcEntity::SetVelocity(const mVec3 &)` — `src/gcEntity.cpp`
- `0x0011887c` 96B `gcEntity::GetSoundFrequencyOffset(int) const` — `src/gcEntity.cpp`
- `0x00118964` 96B `gcEntity::GetSoundVolumeOffset(int) const` — `src/gcEntity.cpp`
- `0x0011b238` 12B `gcEntity::SetInitialControllerIndex(int)` — `src/gcEntity_SetInitialControllerIndex.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcMap::Write(cFile &) const`

- Address: `0x000f0fcc`
- Size: `428` bytes
- Source: `src/gcMap.cpp`

```cpp
void gcMap::Write(cFile &file) const {
    cWriteBlock wb(file, 0x10);
    ((const gcRegionBase *)this)->Write(file);
    wb.Write(*(const unsigned int *)((const char *)this + 0x20C));
    ((const cBaseArray *)((const char *)this + 0x398))->Write(wb);

    {
        char *typePtr = *(char **)((const char *)this + 0x7C);
        TypeMethod *entry = (TypeMethod *)(typePtr + 0x28);
        char *base = (char *)this + 0x78;
        entry->fn(base + entry->offset, wb._file);
    }

    {
        char *typePtr = *(char **)((const char *)this + 0x98);
        TypeMethod *entry = (TypeMethod *)(typePtr + 0x28);
        char *base = (char *)this + 0x94;
        entry->fn(base + entry->offset, wb._file);
    }

    {
        char *typePtr = *(char **)((const char *)this + 0xB4);
        TypeMethod *entry = (TypeMethod *)(typePtr + 0x28);
        char *base = (char *)this + 0xB0;
        entry->fn(base + entry->offset, wb._file);
    }

    {
        char *typePtr = *(char **)((const char *)this + 0xD0);
        TypeMethod *entry = (TypeMethod *)(typePtr + 0x28);
        char *base = (char *)this + 0xCC;
        entry->fn(base + entry->offset, wb._file);
    }

    {
        char *typePtr = *(char **)((const char *)this + 0xEC);
        TypeMethod *entry = (TypeMethod *)(typePtr + 0x28);
        char *base = (char *)this + 0xE8;
        entry->fn(base + entry->offset, wb._file);
    }

    {
        char *typePtr = *(char **)((const char *)this + 0x124);
        TypeMethod *entry = (TypeMethod *)(typePtr + 0x28);
        char *base = (char *)this + 0x120;
        entry->fn(base + entry->offset, wb._file);
    }

    {
        char *typePtr = *(char **)((const char *)this + 0x108);
        TypeMethod *entry = (TypeMethod *)(typePtr + 0x28);
        char *base = (char *)this + 0x104;
        entry->fn(base + entry->offset, wb._file);
    }

    {
        TypeMethod *entry =
            (TypeMethod *)((char *)*(void **)((const char *)this + 0x3F0) + 0x28);
        void *base = (char *)this + 0x3EC;
        entry->fn((char *)base + entry->offset, &file);
    }

    {
        TypedObject *obj = *(TypedObject *const *)((const char *)this + 0x1F4);
        TypeMethod *entry = (TypeMethod *)((char *)obj->mType + 0x28);
        entry->fn((char *)obj + entry->offset, &file);
    }

    ((const cHandle *)((const char *)this + 0x1F8))->Write(wb);
    wb.Write(3, (const float *)((const char *)this + 0x420));
    ((const cHandle *)((const char *)this + 0x1F0))->Write(wb);
    wb.End();
}
```

### Exemplar 2: `gcMsgUpdatePlayerState::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const`

- Address: `0x00136058`
- Size: `372` bytes
- Source: `src/gcMsgUpdatePlayerState.cpp`

```cpp
void gcMsgUpdatePlayerState::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle conn) const {
    ((cOutStreamBit *)&s)->WriteBit(mFlag);
    s.Write((unsigned int)mPlayerIndex, 4, true);

    gcReplicationVisitor visitor;
    visitor.mMode = 0x101;
    visitor.mOutStream = 0;
    visitor.mInStream = 0;
    visitor.mStreamPos = -1;
    visitor.mNetConnection = -1;
    int localConn = conn.mValue;
    __asm__ volatile("" : "+r"(localConn));
    visitor.mReadActive = 0;
    unsigned char flag = mFlag;
    visitor.mWriteEnabled = true;
    __0fUgcReplicationVisitorMSetNetStreamP6KcOutStream6SnwConnectionHandlebTD(
        &visitor, &s, localConn, flag, true);

    int index = mPlayerIndex;
    void *player = 0;
    void *playerCheck;
    if (index >= 0) {
        if (index < 8) {
            player = *(char **)0x37D87C + index * 68;
        }
        goto check_player;
    } else {
check_player:
        playerCheck = player;
    }
    if (playerCheck != 0) {
        char *type = *(char **)((char *)playerCheck + 4);
        gcTypeDispatchEntry *entry = (gcTypeDispatchEntry *)(type + 0x70);
        entry->fn((char *)player + entry->offset, &visitor);
    }
}
```

### Exemplar 3: `gcMsgUpdateRegionState::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const`

- Address: `0x00136880`
- Size: `332` bytes
- Source: `src/gcMsgUpdateRegionState.cpp`

```cpp
void gcMsgUpdateRegionState::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle conn) const {
    const cGUIDT<gcRegion> *guid = (const cGUIDT<gcRegion> *)((const char *)this + 8);
    ((const cGUID *)guid)->Write(s);
    ((cOutStreamBit *)&s)->WriteBit(*(const unsigned char *)((const char *)this + 4));

    gcReplicationVisitor visitor;
    visitor.mMode = 0x101;
    visitor.mOutStream = 0;
    visitor.mInStream = 0;
    visitor.mStreamPos = -1;
    visitor.mNetConnection = -1;
    int localConn = conn.mValue;
    __asm__ volatile("" : "+r"(localConn));
    visitor.mReadActive = 0;
    unsigned char flag = *(const unsigned char *)((const char *)this + 4);
    visitor.mWriteEnabled = true;
    __0fUgcReplicationVisitorMSetNetStreamP6KcOutStream6SnwConnectionHandlebTD(
        &visitor, &s, localConn, flag, true);

    gcRegion *region = (*(gcMap **)0x37D7FC)->FindLoadedRegion(*guid);
    if (region != 0) {
        char *type = *(char **)((char *)region + 4);
        gcTypeDispatchEntry *entry = (gcTypeDispatchEntry *)(type + 0xD0);
        entry->fn((char *)region + entry->offset, &visitor);
    }
}
```

### Exemplar 4: `gcMsgUpdateMapState::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const`

- Address: `0x0013341c`
- Size: `308` bytes
- Source: `src/gcMsgUpdateGameState.cpp`

```cpp
void gcMsgUpdateMapState::Write(cOutStream &stream, nwSocketHandle sock, const nwAddress &, nwConnectionHandle conn) const {
    (void)sock;

    mGuid.Write(stream);
    int flag = mFlag & 0xFF;
    int pos = ((cOutStreamBits *)&stream)->mBitPos;
    unsigned char *data = ((cOutStreamBits *)&stream)->mBuf;
    int bit = pos & 7;
    unsigned char *byte = data + (pos >> 3);
    pos = pos + 1;
    unsigned char overflow = ((cOutStreamBits *)&stream)->mOverflow;
    ((cOutStreamBits *)&stream)->mBitPos = pos;


    if (overflow == 0) {
        int newPos = ((cOutStreamBits *)&stream)->mBitPos;
        int capacity = ((cOutStreamBits *)&stream)->mCapacity;
        int size = capacity;
        if (size < ((newPos + 7) >> 3)) {
            ((cOutStreamBits *)&stream)->mOverflow = 1;
        }
    }

    if ((unsigned char)(((cOutStreamBits *)&stream)->mOverflow == 0)) {
        unsigned char cur = *byte;
        int mask = ~(1 << bit);
        int bitValue = (flag != 0);
        *byte = (unsigned char)((cur & mask) | (bitValue << bit));
    }

    gcReplicationVisitor visitor;
    visitor.mMode = 0x101;
    visitor.mOutStream = 0;
    visitor.mInStream = 0;
    visitor.mStreamPos = -1;
    visitor.mNetConnection = -1;
    visitor.mReadActive = 0;
    visitor.mWriteEnabled = true;
    visitor.SetNetStream(&stream, conn, mFlag, true);

    gcReplicationManager *mgr = *(gcReplicationManager **)0x37D7FC;
    if (mgr) {
        char *vtable = *(char **)((char *)mgr + 4);
        struct Entry { short thisOff; short pad; void (*fn)(void *, gcReplicationVisitor *); };
        Entry *entry = (Entry *)(vtable + 0xD0);
        short off = entry->thisOff;
        void (*fn)(void *, gcReplicationVisitor *) = entry->fn;
        fn((char *)mgr + off, &visitor);
    }
}
```


## Prior Failure Notes

### Note 1

- session `11a032a2`; src `src/gcEntity_Write.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/11a032a2/00113acc__agent_self_reported_failure__src_gcEntity_Write.cpp`

Failed near-miss, 12-byte diff (3 instructions out of 644-byte / 161-instruction function). Closest size: 632 bytes. All call patterns, struct dispatches (this+0x84 + 0xB8 single-arg, this+0x84 + 0x28 two-arg with sp+8 base reload), the four wb.Write(3, float*) calls, cBaseArray::Write at this+0x210, and the cHandle/uint loop at this+0x48 all match byte-for-byte. The 3 missing instructions are all `addu reg, reg, a0` (with a0=0, one-time-before-loop strength reduction artifacts) plus the inner loop's beqzl→beqz conversion:

  1. Outer loop entry needs: `li a0,0` + `addu s3,s3,a0` + `move s0,s3` pattern. Compiler currently emits just `move s2,s3` (using s2 instead of s0).
  2. Inner loop entry needs: `li a0,0` + `addu s0,s0,a0` pattern. Mine has neither (compiler uses beqzl with s4++ in delay slot instead).
  3. Inner loop comparison: target uses beqz with safe `li a0,0` delay slot. Mine uses beqzl (branch-likely) with s4++ in delay slot.

Root cause: target's source uses explicit `int offset = 0` variables for both outer (`slot + offset` initial, then offset += 4 per iter) and inner (`innerB + offset` initial, then offset += 4) loops. The compiler strength-reduces these to `s3 = arr + 0` (one-time addu) plus per-iter increments. When I added explicit offset variables, register allocation shifted heavily (s7→s2 for this; s8→s0 for &file; 458/644 bytes differ). The extra int variables created register pressure even though they're strength-reduced away.

To match: need to introduce offset variables WITHOUT disturbing register allocation. The pattern in cBaseArray::Write at src/cBaseArray.cpp:196 uses `int *tmp = mData; __asm__("": "+r"(tmp)); int offset = 0; loop: ...; offset += 4; goto loop;` — an asm-barrier-bound tmp variable + goto-loop pattern. Worth retrying with this exact shape adapted for the nested-array case. Also try permuter with --time 600 targeting this 12-byte gap.

The file is a split-TU at src/gcEntity_Write.cpp (uses local gcEntity redeclaration with Write method, per SPLIT-TU rule, to avoid header edits that would regress gcEntity sibling methods).


## Disassembly

```asm
113acc:	27bdffc0 	addiu	sp,sp,-64
113ad0:	afb70028 	sw	s7,40(sp)
113ad4:	0080b825 	move	s7,a0
113ad8:	afbe002c 	sw	s8,44(sp)
113adc:	00a0f025 	move	s8,a1
113ae0:	03a02025 	move	a0,sp
113ae4:	afb0000c 	sw	s0,12(sp)
113ae8:	afb10010 	sw	s1,16(sp)
113aec:	afb20014 	sw	s2,20(sp)
113af0:	afb30018 	sw	s3,24(sp)
113af4:	afb4001c 	sw	s4,28(sp)
113af8:	afb50020 	sw	s5,32(sp)
113afc:	afb60024 	sw	s6,36(sp)
113b00:	afbf0030 	sw	ra,48(sp)
113b04:	0c001516 	jal	0x5458
113b08:	3406000f 	li	a2,0xf
113b0c:	02e02025 	move	a0,s7
113b10:	0c0026cc 	jal	0x9b30
113b14:	03c02825 	move	a1,s8
113b18:	26e40044 	addiu	a0,s7,68
113b1c:	0c00240c 	jal	0x9030
113b20:	03a02825 	move	a1,sp
113b24:	82e50060 	lb	a1,96(s7)
113b28:	0c001544 	jal	0x5510
113b2c:	03a02025 	move	a0,sp
113b30:	26e40058 	addiu	a0,s7,88
113b34:	0c00240c 	jal	0x9030
113b38:	03a02825 	move	a1,sp
113b3c:	92e50061 	lbu	a1,97(s7)
113b40:	0c00154f 	jal	0x553c
113b44:	03a02025 	move	a0,sp
113b48:	92e50062 	lbu	a1,98(s7)
113b4c:	0c00154f 	jal	0x553c
113b50:	03a02025 	move	a0,sp
113b54:	92e50063 	lbu	a1,99(s7)
113b58:	0c00154f 	jal	0x553c
113b5c:	03a02025 	move	a0,sp
113b60:	8ee5006c 	lw	a1,108(s7)
113b64:	0c0015c1 	jal	0x5704
113b68:	03a02025 	move	a0,sp
113b6c:	92e4010c 	lbu	a0,268(s7)
113b70:	26e60080 	addiu	a2,s7,128
113b74:	30840004 	andi	a0,a0,0x4
113b78:	26f00048 	addiu	s0,s7,72
113b7c:	10800007 	beqz	a0,0x113b9c
113b80:	afa60008 	sw	a2,8(sp)
113b84:	8ee40084 	lw	a0,132(s7)
113b88:	248400b8 	addiu	a0,a0,184
113b8c:	84850000 	lh	a1,0(a0)
113b90:	8c870004 	lw	a3,4(a0)
113b94:	00e0f809 	jalr	a3
113b98:	00c52021 	addu	a0,a2,a1
113b9c:	02008825 	move	s1,s0
113ba0:	26e600c0 	addiu	a2,s7,192
113ba4:	03a02025 	move	a0,sp
113ba8:	0c0016cf 	jal	0x5b3c
113bac:	34050003 	li	a1,0x3
113bb0:	26e60090 	addiu	a2,s7,144
113bb4:	03a02025 	move	a0,sp
113bb8:	0c0016cf 	jal	0x5b3c
113bbc:	34050003 	li	a1,0x3
113bc0:	26e600a0 	addiu	a2,s7,160
113bc4:	03a02025 	move	a0,sp
113bc8:	0c0016cf 	jal	0x5b3c
113bcc:	34050003 	li	a1,0x3
113bd0:	26e600b0 	addiu	a2,s7,176
113bd4:	03a02025 	move	a0,sp
113bd8:	0c0016cf 	jal	0x5b3c
113bdc:	34050003 	li	a1,0x3
113be0:	26e40210 	addiu	a0,s7,528
113be4:	0c001c91 	jal	0x7244
113be8:	03a02825 	move	a1,sp
113bec:	34040000 	li	a0,0x0
113bf0:	34100000 	li	s0,0x0
113bf4:	02248821 	addu	s1,s1,a0
113bf8:	02202025 	move	a0,s1
113bfc:	0c00240c 	jal	0x9030
113c00:	03a02825 	move	a1,sp
113c04:	8e250004 	lw	a1,4(s1)
113c08:	0c0015c1 	jal	0x5704
113c0c:	03a02025 	move	a0,sp
113c10:	26100001 	addiu	s0,s0,1
113c14:	2a040002 	slti	a0,s0,2
113c18:	1480fff7 	bnez	a0,0x113bf8
113c1c:	26310008 	addiu	s1,s1,8
113c20:	8ee40208 	lw	a0,520(s7)
113c24:	10800005 	beqz	a0,0x113c3c
113c28:	34130000 	li	s3,0x0
113c2c:	8c93fffc 	lw	s3,-4(a0)
113c30:	3c044000 	lui	a0,0x4000
113c34:	2484ffff 	addiu	a0,a0,-1
113c38:	02649824 	and	s3,s3,a0
113c3c:	03a02025 	move	a0,sp
113c40:	0c0015a4 	jal	0x5690
113c44:	02602825 	move	a1,s3
113c48:	8ef30208 	lw	s3,520(s7)
113c4c:	12600005 	beqz	s3,0x113c64
113c50:	34160000 	li	s6,0x0
113c54:	8e76fffc 	lw	s6,-4(s3)
113c58:	3c044000 	lui	a0,0x4000
113c5c:	2484ffff 	addiu	a0,a0,-1
113c60:	02c4b024 	and	s6,s6,a0
113c64:	34140000 	li	s4,0x0
113c68:	0296202a 	slt	a0,s4,s6
113c6c:	10800022 	beqz	a0,0x113cf8
113c70:	34040000 	li	a0,0x0
113c74:	3c154000 	lui	s5,0x4000
113c78:	02649821 	addu	s3,s3,a0
113c7c:	26b5ffff 	addiu	s5,s5,-1
113c80:	02608025 	move	s0,s3
113c84:	8e050000 	lw	a1,0(s0)
113c88:	10a00003 	beqz	a1,0x113c98
113c8c:	34040000 	li	a0,0x0
113c90:	8ca4fffc 	lw	a0,-4(a1)
113c94:	00952024 	and	a0,a0,s5
113c98:	00802825 	move	a1,a0
113c9c:	0c0015a4 	jal	0x5690
113ca0:	03a02025 	move	a0,sp
113ca4:	8e100000 	lw	s0,0(s0)
113ca8:	12000003 	beqz	s0,0x113cb8
113cac:	34120000 	li	s2,0x0
113cb0:	8e12fffc 	lw	s2,-4(s0)
113cb4:	02559024 	and	s2,s2,s5
113cb8:	34110000 	li	s1,0x0
113cbc:	0232202a 	slt	a0,s1,s2
113cc0:	10800009 	beqz	a0,0x113ce8
113cc4:	34040000 	li	a0,0x0
113cc8:	02048021 	addu	s0,s0,a0
113ccc:	02002025 	move	a0,s0
113cd0:	0c00240c 	jal	0x9030
113cd4:	03a02825 	move	a1,sp
113cd8:	26310001 	addiu	s1,s1,1
113cdc:	0232202a 	slt	a0,s1,s2
113ce0:	1480fffa 	bnez	a0,0x113ccc
113ce4:	26100004 	addiu	s0,s0,4
113ce8:	26940001 	addiu	s4,s4,1
113cec:	0296202a 	slt	a0,s4,s6
113cf0:	1480ffe3 	bnez	a0,0x113c80
113cf4:	26730004 	addiu	s3,s3,4
113cf8:	8ee40084 	lw	a0,132(s7)
113cfc:	8fa50008 	lw	a1,8(sp)
113d00:	24840028 	addiu	a0,a0,40
113d04:	84860000 	lh	a2,0(a0)
113d08:	8c870004 	lw	a3,4(a0)
113d0c:	00a62021 	addu	a0,a1,a2
113d10:	00e0f809 	jalr	a3
113d14:	03c02825 	move	a1,s8
113d18:	0c001724 	jal	0x5c90
113d1c:	03a02025 	move	a0,sp
113d20:	8fb0000c 	lw	s0,12(sp)
113d24:	8fb10010 	lw	s1,16(sp)
113d28:	8fb20014 	lw	s2,20(sp)
113d2c:	8fb30018 	lw	s3,24(sp)
113d30:	8fb4001c 	lw	s4,28(sp)
113d34:	8fb50020 	lw	s5,32(sp)
113d38:	8fb60024 	lw	s6,36(sp)
113d3c:	8fb70028 	lw	s7,40(sp)
113d40:	8fbe002c 	lw	s8,44(sp)
113d44:	8fbf0030 	lw	ra,48(sp)
113d48:	03e00008 	jr	ra
113d4c:	27bd0040 	addiu	sp,sp,64
```

## m2c Starting Point

```c
// Decompiled: gcEntity::Write(cFile &) const
// Address: 0x00113acc, Size: 644B
// Obj: gcAll_psp.obj

? cBaseArray__Write_cWriteBlockrefconst(void *, void *); /* extern */
? cHandle__Write_cWriteBlockrefconst(void *, void *); /* extern */
? cObject__Write_cFilerefconst(void *, ?);          /* extern */
? cWriteBlock__End_void(void *);                    /* extern */
? cWriteBlock__Write_char(void *, s8);              /* extern */
? cWriteBlock__Write_int(void *, s32);              /* extern */
? cWriteBlock__Write_int_constfloatptr(void *, ?, void *); /* extern */
? cWriteBlock__Write_unsignedchar(void *, u8);      /* extern */
? cWriteBlock__Write_unsignedint(void *, s32);      /* extern */
? cWriteBlock__cWriteBlock_cFileref_unsignedint(void *, ?); /* extern */

void gcEntity__Write_cFilerefconst(void *arg0, ? arg1) {
    ? (*temp_a3)(void *, s16, void *, ?);
    ? (*temp_a3_2)(s32, ?, s16, ?);
    s16 temp_a1;
    s16 temp_a2_2;
    s32 var_a0;
    s32 var_s0;
    s32 var_s1_2;
    s32 var_s2;
    s32 var_s3;
    s32 var_s4;
    s32 var_s6;
    void **temp_a0_2;
    void **temp_s3;
    void **var_s3_2;
    void *temp_a0;
    void *temp_a0_3;
    void *temp_a1_2;
    void *temp_a2;
    void *temp_s0;
    void *var_s0_2;
    void *var_s1;

    cWriteBlock__cWriteBlock_cFileref_unsignedint(sp, 0xF);
    cObject__Write_cFilerefconst(arg0, arg1);
    cHandle__Write_cWriteBlockrefconst(arg0 + 0x44, sp);
    cWriteBlock__Write_char(sp, arg0->unk60);
    cHandle__Write_cWriteBlockrefconst(arg0 + 0x58, sp);
    cWriteBlock__Write_unsignedchar(sp, arg0->unk61);
    cWriteBlock__Write_unsignedchar(sp, arg0->unk62);
    cWriteBlock__Write_unsignedchar(sp, arg0->unk63);
    cWriteBlock__Write_unsignedint(sp, arg0->unk6C);
    temp_a2 = arg0 + 0x80;
    if (arg0->unk10C & 4) {
        temp_a0 = arg0->unk84;
        temp_a1 = temp_a0->unkB8;
        temp_a3 = (temp_a0 + 0xB8)->unk4;
        temp_a3(temp_a2 + temp_a1, temp_a1, temp_a2, temp_a3);
    }
    cWriteBlock__Write_int_constfloatptr(sp, 3, arg0 + 0xC0);
    cWriteBlock__Write_int_constfloatptr(sp, 3, arg0 + 0x90);
    cWriteBlock__Write_int_constfloatptr(sp, 3, arg0 + 0xA0);
    cWriteBlock__Write_int_constfloatptr(sp, 3, arg0 + 0xB0);
    cBaseArray__Write_cWriteBlockrefconst(arg0 + 0x210, sp);
    var_s0 = 0;
    var_s1 = arg0 + 0x48;
    do {
        cHandle__Write_cWriteBlockrefconst(var_s1, sp);
        cWriteBlock__Write_unsignedint(sp, var_s1->unk4);
        var_s0 += 1;
        var_s1 += 8;
    } while (var_s0 < 2);
    temp_a0_2 = arg0->unk208;
    var_s3 = 0;
    if (temp_a0_2 != NULL) {
        var_s3 = temp_a0_2->unk-4 & 0x3FFFFFFF;
    }
    cWriteBlock__Write_int(sp, var_s3);
    temp_s3 = arg0->unk208;
    var_s6 = 0;
    if (temp_s3 != NULL) {
        var_s6 = temp_s3->unk-4 & 0x3FFFFFFF;
    }
    var_s4 = 0;
    if (var_s6 > 0) {
        var_s3_2 = temp_s3;
        do {
            temp_a1_2 = *var_s3_2;
            var_a0 = 0;
            if (temp_a1_2 != NULL) {
                var_a0 = temp_a1_2->unk-4 & 0x3FFFFFFF;
            }
            cWriteBlock__Write_int(sp, var_a0);
            temp_s0 = *var_s3_2;
            var_s2 = 0;
            if (temp_s0 != NULL) {
                var_s2 = temp_s0->unk-4 & 0x3FFFFFFF;
            }
            var_s1_2 = 0;
            if (var_s2 > 0) {
                var_s0_2 = temp_s0;
                do {
                    cHandle__Write_cWriteBlockrefconst(var_s0_2, sp);
                    var_s1_2 += 1;
                    var_s0_2 += 4;
                } while (var_s1_2 < var_s2);
            }
            var_s4 += 1;
            var_s3_2 += 4;
        } while (var_s4 < var_s6);
    }
    temp_a0_3 = arg0->unk84;
    temp_a2_2 = temp_a0_3->unk28;
    temp_a3_2 = (temp_a0_3 + 0x28)->unk4;
    temp_a3_2(subroutine_arg2 + temp_a2_2, arg1, temp_a2_2, temp_a3_2);
    cWriteBlock__End_void(sp);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
