# Battle Packet: `eHeightmapTemplate::~eHeightmapTemplate(void)`

## Target

- Address: `0x00052168`
- Size: `348` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eHeightmapTemplate`
- Method family: `Destructor`
- Leaf: `False`

## Queue Metadata

- score: `151`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 16B; codegen-specific diagnosis; manageable size`
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
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x00009dc4` `failed` 432B `cObject::~cObject(void)`
- `0x000520b8` `failed` 56B `eHeightmapTemplate::PlatformFree(void)`

## Placement

- Canonical source: `src/eHeightmapTemplate.cpp`
- Header(s): `include/eHeightmapTemplate.h`
- Allowed source(s): `src/eHeightmapTemplate.cpp`, `src/eHeightmapTemplate_dtor.cpp`
- Split-TU prefix: `src/eHeightmapTemplate_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eHeightmapTemplate.h`

Not found.

## Matched Same-Class Neighbors

- `0x00051b54` 332B `eHeightmapTemplate::Write(cFile &) const` — `src/eHeightmapTemplate.cpp`
- `0x000520f0` 120B `eHeightmapTemplate::eHeightmapTemplate(cBase *)` — `src/eHeightmapTemplate.cpp`
- `0x00052380` 276B `eHeightmapTemplate::GetInstanceType(void) const` — `src/eHeightmapTemplate.cpp`
- `0x001f55e8` 228B `eHeightmapTemplate::AssignCopy(const cBase *)` — `src/eHeightmapTemplate.cpp`
- `0x001f56cc` 124B `eHeightmapTemplate::New(cMemPool *, cBase *) static` — `src/eHeightmapTemplate.cpp`

## Matched Method Exemplars

### Exemplar 1: `eSky::~eSky(void)`

- Address: `0x0005d1a8`
- Size: `356` bytes
- Source: `src/eSky.cpp`

```cpp
//   0x0005d1a8  eSky::~eSky(void)                           (356B)
//
// eSky inherits from cObject. Layout:
//   [0x00..0x44) cObject base
//   [0x44..0x48) cArrayBase<cHandleT<eGeomTemplate> >
//   [0x48..0x50) cBaseArray
//   [0x50..0x58) cBaseArray
//   [0x58..0x60) cBaseArray
// sizeof(eSky) = 0x60

class cBase;
class cFile;
class eGeomTemplate;
class cType;

template <class T> T *dcast(const cBase *);

template <class T>
class cHandleT {
public:
    int mIndex;
};
```

### Exemplar 2: `eSimulatedController::~eSimulatedController(void)`

- Address: `0x0006bf10`
- Size: `364` bytes
- Source: `src/eSimulatedController_dtor.cpp`

```cpp
eSimulatedController::~eSimulatedController() {
    *(void **)((char *)this + 4) = (void *)0x3834D8;
    cBaseArray *arr44 = (cBaseArray *)((char *)this + 0x44);
    cBaseArray *arr3C = (cBaseArray *)((char *)this + 0x3C);
    if (arr44 != 0) {
        arr44->RemoveAll();
    }
    if (arr3C != 0) {
        arr3C->RemoveAll();
    }
    void *bodyArr = (char *)this + 0x38;
    if (bodyArr != 0) {
        void *entries = *(void **)((char *)this + 0x38);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries -
                            (((unsigned int)*(int *)((char *)entries - 4) >> 30) * 4) -
                            4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                __asm__ volatile("" ::: "memory");
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x38) = 0;
        }
    }
    ePhysicsController_dtor(this, 0);
}
```

### Exemplar 3: `eDynamicMeshVisData::~eDynamicMeshVisData(void)`

- Address: `0x0008f628`
- Size: `316` bytes
- Source: `src/eDynamicMeshVisData.cpp`

```cpp
eDynamicMeshVisData::~eDynamicMeshVisData(void) {
    void *first = field_10C;
    if (first != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(first);
        char *block = ((char **)pool)[9];
        register volatile eDynamicMeshVisData_DeleteRecord *rec __asm__("a1") =
            (volatile eDynamicMeshVisData_DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void *base = block + off;
        void (*fn)(void *, void *, short, void *) =
            (void (*)(void *, void *, short, void *))rec->fn;
        fn(base, field_10C, off, (void *)fn);
        field_10C = 0;
    }

    void *second = field_114;
    if (second != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(second);
        char *block = ((char **)pool)[9];
        register volatile eDynamicMeshVisData_DeleteRecord *rec __asm__("a1") =
            (volatile eDynamicMeshVisData_DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void *base = block + off;
        void (*fn)(void *, void *, short, void *) =
            (void (*)(void *, void *, short, void *))rec->fn;
        fn(base, field_114, off, (void *)fn);
        field_114 = 0;
    }

    void *morphArray = morphLists;
    __asm__ volatile("" : "+r"(morphArray));
    void (*morphDtor)(void *) = (void (*)(void *))0x21DCA8;
    __asm__ volatile("" : "+r"(morphDtor));
    __vec_delete(morphArray, 0x20, 4, morphDtor, 0, 0);
    void *skinArray = skinLists;
    __asm__ volatile("" : "+r"(skinArray));
    void (*skinDtor)(void *) = (void (*)(void *))0x21DB7C;
    __asm__ volatile("" : "+r"(skinDtor));
    __vec_delete(skinArray, 0x20, 4, skinDtor, 0, 0);
}
```

### Exemplar 4: `eWeatherSystem::~eWeatherSystem(void)`

- Address: `0x00061a10`
- Size: `312` bytes
- Source: `src/eWeatherSystem.cpp`

```cpp
// ── eWeatherSystem::~eWeatherSystem(void) @ 0x00061a10 ──
#pragma control sched=1
eWeatherSystem::~eWeatherSystem(void) {
    *(void **)((char *)this + 4) = (void *)0x382B80;
    *(eWeatherSystem **)0x37D31C = 0;

    void *baseArray = (char *)this + 0x0C;
    void *handleArray = (char *)this + 0x08;
    if (baseArray != 0) {
        ((cBaseArray *)baseArray)->RemoveAll();
    }

    if (handleArray != 0) {
        void *entries = *(void **)((char *)this + 0x08);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries - 4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                __asm__ volatile("" ::: "memory");
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x08) = 0;
        }
    }

    if (this != 0) {
        *(void **)((char *)this + 4) = (void *)0x37E6A8;
    }
}
```


## Prior Failure Notes

### Note 1

- session `7f5942e0`; src `src/eHeightmapTemplate_dtor.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/7f5942e0/00052168__agent_self_reported_failure__src_eHeightmapTemplate_dtor.cpp`

Created split-TU file following ePath_dtor.cpp pattern. Got to MISMATCH: 16/348 bytes differ. Size matches (348B). Structural diff is purely in the data84 deleting destructor inline call sequence (bytes 0x38-0x50, 7 instructions). Expected: lw a1,4(a0); addiu a1,a1,80; lh a2,0(a1); addu a0,a0,a2; lw a2,4(a1); jalr a2; li a1,3 (delay). Mine: lw a2,4(a0); li a1,3; addiu a2,a2,80; lh a3,0(a2); lw a2,4(a2); jalr a2; addu a0,a0,a3 (delay). Both 7 instructions but expected reuses a1 across vtable→entry→arg sequence while mine reserves a1 early for the arg=3 constant, forcing offset into a3 instead of a2 and base computation into the delay slot. This is REG_ALLOC drift — SNC chooses to pre-load the call arg early in mine. Tried: (a) inline call without fn variable (no change), (b) DeleteEntry struct vs raw (entry+4) access (DeleteEntry folds correctly to lw 4(a1)), (c) memory barrier after vtable store (fixed earlier issue where data84 was loaded to a1 then moved to a0 — saved 1 instruction). Suggestion for next agent: try inline asm hint to force a1 = 3 only at call site, or restructure the inline call so the second arg uses a value that depends on the vtable manipulation. Permuter recommended (5+ min).


## Disassembly

```asm
52168:	27bdfff0 	addiu	sp,sp,-16
5216c:	afb00000 	sw	s0,0(sp)
52170:	afb10004 	sw	s1,4(sp)
52174:	afb20008 	sw	s2,8(sp)
52178:	afbf000c 	sw	ra,12(sp)
5217c:	00808825 	move	s1,a0
52180:	10800048 	beqz	a0,0x522a4
52184:	00a08025 	move	s0,a1
52188:	3c040038 	lui	a0,0x38
5218c:	24841a90 	addiu	a0,a0,6800
52190:	ae240004 	sw	a0,4(s1)
52194:	8e240084 	lw	a0,132(s1)
52198:	10800009 	beqz	a0,0x521c0
5219c:	2632007c 	addiu	s2,s1,124
521a0:	8c850004 	lw	a1,4(a0)
521a4:	24a50050 	addiu	a1,a1,80
521a8:	84a60000 	lh	a2,0(a1)
521ac:	00862021 	addu	a0,a0,a2
521b0:	8ca60004 	lw	a2,4(a1)
521b4:	00c0f809 	jalr	a2
521b8:	34050003 	li	a1,0x3
521bc:	ae200084 	sw	zero,132(s1)
521c0:	0c01482e 	jal	0x520b8
521c4:	02202025 	move	a0,s1
521c8:	1240001f 	beqz	s2,0x52248
521cc:	00000000 	nop
521d0:	8e32007c 	lw	s2,124(s1)
521d4:	12400005 	beqz	s2,0x521ec
521d8:	34050000 	li	a1,0x0
521dc:	8e45fffc 	lw	a1,-4(s2)
521e0:	3c044000 	lui	a0,0x4000
521e4:	2484ffff 	addiu	a0,a0,-1
521e8:	00a42824 	and	a1,a1,a0
521ec:	34040000 	li	a0,0x0
521f0:	0085302a 	slt	a2,a0,a1
521f4:	10c00005 	beqz	a2,0x5220c
521f8:	00000000 	nop
521fc:	24840001 	addiu	a0,a0,1
52200:	0085302a 	slt	a2,a0,a1
52204:	14c0fffe 	bnez	a2,0x52200
52208:	24840001 	addiu	a0,a0,1
5220c:	1240000e 	beqz	s2,0x52248
52210:	2652fffc 	addiu	s2,s2,-4
52214:	5240000c 	beqzl	s2,0x52248
52218:	ae20007c 	sw	zero,124(s1)
5221c:	0c0011e8 	jal	0x47a0
52220:	02402025 	move	a0,s2
52224:	8c440024 	lw	a0,36(v0)
52228:	8c85001c 	lw	a1,28(a0)
5222c:	24a50030 	addiu	a1,a1,48
52230:	84a60000 	lh	a2,0(a1)
52234:	00862021 	addu	a0,a0,a2
52238:	8ca60004 	lw	a2,4(a1)
5223c:	00c0f809 	jalr	a2
52240:	02402825 	move	a1,s2
52244:	ae20007c 	sw	zero,124(s1)
52248:	12200008 	beqz	s1,0x5226c
5224c:	32040001 	andi	a0,s0,0x1
52250:	3c040038 	lui	a0,0x38
52254:	24840c18 	addiu	a0,a0,3096
52258:	ae240004 	sw	a0,4(s1)
5225c:	02202025 	move	a0,s1
52260:	0c002771 	jal	0x9dc4
52264:	00002825 	move	a1,zero
52268:	32040001 	andi	a0,s0,0x1
5226c:	1080000d 	beqz	a0,0x522a4
52270:	00000000 	nop
52274:	1220000b 	beqz	s1,0x522a4
52278:	00000000 	nop
5227c:	0c0011e8 	jal	0x47a0
52280:	02202025 	move	a0,s1
52284:	8c440024 	lw	a0,36(v0)
52288:	8c85001c 	lw	a1,28(a0)
5228c:	24a50030 	addiu	a1,a1,48
52290:	84a60000 	lh	a2,0(a1)
52294:	00862021 	addu	a0,a0,a2
52298:	8ca60004 	lw	a2,4(a1)
5229c:	00c0f809 	jalr	a2
522a0:	02202825 	move	a1,s1
522a4:	8fb00000 	lw	s0,0(sp)
522a8:	8fb10004 	lw	s1,4(sp)
522ac:	8fb20008 	lw	s2,8(sp)
522b0:	8fbf000c 	lw	ra,12(sp)
522b4:	03e00008 	jr	ra
522b8:	27bd0010 	addiu	sp,sp,16
522bc:	1000ffff 	b	0x522bc
522c0:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: eHeightmapTemplate::~eHeightmapTemplate(void)
// Address: 0x00052168, Size: 348B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *, s32, s32); /* extern */
? cObject___dtor_cObject_void(void *, ?);           /* extern */
? eHeightmapTemplate__PlatformFree_void(void *);    /* extern */

void eHeightmapTemplate___dtor_eHeightmapTemplate_void(void *arg0, s32 arg1) {
    ? (*temp_a2)(void *, ?, ?);
    ? (*temp_a2_2)(void *, void *, ?);
    ? (*temp_a2_3)(void *, void *, ?);
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a1;
    s32 var_a2;
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a1;
    void *temp_a1_2;
    void *temp_a1_3;
    void *temp_s2;
    void *temp_s2_2;

    if (arg0 != NULL) {
        arg0->unk4 = 0x381A90;
        temp_a0 = arg0->unk84;
        if (temp_a0 != NULL) {
            temp_a1 = temp_a0->unk4;
            temp_a2 = (temp_a1 + 0x50)->unk4;
            temp_a2(temp_a0 + temp_a1->unk50, 3, temp_a2);
            arg0->unk84 = NULL;
        }
        eHeightmapTemplate__PlatformFree_void(arg0);
        if ((arg0 + 0x7C) != NULL) {
            temp_s2 = arg0->unk7C;
            var_a1 = 0;
            if (temp_s2 != NULL) {
                var_a1 = temp_s2->unk-4 & 0x3FFFFFFF;
            }
            var_a2 = var_a1 > 0;
            if (var_a2 != 0) {
                var_a0 = 1;
                do {
                    var_a2 = var_a0 < var_a1;
                    var_a0 += 1;
                } while (var_a2 != 0);
            }
            temp_s2_2 = temp_s2 - 4;
            if (temp_s2 != NULL) {
                if (temp_s2_2 != NULL) {
                    temp_a0_2 = cMemPool__GetPoolFromPtr_constvoidptrstatic(temp_s2_2, var_a1, var_a2)->unk24;
                    temp_a1_2 = temp_a0_2->unk1C;
                    temp_a2_2 = (temp_a1_2 + 0x30)->unk4;
                    temp_a2_2(temp_a0_2 + temp_a1_2->unk30, temp_s2_2, temp_a2_2);
                }
                arg0->unk7C = NULL;
            }
        }
        var_a0_2 = arg1 & 1;
        if (arg0 != NULL) {
            arg0->unk4 = 0x380C18;
            cObject___dtor_cObject_void(arg0, 0);
            var_a0_2 = arg1 & 1;
        }
        if ((var_a0_2 != 0) && (arg0 != NULL)) {
            temp_a0_3 = cMemPool__GetPoolFromPtr_constvoidptrstatic(arg0)->unk24;
            temp_a1_3 = temp_a0_3->unk1C;
            temp_a2_3 = (temp_a1_3 + 0x30)->unk4;
            temp_a2_3(temp_a0_3 + temp_a1_3->unk30, arg0, temp_a2_3);
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
