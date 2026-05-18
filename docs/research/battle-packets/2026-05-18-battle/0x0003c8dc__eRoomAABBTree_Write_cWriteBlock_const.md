# Battle Packet: `eRoomAABBTree::Write(cWriteBlock &) const`

## Target

- Address: `0x0003c8dc`
- Size: `300` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eRoomAABBTree`
- Method family: `Write`
- Leaf: `False`

## Queue Metadata

- score: `191`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 11B; correct function size; codegen-specific diagnosis; Write retry family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `11`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `11`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00005690` `matched` 116B `cWriteBlock::Write(int)`
- `0x00009030` `failed` 664B `cHandle::Write(cWriteBlock &) const`
- `0x0003c7fc` `matched` 100B `eRoomTreeNode::Write(cWriteBlock &) const`

Callers:
- `0x0003d2e8` `matched` 748B `eRoom::Write(cFile &) const`
- `0x0003f934` `matched` 208B `eRoomSet::Write(cFile &) const`

## Placement

- Canonical source: `src/eRoomAABBTree.cpp`
- Header(s): `include/eRoomAABBTree.h`
- Allowed source(s): `src/eRoomAABBTree.cpp`, `src/eRoomAABBTree_Write.cpp`
- Split-TU prefix: `src/eRoomAABBTree_*.cpp`

## Compiler Guidance

Write template: preserve cWriteBlock/cOutStream construction, field write order, and End() placement. SNC is sensitive to temporary bool/cast shapes and to independent store ordering.

## Class Header: `include/eRoomAABBTree.h`

Not found.

## Matched Method Exemplars

### Exemplar 1: `eShadowTemplate::Write(cFile &) const`

- Address: `0x0007e724`
- Size: `284` bytes
- Source: `src/eShadowTemplate.cpp`

```cpp
void eShadowTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    ((const eDynamicGeomTemplate *)this)->Write(file);
    wb.Write(*(const bool *)((const char *)this + 0x48));
    ((const cHandle *)((const char *)this + 0x4C))->Write(wb);
    ((const cHandle *)((const char *)this + 0x50))->Write(wb);

    int *handles = (int *)((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x54))->mData;
    int count0 = 0;
    if (handles != 0) {
        count0 = handles[-1] & 0x3FFFFFFF;
    }
    wb.Write(count0);

    int count = 0;
    handles = (int *)((cArrayBase<cHandleT<eMaterial> > *)((char *)this + 0x54))->mData;
    if (handles != 0) {
        count = handles[-1] & 0x3FFFFFFF;
    }

    int *baseHandles = handles;
    int i = 0;
    if (i < count) {
        int offset = 0;
        int *handle = baseHandles + offset;
        do {
            ((cHandle *)handle)->Write(wb);
            i++;
            handle++;
        } while (i < count);
    }

    wb.Write(*(const float *)((const char *)this + 0x58));
    wb.Write(*(const float *)((const char *)this + 0x5C));
    wb.Write(*(const float *)((const char *)this + 0x60));
    wb.End();
}
```

### Exemplar 2: `eWeatherEffectTemplate::Write(cFile &) const`

- Address: `0x00061044`
- Size: `280` bytes
- Source: `src/eWeatherEffectTemplate.cpp`

```cpp
void eWeatherEffectTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    ((const eGeomTemplate *)this)->Write(file);
    ((const cHandle *)((const char *)this + 0x70))->Write(wb);
    wb.Write(*(int *)((const char *)this + 0x44));
    wb.Write(*(float *)((const char *)this + 0x60));
    wb.Write(*(float *)((const char *)this + 0x48));
    wb.Write(*(float *)((const char *)this + 0x4C));
    wb.Write(*(float *)((const char *)this + 0x50));
    wb.Write(*(float *)((const char *)this + 0x58));
    wb.Write(*(float *)((const char *)this + 0x64));
    wb.Write(*(float *)((const char *)this + 0x5C));
    wb.Write(*(float *)((const char *)this + 0x54));
    wb.Write(*(float *)((const char *)this + 0x68));
    wb.Write(*(float *)((const char *)this + 0x6C));
    wb.Write(*(const bool *)((const char *)this + 0x74));
    wb.Write(*(float *)((const char *)this + 0x78));
    wb.Write(*(float *)((const char *)this + 0x7C));
    wb.Write(*(float *)((const char *)this + 0x80));
    wb.Write(*(float *)((const char *)this + 0x84));
    wb.End();
}
```

### Exemplar 3: `eSkin::Write(cFile &) const`

- Address: `0x0004bcbc`
- Size: `328` bytes
- Source: `src/ePortal.cpp`

```cpp
void eSkin::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cObject::Write(file);

    int *handles0 = *(int **)((const char *)this + 0x44);
    int count0 = 0;
    if (handles0 != 0) {
        count0 = handles0[-1] & 0x3FFFFFFF;
    }
    wb.Write(count0);

    int *handles = *(int **)((const char *)this + 0x44);
    int count = 0;
    if (handles != 0) {
        count = handles[-1] & 0x3FFFFFFF;
    }

    int i = 0;
    if (i < count) {
        int offset = 0;
        int *handle = handles + offset;
        do {
            ((const cHandle *)handle)->Write(wb);
            i++;
            handle++;
        } while (i < count);
    }

    int *handles1 = *(int **)((const char *)this + 0x48);
    int count1 = 0;
    if (handles1 != 0) {
        count1 = handles1[-1] & 0x3FFFFFFF;
    }
    __asm__ volatile("move $s1, %0" : : "r"(this));
    wb.Write(count1);

    int *handles2;
    __asm__ volatile("lw %0, 0x48($s1)" : "=r"(handles2));
    int count2 = 0;
    if (handles2 != 0) {
        count2 = handles2[-1] & 0x3FFFFFFF;
    }

    int j = 0;
    if (j < count2) {
        int offset = 0;
        int *handle = handles2 + offset;
        do {
            ((const cHandle *)handle)->Write(wb);
            j++;
            handle++;
        } while (j < count2);
    }

    wb.End();
}
```

### Exemplar 4: `eHeightmapTemplate::Write(cFile &) const`

- Address: `0x00051b54`
- Size: `332` bytes
- Source: `src/eHeightmapTemplate.cpp`

```cpp
void eHeightmapTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 6);
    ((const eStaticGeomTemplate *)this)->Write(file);
    ((const cName *)((const char *)this + 0x44))->Write(wb);
    wb.Write(*(int *)((const char *)this + 0x5C));
    wb.Write(*(int *)((const char *)this + 0x60));
    wb.Write(*(int *)((const char *)this + 0x68));
    wb.Write(*(int *)((const char *)this + 0x6C));
    wb.Write(*(float *)((const char *)this + 0x70));
    ((const cHandle *)((const char *)this + 0x88))->Write(wb);

    int *handles = *(int **)((const char *)this + 0x7C);
    int count0 = 0;
    if (handles != 0) {
        count0 = handles[-1] & 0x3FFFFFFF;
    }
    wb.Write(count0);

    int count = 0;
    handles = *(int **)((const char *)this + 0x7C);
    if (handles != 0) {
        count = handles[-1] & 0x3FFFFFFF;
    }

    char *baseHandles = (char *)handles;
    int i = 0;
    if (i < count) {
        int offset = 0;
        char *tile = baseHandles + offset;
        do {
            ((const eHeightmapTile *)tile)->Write(wb);
            i++;
            tile += 0x14;
        } while (i < count);
    }

    wb.Write(*(float *)((const char *)this + 0x74));
    wb.Write(*(float *)((const char *)this + 0x78));
    wb.WriteBase(*(const cBase **)((const char *)this + 0x84));
    wb.End();
}
```


## Prior Failure Notes

### Note 1

- session `f34575f8`; src `src/eRoomAABBTree_Write.cpp`; snapshot `logs/failure_snapshots/match_20260506_235212/f34575f8/0003c8dc__agent_self_reported_failure__src_eRoomAABBTree_Write.cpp`

Closest attempt is a split-TU canonical eRoomAABBTree::Write using typed eRoomTreeNode/cHandle array elements, duplicated count reloads, and typed pointer loops. This reaches the correct 300-byte function size and leaves an 11-byte mismatch isolated to the first node loop: generated code keeps the node base in s4 and uses a0 as the zero offset (`lw s4,0(s0)`, `beqz s4`, `addu s4,s4,a0`), while the original keeps the base in a0 and uses s4 for the zero offset (`lw a0,0(s0)`, `beqz a0`, `addu s4,a0,s4`). The handle count/write block and handle loop match structurally in the closest typed version.

Tried raw int pointers, raw char pointers, typed eRoomTreeNode/cHandle pointers with local size padding, offset temporaries, empty asm barriers on offsets/bases, sched=1, and a 300-second permuter run. The permuter compiled 7,045 candidates and found no improvement from the 11-byte baseline. This looks like local register-allocation/branch-likely drift in the node loop rather than a semantic ordering issue, but it is above the <=8-byte REG_ALLOC stop threshold, so it remains ordinary failed with this closest source.

### Note 2

- session `cf03c06d`

Created a new canonical class-method file and reconstructed the serializer as real C++ with duplicated count loads and two counted loops. I then iterated through several source shapes for the loop bases: typed pointers, `void *`, raw `int` bases, separate predeclared offset temporaries, and finally raw `char *` bases with explicit byte offsets. I also extracted the expected object and compared the generated assembly directly. The function stays close at 292B vs the expected 300B.

The remaining mismatch is structural, not semantic. The second count/write block now matches the original more closely, but the node loop still compiles as a pointer-walk with `lw s4,0(s0)` and `beqzl`, while the original uses `lw a0,0(s0)`, `beqz` with `li s4,0` in the delay slot, then `addu s4,a0,s4` before the call. The handle loop also keeps the base pointer in a saved register instead of the original `a0 + offset` form. I attempted the permuter, but it refused to run because the compiled function size (292B) did not match the target size (300B), so it could not locate the function in the .o.


## Disassembly

```asm
3c8dc:	27bdffe0 	addiu	sp,sp,-32
3c8e0:	afb00000 	sw	s0,0(sp)
3c8e4:	00808025 	move	s0,a0
3c8e8:	8e040000 	lw	a0,0(s0)
3c8ec:	afb20008 	sw	s2,8(sp)
3c8f0:	34120000 	li	s2,0x0
3c8f4:	afb10004 	sw	s1,4(sp)
3c8f8:	afb3000c 	sw	s3,12(sp)
3c8fc:	afb40010 	sw	s4,16(sp)
3c900:	afbf0014 	sw	ra,20(sp)
3c904:	10800005 	beqz	a0,0x3c91c
3c908:	00a08825 	move	s1,a1
3c90c:	8c92fffc 	lw	s2,-4(a0)
3c910:	3c044000 	lui	a0,0x4000
3c914:	2484ffff 	addiu	a0,a0,-1
3c918:	02449024 	and	s2,s2,a0
3c91c:	02202025 	move	a0,s1
3c920:	0c0015a4 	jal	0x5690
3c924:	02402825 	move	a1,s2
3c928:	8e040000 	lw	a0,0(s0)
3c92c:	10800005 	beqz	a0,0x3c944
3c930:	34120000 	li	s2,0x0
3c934:	8c92fffc 	lw	s2,-4(a0)
3c938:	3c054000 	lui	a1,0x4000
3c93c:	24a5ffff 	addiu	a1,a1,-1
3c940:	02459024 	and	s2,s2,a1
3c944:	34130000 	li	s3,0x0
3c948:	0272282a 	slt	a1,s3,s2
3c94c:	10a00009 	beqz	a1,0x3c974
3c950:	34140000 	li	s4,0x0
3c954:	0094a021 	addu	s4,a0,s4
3c958:	02802025 	move	a0,s4
3c95c:	0c00f1ff 	jal	0x3c7fc
3c960:	02202825 	move	a1,s1
3c964:	26730001 	addiu	s3,s3,1
3c968:	0272202a 	slt	a0,s3,s2
3c96c:	1480fffa 	bnez	a0,0x3c958
3c970:	26940030 	addiu	s4,s4,48
3c974:	8e050004 	lw	a1,4(s0)
3c978:	10a00005 	beqz	a1,0x3c990
3c97c:	34040000 	li	a0,0x0
3c980:	8ca4fffc 	lw	a0,-4(a1)
3c984:	3c054000 	lui	a1,0x4000
3c988:	24a5ffff 	addiu	a1,a1,-1
3c98c:	00852024 	and	a0,a0,a1
3c990:	00802825 	move	a1,a0
3c994:	0c0015a4 	jal	0x5690
3c998:	02202025 	move	a0,s1
3c99c:	8e040004 	lw	a0,4(s0)
3c9a0:	10800005 	beqz	a0,0x3c9b8
3c9a4:	34100000 	li	s0,0x0
3c9a8:	8c90fffc 	lw	s0,-4(a0)
3c9ac:	3c054000 	lui	a1,0x4000
3c9b0:	24a5ffff 	addiu	a1,a1,-1
3c9b4:	02058024 	and	s0,s0,a1
3c9b8:	34120000 	li	s2,0x0
3c9bc:	0250282a 	slt	a1,s2,s0
3c9c0:	10a00009 	beqz	a1,0x3c9e8
3c9c4:	34130000 	li	s3,0x0
3c9c8:	00939821 	addu	s3,a0,s3
3c9cc:	02602025 	move	a0,s3
3c9d0:	0c00240c 	jal	0x9030
3c9d4:	02202825 	move	a1,s1
3c9d8:	26520001 	addiu	s2,s2,1
3c9dc:	0250202a 	slt	a0,s2,s0
3c9e0:	1480fffa 	bnez	a0,0x3c9cc
3c9e4:	26730004 	addiu	s3,s3,4
3c9e8:	8fb00000 	lw	s0,0(sp)
3c9ec:	8fb10004 	lw	s1,4(sp)
3c9f0:	8fb20008 	lw	s2,8(sp)
3c9f4:	8fb3000c 	lw	s3,12(sp)
3c9f8:	8fb40010 	lw	s4,16(sp)
3c9fc:	8fbf0014 	lw	ra,20(sp)
3ca00:	03e00008 	jr	ra
3ca04:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: eRoomAABBTree::Write(cWriteBlock &) const
// Address: 0x0003c8dc, Size: 300B
// Obj: eAll_psp.obj

? cHandle__Write_cWriteBlockrefconst(void *, ?);    /* extern */
? cWriteBlock__Write_int(?, s32);                   /* extern */
? eRoomTreeNode__Write_cWriteBlockrefconst(void *, ?); /* extern */

void eRoomAABBTree__Write_cWriteBlockrefconst(void *arg0, ? arg1) {
    s32 var_a0;
    s32 var_s0;
    s32 var_s2;
    s32 var_s2_2;
    s32 var_s2_3;
    s32 var_s3;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a1;
    void *var_s3_2;
    void *var_s4;

    temp_a0 = arg0->unk0;
    var_s2 = 0;
    if (temp_a0 != NULL) {
        var_s2 = temp_a0->unk-4 & 0x3FFFFFFF;
    }
    cWriteBlock__Write_int(arg1, var_s2);
    temp_a0_2 = arg0->unk0;
    var_s2_2 = 0;
    if (temp_a0_2 != NULL) {
        var_s2_2 = temp_a0_2->unk-4 & 0x3FFFFFFF;
    }
    var_s3 = 0;
    if (var_s2_2 > 0) {
        var_s4 = temp_a0_2;
        do {
            eRoomTreeNode__Write_cWriteBlockrefconst(var_s4, arg1);
            var_s3 += 1;
            var_s4 += 0x30;
        } while (var_s3 < var_s2_2);
    }
    temp_a1 = arg0->unk4;
    var_a0 = 0;
    if (temp_a1 != NULL) {
        var_a0 = temp_a1->unk-4 & 0x3FFFFFFF;
    }
    cWriteBlock__Write_int(arg1, var_a0);
    temp_a0_3 = arg0->unk4;
    var_s0 = 0;
    if (temp_a0_3 != NULL) {
        var_s0 = temp_a0_3->unk-4 & 0x3FFFFFFF;
    }
    var_s2_3 = 0;
    if (var_s0 > 0) {
        var_s3_2 = temp_a0_3;
        do {
            cHandle__Write_cWriteBlockrefconst(var_s3_2, arg1);
            var_s2_3 += 1;
            var_s3_2 += 4;
        } while (var_s2_3 < var_s0);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
