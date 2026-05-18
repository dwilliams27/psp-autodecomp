# Battle Packet: `cObject::~cObject(void)`

## Target

- Address: `0x00009dc4`
- Size: `432` bytes
- Status: `failed`
- Object: `cAll_psp.obj`
- Class: `cObject`
- Method family: `Destructor`
- Leaf: `False`

## Queue Metadata

- score: `131`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 18B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `18`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `18`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x00007534` `untried` 264B `cType::InitializeType(const char *, const char *, unsigned int, const cType *, cBase * (*)(cMemPool *, cBase *), const char *, const char *, unsigned int) static`
- `0x00009824` `untried` 224B `cManager::Remove(cObject *) static`
- `0x0000cc84` `matched` 100B `cGroup::OnObjectDeleted(cObject *)`

Callers:
- `0x00020d78` `matched` 132B `eSoundData::~eSoundData(void)`
- `0x0002bed0` `failed` 124B `eTexture::~eTexture(void)`
- `0x0002c0d0` `matched` 132B `eMaterial::~eMaterial(void)`
- `0x0003bf30` `matched` 152B `eLensFlare::~eLensFlare(void)`
- `0x0003d6b8` `untried` 1032B `eRoom::~eRoom(void)`
- `0x0003fe54` `failed` 576B `eRoomSet::~eRoomSet(void)`
- `0x000405c4` `failed` 124B `ePortal::~ePortal(void)`
- `0x00043f74` `matched` 124B `eMesh::~eMesh(void)`
- `0x000465b4` `untried` 1524B `eAnimation::~eAnimation(void)`
- `0x0004c518` `untried` 676B `eDynamicModelTemplate::~eDynamicModelTemplate(void)`
- `0x00052168` `failed` 348B `eHeightmapTemplate::~eHeightmapTemplate(void)`
- `0x00053bc0` `matched` 124B `eSurface::~eSurface(void)`
- `0x0005777c` `untried` 700B `eNavMesh::~eNavMesh(void)`
- `0x0005b5e0` `matched` 300B `ePath::~ePath(void)`
- `0x0005c424` `failed` 124B `ePoint::~ePoint(void)`
- `0x0005c8c0` `matched` 124B `eRoomEnvironment::~eRoomEnvironment(void)`
- `0x0005d1a8` `matched` 356B `eSky::~eSky(void)`
- `0x0005ee48` `matched` 124B `eStaticLight::~eStaticLight(void)`
- `0x00061448` `matched` 124B `eWeatherEffectTemplate::~eWeatherEffectTemplate(void)`
- `0x000d6d54` `failed` 152B `gcStringTable::~gcStringTable(void)`
- `0x000e2464` `failed` 476B `gcUI::gcFader::Draw(const eCamera &, const mOCS &, const eRect &, bool) const`
- `0x000e3340` `untried` 864B `gcUIDialog::~gcUIDialog(void)`
- `0x000e7a94` `untried` 344B `gcUIDialog::DrawBackdrop(const eCamera &, const mOCS &, const eRect &, float, float) const`
- `0x000e9b74` `failed` 212B `gcEntityCustomAnimation::~gcEntityCustomAnimation(void)`

## Placement

- Canonical source: `src/cObject.cpp`
- Header(s): `include/cObject.h`
- Allowed source(s): `src/cObject.cpp`, `src/cObject_dtor.cpp`
- Split-TU prefix: `src/cObject_*.cpp`

## Compiler Guidance

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/cObject.h`

```cpp
#ifndef COBJECT_H
#define COBJECT_H

class cBase;
class cFile;
class cFilename;
class cGUID;
class cMemPool;
class cName;
class cStr;
class cType;

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
    const cType *GetType(void) const;
    void SetDirty(void);
    void GetExternalDependency(int, cFilename *) const;
    int GetNumExternalDependencies(void) const;
    int GetUserData(void) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    cObject &operator=(const cObject &);
    void Copy(cMemPool *, cBase *) const;
    void Write(cFile &) const;
    int WriteHeader(cFile &);
    int Load(void);
    int IsEditable(void) const;
    static int GetNameFromGUID(const cGUID &, const cType *, bool, bool, cName *);
    static void GetLocalizedFilename(const cType *, const cGUID &, cStr *);
};

class cFactory {
public:
    char _pad0[72];         // 0x00
    unsigned int mField48;  // 0x48

    int AllowParentCreate(void) const;
    int GetContainedFactories(int *count);
    void CopyObject(const cObject *, const cGUID &);
    void Write(cFile &) const;
    void Clean(bool flag);
    void LoadLocalized(void);
    void LoadLocalized(const char *);
    int Load(void);
    ~cFactory(void);
    void ClearVisitedReferences(unsigned int);
    void MarkForClean(unsigned int);
    void DeleteMarkedForClean(unsigned int, bool);
};

#endif
```

## Matched Same-Class Neighbors

- `0x00009b30` 92B `cObject::Write(cFile &) const` — `src/cObject.cpp`
- `0x00009d28` 72B `cObject::cObject(cBase *)` — `src/gcTableColumn.cpp`
- `0x00009d70` 84B `cObject::operator=(const cObject &)` — `src/cObject.cpp`
- `0x00009f74` 84B `cObject::Copy(cMemPool *, cBase *) const` — `src/cObject.cpp`
- `0x0000a4c0` 76B `cObject::GetNameFromGUID(const cGUID &, const cType *, bool, bool, cName *) static` — `src/cObject.cpp`

## Matched Method Exemplars

### Exemplar 1: `cMemPool::~cMemPool(void)`

- Address: `0x00004690`
- Size: `272` bytes
- Source: `src/cMemPool.cpp`

```cpp
cMemPool::~cMemPool(void) {
    void *classDesc = (void *)0x37E698;
    mClassDesc = classDesc;

    sceKernelLockMutex(*GetLock(), 1, 0);
    if (mNext != 0) {
        cMemPool *prev = mPrev;
        if (prev != 0) {
            cMemPool **head = (cMemPool **)0x380000;
            if (head[-4076] == this) {
                head[-4076] = prev;
                prev = mPrev;
            }
            cMemPool *next = mNext;
            next->mPrev = prev;
            next = mNext;
            prev->mNext = next;
            mNext = 0;
            mPrev = 0;
            if (head[-4076] == this) {
                head[-4076] = 0;
            }
        }
    }
    sceKernelUnlockMutex(*GetLock(), 1);

    mAllocator->FreePoolMemory();
}
```

### Exemplar 2: `cMemPoolPlatform::~cMemPoolPlatform(void)`

- Address: `0x001c5ee8`
- Size: `196` bytes
- Source: `src/cMemPoolPlatform.cpp`

```cpp
cMemPoolPlatform::~cMemPoolPlatform(void) {
    if (mAllocHandle >= 0) {
        int total = cMemPoolPlatform_totalAllocatedBytes;
        cMemPoolPlatform_totalAllocatedBytes = total - mSize;
        sceKernelFreeFpl(mAllocHandle, mBlock);
        sceKernelDeleteFpl(mAllocHandle);
        mSize = 0;
        mAllocHandle = -1;
        mBlock = 0;
    }
}
```

### Exemplar 3: `cThread::~cThread(void)`

- Address: `0x00001fe0`
- Size: `192` bytes
- Source: `src/cThread.cpp`

```cpp
cThread::~cThread(void) {
    m_vtable = cThreadvirtualtable;
    __asm__ volatile("" ::: "memory");
    int running = (((m_flags & 0x1000) != 0) & 0xFF);
    if (running != 0) {
        if (m_threadId > 0) {
            WaitForFinish();
            sceKernelTerminateDeleteThread(m_threadId);
        }
    }
}
```

### Exemplar 4: `cBufferedFile::~cBufferedFile(void)`

- Address: `0x0000e3d0`
- Size: `180` bytes
- Source: `src/cBufferedFile.cpp`

```cpp
cBufferedFile::~cBufferedFile(void) {
    Close();
    sceKernelFreeFpl(mAllocHandle, mBufferPtr[0]);
    sceKernelFreeFpl(mAllocHandle, mBufferPtr[1]);
    sceKernelDeleteFpl(mAllocHandle);
}
```


## Prior Failure Notes

### Note 1

- session `0e1573e4`; src `src/cObject_dtor.cpp`; snapshot `logs/failure_snapshots/match_20260510_175602/0e1573e4/00009dc4__agent_self_reported_failure__src_cObject_dtor.cpp`

category=REG_ALLOC. Closest attempt is 18/432 bytes after one permuter pass (started at 27 bytes, permuter improved by reordering local declarations to fix s2/s3 swap). All remaining diffs are s0/s1 register naming swap: expected has this=s0 and flag=s1; SNC compiles ours with this=s1 and flag=s0. Structural code (prologue, control flow, xor+sltiu+andi pattern for field30 zero test via 'register int zero __asm__("$0")', the redundant beqz s0 + andi delay slot around vtable B set, OnObjectDeleted/Remove calls, deleting-destructor tail) all match exactly.

Attempts: (1) Initial source produced 27 bytes diff and 412B size (missing xor pattern and redundant beqz s0); added 'register int zero __asm__("$0")' with 'if (((zero == x) & 0xFF) == 0)' pattern (copied from src/nwSocket.cpp:243-247 cObject::VisitReferences). (2) Initially had cBase_local inheritance with inline ~cBase_local body to inline parent dtor and get redundant beqz s0 check; this produced the 432B size but extra unused mangled symbol. (3) Replaced with explicit 'if (this != 0)' around the vtable B set - same diff but cleaner. (4) Tried register pinning 'register cObject *self __asm__("$16")' to force this=s0 - SNC ignored the hint. Per docs/research/snc-register-allocation.md: this pattern (callee-save register rename only, structural code identical) is the documented REG_ALLOC drift and source-level fixes do not work. One permuter pass already run.


## Disassembly

```asm
9dc4:	27bdffe0 	addiu	sp,sp,-32
9dc8:	afb00000 	sw	s0,0(sp)
9dcc:	00808025 	move	s0,a0
9dd0:	afb10004 	sw	s1,4(sp)
9dd4:	afb20008 	sw	s2,8(sp)
9dd8:	afb3000c 	sw	s3,12(sp)
9ddc:	afb40010 	sw	s4,16(sp)
9de0:	afb50014 	sw	s5,20(sp)
9de4:	afbf0018 	sw	ra,24(sp)
9de8:	10800059 	beqz	a0,0x9f50
9dec:	00a08825 	move	s1,a1
9df0:	8e040030 	lw	a0,48(s0)
9df4:	3c050038 	lui	a1,0x38
9df8:	00042026 	xor	a0,zero,a0
9dfc:	24a5e920 	addiu	a1,a1,-5856
9e00:	2c840001 	sltiu	a0,a0,1
9e04:	308400ff 	andi	a0,a0,0xff
9e08:	1480003d 	bnez	a0,0x9f00
9e0c:	ae050004 	sw	a1,4(s0)
9e10:	8e120000 	lw	s2,0(s0)
9e14:	12400034 	beqz	s2,0x9ee8
9e18:	34130000 	li	s3,0x0
9e1c:	3c140004 	lui	s4,0x4
9e20:	8e840c94 	lw	a0,3220(s4)
9e24:	14800019 	bnez	a0,0x9e8c
9e28:	3c150004 	lui	s5,0x4
9e2c:	8ea485dc 	lw	a0,-31268(s5)
9e30:	1480000c 	bnez	a0,0x9e64
9e34:	3c040037 	lui	a0,0x37
9e38:	3c050037 	lui	a1,0x37
9e3c:	34060001 	li	a2,0x1
9e40:	00003825 	move	a3,zero
9e44:	00004025 	move	t0,zero
9e48:	00004825 	move	t1,zero
9e4c:	00005025 	move	t2,zero
9e50:	00005825 	move	t3,zero
9e54:	2484c830 	addiu	a0,a0,-14288
9e58:	0c001d4d 	jal	0x7534
9e5c:	24a5c838 	addiu	a1,a1,-14280
9e60:	aea285dc 	sw	v0,-31268(s5)
9e64:	8ea785dc 	lw	a3,-31268(s5)
9e68:	00002025 	move	a0,zero
9e6c:	00002825 	move	a1,zero
9e70:	34060004 	li	a2,0x4
9e74:	00004025 	move	t0,zero
9e78:	00004825 	move	t1,zero
9e7c:	00005025 	move	t2,zero
9e80:	0c001d4d 	jal	0x7534
9e84:	00005825 	move	t3,zero
9e88:	ae820c94 	sw	v0,3220(s4)
9e8c:	8e440004 	lw	a0,4(s2)
9e90:	8e940c94 	lw	s4,3220(s4)
9e94:	24840008 	addiu	a0,a0,8
9e98:	84850000 	lh	a1,0(a0)
9e9c:	8c860004 	lw	a2,4(a0)
9ea0:	00c0f809 	jalr	a2
9ea4:	02452021 	addu	a0,s2,a1
9ea8:	16800003 	bnez	s4,0x9eb8
9eac:	00402025 	move	a0,v0
9eb0:	1000000a 	b	0x9edc
9eb4:	34040000 	li	a0,0x0
9eb8:	50800008 	beqzl	a0,0x9edc
9ebc:	34040000 	li	a0,0x0
9ec0:	54940003 	bnel	a0,s4,0x9ed0
9ec4:	8c84001c 	lw	a0,28(a0)
9ec8:	10000004 	b	0x9edc
9ecc:	34040001 	li	a0,0x1
9ed0:	1480fffb 	bnez	a0,0x9ec0
9ed4:	00000000 	nop
9ed8:	34040000 	li	a0,0x0
9edc:	10800002 	beqz	a0,0x9ee8
9ee0:	00000000 	nop
9ee4:	02409825 	move	s3,s2
9ee8:	12600003 	beqz	s3,0x9ef8
9eec:	02602025 	move	a0,s3
9ef0:	0c003321 	jal	0xcc84
9ef4:	02002825 	move	a1,s0
9ef8:	0c002609 	jal	0x9824
9efc:	02002025 	move	a0,s0
9f00:	12000005 	beqz	s0,0x9f18
9f04:	32240001 	andi	a0,s1,0x1
9f08:	3c040038 	lui	a0,0x38
9f0c:	2484e6a8 	addiu	a0,a0,-6488
9f10:	ae040004 	sw	a0,4(s0)
9f14:	32240001 	andi	a0,s1,0x1
9f18:	1080000d 	beqz	a0,0x9f50
9f1c:	00000000 	nop
9f20:	1200000b 	beqz	s0,0x9f50
9f24:	00000000 	nop
9f28:	0c0011e8 	jal	0x47a0
9f2c:	02002025 	move	a0,s0
9f30:	8c440024 	lw	a0,36(v0)
9f34:	02002825 	move	a1,s0
9f38:	8c86001c 	lw	a2,28(a0)
9f3c:	24c60030 	addiu	a2,a2,48
9f40:	84c70000 	lh	a3,0(a2)
9f44:	8cc60004 	lw	a2,4(a2)
9f48:	00c0f809 	jalr	a2
9f4c:	00872021 	addu	a0,a0,a3
9f50:	8fb00000 	lw	s0,0(sp)
9f54:	8fb10004 	lw	s1,4(sp)
9f58:	8fb20008 	lw	s2,8(sp)
9f5c:	8fb3000c 	lw	s3,12(sp)
9f60:	8fb40010 	lw	s4,16(sp)
9f64:	8fb50014 	lw	s5,20(sp)
9f68:	8fbf0018 	lw	ra,24(sp)
9f6c:	03e00008 	jr	ra
9f70:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: cObject::~cObject(void)
// Address: 0x00009dc4, Size: 432B
// Obj: cAll_psp.obj

? cGroup__OnObjectDeleted_cObjectptr(void *, void *); /* extern */
? cManager__Remove_cObjectptrstatic(void *);        /* extern */
void *cMemPool__GetPoolFromPtr_constvoidptrstatic(void *); /* extern */
s32 cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(?, ?, ?, s32); /* extern */

void cObject___dtor_cObject_void(void *arg0, s32 arg1) {
    ? (*temp_a2_3)(void *, void *, ?, s16);
    s16 temp_a1;
    s16 temp_a3;
    s32 temp_s4;
    s32 var_a0_2;
    s32 var_a0_3;
    void *(*temp_a2)(void *, s16, ?);
    void *temp_a0;
    void *temp_a0_2;
    void *temp_a2_2;
    void *temp_s2;
    void *var_a0;
    void *var_s3;

    if (arg0 != NULL) {
        arg0->unk4 = 0x37E920;
        if (!((arg0->unk30 == 0) & 0xFF)) {
            temp_s2 = arg0->unk0;
            var_s3 = NULL;
            if (temp_s2 != NULL) {
                if (*(s32 *)0x40C94 == 0) {
                    if (*(s32 *)0x385DC == 0) {
                        *(s32 *)0x385DC = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0x36C830, 0x36C838, 1, 0);
                    }
                    *(s32 *)0x40C94 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 4, *(s32 *)0x385DC);
                }
                temp_a0 = temp_s2->unk4;
                temp_s4 = *(s32 *)0x40C94;
                temp_a1 = temp_a0->unk8;
                temp_a2 = (temp_a0 + 8)->unk4;
                var_a0 = temp_a2(temp_s2 + temp_a1, temp_a1, temp_a2);
                if (temp_s4 == 0) {
                    goto block_15;
                }
                if (var_a0 != NULL) {
loop_10:
                    if (var_a0 != temp_s4) {
                        var_a0 = var_a0->unk1C;
                        if (var_a0 == NULL) {
                            goto block_15;
                        }
                        goto loop_10;
                    }
                    var_a0_2 = 1;
                } else {
block_15:
                    var_a0_2 = 0;
                }
                if (var_a0_2 != 0) {
                    var_s3 = temp_s2;
                }
            }
            if (var_s3 != NULL) {
                cGroup__OnObjectDeleted_cObjectptr(var_s3, arg0);
            }
            cManager__Remove_cObjectptrstatic(arg0);
        }
        var_a0_3 = arg1 & 1;
        if (arg0 != NULL) {
            arg0->unk4 = 0x37E6A8;
            var_a0_3 = arg1 & 1;
        }
        if ((var_a0_3 != 0) && (arg0 != NULL)) {
            temp_a0_2 = cMemPool__GetPoolFromPtr_constvoidptrstatic(arg0)->unk24;
            temp_a2_2 = temp_a0_2->unk1C;
            temp_a3 = temp_a2_2->unk30;
            temp_a2_3 = (temp_a2_2 + 0x30)->unk4;
            temp_a2_3(temp_a0_2 + temp_a3, arg0, temp_a2_3, temp_a3);
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
