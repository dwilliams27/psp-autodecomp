# Battle Packet: `eVolume::~eVolume(void)`

## Target

- Address: `0x0005d6f0`
- Size: `248` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eVolume`
- Method family: `Destructor`
- Leaf: `False`

## Queue Metadata

- score: `127`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 26B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `26`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`
- Near miss: `26`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x0003f474` `matched` 40B `eRoom::RemoveVolume(eVolume *)`

Callers:
- `0x0012d464` `matched` 332B `gcTrigger::~gcTrigger(void)`

## Placement

- Canonical source: `src/eVolume.cpp`
- Header(s): `include/eVolume.h`
- Allowed source(s): `src/eVolume.cpp`
- Split-TU prefix: `src/eVolume_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/eVolume.h`

Not found.

## Matched Same-Class Neighbors

- `0x0005d4a8` 156B `eVolume::Write(cFile &) const` — `src/eVolume.cpp`
- `0x0005d544` 272B `eVolume::Read(cFile &, cMemPool *)` — `src/eVolume.cpp`
- `0x0005d654` 156B `eVolume::eVolume(cBase *)` — `src/eVolume.cpp`
- `0x0005d7e8` 56B `eVolume::SetLocalToWorld(const mOCS &)` — `src/std.cpp`
- `0x0005d928` 44B `eVolume::UpdateLocalToWorld(void)` — `src/eVolume.cpp`

## Matched Method Exemplars

### Exemplar 1: `eStaticMeshVisTriList::~eStaticMeshVisTriList(void)`

- Address: `0x0008ff88`
- Size: `240` bytes
- Source: `src/eStaticMeshVisTriList.cpp`

```cpp
eStaticMeshVisTriList::~eStaticMeshVisTriList() {
    void *field10 = mField10;
    if (field10 != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(field10);
        char *block = ((char **)pool)[9];
        register volatile eStaticMeshVisTriList_DeleteRecord4 *rec __asm__("a1") =
            (volatile eStaticMeshVisTriList_DeleteRecord4 *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void *base = block + off;
        void (*fn)(void *, void *, short, void *) = rec->fn;
        fn(base, *(void **)((char *)this + 0x10), off, (void *)fn);
        mField10 = 0;
    }
    void *field0 = mField0;
    if (field0 != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(field0);
        char *block = ((char **)pool)[9];
        register volatile eStaticMeshVisTriList_DeleteRecord4 *rec __asm__("a1") =
            (volatile eStaticMeshVisTriList_DeleteRecord4 *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void *base = block + off;
        void (*fn)(void *, void *, short, void *) = rec->fn;
        fn(base, *(void **)this, off, (void *)fn);
        mField0 = 0;
    }
}
```

### Exemplar 2: `ePhysicsControllerTemplate::~ePhysicsControllerTemplate(void)`

- Address: `0x0004bab4`
- Size: `232` bytes
- Source: `src/ePhysicsControllerTemplate.cpp`

```cpp
ePhysicsControllerTemplate::~ePhysicsControllerTemplate() {
    mClassDesc = ePhysicsControllerTemplateclassdesc;
    cBaseArray *a3 = &mArr3;
    cBaseArray *a2 = &mArr2;
    cBaseArray *a1 = &mArr1;
    if (a3 != 0) a3->RemoveAll();
    if (a2 != 0) a2->RemoveAll();
    if (a1 != 0) a1->RemoveAll();
    mClassDesc = ePhysicsControllerTemplate_base_classdesc;
}
```

### Exemplar 3: `eBipedController::~eBipedController(void)`

- Address: `0x00064b1c`
- Size: `276` bytes
- Source: `src/eBipedController.cpp`

```cpp
eBipedController::~eBipedController() {
    *(void **)((char *)this + 4) = eBipedControllerclassdesc;
    void *field = (char *)this + 0x30;
    if (field != 0) {
        void *entries = bodyEntries;
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
                eBipedControllerDeleteRec *rec =
                    (eBipedControllerDeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                __asm__ volatile("" ::: "memory");
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            bodyEntries = 0;
        }
    }
}
```

### Exemplar 4: `eDynamicMeshObjectVisData::~eDynamicMeshObjectVisData(void)`

- Address: `0x0008f1b8`
- Size: `216` bytes
- Source: `src/eDynamicMeshObjectVisData.cpp`

```cpp
eDynamicMeshObjectVisData::~eDynamicMeshObjectVisData(void) {
    void *displayList = (void *)field_84;
    if (displayList != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(displayList);
        char *block = ((char **)pool)[9];
        register volatile eDynamicMeshObjectVisData_DeleteRecord *rec __asm__("a1") =
            (volatile eDynamicMeshObjectVisData_DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void *base = block + off;
        void (*fn)(void *, void *, short, void *) =
            (void (*)(void *, void *, short, void *))rec->fn;
        fn(base, *(void **)((char *)this + 0x84), off, (void *)fn);
        field_84 = 0;
    }
    void (*dtor)(void *) = (void (*)(void *))0x21DA38;
    __asm__ volatile("" : "+r"(dtor));
    vec_delete6(this, 0x20, 4, dtor, 0, 0);
}
```


## Prior Failure Notes

### Note 1

- session `d9cd2154`

Implemented eVolume::~eVolume() with canonical C++ destructor syntax and a class-local operator delete. The pool-delete tail now matches the target exactly after adding the same memory-barrier pattern used by other matched destructors, and the vtable restore/remove-volume/base-vtable/delete-tail structure is correct. The current build is a near-match: 26/248 bytes differ, all concentrated in the linked-list unlink block for the no-room path.

Tried multiple source shapes for the unlink block: locals for world/prev/next, repeated field loads matching the m2c output, array-style head access, a typed head struct, volatile head pointer access, explicit inverted head-compare branches, sched=1 versus sched=2 for the destructor, and a 5-minute permuter run (`7776` candidates generated, `3591` compiled, zero improvement). The stubborn mismatch is that our SNC prefers `beqz`/`beql` forms and temporary address addius around `world + 0x1c`, while the target uses `beqzl` for the next-pointer guard and a `bnel` skip with `lw this->prev` in the delay slot, followed by direct offset stores (`sw next,124(prev)` and `sw prev,120(next)`). This is not classifiable as unmatchable_bnel under the project criteria because the remaining diff is larger than 8 bytes and includes scheduling/addressing differences, not only bnel vs bne+nop.


## Disassembly

```asm
5d6f0:	27bdfff0 	addiu	sp,sp,-16
5d6f4:	afb00000 	sw	s0,0(sp)
5d6f8:	afb10004 	sw	s1,4(sp)
5d6fc:	afbf0008 	sw	ra,8(sp)
5d700:	00808825 	move	s1,a0
5d704:	10800033 	beqz	a0,0x5d7d4
5d708:	00a08025 	move	s0,a1
5d70c:	3c040038 	lui	a0,0x38
5d710:	24842260 	addiu	a0,a0,8800
5d714:	ae240004 	sw	a0,4(s1)
5d718:	8e240024 	lw	a0,36(s1)
5d71c:	50800006 	beqzl	a0,0x5d738
5d720:	8e240020 	lw	a0,32(s1)
5d724:	0c00fd1d 	jal	0x3f474
5d728:	02202825 	move	a1,s1
5d72c:	1000001a 	b	0x5d798
5d730:	3c040038 	lui	a0,0x38
5d734:	8e240020 	lw	a0,32(s1)
5d738:	50800017 	beqzl	a0,0x5d798
5d73c:	3c040038 	lui	a0,0x38
5d740:	8e250078 	lw	a1,120(s1)
5d744:	50a00014 	beqzl	a1,0x5d798
5d748:	3c040038 	lui	a0,0x38
5d74c:	8e25007c 	lw	a1,124(s1)
5d750:	50a00011 	beqzl	a1,0x5d798
5d754:	3c040038 	lui	a0,0x38
5d758:	8c86001c 	lw	a2,28(a0)
5d75c:	54d10004 	bnel	a2,s1,0x5d770
5d760:	8e260078 	lw	a2,120(s1)
5d764:	ac85001c 	sw	a1,28(a0)
5d768:	8e25007c 	lw	a1,124(s1)
5d76c:	8e260078 	lw	a2,120(s1)
5d770:	acc5007c 	sw	a1,124(a2)
5d774:	8e260078 	lw	a2,120(s1)
5d778:	aca60078 	sw	a2,120(a1)
5d77c:	ae200078 	sw	zero,120(s1)
5d780:	ae20007c 	sw	zero,124(s1)
5d784:	8c85001c 	lw	a1,28(a0)
5d788:	54b10003 	bnel	a1,s1,0x5d798
5d78c:	3c040038 	lui	a0,0x38
5d790:	ac80001c 	sw	zero,28(a0)
5d794:	3c040038 	lui	a0,0x38
5d798:	2484e6a8 	addiu	a0,a0,-6488
5d79c:	ae240004 	sw	a0,4(s1)
5d7a0:	32040001 	andi	a0,s0,0x1
5d7a4:	1080000b 	beqz	a0,0x5d7d4
5d7a8:	00000000 	nop
5d7ac:	0c0011e8 	jal	0x47a0
5d7b0:	02202025 	move	a0,s1
5d7b4:	8c440024 	lw	a0,36(v0)
5d7b8:	8c85001c 	lw	a1,28(a0)
5d7bc:	24a50030 	addiu	a1,a1,48
5d7c0:	84a60000 	lh	a2,0(a1)
5d7c4:	00862021 	addu	a0,a0,a2
5d7c8:	8ca60004 	lw	a2,4(a1)
5d7cc:	00c0f809 	jalr	a2
5d7d0:	02202825 	move	a1,s1
5d7d4:	8fb00000 	lw	s0,0(sp)
5d7d8:	8fb10004 	lw	s1,4(sp)
5d7dc:	8fbf0008 	lw	ra,8(sp)
5d7e0:	03e00008 	jr	ra
5d7e4:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eVolume::~eVolume(void)
// Address: 0x0005d6f0, Size: 248B
// Obj: eAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *); /* extern */
? eRoom__RemoveVolume_eVolumeptr(s32, void *);      /* extern */

void eVolume___dtor_eVolume_void(void *arg0, s32 arg1) {
    ? (*temp_a2)(void *, void *, ?);
    s32 temp_a0;
    void *temp_a0_2;
    void *temp_a0_3;
    void *temp_a1;
    void *temp_a1_2;

    if (arg0 != NULL) {
        arg0->unk4 = 0x382260;
        temp_a0 = arg0->unk24;
        if (temp_a0 != 0) {
            eRoom__RemoveVolume_eVolumeptr(temp_a0, arg0);
        } else {
            temp_a0_2 = arg0->unk20;
            if ((temp_a0_2 != NULL) && (arg0->unk78 != NULL)) {
                temp_a1 = arg0->unk7C;
                if (temp_a1 != NULL) {
                    if (temp_a0_2->unk1C == arg0) {
                        temp_a0_2->unk1C = temp_a1;
                    }
                    arg0->unk78->unk7C = arg0->unk7C;
                    arg0->unk7C->unk78 = (void *) arg0->unk78;
                    arg0->unk78 = NULL;
                    arg0->unk7C = NULL;
                    if (temp_a0_2->unk1C == arg0) {
                        temp_a0_2->unk1C = NULL;
                    }
                }
            }
        }
        arg0->unk4 = 0x37E6A8;
        if (arg1 & 1) {
            temp_a0_3 = cMemPool__GetPoolFromPtr_constvoidptrstatic(arg0)->unk24;
            temp_a1_2 = temp_a0_3->unk1C;
            temp_a2 = (temp_a1_2 + 0x30)->unk4;
            temp_a2(temp_a0_3 + temp_a1_2->unk30, arg0, temp_a2);
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
