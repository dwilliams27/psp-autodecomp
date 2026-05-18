# Battle Packet: `eSurfaceSet::Write(cFile &) const`

## Target

- Address: `0x00053c3c`
- Size: `200` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eSurfaceSet`
- Method family: `Write`
- Leaf: `False`

## Queue Metadata

- score: `147`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 21B; codegen-specific diagnosis; Write retry family; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `21`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `needs_layout`
- Near miss: `21`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00005458` `failed` 136B `cWriteBlock::cWriteBlock(cFile &, unsigned int)`
- `0x00005690` `matched` 116B `cWriteBlock::Write(int)`
- `0x00005c90` `matched` 112B `cWriteBlock::End(void)`
- `0x00009030` `failed` 664B `cHandle::Write(cWriteBlock &) const`
- `0x00009b30` `matched` 92B `cObject::Write(cFile &) const`

## Placement

- Canonical source: `src/eSurfaceSet.cpp`
- Header(s): `include/eSurfaceSet.h`
- Allowed source(s): `src/eSurfaceSet.cpp`
- Split-TU prefix: `src/eSurfaceSet_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.

Write template: preserve cWriteBlock/cOutStream construction, field write order, and End() placement. SNC is sensitive to temporary bool/cast shapes and to independent store ordering.

## Class Header: `include/eSurfaceSet.h`

Not found.

## Matched Same-Class Neighbors

- `0x00053d04` 200B `eSurfaceSet::Read(cFile &, cMemPool *)` — `src/eSurfaceSet.cpp`
- `0x001f65e8` 76B `eSurfaceSet::AssignCopy(const cBase *)` — `src/eSurfaceSet.cpp`
- `0x001f6634` 140B `eSurfaceSet::New(cMemPool *, cBase *) static` — `src/eSurfaceSet.cpp`
- `0x001f66c0` 288B `eSurfaceSet::GetType(void) const` — `src/eSurfaceSet.cpp`
- `0x001f6b68` 284B `eSurfaceSet::~eSurfaceSet(void)` — `src/eSurfaceSet.cpp`

## Matched Method Exemplars

### Exemplar 1: `eStaticModelTemplate::Write(cFile &) const`

- Address: `0x00041158`
- Size: `200` bytes
- Source: `src/eStaticModelTemplate.cpp`

```cpp
// -- eStaticModelTemplate::Write(cFile &) const @ 0x00041158 --
void eStaticModelTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    ((const eStaticGeomTemplate *)this)->Write(file);
    mHandle44.Write(wb);
    mHandle48.Write(wb);
    mHandle4C.Write(wb);
    wb.Write(mField50);

    int count = 0;
    if (mValues.mData != 0) {
        count = ((int *)mValues.mData)[-1] & 0x3FFFFFFF;
    }
    wb.Write(count);

    count = 0;
    if (mValues.mData != 0) {
        count = ((int *)mValues.mData)[-1] & 0x3FFFFFFF;
    }
    wb.Write(count, mValues.mData);
    wb.End();
}
```

### Exemplar 2: `eConfigPSP::Write(cFile &) const`

- Address: `0x0001cb34`
- Size: `196` bytes
- Source: `src/eConfigPSP.cpp`

```cpp
// eConfigPSP::Write(cFile &) const @ 0x0001cb34
void eConfigPSP::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    eConfigBase::Write(file);
    wb.Write(*(const int *)((const char *)this + 0x6C));
    wb.Write(*(const int *)((const char *)this + 0x70));
    wb.Write(*(const int *)((const char *)this + 0x74));
    wb.Write(*(const float *)((const char *)this + 0x78));
    wb.Write(*(const float *)((const char *)this + 0x7C));
    wb.Write(*(const bool *)((const char *)this + 0x80));
    wb.Write(*(const int *)((const char *)this + 0x84));
    wb.Write(*(const int *)((const char *)this + 0x88));
    wb.Write(*(const int *)((const char *)this + 0x8C));
    wb.Write(*(const int *)((const char *)this + 0x90));
    wb.End();
}
```

### Exemplar 3: `eSoundData::Write(cFile &) const`

- Address: `0x00020690`
- Size: `196` bytes
- Source: `src/eSoundData.cpp`

```cpp
void eSoundData::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    cObject::Write(file);
    wb.Write(mField64);
    wb.Write(mField44);
    wb.Write(mField48);
    wb.Write(mField4C);
    wb.Write(mField50);
    wb.Write(mField51);
    wb.Write(mField52);
    wb.Write(mField54);
    wb.Write(mField58);
    wb.Write(mField5C);
    wb.End();
}
```

### Exemplar 4: `eTwoPassModelMtl::Write(cFile &) const`

- Address: `0x0008afb8`
- Size: `204` bytes
- Source: `src/eTwoPassModelMtl.cpp`

```cpp
void eTwoPassModelMtl::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eModelMtl *)this)->Write(file);

    cHandle *h = (cHandle *)((char *)this + 0x48);
    h->Write(wb);
    ((cHandle *)((char *)h + 4))->Write(wb);

    void *desc1 = *(void **)((char *)this + 0x80);
    ClassDescEntry *entry1 = (ClassDescEntry *)((char *)desc1 + 0x28);
    short off1 = entry1->offset;
    __asm__ volatile("" ::: "memory");
    char *tcg1_base = (char *)this + 0x7C;
    void *target1 = tcg1_base + off1;
    entry1->fn(target1, (void *)wb._data[0]);

    void *desc2 = *(void **)((char *)this + 0xF4);
    ClassDescEntry *entry2 = (ClassDescEntry *)((char *)desc2 + 0x28);
    short off2 = entry2->offset;
    __asm__ volatile("" ::: "memory");
    char *tcg2_base = (char *)this + 0xF0;
    void *target2 = tcg2_base + off2;
    entry2->fn(target2, (void *)wb._data[0]);

    wb.Write(((int *)this)[0x74 / 4]);
    wb.Write(((int *)this)[0x78 / 4]);
    wb.Write((unsigned int)((int *)this)[0x164 / 4]);
    wb.End();
}
```


## Prior Failure Notes

### Note 1

- session `9e194a67`; src `src/eSurfaceSet.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/9e194a67/00053c3c__agent_self_reported_failure__src_eSurfaceSet.cpp`

Closer than prior attempt — got down to 21/200 bytes diff (vs prior session's near-miss with different shape) by reformulating the loop as `data[i].Write(wb)` array indexing. This triggered the strength-reduction pattern in the second pass: SNC now emits the `li s2,0; addu s2,a0,s2` (induction-variable initialization) and uses caller-saved a0 for the second mData load, leaving s0/s1/s2 as count/i/ptr respectively — matching target exactly in the loop body. Function size is now 200B (matching DB).

Residual diff is concentrated in the FIRST-pass register choice: target loads mData→a0 and routes count→a1 directly into the call-arg slot, then emits `move s1,s0` to keep `this` alive in s1 across wb.Write(int). Our codegen loads mData→a1 and routes count→a0, ending in `move a1,a0` instead of `move s1,s0`. As a downstream consequence, our second-pass mData load is `lw a0,68(s0)` (this stays in s0) where target has `lw a0,68(s1)` — same instruction count, different bytes. Tried: ternary inline of count, sched=2 per-function pragma, register __asm__("s1") on `this`, s0 clobber barrier between calls, sharing one `data`/`total` across both passes (this got `move s0,a1; move s1,a0` swap — same idea but mirrored, 25-byte diff), single-name vs separate-name variables, declaring i early. SNC's allocator deterministically picks mData→a1 here regardless. The trigger appears to be a global decision the allocator makes about which of {first-count, this} owns s0 across the call; without a way to influence that choice from C, this cascades. Permuter would likely fix in <5min on PRNG. Recommend permuter for the remaining 21 bytes.

### Note 2

- session `e936a10a`

Implemented the serializer as the obvious eMaterialSet-style handle-array writer: construct cWriteBlock(file, 1), call cObject::Write(file), compute the count twice from the header word at mData[-1] & 0x3FFFFFFF, write the count, then iterate handles at +0x44 and call cHandle::Write(wb), then End(). That clean C shape consistently compiles to a 196-byte function. The generated code is very close to target: same overall control flow and call sequence, but the first count path keeps the count in s1 and calls wb.Write without the target's extra `move s1,s0`, and the second pass uses `lw s2,68(s0)` / `move a0,s2` instead of the target's `lw a0,68(s1)` / `addu s2,a0,s2` setup.

Tried several source-level restructurings targeting exactly that register-lifetime gap: reordering declarations into m2c-style top-of-function locals, introducing a short-lived `this` alias between the first count and the second array load, rewriting the loop as both pointer-increment and base+offset forms, and testing a local `#pragma control sched=2` override because this function sits in the 0x040000-0x06e000 transition zone. None of those variants produced the missing 4-byte instruction; sched=1 and sched=2 both settled on the same 196-byte shape. The likely root cause is transition-zone register allocation/scheduling peculiarities for this class rather than high-level control-flow mismatch.


## Disassembly

```asm
53c3c:	27bdffe0 	addiu	sp,sp,-32
53c40:	afb00008 	sw	s0,8(sp)
53c44:	afb1000c 	sw	s1,12(sp)
53c48:	afb20010 	sw	s2,16(sp)
53c4c:	afbf0014 	sw	ra,20(sp)
53c50:	00a08825 	move	s1,a1
53c54:	00808025 	move	s0,a0
53c58:	03a02025 	move	a0,sp
53c5c:	0c001516 	jal	0x5458
53c60:	34060001 	li	a2,0x1
53c64:	02002025 	move	a0,s0
53c68:	0c0026cc 	jal	0x9b30
53c6c:	02202825 	move	a1,s1
53c70:	8e040044 	lw	a0,68(s0)
53c74:	10800005 	beqz	a0,0x53c8c
53c78:	34050000 	li	a1,0x0
53c7c:	8c85fffc 	lw	a1,-4(a0)
53c80:	3c044000 	lui	a0,0x4000
53c84:	2484ffff 	addiu	a0,a0,-1
53c88:	00a42824 	and	a1,a1,a0
53c8c:	02008825 	move	s1,s0
53c90:	0c0015a4 	jal	0x5690
53c94:	03a02025 	move	a0,sp
53c98:	8e240044 	lw	a0,68(s1)
53c9c:	10800005 	beqz	a0,0x53cb4
53ca0:	34100000 	li	s0,0x0
53ca4:	8c90fffc 	lw	s0,-4(a0)
53ca8:	3c054000 	lui	a1,0x4000
53cac:	24a5ffff 	addiu	a1,a1,-1
53cb0:	02058024 	and	s0,s0,a1
53cb4:	34110000 	li	s1,0x0
53cb8:	0230282a 	slt	a1,s1,s0
53cbc:	10a00009 	beqz	a1,0x53ce4
53cc0:	34120000 	li	s2,0x0
53cc4:	00929021 	addu	s2,a0,s2
53cc8:	02402025 	move	a0,s2
53ccc:	0c00240c 	jal	0x9030
53cd0:	03a02825 	move	a1,sp
53cd4:	26310001 	addiu	s1,s1,1
53cd8:	0230202a 	slt	a0,s1,s0
53cdc:	1480fffa 	bnez	a0,0x53cc8
53ce0:	26520004 	addiu	s2,s2,4
53ce4:	0c001724 	jal	0x5c90
53ce8:	03a02025 	move	a0,sp
53cec:	8fb00008 	lw	s0,8(sp)
53cf0:	8fb1000c 	lw	s1,12(sp)
53cf4:	8fb20010 	lw	s2,16(sp)
53cf8:	8fbf0014 	lw	ra,20(sp)
53cfc:	03e00008 	jr	ra
53d00:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: eSurfaceSet::Write(cFile &) const
// Address: 0x00053c3c, Size: 200B
// Obj: eAll_psp.obj

? cHandle__Write_cWriteBlockrefconst(void *, void *); /* extern */
? cObject__Write_cFilerefconst(void *, ?);          /* extern */
? cWriteBlock__End_void(void *);                    /* extern */
? cWriteBlock__Write_int(void *, s32);              /* extern */
? cWriteBlock__cWriteBlock_cFileref_unsignedint(void *, ?); /* extern */

void eSurfaceSet__Write_cFilerefconst(void *arg0, ? arg1) {
    s32 var_a1;
    s32 var_s0;
    s32 var_s1;
    void *temp_a0;
    void *temp_a0_2;
    void *var_s2;

    cWriteBlock__cWriteBlock_cFileref_unsignedint(sp, 1);
    cObject__Write_cFilerefconst(arg0, arg1);
    temp_a0 = arg0->unk44;
    var_a1 = 0;
    if (temp_a0 != NULL) {
        var_a1 = temp_a0->unk-4 & 0x3FFFFFFF;
    }
    cWriteBlock__Write_int(sp, var_a1);
    temp_a0_2 = arg0->unk44;
    var_s0 = 0;
    if (temp_a0_2 != NULL) {
        var_s0 = temp_a0_2->unk-4 & 0x3FFFFFFF;
    }
    var_s1 = 0;
    if (var_s0 > 0) {
        var_s2 = temp_a0_2;
        do {
            cHandle__Write_cWriteBlockrefconst(var_s2, sp);
            var_s1 += 1;
            var_s2 += 4;
        } while (var_s1 < var_s0);
    }
    cWriteBlock__End_void(sp);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
