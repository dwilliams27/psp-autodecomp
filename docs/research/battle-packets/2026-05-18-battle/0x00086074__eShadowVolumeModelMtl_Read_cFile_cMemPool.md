# Battle Packet: `eShadowVolumeModelMtl::Read(cFile &, cMemPool *)`

## Target

- Address: `0x00086074`
- Size: `344` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eShadowVolumeModelMtl`
- Method family: `Read`
- Leaf: `False`

## Queue Metadata

- score: `146`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 2B; codegen-specific diagnosis; Read hard family; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `2`

## Failure Classification

- Primary: `read_prologue_compiler`
- Action: `research`
- Confidence: `high`
- Tags: `read_prologue_compiler`, `regalloc_drift`, `needs_layout`
- Near miss: `2`
- Summary: cReadBlock Read prologue/scheduler compiler issue.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x00004ff4` `untried` 128B `cFileSystem::Read(cFileHandle *, void *, unsigned int) static`
- `0x000053c8` `matched` 28B `cFile::SetCurrentPos(unsigned int)`
- `0x00005d00` `failed` 188B `cReadBlock::cReadBlock(cFile &, unsigned int, bool)`
- `0x00005dbc` `failed` 212B `cReadBlock::~cReadBlock(void)`
- `0x0005df20` `matched` 228B `eModelMtl::Read(cFile &, cMemPool *)`
- `0x000861cc` `matched` 168B `eShadowVolumeModelMtl::PlatformRead(cFile &, cMemPool *)`
- `0x000868bc` `matched` 8B `eShadowVolumeModelMtl::CreateData(void)`

Callers:
- `0x00086910` `matched` 200B `eExtrudedShadowVolumeModelMtl::Read(cFile &, cMemPool *)`

## Placement

- Canonical source: `src/eShadowVolumeModelMtl.cpp`
- Header(s): `include/eShadowVolumeModelMtl.h`
- Allowed source(s): `src/eShadowVolumeModelMtl.cpp`
- Split-TU prefix: `src/eShadowVolumeModelMtl_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.

Read warning: many `Read(cFile &, cMemPool *)` methods are blocked by the known cReadBlock prologue scheduler divergence. If the body matches and only the 20-byte prologue differs, record that blocker instead of spending the session on retries.

## Class Header: `include/eShadowVolumeModelMtl.h`

Not found.

## Matched Same-Class Neighbors

- `0x00086010` 100B `eShadowVolumeModelMtl::Write(cFile &) const` — `src/eShadowVolumeModelMtl.cpp`
- `0x000861cc` 168B `eShadowVolumeModelMtl::PlatformRead(cFile &, cMemPool *)` — `src/eShadowVolumeModelMtl.cpp`
- `0x00086274` 8B `eShadowVolumeModelMtl::PlatformFree(void)` — `src/eShadowVolumeModelMtl.cpp`
- `0x0008627c` 116B `eShadowVolumeModelMtl::eShadowVolumeModelMtl(cBase *)` — `src/eShadowVolumeModelMtl.cpp`
- `0x000868bc` 8B `eShadowVolumeModelMtl::CreateData(void)` — `src/eShadowVolumeModelMtl_CreateData.cpp`

## Matched Method Exemplars

### Exemplar 1: `eStaticModel::Read(cFile &, cMemPool *)`

- Address: `0x00041b94`
- Size: `336` bytes
- Source: `src/eStaticModel_Read.cpp`

```cpp
// -- eStaticModel::Read(cFile &, cMemPool *) @ 0x00041b94 --
int eStaticModel::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    unsigned int version = (unsigned int)rb[3];
    if (version >= 3 || version < 1) goto fail;
    if (!((eStaticGeom *)this)->Read(file, pool)) goto fail;

    {
        void *h = *(void **)rb[0];
        cFileSystem::Read(h, (char *)this + 0x40, 0xC);
    }
    {
        void *h = *(void **)rb[0];
        cFileSystem::Read(h, (char *)this + 0x10, 0xC);
    }
    {
        void *h = *(void **)rb[0];
        cFileSystem::Read(h, (char *)this + 0x20, 0xC);
    }
    {
        void *h = *(void **)rb[0];
        cFileSystem::Read(h, (char *)this + 0x30, 0xC);
    }
    if ((unsigned int)rb[3] >= 2) goto read_field98;
    goto platform;

fail:
    cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
    __0oKcReadBlockdtv(rb, 2);
    return 0;

read_field98:
    {
        void *h = *(void **)rb[0];
        cFileSystem::Read(h, (char *)this + 0x98, 4);
    }
platform:
    PlatformRead(file, pool);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}
```

### Exemplar 2: `eSilhouetteModelMtl::Read(cFile &, cMemPool *)`

- Address: `0x00086de4`
- Size: `336` bytes
- Source: `src/eSilhouetteModelMtl_Read.cpp`

```cpp
// -- eSilhouetteModelMtl::Read(cFile &, cMemPool *) @ 0x00086de4 --
int eSilhouetteModelMtl::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 2, true);
    if ((unsigned int)rb._data[3] != 2) goto fail;
    if (((eModelMtl *)this)->Read(file, pool)) goto success;

fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;

success:
    *(int *)((char *)this + 0x48) = 0;
    __asm__ volatile("" ::: "memory");
    {
        cHandle *h = (cHandle *)((char *)this + 0x48);
        cMemPool *handlePool = cMemPool::GetPoolFromPtr(h);
        h->Read(rb, handlePool);
    }
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x78, 4);
    }
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x74, 4);
    }
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x80, 4);
    }
    {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x7C, 4);
    }
    PlatformRead(file, pool);
    CreateData();
    return result;
}
```

### Exemplar 3: `ePhysicsControllerTemplate::Read(cFile &, cMemPool *)`

- Address: `0x0004b918`
- Size: `352` bytes
- Source: `src/ePhysicsControllerTemplate.cpp`

```cpp
int ePhysicsControllerTemplate::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 5, true);
    unsigned int version = (unsigned int)rb._data[3];

    if (version >= 6 || version < 1) goto fail;
    if (version != 1) goto read_arr1;

    {
        cName name;
        __asm__ volatile("" ::: "memory");
        name.mTail0 = 0;
        name.mTail1 = 0;
        *(char *)&name = 0;
        name.Read(rb);
        int *dst = (int *)((char *)*(cBase **)mArr1.mCount + 8);
        int n0 = name._data[0];
        int n1 = name._data[1];
        int n2 = name._data[2];
        dst[0] = n0;
        dst[1] = n1;
        dst[2] = n2;
        int n3 = name._data[3];
        int n4 = name._data[4];
        int n5 = name._data[5];
        dst[3] = n3;
        dst[4] = n4;
        dst[5] = n5;
        version = (unsigned int)rb._data[3];
        goto after_arr1;
    }

fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;

read_arr1:
    mArr1.Read(rb);
    version = (unsigned int)rb._data[3];
after_arr1:
    if (version >= 3) {
        mArr2.Read(rb);
        version = (unsigned int)rb._data[3];
    }

    if (version >= 4) {
        mArr3.Read(rb);
        version = (unsigned int)rb._data[3];
    }

    if (version >= 5) {
        void *h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &mField20, 4);
    }

    return result;
}
```

### Exemplar 4: `eBodyWorldConstraintConfig::Read(cFile &, cMemPool *)`

- Address: `0x00075f6c`
- Size: `352` bytes
- Source: `src/eBodyWorldConstraintConfig.cpp`

```cpp
// eBodyWorldConstraintConfig::Read(cFile &, cMemPool *) @ 0x00075f6c
int eBodyWorldConstraintConfig::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] != 1) goto fail;
    if (((ePhysicsConstraintConfig *)this)->Read(file, pool)) goto success;

fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;

success:
    {
        cFileHandle *h = *(cFileHandle **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &mVec20, 0x0C);
    }
    {
        cFileHandle *h = *(cFileHandle **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &mFloat50, 4);
    }
    {
        cFileHandle *h = *(cFileHandle **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &mVec30, 0x0C);
    }
    {
        cFileHandle *h = *(cFileHandle **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &mVec40, 0x0C);
    }
    {
        cFileHandle *h = *(cFileHandle **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &mInt54, 4);
    }
    {
        char tmp;
        cFileHandle *h = *(cFileHandle **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &tmp, 1);
        mBool58 = tmp != 0;
    }
    {
        char tmp;
        cFileHandle *h = *(cFileHandle **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &tmp, 1);
        mBool59 = tmp != 0;
    }
    return result;
}
```


## Prior Failure Notes

### Note 1

- session `3b4cbe17`; src `src/eShadowVolumeModelMtl.cpp`; snapshot `logs/failure_snapshots/match_20260508_185952/3b4cbe17/00086074__agent_self_reported_failure__src_eShadowVolumeModelMtl.cpp`

Implemented eShadowVolumeModelMtl::Read in canonical C++ syntax in src/eShadowVolumeModelMtl.cpp. The closest version uses the raw int rb[5] cReadBlock constructor/destructor pattern so the stack frame matches, reads the two 1-byte fields into stack offsets 0x14/0x15, normalizes them into offsets 0x74/0x75, sets 0x5C/0x5F/0x5D, calls PlatformRead and CreateData, and preserves all existing matched siblings in the file. Handle barriers were needed to match the cFileSystem::Read argument scheduling.

Final compare is a 2/344-byte mismatch only in the mField75 branch test: generated code still uses lbu/bnez with a0 where the target uses a1. I tried register-bound variables with both "$5" and "a1", a direct inline lbu output, volatile stores, label/layout changes around the fail block, and the prior failure snapshot's register-pressure pattern. The compiler keeps allocating the test to a0, so this appears to be the same small REG_ALLOC drift reported by the prior agent rather than a remaining control-flow issue.

### Note 2

- session `cc776ec7`; src `src/eShadowVolumeModelMtl.cpp`; snapshot `logs/failure_snapshots/match_20260508_101832/cc776ec7/00086074__agent_self_reported_failure__src_eShadowVolumeModelMtl.cpp`

Implemented eShadowVolumeModelMtl::Read in src/eShadowVolumeModelMtl.cpp under the existing sched=1 region. The method now has the target control-flow layout: version 2 cReadBlock, eModelMtl::Read, two one-byte cFileSystem::Read calls into stack temps, bool normalization to offsets 0x74/0x75, 0x5C and 0x5F byte setup, 0x5D |= 0x40, PlatformRead, CreateData, and the cReadBlock destructor exits. Existing eShadowVolumeModelMtl siblings still compare as MATCH. Manual restructuring reduced the mismatch from a 332B size mismatch to only 2 bytes in the 344B target; a 180s permuter run generated 4176 candidates and found no improvement. The remaining diff is a register-encoding drift in the mField75 test: generated code uses lbu/bnez with a0 where the target uses a1. Attempts with register asm bindings and live-range pressure did not move it, so this is best classified as a small REG_ALLOC drift.


## Disassembly

```asm
86074:	27bdffd0 	addiu	sp,sp,-48
86078:	afb00018 	sw	s0,24(sp)
8607c:	afb1001c 	sw	s1,28(sp)
86080:	afb20020 	sw	s2,32(sp)
86084:	afb30024 	sw	s3,36(sp)
86088:	afbf0028 	sw	ra,40(sp)
8608c:	00c08025 	move	s0,a2
86090:	00a08825 	move	s1,a1
86094:	00809025 	move	s2,a0
86098:	34130001 	li	s3,0x1
8609c:	03a02025 	move	a0,sp
860a0:	34060002 	li	a2,0x2
860a4:	0c001740 	jal	0x5d00
860a8:	34070001 	li	a3,0x1
860ac:	8fa4000c 	lw	a0,12(sp)
860b0:	34050002 	li	a1,0x2
860b4:	14850021 	bne	a0,a1,0x8613c
860b8:	02402025 	move	a0,s2
860bc:	02202825 	move	a1,s1
860c0:	0c0177c8 	jal	0x5df20
860c4:	02003025 	move	a2,s0
860c8:	5040001d 	beqzl	v0,0x86140
860cc:	8fa40000 	lw	a0,0(sp)
860d0:	8fa40000 	lw	a0,0(sp)
860d4:	8c840000 	lw	a0,0(a0)
860d8:	27a50014 	addiu	a1,sp,20
860dc:	0c0013fd 	jal	0x4ff4
860e0:	34060001 	li	a2,0x1
860e4:	83a40014 	lb	a0,20(sp)
860e8:	0004202b 	sltu	a0,zero,a0
860ec:	a2440074 	sb	a0,116(s2)
860f0:	8fa40000 	lw	a0,0(sp)
860f4:	8c840000 	lw	a0,0(a0)
860f8:	27a50015 	addiu	a1,sp,21
860fc:	0c0013fd 	jal	0x4ff4
86100:	34060001 	li	a2,0x1
86104:	83a40015 	lb	a0,21(sp)
86108:	0004202b 	sltu	a0,zero,a0
8610c:	a2440075 	sb	a0,117(s2)
86110:	3404004a 	li	a0,0x4a
86114:	a244005c 	sb	a0,92(s2)
86118:	9244005c 	lbu	a0,92(s2)
8611c:	2405fffe 	li	a1,-2
86120:	00852024 	and	a0,a0,a1
86124:	a244005c 	sb	a0,92(s2)
86128:	92450075 	lbu	a1,117(s2)
8612c:	14a00011 	bnez	a1,0x86174
86130:	34040005 	li	a0,0x5
86134:	10000011 	b	0x8617c
86138:	a244005f 	sb	a0,95(s2)
8613c:	8fa40000 	lw	a0,0(sp)
86140:	0c0014f2 	jal	0x53c8
86144:	8fa50004 	lw	a1,4(sp)
86148:	03a02025 	move	a0,sp
8614c:	0c00176f 	jal	0x5dbc
86150:	34050002 	li	a1,0x2
86154:	00001025 	move	v0,zero
86158:	8fb00018 	lw	s0,24(sp)
8615c:	8fb1001c 	lw	s1,28(sp)
86160:	8fb20020 	lw	s2,32(sp)
86164:	8fb30024 	lw	s3,36(sp)
86168:	8fbf0028 	lw	ra,40(sp)
8616c:	03e00008 	jr	ra
86170:	27bd0030 	addiu	sp,sp,48
86174:	34040004 	li	a0,0x4
86178:	a244005f 	sb	a0,95(s2)
8617c:	9244005d 	lbu	a0,93(s2)
86180:	34840040 	ori	a0,a0,0x40
86184:	a244005d 	sb	a0,93(s2)
86188:	02402025 	move	a0,s2
8618c:	02202825 	move	a1,s1
86190:	0c021873 	jal	0x861cc
86194:	02003025 	move	a2,s0
86198:	0c021a2f 	jal	0x868bc
8619c:	02402025 	move	a0,s2
861a0:	03a02025 	move	a0,sp
861a4:	0c00176f 	jal	0x5dbc
861a8:	34050002 	li	a1,0x2
861ac:	02601025 	move	v0,s3
861b0:	8fb00018 	lw	s0,24(sp)
861b4:	8fb1001c 	lw	s1,28(sp)
861b8:	8fb20020 	lw	s2,32(sp)
861bc:	8fb30024 	lw	s3,36(sp)
861c0:	8fbf0028 	lw	ra,40(sp)
861c4:	03e00008 	jr	ra
861c8:	27bd0030 	addiu	sp,sp,48
```

## m2c Starting Point

```c
// Decompiled: eShadowVolumeModelMtl::Read(cFile &, cMemPool *)
// Address: 0x00086074, Size: 344B
// Obj: eAll_psp.obj

? cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(s32, s8 *, ?); /* extern */
? cFile__SetCurrentPos_unsignedint(s32, s32);       /* extern */
? cReadBlock___dtor_cReadBlock_void(void *, ?);     /* extern */
? cReadBlock__cReadBlock_cFileref_unsignedint_bool(void *, ?, ?); /* extern */
s32 eModelMtl__Read_cFileref_cMemPoolptr(void *, ?, ?); /* extern */
? eShadowVolumeModelMtl__CreateData_void(void *);   /* extern */
? eShadowVolumeModelMtl__PlatformRead_cFileref_cMemPoolptr(void *, ?, ?); /* extern */

s32 eShadowVolumeModelMtl__Read_cFileref_cMemPoolptr(void *arg0, ? arg1, ? arg2) {
    s8 sp14;
    s8 sp15;
    s8 var_a0;

    cReadBlock__cReadBlock_cFileref_unsignedint_bool(sp, 2, 1);
    if ((subroutine_arg3 == 2) && (eModelMtl__Read_cFileref_cMemPoolptr(arg0, arg1, arg2) != 0)) {
        cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(*subroutine_arg0, &sp14, 1);
        arg0->unk74 = (s8) (sp14 != 0);
        cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(*subroutine_arg0, &sp15, 1);
        arg0->unk75 = (u8) (sp15 != 0);
        arg0->unk5C = 0x4AU;
        arg0->unk5C = (u8) (arg0->unk5C & ~1);
        var_a0 = 5;
        if (arg0->unk75 == 0) {

        } else {
            var_a0 = 4;
        }
        arg0->unk5F = var_a0;
        arg0->unk5D = (u8) (arg0->unk5D | 0x40);
        eShadowVolumeModelMtl__PlatformRead_cFileref_cMemPoolptr(arg0, arg1, arg2);
        eShadowVolumeModelMtl__CreateData_void(arg0);
        cReadBlock___dtor_cReadBlock_void(sp, 2);
        return 1;
    }
    cFile__SetCurrentPos_unsignedint(subroutine_arg0, subroutine_arg1);
    cReadBlock___dtor_cReadBlock_void(sp, 2);
    return 0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
