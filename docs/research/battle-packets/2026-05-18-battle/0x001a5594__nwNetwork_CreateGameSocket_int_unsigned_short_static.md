# Battle Packet: `nwNetwork::CreateGameSocket(int, unsigned short) static`

## Target

- Address: `0x001a5594`
- Size: `184` bytes
- Status: `failed`
- Object: `nwAll_psp.obj`
- Class: `nwNetwork`
- Method family: `CreateGameSocket`
- Leaf: `False`

## Queue Metadata

- score: `137`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 41B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `41`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `41`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x001a5658` `matched` 16B `nwNetwork::GetGameVersion(void) static`
- `0x001a58c8` `matched` 8B `nwNetwork::PlatformGetGameTransport(void) static`
- `0x001a7a04` `failed` 468B `nwSocket::Create(nwTransportType, unsigned short, unsigned int, int, int) static`
- `0x001a7bd8` `matched` 28B `nwSocket::Destroy(void)`
- `0x001a8370` `matched` 120B `nwSocket::GetSocket(nwSocketHandle) static`

Callers:
- `0x00108300` `untried` 288B `gcNetGame::StartClient(bool, const nwAddress &, const char *) static`
- `0x0010904c` `untried` 832B `gcNetGame::HandleCommandLine(void) static`

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

- session `dd59bc1b`; src `src/nwNetwork.cpp`; snapshot `logs/failure_snapshots/match_20260430_231646/dd59bc1b/001a5594__agent_self_reported_failure__src_nwNetwork.cpp`

Closest attempt is in src/nwNetwork.cpp and compiles to the correct 184-byte function with only 9 bytes differing. The remaining diff is in the nwSocket::Create call setup after GetGameVersion(): current SNC emits `lw a0,D_0037D94C(s2); lw t0,0x10(a0); move a0,s4`, while the target emits `lw a1,D_0037D94C(s2); move a0,s4; lw t0,0x10(a1)`. All surrounding prologue, default-port path, GetSocket/Destroy calls, volatile result stack spill, D_0009F930 store, return load, and epilogue match after relocation masking.

Tried direct absolute-address source, extern-symbol globals to avoid addiu base materialization, struct-typed config access, volatile D_0009F930/result spill to recover the store/load tail, inline GetGameVersion vs local version, register-pinned transport/config variants, and a sched=1 override (worse, 41-byte diff). Read docs/research/snc-delay-slots.md before further scheduling variants. Ran `python3 tools/permuter.py src/nwNetwork.cpp 0x001a5594 --time 300 --save-best`; it generated 9864 candidates, compiled 3900, and found no improvement over the 9-byte baseline. This is retryable register-allocation/source-shape work, not a bnel/unmatchable case.


## Disassembly

```asm
1a5594:	27bdffe0 	addiu	sp,sp,-32
1a5598:	afb30010 	sw	s3,16(sp)
1a559c:	30b3ffff 	andi	s3,a1,0xffff
1a55a0:	afb00004 	sw	s0,4(sp)
1a55a4:	afb2000c 	sw	s2,12(sp)
1a55a8:	3405ffff 	li	a1,0xffff
1a55ac:	00808025 	move	s0,a0
1a55b0:	3c120038 	lui	s2,0x38
1a55b4:	afb10008 	sw	s1,8(sp)
1a55b8:	afb40014 	sw	s4,20(sp)
1a55bc:	afbf0018 	sw	ra,24(sp)
1a55c0:	16650003 	bne	s3,a1,0x1a55d0
1a55c4:	3c11000a 	lui	s1,0xa
1a55c8:	8e44d94c 	lw	a0,-9908(s2)
1a55cc:	9493000c 	lhu	s3,12(a0)
1a55d0:	0c06a0dc 	jal	0x1a8370
1a55d4:	8e24f930 	lw	a0,-1744(s1)
1a55d8:	10400005 	beqz	v0,0x1a55f0
1a55dc:	00000000 	nop
1a55e0:	0c06a0dc 	jal	0x1a8370
1a55e4:	8e24f930 	lw	a0,-1744(s1)
1a55e8:	0c069ef6 	jal	0x1a7bd8
1a55ec:	00402025 	move	a0,v0
1a55f0:	0c069632 	jal	0x1a58c8
1a55f4:	00000000 	nop
1a55f8:	0c069596 	jal	0x1a5658
1a55fc:	0040a025 	move	s4,v0
1a5600:	8e45d94c 	lw	a1,-9908(s2)
1a5604:	02802025 	move	a0,s4
1a5608:	8ca80010 	lw	t0,16(a1)
1a560c:	02602825 	move	a1,s3
1a5610:	00403025 	move	a2,v0
1a5614:	0c069e81 	jal	0x1a7a04
1a5618:	02003825 	move	a3,s0
1a561c:	afa20000 	sw	v0,0(sp)
1a5620:	8fa40000 	lw	a0,0(sp)
1a5624:	ae24f930 	sw	a0,-1744(s1)
1a5628:	8e22f930 	lw	v0,-1744(s1)
1a562c:	8fb00004 	lw	s0,4(sp)
1a5630:	8fb10008 	lw	s1,8(sp)
1a5634:	8fb2000c 	lw	s2,12(sp)
1a5638:	8fb30010 	lw	s3,16(sp)
1a563c:	8fb40014 	lw	s4,20(sp)
1a5640:	8fbf0018 	lw	ra,24(sp)
1a5644:	03e00008 	jr	ra
1a5648:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: nwNetwork::CreateGameSocket(int, unsigned short) static
// Address: 0x001a5594, Size: 184B
// Obj: nwAll_psp.obj

s32 nwNetwork__GetGameVersion_voidstatic();         /* extern */
s32 nwNetwork__PlatformGetGameTransport_voidstatic(); /* extern */
? nwSocket__Create_nwTransportType_unsignedshort_unsignedint_int_intstatic(s32, u16, s32, s32); /* extern */
? nwSocket__Destroy_void(s32);                      /* extern */
s32 nwSocket__GetSocket_nwSocketHandlestatic(s32, ?); /* extern */

s32 nwNetwork__CreateGameSocket_int_unsignedshortstatic(s32 arg0, s32 arg1) {
    s32 temp_s4;
    u16 var_s3;

    var_s3 = arg1 & 0xFFFF;
    if (var_s3 == 0xFFFF) {
        var_s3 = (*(void **)0x37D94C)->unkC;
    }
    if (nwSocket__GetSocket_nwSocketHandlestatic(*(s32 *)0x9F930, 0xFFFF) != 0) {
        nwSocket__Destroy_void(nwSocket__GetSocket_nwSocketHandlestatic(*(s32 *)0x9F930));
    }
    temp_s4 = nwNetwork__PlatformGetGameTransport_voidstatic();
    nwSocket__Create_nwTransportType_unsignedshort_unsignedint_int_intstatic(temp_s4, var_s3, nwNetwork__GetGameVersion_voidstatic(), arg0);
    *(s32 *)0x9F930 = subroutine_arg0;
    return *(s32 *)0x9F930;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
