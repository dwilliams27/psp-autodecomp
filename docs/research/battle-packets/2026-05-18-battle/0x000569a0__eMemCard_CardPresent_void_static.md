# Battle Packet: `eMemCard::CardPresent(void) static`

## Target

- Address: `0x000569a0`
- Size: `124` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eMemCard`
- Method family: `CardPresent`
- Leaf: `False`

## Queue Metadata

- score: `146`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 3B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `3`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `3`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x0036bbcc` `untried` 0B `sceKernelUnlockMutex`
- `0x0036bc24` `untried` 0B `sceKernelLockMutex`

Callers:
- `0x00103f34` `untried` 1084B `gcGameSettings::SaveGameListGames(void)`
- `0x001045cc` `untried` 1484B `gcGameSettings::ProfileList(void)`
- `0x003502f4` `untried` 336B `gcValMemCardStatus::Evaluate(void) const`

## Placement

- Canonical source: `src/eMemCard.cpp`
- Header(s): `include/eMemCard.h`
- Allowed source(s): `src/eMemCard.cpp`
- Split-TU prefix: `src/eMemCard_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.


## Class Header: `include/eMemCard.h`

```cpp
#ifndef EMEMCARD_H
#define EMEMCARD_H

class eMemCard {
public:
    struct eLoadParams {};
    struct eSaveParams {};

    static int Reset(void);
    static void SetSlot(int);
    static float GetMinSaveTime(void);
    static float GetMinLoadTime(void);
    static void Format(void);
    static int GetSaveSize(int a, int b, int c);
    static void BeginLoad(const eLoadParams &);
    static void EndLoad(const eLoadParams &);
    static void EndSave(const eSaveParams &);
    static void Initialize(void);
    static unsigned char CardDifferent(void);
    static unsigned char CardPresent(void);
    static unsigned char CardChanged(void);
    static int GetCardFreeSpace(void);
};

class eMemCardPlatform {
public:
    static void Initialize(void);
};

#endif
```

## Matched Same-Class Neighbors

- `0x000553dc` 28B `eMemCard::Initialize(void) static` — `src/eMemCard.cpp`
- `0x000553f8` 8B `eMemCard::Reset(void) static` — `src/eHeightmap.cpp`
- `0x00055400` 12B `eMemCard::SetSlot(int) static` — `src/eMemCard.cpp`
- `0x0005540c` 12B `eMemCard::GetMinSaveTime(void) static` — `src/eMemCard.cpp`
- `0x00055418` 12B `eMemCard::GetMinLoadTime(void) static` — `src/eMemCard.cpp`

## Prior Failure Notes

### Note 1

- session `4af4dbe1`

Got CardPresent to a stable 3/124 byte mismatch while preserving the matched CardDifferent scoped-lock shape. Current output differs only in the register used as the HI-base for gMemCardPresent: compiled has `lui a1,%hi(gMemCardPresent); lbu s0,%lo(gMemCardPresent)(a1)`, while expected has `lui s0,%hi(gMemCardPresent); lbu s0,%lo(gMemCardPresent)(s0)`. All control flow, status update, destructor active check, nop delay slot, and unlock sequence match after relocation masking.

Tried direct global load, pointer local, integer-address local, register-bound pointer/address locals, empty asm inputs/clobbers to influence allocation, explicit status local before the present load, a memory barrier after the status store, source-local sched=1, and a tiny inline-asm load experiment. The inline-asm load either failed SNC symbol syntax or introduced extra `andi` normalization, so it was reverted. Permuter ran for 180s from the 3-byte baseline (3120 generated, 1662 compiled) and found zero improvements. This is a tiny register-allocation divergence, not a bnel issue.

### Note 2

- session `3ab40608`

Same as CardDifferent: RAII mutex lock + conditional status update + read gMemCardPresent (u8 return). Semantics verified: gMemCardStatus = gMemCardPresent ? 0 : 3; return gMemCardPresent. 63/120 bytes differing. Same sched=2-vs-sched=1 fingerprint as CardDifferent: prologue s0/ra saves relocated to end of ctor inlining, and the `li a0,3/bnezl` pair that sets the status constant has different ordering relative to the lbu of gMemCardPresent (expected: li-then-lbu; sched=2: lbu-then-li). Expected also inserts status-write between the active-check lbu and the beqz branch (sw in delay slot); our sched=2 puts the lbu later. All semantic/structural choices are correct. RECOMMENDATION: Add sched=1 override for eMemCard%.o in Makefile.


## Disassembly

```asm
569a0:	27bdfff0 	addiu	sp,sp,-16
569a4:	afb00008 	sw	s0,8(sp)
569a8:	afbf000c 	sw	ra,12(sp)
569ac:	34040001 	li	a0,0x1
569b0:	3c05000a 	lui	a1,0xa
569b4:	24a68670 	addiu	a2,a1,-31120
569b8:	afa60000 	sw	a2,0(sp)
569bc:	a3a40004 	sb	a0,4(sp)
569c0:	8ca48670 	lw	a0,-31120(a1)
569c4:	34050001 	li	a1,0x1
569c8:	0c0daf09 	jal	0x36bc24
569cc:	00003025 	move	a2,zero
569d0:	34040003 	li	a0,0x3
569d4:	3c100038 	lui	s0,0x38
569d8:	9210d3bc 	lbu	s0,-11332(s0)
569dc:	56000001 	bnezl	s0,0x569e4
569e0:	34040000 	li	a0,0x0
569e4:	3c050038 	lui	a1,0x38
569e8:	aca4d2c8 	sw	a0,-11576(a1)
569ec:	93a40004 	lbu	a0,4(sp)
569f0:	10800005 	beqz	a0,0x56a08
569f4:	00000000 	nop
569f8:	8fa40000 	lw	a0,0(sp)
569fc:	8c840000 	lw	a0,0(a0)
56a00:	0c0daef3 	jal	0x36bbcc
56a04:	34050001 	li	a1,0x1
56a08:	02001025 	move	v0,s0
56a0c:	8fb00008 	lw	s0,8(sp)
56a10:	8fbf000c 	lw	ra,12(sp)
56a14:	03e00008 	jr	ra
56a18:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: eMemCard::CardPresent(void) static
// Address: 0x000569a0, Size: 124B
// Obj: eAll_psp.obj

? sceKernelLockMutex(s32, ?, ?);                    /* extern */
? sceKernelUnlockMutex(s32, ?);                     /* extern */

u8 eMemCard__CardPresent_voidstatic(void) {
    s32 var_a0;
    u8 temp_s0;

    sceKernelLockMutex(*(s32 *)0x98670, 1, 0);
    var_a0 = 3;
    temp_s0 = *(u8 *)0x37D3BC;
    if (temp_s0 != 0) {
        var_a0 = 0;
    }
    *(s32 *)0x37D2C8 = var_a0;
    if (subroutine_arg1 != 0) {
        sceKernelUnlockMutex(*subroutine_arg0, 1);
    }
    return temp_s0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
