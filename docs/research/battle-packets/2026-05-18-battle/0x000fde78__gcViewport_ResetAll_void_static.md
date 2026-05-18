# Battle Packet: `gcViewport::ResetAll(void) static`

## Target

- Address: `0x000fde78`
- Size: `148` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcViewport`
- Method family: `ResetAll`
- Leaf: `False`

## Queue Metadata

- score: `151`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 11B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `11`

## Failure Classification

- Primary: `constant_cse_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `constant_cse_codegen`
- Near miss: `11`
- Summary: Compiler hoists/reuses constants differently from target.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x000e0520` `matched` 48B `gcUI::Reset(void)`
- `0x000f88f8` `untried` 392B `gcCamera::Reset(void)`
- `0x000fdc34` `matched` 100B `gcViewport::SetFullscreen(bool) static`

Callers:
- `0x000f2ed8` `untried` 716B `gcMap::Run(void)`
- `0x000fdbc8` `matched` 108B `gcViewport::Initialize(void) static`
- `0x0016f4bc` `untried` 2192B `gcGame::Loop(void)`

## Placement

- Canonical source: `src/gcViewport.cpp`
- Header(s): `include/gcViewport.h`
- Allowed source(s): `src/gcViewport.cpp`
- Split-TU prefix: `src/gcViewport_*.cpp`

## Class Header: `include/gcViewport.h`

Not found.

## Matched Same-Class Neighbors

- `0x000ccd94` 148B `gcViewport::~gcViewport(void)` — `src/gcViewport.cpp`
- `0x000fdadc` 40B `gcViewport::Write(cFile &) const` — `src/eRoom.cpp`
- `0x000fdb04` 120B `gcViewport::Read(cFile &, cMemPool *)` — `src/gcViewport.cpp`
- `0x000fdb7c` 76B `gcViewport::gcViewport(cBase *)` — `src/gcViewport.cpp`
- `0x000fdbc8` 108B `gcViewport::Initialize(void) static` — `src/gcViewport.cpp`

## Prior Failure Notes

### Note 1

- session `78471c89`; src `src/gcViewport.cpp`; snapshot `logs/failure_snapshots/match_20260505_191203/78471c89/000fde78__agent_self_reported_failure__src_gcViewport.cpp`

gcViewport::ResetAll(void) static — 11 bytes diff (out of 148). Function structure is correct: do-while loop calling gcCamera::Reset() and writing unk1381=1 for each of 5 viewports, then post-loop writes viewport[4].unk1380=0, calls gcUI::Reset() at 0x99928, calls SetFullscreen(true). Two unresolvable diffs:

(1) Prologue li-instruction ordering: SNC emits `li s2,0; li s0,1` while expected has `li s0,1; li s2,0` (s0=hoisted constant 1 used in loop sb, s2=offset). Pure scheduler decision — tried multiple variable declaration orders, no change.

(2) Post-loop store: expected emits `addiu a0,a0,0x4E40; li a1,0; sb a1,0x1380(a0)` (3 instr). My best emits `addiu a0,a0,0x4E40; addiu a0,a0,0x1380; sb zero,0(a0)` via `char *vp4 = D_0037D840 + 0x4E40; *(unsigned char*)(vp4 + 0x1380) = 0;`. Without `vp4` indirection the compiler folds to single `+0x61C0`. With struct array indexing `((gcViewportRow*)D_0037D840)[4].unk1380 = val` it also folds. Tried adding `unsigned char val = 0` to force `li a1,0` instead of `sb $zero` — that gets emitted but still doesn't fold the offset into sb's immediate.

Permuter: 60s, 618 candidates compiled, no improvement past 11 bytes. Root cause: SNC's constant folding rules differ between source forms — the original SNC build emitted the split form here, but our pspsnc 1.2.7503.0 always either folds both to a single sb or splits into two addiu+sb (instead of expected addiu+sb-with-imm). Likely needs a specific source idiom we haven't found. Closest source: `char *vp4 = D_0037D840 + 0x4E40; *(unsigned char*)(vp4 + 0x1380) = 0;`.


## Disassembly

```asm
fde78:	27bdffe0 	addiu	sp,sp,-32
fde7c:	afb10004 	sw	s1,4(sp)
fde80:	3c110038 	lui	s1,0x38
fde84:	afb00000 	sw	s0,0(sp)
fde88:	afb20008 	sw	s2,8(sp)
fde8c:	afb3000c 	sw	s3,12(sp)
fde90:	34130000 	li	s3,0x0
fde94:	34100001 	li	s0,0x1
fde98:	34120000 	li	s2,0x0
fde9c:	8e24d840 	lw	a0,-10176(s1)
fdea0:	afbf0010 	sw	ra,16(sp)
fdea4:	00922021 	addu	a0,a0,s2
fdea8:	0c03e23e 	jal	0xf88f8
fdeac:	24840010 	addiu	a0,a0,16
fdeb0:	8e24d840 	lw	a0,-10176(s1)
fdeb4:	26730001 	addiu	s3,s3,1
fdeb8:	00922021 	addu	a0,a0,s2
fdebc:	a0901381 	sb	s0,4993(a0)
fdec0:	26521390 	addiu	s2,s2,5008
fdec4:	2a650005 	slti	a1,s3,5
fdec8:	14a0fff6 	bnez	a1,0xfdea4
fdecc:	8e24d840 	lw	a0,-10176(s1)
fded0:	24844e40 	addiu	a0,a0,20032
fded4:	34050000 	li	a1,0x0
fded8:	a0851380 	sb	a1,4992(a0)
fdedc:	3c04000a 	lui	a0,0xa
fdee0:	0c038148 	jal	0xe0520
fdee4:	24849928 	addiu	a0,a0,-26328
fdee8:	0c03f70d 	jal	0xfdc34
fdeec:	34040001 	li	a0,0x1
fdef0:	8fb00000 	lw	s0,0(sp)
fdef4:	8fb10004 	lw	s1,4(sp)
fdef8:	8fb20008 	lw	s2,8(sp)
fdefc:	8fb3000c 	lw	s3,12(sp)
fdf00:	8fbf0010 	lw	ra,16(sp)
fdf04:	03e00008 	jr	ra
fdf08:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcViewport::ResetAll(void) static
// Address: 0x000fde78, Size: 148B
// Obj: gcAll_psp.obj

? gcCamera__Reset_void(s32, s32);                   /* extern */
? gcUI__Reset_void(?, ?);                           /* extern */
? gcViewport__SetFullscreen_boolstatic(?);          /* extern */

void gcViewport__ResetAll_voidstatic(s32 arg1) {
    s32 var_a1;
    s32 var_s2;
    s32 var_s3;

    var_a1 = arg1;
    var_s3 = 0;
    var_s2 = 0;
    do {
        gcCamera__Reset_void(*(s32 *)0x37D840 + var_s2 + 0x10, var_a1);
        var_s3 += 1;
        (*(s32 *)0x37D840 + var_s2)->unk1381 = 1;
        var_s2 += 0x1390;
        var_a1 = var_s3 < 5;
    } while (var_a1 != 0);
    (*(s32 *)0x37D840 + 0x4E40)->unk1380 = 0;
    gcUI__Reset_void(0x99928, 0);
    gcViewport__SetFullscreen_boolstatic(1);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
