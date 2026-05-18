# Battle Packet: `nwNetwork::GetMemPool(void) static`

## Target

- Address: `0x001a539c`
- Size: `176` bytes
- Status: `failed`
- Object: `nwAll_psp.obj`
- Class: `nwNetwork`
- Method family: `GetMemPool`
- Leaf: `False`

## Queue Metadata

- score: `131`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 32B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `32`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `32`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000030a8` `matched` 140B `cDynamicMemAllocator::cDynamicMemAllocator(const char *, unsigned int, unsigned int, void *)`
- `0x00004540` `failed` 336B `cMemPool::cMemPool(cMemAllocator *, const char *)`
- `0x001b95d4` `untried` 84B `__record_needed_destruction`

Callers:
- `0x001a52ec` `matched` 176B `nwNetwork::Initialize(void) static`
- `0x001a67e4` `untried` 312B `nwConnection::AllocateBuffers(void)`
- `0x001a7a04` `failed` 468B `nwSocket::Create(nwTransportType, unsigned short, unsigned int, int, int) static`
- `0x001a81b4` `untried` 172B `nwSocket::Create(void)`
- `0x001a848c` `untried` 396B `nwSocket::CreateConnection(const nwAddress &, void (*)(nwConnectionHandle, nwConnection::nwConnectionError))`

## Placement

- Canonical source: `src/nwNetwork.cpp`
- Header(s): `include/nwNetwork.h`
- Allowed source(s): `src/nwNetwork.cpp`
- Split-TU prefix: `src/nwNetwork_*.cpp`

## Class Header: `include/nwNetwork.h`

Not found.

## Matched Same-Class Neighbors

- `0x001a52ec` 176B `nwNetwork::Initialize(void) static` — `src/nwNetwork.cpp`
- `0x001a544c` 176B `nwNetwork::Update(cTimeValue) static` — `src/nwNetwork.cpp`
- `0x001a54fc` 8B `nwNetwork::GetLobby(void) static` — `src/gcUIGeomTemplate.cpp`
- `0x001a564c` 12B `nwNetwork::GetGameSocket(void) static` — `src/gcDesiredObjectHelper_GetPrimaryTextgcDesiredObjectHelper.cpp`
- `0x001a5658` 16B `nwNetwork::GetGameVersion(void) static` — `src/nwNetwork_GetGameVersion.cpp`

## Prior Failure Notes

### Note 1

- session `cc9a74ab`; src `src/nwNetwork.cpp`; snapshot `logs/failure_snapshots/match_20260430_231646/cc9a74ab/001a539c__agent_self_reported_failure__src_nwNetwork.cpp`

Closest attempt is 32 bytes off after relocation masking. The control flow, calls, stack size, and instruction count match; remaining differences are scheduler/register-allocation around the static guard store and size calculation. Expected spills the pool pointer before loading/storing the guard value, uses a2/a1 for configSize*2 + 0x13000, and keeps the cMemPool ctor name addiu in the jal delay slot. Current source emits the same operations but orders the guard store before the spill and uses a1/a2 differently in the size block. Tried raw absolute addresses, shared 0x30000 base addressing, extern relocation symbols for D_00034C5C/D_00034C60/D_00000328, placement-new construction, direct constructor symbol calls, sched=1 override, register hints, and memory barriers. Placement-new introduced null checks; sched=1 worsened the prologue. Permuter ran 300s against nwNetwork::GetMemPool with 11,424 candidates generated / 4,067 compiled and found zero improvement over 32 bytes.


## Disassembly

```asm
1a539c:	27bdfff0 	addiu	sp,sp,-16
1a53a0:	3c040003 	lui	a0,0x3
1a53a4:	8c854c5c 	lw	a1,19548(a0)
1a53a8:	3c020003 	lui	v0,0x3
1a53ac:	afb00004 	sw	s0,4(sp)
1a53b0:	afb10008 	sw	s1,8(sp)
1a53b4:	afbf000c 	sw	ra,12(sp)
1a53b8:	14a0001f 	bnez	a1,0x1a5438
1a53bc:	24424c60 	addiu	v0,v0,19552
1a53c0:	afa20000 	sw	v0,0(sp)
1a53c4:	34060001 	li	a2,0x1
1a53c8:	ac864c5c 	sw	a2,19548(a0)
1a53cc:	3c040038 	lui	a0,0x38
1a53d0:	8c84d94c 	lw	a0,-9908(a0)
1a53d4:	2450009c 	addiu	s0,v0,156
1a53d8:	8c860010 	lw	a2,16(a0)
1a53dc:	00402025 	move	a0,v0
1a53e0:	00c62821 	addu	a1,a2,a2
1a53e4:	3c060001 	lui	a2,0x1
1a53e8:	24c63000 	addiu	a2,a2,12288
1a53ec:	00a68821 	addu	s1,a1,a2
1a53f0:	3c060037 	lui	a2,0x37
1a53f4:	02002825 	move	a1,s0
1a53f8:	0c001150 	jal	0x4540
1a53fc:	24c6118c 	addiu	a2,a2,4492
1a5400:	8fa50000 	lw	a1,0(sp)
1a5404:	3c040000 	lui	a0,0x0
1a5408:	24840328 	addiu	a0,a0,808
1a540c:	aca40098 	sw	a0,152(a1)
1a5410:	24a50004 	addiu	a1,a1,4
1a5414:	02002025 	move	a0,s0
1a5418:	02203025 	move	a2,s1
1a541c:	00003825 	move	a3,zero
1a5420:	0c000c2a 	jal	0x30a8
1a5424:	00004025 	move	t0,zero
1a5428:	3c040038 	lui	a0,0x38
1a542c:	0c06e575 	jal	0x1b95d4
1a5430:	2484d970 	addiu	a0,a0,-9872
1a5434:	8fa20000 	lw	v0,0(sp)
1a5438:	8fb00004 	lw	s0,4(sp)
1a543c:	8fb10008 	lw	s1,8(sp)
1a5440:	8fbf000c 	lw	ra,12(sp)
1a5444:	03e00008 	jr	ra
1a5448:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: nwNetwork::GetMemPool(void) static
// Address: 0x001a539c, Size: 176B
// Obj: nwAll_psp.obj

? __record_needed_destruction(?);                   /* extern */
? cDynamicMemAllocator__cDynamicMemAllocator_constcharptr_unsignedint_unsignedint_voidptr(?, void *, s32, ?); /* extern */
? cMemPool__cMemPool_cMemAllocatorptr_constcharptr(?, ?, ?); /* extern */

s32 nwNetwork__GetMemPool_voidstatic(void) {
    s32 temp_s1;
    s32 var_v0;

    var_v0 = 0x34C60;
    if (*(s32 *)0x34C5C == 0) {
        *(s32 *)0x34C5C = 1;
        temp_s1 = ((*(void **)0x37D94C)->unk10 * 2) + 0x13000;
        cMemPool__cMemPool_cMemAllocatorptr_constcharptr(0x34C60, 0x34CFC, 0x37118C);
        subroutine_arg0->unk98 = 0x328;
        cDynamicMemAllocator__cDynamicMemAllocator_constcharptr_unsignedint_unsignedint_voidptr(0x34CFC, subroutine_arg0 + 4, temp_s1, 0);
        __record_needed_destruction(0x37D970);
        var_v0 = subroutine_arg0;
    }
    return var_v0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
