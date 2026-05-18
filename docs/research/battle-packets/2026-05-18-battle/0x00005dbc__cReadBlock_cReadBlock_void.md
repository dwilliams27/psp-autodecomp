# Battle Packet: `cReadBlock::~cReadBlock(void)`

## Target

- Address: `0x00005dbc`
- Size: `212` bytes
- Status: `failed`
- Object: `cAll_psp.obj`
- Class: `cReadBlock`
- Method family: `Destructor`
- Leaf: `False`

## Queue Metadata

- score: `166`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 8B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`
- Near miss: `8`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`
- `0x00004ff4` `untried` 128B `cFileSystem::Read(cFileHandle *, void *, unsigned int) static`
- `0x000053ac` `matched` 28B `cFile::GetCurrentPos(void) const`
- `0x000053c8` `matched` 28B `cFile::SetCurrentPos(unsigned int)`
- `0x001ac690` `matched` 36B `free`

Callers:
- `0x00005efc` `untried` 116B `cReadBlock::SkipBase(void)`
- `0x00005f70` `untried` 424B `cReadBlock::CreateBase(cMemPool *, cBase *, cBase *&)`
- `0x0000770c` `matched` 120B `cListSubscriber::Read(cFile &, cMemPool *)`
- `0x00008550` `failed` 356B `cConfigBase::Read(cFile &, cMemPool *)`
- `0x00008870` `matched` 440B `cConfigPSP::Read(cFile &, cMemPool *)`
- `0x00008ea4` `failed` 172B `cNamed::Read(cFile &, cMemPool *)`
- `0x000093ec` `untried` 828B `cHandle::Read(cReadBlock &, cMemPool *)`
- `0x00009b8c` `failed` 292B `cObject::Read(cFile &, cMemPool *)`
- `0x00009fc8` `untried` 392B `cObject::Load(void)`
- `0x0000a828` `untried` 236B `cObject::ReadHeader(cFile &, cObject::cFileInfo *) static`
- `0x0000a914` `untried` 196B `cObject::GetFileInfo(const char *, cObject::cFileInfo *) static`
- `0x0000ab98` `matched` 188B `cFactory::Read(cFile &, cMemPool *)`
- `0x0000b70c` `untried` 236B `cFactory::LoadLocalized(const char *)`
- `0x0000be30` `untried` 520B `cFactory::ReadGroups(cReadBlock &, cMemPool *)`
- `0x0000c22c` `failed` 356B `cGroup::Read(cFile &, cMemPool *)`
- `0x0000cce8` `untried` 288B `cGroup::ReadLocalized(cFile &, cMemPool *) static`
- `0x00013778` `matched` 188B `eTextureGroup::Read(cFile &, cMemPool *)`
- `0x00013d2c` `matched` 188B `eMaterialGroup::Read(cFile &, cMemPool *)`
- `0x000142e0` `matched` 188B `ePathGroup::Read(cFile &, cMemPool *)`
- `0x000148a4` `matched` 188B `eMeshGroup::Read(cFile &, cMemPool *)`
- `0x00014e58` `matched` 188B `eGeomTemplateGroup::Read(cFile &, cMemPool *)`
- `0x0001540c` `matched` 188B `eMaterialSetGroup::Read(cFile &, cMemPool *)`
- `0x000159c0` `matched` 188B `eSoundDataGroup::Read(cFile &, cMemPool *)`
- `0x00015f74` `matched` 188B `eAnimationGroup::Read(cFile &, cMemPool *)`

## Placement

- Canonical source: `src/cReadBlock.cpp`
- Header(s): `include/cReadBlock.h`
- Allowed source(s): `src/cReadBlock.cpp`
- Split-TU prefix: `src/cReadBlock_*.cpp`

## Compiler Guidance

Destructor template: use canonical C++ destructor syntax. For deleting destructors, an inline class-local operator delete often lets SNC emit the pool-delete tail; avoid extern-C destructor wrappers.

## Class Header: `include/cReadBlock.h`

Not found.

## Matched Method Exemplars

### Exemplar 1: `cMemPoolPlatform::~cMemPoolPlatform(void)`

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

### Exemplar 2: `cThread::~cThread(void)`

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

### Exemplar 3: `cBufferedFile::~cBufferedFile(void)`

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

### Exemplar 4: `cFileHandle::~cFileHandle(void)`

- Address: `0x001c8370`
- Size: `176` bytes
- Source: `src/cFileHandle.cpp`

```cpp
// cFileHandle::~cFileHandle(void) @ 0x001c8370 (176B, cAll_psp.obj)
//
// cFileHandle embeds a cBufferedFile member at offset 0x00 and adds 4
// trailing fields used to track an in-flight read request.  The destructor
// optionally re-issues a Close() before zeroing the request bookkeeping;
// SNC then auto-emits ~mBuffered(2) and the deleting-tail dispatch.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
```


## Prior Failure Notes

### Note 1

- session `650bf8ca`; src `src/cReadBlock.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/650bf8ca/00005dbc__agent_self_reported_failure__src_cReadBlock.cpp`

8/212 byte diff after extensive iteration. Function size matches (212B); sltu+andi 0xff bool-cast pattern matches; call structure matches; deleting-tail dispatch via inline operator delete + free fallback matches. Discovered that explicit '((expr != 0) & 0xFF)' cast pattern triggers SNC's canonical bool emission (sltu+andi) — was missing from prior simpler bool patterns. Used 'cFile *fr = *(cFile * volatile *)&mFile' volatile-load trick to force mFile reload between cached f (s2) and call sites; this matched the GetCurrentPos arg-setup ordering. Final 8 bytes are 2 instructions swapped in cFileSystem::Read setup at +0x5c/+0x60: expected has 'lw a0,0(s0); move a1,sp; lw a0,0(a0); jal' but mine emits 'lw a0,0(s0); lw a0,0(a0); move a1,sp; jal'. Permuter ran 180s/6800 candidates and improved 13->8 bytes (saved current source) but cannot break further. Tried: register int *pd asm("$5") hint to force a1 allocation, explicit '&dummy' materialization barrier, separate cFileHandle *h cache. Root cause: SNC -Xsched=2 scheduler choice for the Read arg-setup window — my source produces equivalent semantics but the scheduler interleaves the deref-mHandle and load-stack-addr differently. Not a bnel/beql divergence (no branch-likely involved). Recording as failed pending permuter exhaustion or sched=N variant. Best-effort source preserved in src/cReadBlock.cpp.


## Disassembly

```asm
5dbc:	27bdffe0 	addiu	sp,sp,-32
5dc0:	afb00004 	sw	s0,4(sp)
5dc4:	00808025 	move	s0,a0
5dc8:	afb10008 	sw	s1,8(sp)
5dcc:	afb2000c 	sw	s2,12(sp)
5dd0:	afbf0010 	sw	ra,16(sp)
5dd4:	10800028 	beqz	a0,0x5e78
5dd8:	00a08825 	move	s1,a1
5ddc:	8e120000 	lw	s2,0(s0)
5de0:	8e440000 	lw	a0,0(s2)
5de4:	0004202b 	sltu	a0,zero,a0
5de8:	308400ff 	andi	a0,a0,0xff
5dec:	1080000f 	beqz	a0,0x5e2c
5df0:	32240001 	andi	a0,s1,0x1
5df4:	8e040000 	lw	a0,0(s0)
5df8:	0c0014eb 	jal	0x53ac
5dfc:	afa00000 	sw	zero,0(sp)
5e00:	24440003 	addiu	a0,v0,3
5e04:	00042882 	srl	a1,a0,0x2
5e08:	00052880 	sll	a1,a1,0x2
5e0c:	0c0014f2 	jal	0x53c8
5e10:	02402025 	move	a0,s2
5e14:	8e040000 	lw	a0,0(s0)
5e18:	03a02825 	move	a1,sp
5e1c:	8c840000 	lw	a0,0(a0)
5e20:	0c0013fd 	jal	0x4ff4
5e24:	34060004 	li	a2,0x4
5e28:	32240001 	andi	a0,s1,0x1
5e2c:	10800012 	beqz	a0,0x5e78
5e30:	00000000 	nop
5e34:	0c0011e8 	jal	0x47a0
5e38:	02002025 	move	a0,s0
5e3c:	00402025 	move	a0,v0
5e40:	1080000b 	beqz	a0,0x5e70
5e44:	00000000 	nop
5e48:	8c840024 	lw	a0,36(a0)
5e4c:	02002825 	move	a1,s0
5e50:	8c86001c 	lw	a2,28(a0)
5e54:	24c60030 	addiu	a2,a2,48
5e58:	84c70000 	lh	a3,0(a2)
5e5c:	8cc60004 	lw	a2,4(a2)
5e60:	00c0f809 	jalr	a2
5e64:	00872021 	addu	a0,a0,a3
5e68:	10000003 	b	0x5e78
5e6c:	00000000 	nop
5e70:	0c06b1a4 	jal	0x1ac690
5e74:	02002025 	move	a0,s0
5e78:	8fb00004 	lw	s0,4(sp)
5e7c:	8fb10008 	lw	s1,8(sp)
5e80:	8fb2000c 	lw	s2,12(sp)
5e84:	8fbf0010 	lw	ra,16(sp)
5e88:	03e00008 	jr	ra
5e8c:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: cReadBlock::~cReadBlock(void)
// Address: 0x00005dbc, Size: 212B
// Obj: cAll_psp.obj

? cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(s32, void *, ?); /* extern */
s32 cFile__GetCurrentPos_voidconst(s32 *);          /* extern */
? cFile__SetCurrentPos_unsignedint(s32 *, s32);     /* extern */
void *cMemPool__GetPoolFromPtr_constvoidptrstatic(s32 **); /* extern */
? free(s32 **);                                     /* extern */

void cReadBlock___dtor_cReadBlock_void(s32 **arg0, s32 arg1) {
    ? (*temp_a2_2)(void *, s32 **, ?, s16);
    s16 temp_a3;
    s32 *temp_s2;
    s32 var_a0;
    void *temp_a0;
    void *temp_a2;
    void *temp_v0;

    if (arg0 != NULL) {
        temp_s2 = *arg0;
        var_a0 = arg1 & 1;
        if ((*temp_s2 != 0) & 0xFF) {
            cFile__SetCurrentPos_unsignedint(temp_s2, ((u32) (cFile__GetCurrentPos_voidconst(*arg0) + 3) >> 2) * 4);
            cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(**arg0, sp, 4);
            var_a0 = arg1 & 1;
        }
        if (var_a0 != 0) {
            temp_v0 = cMemPool__GetPoolFromPtr_constvoidptrstatic(arg0);
            if (temp_v0 != NULL) {
                temp_a0 = temp_v0->unk24;
                temp_a2 = temp_a0->unk1C;
                temp_a3 = temp_a2->unk30;
                temp_a2_2 = (temp_a2 + 0x30)->unk4;
                temp_a2_2(temp_a0 + temp_a3, arg0, temp_a2_2, temp_a3);
                return;
            }
            free(arg0);
        }
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
