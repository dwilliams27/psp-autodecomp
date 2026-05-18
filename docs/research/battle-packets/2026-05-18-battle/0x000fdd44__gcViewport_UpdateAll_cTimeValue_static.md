# Battle Packet: `gcViewport::UpdateAll(cTimeValue) static`

## Target

- Address: `0x000fdd44`
- Size: `152` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcViewport`
- Method family: `UpdateAll`
- Leaf: `False`

## Queue Metadata

- score: `151`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 12B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `12`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `12`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000e0550` `untried` 324B `gcUI::Update(cTimeValue)`
- `0x000f8f08` `untried` 208B `gcCamera::Update(cTimeValue)`

Callers:
- `0x000f364c` `untried` 688B `gcMap::Update(cTimeValue)`

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

## Matched Method Exemplars

### Exemplar 1: `nwSocket::UpdateAll(cTimeValue) static`

- Address: `0x001a8260`
- Size: `100` bytes
- Source: `src/nwSocket.cpp`

```cpp
void nwSocket::UpdateAll(cTimeValue dt) {
    int i = 0;
    nwSocket **socket = D_00034958;

    do {
        nwSocket *s = *socket;
        if (s != 0) {
            s->Update(dt);
        }
        i += 1;
        socket += 1;
    } while (i < 1);
}
```


## Prior Failure Notes

### Note 1

- session `78471c89`; src `src/gcViewport.cpp`; snapshot `logs/failure_snapshots/match_20260505_191203/78471c89/000fdd44__agent_self_reported_failure__src_gcViewport.cpp`

gcViewport::UpdateAll(cTimeValue) static — 12 bytes diff (out of 152). Function structure is fully correct: do-while loop checking unk1380, calling gcCamera::Update(dt) per viewport, post-loop checking D_0037D840[0xED0] and calling gcUI::Update(dt) at 0x99928. All instructions and order match expected. The 12-byte diff is purely register allocation:

Expected: i→s3, offset→s1, global_high→s2, dt→s0
Mine:     i→s3, offset→s2, global_high→s1, dt→s0

The s1/s2 swap for offset vs global_high produces different encoded bytes in: prologue saves (sw s1/s2 swapped order), `lui` of global high reg, two `lw a0, ...(sN)` reloads, two `addu a0,a0,sN` (one with operand order also flipped: expected `addu a1,a0,s1` vs mine `addu a1,s2,a0`), and the offset increment `addiu sN,sN,5008`.

Tried: (a) declaring offset before i in source, (b) introducing local `char *vp = D_0037D840 + offset;`, (c) `((unsigned char*)D_0037D840)[offset+0x1380]` array-index form (current best — gives clean `lbu a1, 0x1380(a1)` overwriting + recompute pattern, just wrong reg names), (d) various comparison forms.

Permuter: 90s, 874 candidates, zero improvement. SNC's allocator has a deterministic preference for s2=global / s1=offset that we couldn't trigger. CloseAllDialogs and OnMemPoolResetAll in the same file allocate global_high to LOWER s-regs (s0, s2 respectively), so the rule isn't simply 'global gets s2'. Likely requires understanding SNC's interference graph coloring heuristic, which appears to be loop-shape-dependent. Current source is structurally correct and the closest possible attempt; the only remaining issue is reg allocation outside source-level control.


## Disassembly

```asm
fdd44:	27bdffe0 	addiu	sp,sp,-32
fdd48:	afa40000 	sw	a0,0(sp)
fdd4c:	afb00004 	sw	s0,4(sp)
fdd50:	afb2000c 	sw	s2,12(sp)
fdd54:	8fb00000 	lw	s0,0(sp)
fdd58:	3c120038 	lui	s2,0x38
fdd5c:	afb10008 	sw	s1,8(sp)
fdd60:	afb30010 	sw	s3,16(sp)
fdd64:	34130000 	li	s3,0x0
fdd68:	34110000 	li	s1,0x0
fdd6c:	8e44d840 	lw	a0,-10176(s2)
fdd70:	afbf0014 	sw	ra,20(sp)
fdd74:	00912821 	addu	a1,a0,s1
fdd78:	90a51380 	lbu	a1,4992(a1)
fdd7c:	50a00007 	beqzl	a1,0xfdd9c
fdd80:	26730001 	addiu	s3,s3,1
fdd84:	00912021 	addu	a0,a0,s1
fdd88:	24840010 	addiu	a0,a0,16
fdd8c:	0c03e3c2 	jal	0xf8f08
fdd90:	02002825 	move	a1,s0
fdd94:	8e44d840 	lw	a0,-10176(s2)
fdd98:	26730001 	addiu	s3,s3,1
fdd9c:	2a650005 	slti	a1,s3,5
fdda0:	14a0fff4 	bnez	a1,0xfdd74
fdda4:	26311390 	addiu	s1,s1,5008
fdda8:	90840ed0 	lbu	a0,3792(a0)
fddac:	14800004 	bnez	a0,0xfddc0
fddb0:	3c04000a 	lui	a0,0xa
fddb4:	02002825 	move	a1,s0
fddb8:	0c038154 	jal	0xe0550
fddbc:	24849928 	addiu	a0,a0,-26328
fddc0:	8fb00004 	lw	s0,4(sp)
fddc4:	8fb10008 	lw	s1,8(sp)
fddc8:	8fb2000c 	lw	s2,12(sp)
fddcc:	8fb30010 	lw	s3,16(sp)
fddd0:	8fbf0014 	lw	ra,20(sp)
fddd4:	03e00008 	jr	ra
fddd8:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcViewport::UpdateAll(cTimeValue) static
// Address: 0x000fdd44, Size: 152B
// Obj: gcAll_psp.obj

? gcCamera__Update_cTimeValue(s32, s32);            /* extern */
? gcUI__Update_cTimeValue(?, s32);                  /* extern */

void gcViewport__UpdateAll_cTimeValuestatic(s32 arg0) {
    s32 var_s1;
    s32 var_s3;

    var_s3 = 0;
    var_s1 = 0;
    do {
        if ((*(void **)0x37D840 + var_s1)->unk1380 != 0) {
            gcCamera__Update_cTimeValue(*(void **)0x37D840 + var_s1 + 0x10, subroutine_arg0);
        }
        var_s3 += 1;
        var_s1 += 0x1390;
    } while (var_s3 < 5);
    if ((*(void **)0x37D840)->unkED0 == 0) {
        gcUI__Update_cTimeValue(0x99928, subroutine_arg0);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
