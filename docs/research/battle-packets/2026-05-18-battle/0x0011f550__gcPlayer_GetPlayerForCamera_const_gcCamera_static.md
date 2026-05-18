# Battle Packet: `gcPlayer::GetPlayerForCamera(const gcCamera *) static`

## Target

- Address: `0x0011f550`
- Size: `100` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcPlayer`
- Method family: `GetPlayerForCamera`
- Leaf: `True`

## Queue Metadata

- score: `162`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 8B; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `8`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context


Callers:
- `0x0016ac64` `untried` 1260B `gcCamera::HandlePausedState(void)`

## Placement

- Canonical source: `src/gcPlayer.cpp`
- Header(s): `include/gcPlayer.h`
- Allowed source(s): `src/gcPlayer.cpp`
- Split-TU prefix: `src/gcPlayer_*.cpp`

## Class Header: `include/gcPlayer.h`

```cpp
#ifndef GCPLAYER_H
#define GCPLAYER_H

class cFile;
class gcCamera;
class gcEntity;
class cBase;
class cMemPool;
class cType;
class nwConnectionHandle;
class gcReplicationVisitor;

template <class T>
class cHandleT {
public:
    int mIndex;
};

namespace gcPlayer_priv {
    struct DeleteRecord {
        short offset;
        short pad;
        void (*fn)(void *, void *);
    };
    class cMemPoolNS {
    public:
        static cMemPoolNS *GetPoolFromPtr(const void *);
    };
}

class gcPlayer {
public:
    gcPlayer(cBase *);
    static void HandleClientWarp(void);
    static void Reset(void);
    static void ResetController(int);
    static void ResetControllers(void);
    static void ResetOtherControllers(void);
    static int BindDefaultController(void);
    static int AssignNew(void);
    static void AssignController(int, int);
    static int GetStartingController(void);
    static int BindLocalController(void);
    static void AssignLocalController(int, bool);
    static void AssignLocalControllers(bool);
    static gcPlayer *GetPlayerForConnection(nwConnectionHandle);
    static gcPlayer *GetPlayerForCamera(const gcCamera *);
    static int FindAvailablePlayer(void);
    static void MemCardReplicate(gcReplicationVisitor &);
    static cBase *New(cMemPool *, cBase *);
    void SetEntity(cHandleT<gcEntity>);
    void *GetCamera(void) const;
    void GetName(char *) const;
    void AssignController(int, bool);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    ~gcPlayer();

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        gcPlayer_priv::cMemPoolNS *pool =
            gcPlayer_priv::cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        gcPlayer_priv::DeleteRecord *rec =
            (gcPlayer_priv::DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

#endif
```

## Matched Same-Class Neighbors

- `0x0011ed04` 40B `gcPlayer::Write(cFile &) const` — `src/gcPlayer.cpp`
- `0x0011ed2c` 120B `gcPlayer::Read(cFile &, cMemPool *)` — `src/gcPlayer.cpp`
- `0x0011eda4` 168B `gcPlayer::gcPlayer(cBase *)` — `src/gcPlayer.cpp`
- `0x0011ee4c` 100B `gcPlayer::~gcPlayer(void)` — `src/gcPlayer.cpp`
- `0x0011f3b0` 312B `gcPlayer::SetEntity(cHandleT<gcEntity>)` — `src/gcPlayer.cpp`

## Prior Failure Notes

### Note 1

- session `5bd12b7e`

Still a same-size register-allocation mismatch in gcPlayer::GetPlayerForCamera(const gcCamera *). The current best output is 100B with the same control flow and all immediates/displacements matching; remaining 14 byte differences are register fields: expected keeps viewports in a1, viewport index in a3, candidate viewport pointer in a2, and loop index in t0, while SNC keeps viewports in a3, viewport index in a2, candidate pointer/temp in a1, and loop index in t0. The saved source includes a memory barrier after loading p, which improves scheduling from the previous 19-byte baseline to 14 bytes by moving li t0 before the viewport load, but it does not change allocation.

Tried sched=1, i-before-viewports and viewports-before-i ordering, hoisting vp/vpIdx declarations, typed PlayerSlot/ViewportSlot wrappers, int-address arithmetic for viewports/vp, explicit goto loop spelling, register keyword and local register-variable syntax, empty asm operand/clobber attempts, and a 300s permuter run. The permuter generated 8280 candidates, compiled 4793, improved once from 19 to 14 bytes, and did not match. This does not meet unmatchable_bnel criteria; branch forms already match, and the blocker appears to be SNC register allocation/live-range ordering.

### Note 2

- session `ceb158e2`

gcPlayer::GetPlayerForCamera. Got to 8-byte diff (92/100 bytes match), but could not flip register allocation. The function size is correct, all 25 instructions match by opcode/operand-structure/offset, and the control flow including the bltzl branch-likely is correct. The only divergence is that SNC assigns the loop-local registers differently: expected uses a1=viewports, a3=vpIdx, a2=vp, t0=i; my build produces t0=viewports, a2=vpIdx, a1=vp, a3=i. This rotation of the four non-v0/non-a0 registers produces exactly the 8-word diff seen (register fields differ; every immediate/displacement matches).

Approaches tried that did NOT budge register allocation: (1) reordering declarations of p/viewports/i every which way (i-first, i-last, viewports-first, viewports-last); (2) do-while vs for vs while(1)+break; (3) `if (vpIdx<0) continue;` early-exit form vs nested-if form; (4) hoisting `char *vp = 0;` before the inner if; (5) changing the multiplication operand order; (6) moving the `char *viewports` load inside the loop (SNC hoists it but then drops bltzl and gets worse — 76-byte diff). The permuter (`tools/permuter.py`, 120s + 200s runs, ~4500+ candidates each, 22 cand/sec) improved from a 76-byte baseline down to best=14 in its own metric but couldn't budge further — register allocation is not something its randomized mutations explore.

Root cause is almost certainly SNC's register allocator prioritising differently based on some live-range ordering heuristic I can't reach from C source. This is NOT a bnel divergence (no branch-likely instructions differ). A future agent could try: (a) a struct/class wrapper with a named viewport-index field to give SNC different type info for the offset-48 load; (b) compiling with -Xsched=1 to see if the allocator order changes (docs/decisions/003 says Xsched affects codegen); (c) writing an inline asm constraint that forces viewports to `a1`; (d) deeper permuter run (>30min) with custom mutations.


## Disassembly

```asm
11f550:	3c050038 	lui	a1,0x38
11f554:	8ca2d87c 	lw	v0,-10116(a1)
11f558:	3c050038 	lui	a1,0x38
11f55c:	34080000 	li	t0,0x0
11f560:	8ca5d840 	lw	a1,-10176(a1)
11f564:	8c470030 	lw	a3,48(v0)
11f568:	04e2000b 	bltzl	a3,0x11f598
11f56c:	25080001 	addiu	t0,t0,1
11f570:	10a00005 	beqz	a1,0x11f588
11f574:	34060000 	li	a2,0x0
11f578:	24061390 	li	a2,5008
11f57c:	00e60018 	mult	a3,a2
11f580:	00003012 	mflo	a2
11f584:	00a63021 	addu	a2,a1,a2
11f588:	24c60010 	addiu	a2,a2,16
11f58c:	10c40007 	beq	a2,a0,0x11f5ac
11f590:	00000000 	nop
11f594:	25080001 	addiu	t0,t0,1
11f598:	29060008 	slti	a2,t0,8
11f59c:	14c0fff1 	bnez	a2,0x11f564
11f5a0:	24420044 	addiu	v0,v0,68
11f5a4:	03e00008 	jr	ra
11f5a8:	00001025 	move	v0,zero
11f5ac:	03e00008 	jr	ra
11f5b0:	00000000 	nop
```

## m2c Starting Point

```c
// Decompiled: gcPlayer::GetPlayerForCamera(const gcCamera *) static
// Address: 0x0011f550, Size: 100B
// Obj: gcAll_psp.obj

void *gcPlayer__GetPlayerForCamera_constgcCameraptrstatic(s32 arg0) {
    s32 temp_a1;
    s32 temp_a3;
    s32 var_a2;
    s32 var_t0;
    void *var_v0;

    var_v0 = *(void **)0x37D87C;
    var_t0 = 0;
    temp_a1 = *(s32 *)0x37D840;
loop_1:
    temp_a3 = var_v0->unk30;
    if (temp_a3 >= 0) {
        var_a2 = 0;
        if (temp_a1 != 0) {
            var_a2 = temp_a1 + (temp_a3 * 0x1390);
        }
        if ((var_a2 + 0x10) != arg0) {
            goto block_5;
        }
        return var_v0;
    }
block_5:
    var_t0 += 1;
    var_v0 += 0x44;
    if (var_t0 >= 8) {
        return NULL;
    }
    goto loop_1;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
