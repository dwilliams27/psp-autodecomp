# Battle Packet: `gcTableTemplate::~gcTableTemplate(void)`

## Target

- Address: `0x0012bae8`
- Size: `304` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcTableTemplate`
- Method family: `Destructor`
- Leaf: `False`

## Queue Metadata

- score: `151`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 11B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `11`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `11`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x000070bc` `untried` 240B `cBaseArray::RemoveAll(void)`
- `0x00009dc4` `failed` 432B `cObject::~cObject(void)`

## Placement

- Canonical source: `src/gcTableTemplate.cpp`
- Header(s): `include/gcTableTemplate.h`
- Allowed source(s): `src/gcTableTemplate.cpp`
- Split-TU prefix: `src/gcTableTemplate_*.cpp`

## Compiler Guidance

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/gcTableTemplate.h`

Not found.

## Matched Same-Class Neighbors

- `0x0012b98c` 276B `gcTableTemplate::Read(cFile &, cMemPool *)` — `src/gcTableTemplate.cpp`
- `0x0012baa0` 72B `gcTableTemplate::gcTableTemplate(cBase *)` — `src/gcTableTemplate.cpp`
- `0x0012bcf0` 36B `gcTableTemplate::GetNumRows(void) const` — `src/gcTableTemplate.cpp`
- `0x00273aa0` 112B `gcTableTemplate::AssignCopy(const cBase *)` — `src/gcTableTemplate.cpp`
- `0x00273b10` 124B `gcTableTemplate::New(cMemPool *, cBase *) static` — `src/gcTableTemplate.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcDesiredEntityTemplate::~gcDesiredEntityTemplate(void)`

- Address: `0x0026950c`
- Size: `304` bytes
- Source: `src/gcDesiredEntityTemplate.cpp`

```cpp
gcDesiredEntityTemplate::~gcDesiredEntityTemplate(void) {
    *(void **)((char *)this + 4) = (void *)0x389508;
    char *second = (char *)this + 8;

    if ((void *)((char *)this + 0x14) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x14);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x14) = 0;
            }
        }
    }

    *(void **)((char *)this + 4) = (void *)0x3889A8;
    if ((void *)second != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 8);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 8) = 0;
        }
    }

    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}
```

### Exemplar 2: `gcDesiredString::~gcDesiredString(void)`

- Address: `0x00270d78`
- Size: `304` bytes
- Source: `src/gcDesiredString.cpp`

```cpp
gcDesiredString::~gcDesiredString(void) {
    *(void **)((char *)this + 4) = (void *)0x3899E0;
    char *second = (char *)this + 8;

    if ((void *)((char *)this + 0x18) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x18);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x18) = 0;
            }
        }
    }

    *(void **)((char *)this + 4) = (void *)0x3889A8;
    if ((void *)second != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 8);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 8) = 0;
        }
    }

    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}
```

### Exemplar 3: `gcStatsTrackingStrings::~gcStatsTrackingStrings(void)`

- Address: `0x00289800`
- Size: `296` bytes
- Source: `src/gcStatsTrackingStrings.cpp`

```cpp
gcStatsTrackingStrings::~gcStatsTrackingStrings(void) {
    *(void **)((char *)this + 4) = gcStatsTrackingStringsvirtualtable;
    char *second = (char *)this + 0x0C;

    if ((void *)((char *)this + 0x10) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x10);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x10) = 0;
            }
        }
    }

    if ((void *)second != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x0C);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0x0C) = 0;
        }
    }

    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}
```

### Exemplar 4: `gcConfiguration::~gcConfiguration(void)`

- Address: `0x000f069c`
- Size: `312` bytes
- Source: `src/gcConfiguration.cpp`

```cpp
// ── gcConfiguration::~gcConfiguration(void) @ 0x000f069c ──
gcConfiguration::~gcConfiguration(void) {
    register void *vtable __asm__("$5");
    __asm__ volatile(
        "lui $4, 0x38\n"
        "addiu %0, $4, 0x7b50"
        : "=r"(vtable)
        :
        : "$4");
    cBase *field50 = *(cBase **)((char *)this + 0x50);
    *(void **)((char *)this + 4) = vtable;
    if (field50 != 0) {
        char *typeInfo = *(char **)((char *)field50 + 4);
        DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
        slot->fn((char *)field50 + slot->offset, (void *)3);
        *(cBase **)((char *)this + 0x50) = 0;
    }

    cBase *field4C = *(cBase **)((char *)this + 0x4C);
    if (field4C != 0) {
        char *typeInfo = *(char **)((char *)field4C + 4);
        DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
        slot->fn((char *)field4C + slot->offset, (void *)3);
        *(cBase **)((char *)this + 0x4C) = 0;
    }

    cBase *field48 = *(cBase **)((char *)this + 0x48);
    if (field48 != 0) {
        char *typeInfo = *(char **)((char *)field48 + 4);
        DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
        slot->fn((char *)field48 + slot->offset, (void *)3);
        *(cBase **)((char *)this + 0x48) = 0;
    }

    cBase *field44 = *(cBase **)((char *)this + 0x44);
    if (field44 != 0) {
        char *typeInfo = *(char **)((char *)field44 + 4);
        DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
        slot->fn((char *)field44 + slot->offset, (void *)3);
        *(cBase **)((char *)this + 0x44) = 0;
    }

    *(gcConfiguration **)0x37D7E4 = 0;
}
```


## Prior Failure Notes

### Note 1

- session `5ec22464`; src `src/gcTableTemplate.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/5ec22464/0012bae8__agent_self_reported_failure__src_gcTableTemplate.cpp`

~gcTableTemplate at 11/304 byte diff — pure s2/s3 register allocation swap. Expected uses s2 for slot48 ((char*)this+0x48 / cBaseArray*) and s3 for data (mData int* at this+0x50). My output swaps these. Key wins: (1) inline class-local `operator delete` with null check produces correct pool dispatch (pool=GetPoolFromPtr; block=pool[9]; entry=block->allocTable+0x30; fn(block+entry.offset, p)) — without inlining, compiler emits __0OdlPv global delete instead. (2) `__asm__(".word 0x1000ffff\n.word 0")` + `.size __0oPgcTableTemplatedtv, 0x130` adds the trailing `b $; nop`. Tried: declaring slot48 first vs data first, retyping slot48 as cBaseArray* arr48, both produced identical s2/s3 swap. Next try: -Xsched=1, or move arr48 ref into the inner if-data block, or aliasing trick to tie slot48 lifetime to before data load.


## Disassembly

```asm
12bae8:	27bdffe0 	addiu	sp,sp,-32
12baec:	afb10004 	sw	s1,4(sp)
12baf0:	00808825 	move	s1,a0
12baf4:	afb00000 	sw	s0,0(sp)
12baf8:	afb20008 	sw	s2,8(sp)
12bafc:	afb3000c 	sw	s3,12(sp)
12bb00:	afbf0010 	sw	ra,16(sp)
12bb04:	1080003b 	beqz	a0,0x12bbf4
12bb08:	00a08025 	move	s0,a1
12bb0c:	3c040039 	lui	a0,0x39
12bb10:	24849f60 	addiu	a0,a0,-24736
12bb14:	ae240004 	sw	a0,4(s1)
12bb18:	26240050 	addiu	a0,s1,80
12bb1c:	1080001f 	beqz	a0,0x12bb9c
12bb20:	26320048 	addiu	s2,s1,72
12bb24:	8e330050 	lw	s3,80(s1)
12bb28:	12600005 	beqz	s3,0x12bb40
12bb2c:	34050000 	li	a1,0x0
12bb30:	8e65fffc 	lw	a1,-4(s3)
12bb34:	3c044000 	lui	a0,0x4000
12bb38:	2484ffff 	addiu	a0,a0,-1
12bb3c:	00a42824 	and	a1,a1,a0
12bb40:	34040000 	li	a0,0x0
12bb44:	0085302a 	slt	a2,a0,a1
12bb48:	10c00005 	beqz	a2,0x12bb60
12bb4c:	00000000 	nop
12bb50:	24840001 	addiu	a0,a0,1
12bb54:	0085302a 	slt	a2,a0,a1
12bb58:	14c0fffe 	bnez	a2,0x12bb54
12bb5c:	24840001 	addiu	a0,a0,1
12bb60:	1260000e 	beqz	s3,0x12bb9c
12bb64:	2673fffc 	addiu	s3,s3,-4
12bb68:	5260000c 	beqzl	s3,0x12bb9c
12bb6c:	ae200050 	sw	zero,80(s1)
12bb70:	0c0011e8 	jal	0x47a0
12bb74:	02602025 	move	a0,s3
12bb78:	8c440024 	lw	a0,36(v0)
12bb7c:	02602825 	move	a1,s3
12bb80:	8c86001c 	lw	a2,28(a0)
12bb84:	24c60030 	addiu	a2,a2,48
12bb88:	84c70000 	lh	a3,0(a2)
12bb8c:	8cc60004 	lw	a2,4(a2)
12bb90:	00c0f809 	jalr	a2
12bb94:	00872021 	addu	a0,a0,a3
12bb98:	ae200050 	sw	zero,80(s1)
12bb9c:	12400004 	beqz	s2,0x12bbb0
12bba0:	02202025 	move	a0,s1
12bba4:	0c001c2f 	jal	0x70bc
12bba8:	02402025 	move	a0,s2
12bbac:	02202025 	move	a0,s1
12bbb0:	0c002771 	jal	0x9dc4
12bbb4:	00002825 	move	a1,zero
12bbb8:	32040001 	andi	a0,s0,0x1
12bbbc:	1080000d 	beqz	a0,0x12bbf4
12bbc0:	00000000 	nop
12bbc4:	1220000b 	beqz	s1,0x12bbf4
12bbc8:	00000000 	nop
12bbcc:	0c0011e8 	jal	0x47a0
12bbd0:	02202025 	move	a0,s1
12bbd4:	8c440024 	lw	a0,36(v0)
12bbd8:	02202825 	move	a1,s1
12bbdc:	8c86001c 	lw	a2,28(a0)
12bbe0:	24c60030 	addiu	a2,a2,48
12bbe4:	84c70000 	lh	a3,0(a2)
12bbe8:	8cc60004 	lw	a2,4(a2)
12bbec:	00c0f809 	jalr	a2
12bbf0:	00872021 	addu	a0,a0,a3
12bbf4:	8fb00000 	lw	s0,0(sp)
12bbf8:	8fb10004 	lw	s1,4(sp)
12bbfc:	8fb20008 	lw	s2,8(sp)
12bc00:	8fb3000c 	lw	s3,12(sp)
12bc04:	8fbf0010 	lw	ra,16(sp)
12bc08:	03e00008 	jr	ra
12bc0c:	27bd0020 	addiu	sp,sp,32
12bc10:	1000ffff 	b	0x12bc10
12bc14:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: gcTableTemplate::~gcTableTemplate(void)
// Address: 0x0012bae8, Size: 304B
// Obj: gcAll_psp.obj

? cBaseArray__RemoveAll_void(void *);               /* extern */
void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */
? cObject___dtor_cObject_void(void *, ?);           /* extern */

void gcTableTemplate___dtor_gcTableTemplate_void(void *arg0, s32 arg1) {
    ? (*temp_a2_2)(void *, void *, ?, s16);
    ? (*temp_a2_4)(void *, void *, ?, s16);
    s16 temp_a3;
    s16 temp_a3_2;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a2;
    void *temp_a2_3;
    void *temp_s2;
    void *temp_s3;
    void *temp_s3_2;

    if (arg0 != NULL) {
        arg0->unk4 = 0x389F60;
        temp_s2 = arg0 + 0x48;
        if ((arg0 + 0x50) != NULL) {
            temp_s3 = arg0->unk50;
            var_a1 = 0;
            if (temp_s3 != NULL) {
                var_a1 = temp_s3->unk-4 & 0x3FFFFFFF;
            }
            var_a2 = var_a1 > 0;
            if (var_a2 != 0) {
                var_a0 = 1;
                do {
                    var_a2 = var_a0 < var_a1;
                    var_a0 += 1;
                } while (var_a2 != 0);
            }
            temp_s3_2 = temp_s3 - 4;
            if (temp_s3 != NULL) {
                if (temp_s3_2 != NULL) {
                    temp_a0 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s3_2, var_a1, var_a2)->unk24;
                    temp_a2 = temp_a0->unk1C;
                    temp_a3 = temp_a2->unk30;
                    temp_a2_2 = (temp_a2 + 0x30)->unk4;
                    temp_a2_2(temp_a0 + temp_a3, temp_s3_2, temp_a2_2, temp_a3);
                }
                arg0->unk50 = NULL;
            }
        }
        if (temp_s2 != NULL) {
            cBaseArray__RemoveAll_void(temp_s2);
        }
        cObject___dtor_cObject_void(arg0, 0);
        if ((arg1 & 1) && (arg0 != NULL)) {
            temp_a0_2 = cMemPool__GetPoolFromPtr_constvoidptrstatic(arg0)->unk24;
            temp_a2_3 = temp_a0_2->unk1C;
            temp_a3_2 = temp_a2_3->unk30;
            temp_a2_4 = (temp_a2_3 + 0x30)->unk4;
            temp_a2_4(temp_a0_2 + temp_a3_2, arg0, temp_a2_4, temp_a3_2);
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
