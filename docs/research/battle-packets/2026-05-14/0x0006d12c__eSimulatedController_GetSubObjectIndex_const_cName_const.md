# Battle Packet: `eSimulatedController::GetSubObjectIndex(const cName &) const`

## Target

- Address: `0x0006d12c`
- Size: `212` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eSimulatedController`
- Method family: `GetSubObjectIndex`
- Leaf: `True`

## Queue Metadata

- score: `182`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 8B; codegen-specific diagnosis; manageable size`
- failure_action: `quarantine`
- near_miss_bytes: `8`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `needs_layout`
- Near miss: `8`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context


Callers:
- `0x00358ff4` `untried` 1464B `gcValRigidBodyControllerVariable::Evaluate(void) const`

## Placement

- Canonical source: `src/eSimulatedController.cpp`
- Header(s): `include/eSimulatedController.h`
- Allowed source(s): `src/eSimulatedController.cpp`
- Split-TU prefix: `src/eSimulatedController_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.


## Class Header: `include/eSimulatedController.h`

```cpp
#ifndef ESIMULATEDCONTROLLER_H
#define ESIMULATEDCONTROLLER_H

struct mVec3;

class cBase;
class eGeom;
class cFile;
class cMemPool;
class cName;
class cType;
class eDynamicModel;

class ePhysicsController {
public:
    ePhysicsController(cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;

    char _basePad[0x10];
};

class eSimulatedController : public ePhysicsController {
public:
    eSimulatedController(cBase *);
    void InvalidateCacheEntries(eGeom *);
    void RemoveContacts(void);
    bool IsSleeping(void) const;
    bool IsInFluid(void) const;
    int GetCollisionMask(void) const;
    void SetCollisionMask(unsigned int);
    void SetPosition(int index, const mVec3 &pos);
    void ApplyTorque(int index, const mVec3 &torque);
    void ApplyForce(int index, const mVec3 &force, const mVec3 &pos);
    int GetSubObjectIndex(const cName &) const;
    void ClearExternalForces(void);
    void GetVelocity(int index, const mVec3 &pos, mVec3 *out) const;
    void SnapConstraints(void);
    void Activate(eDynamicModel *);
    void Deactivate(eDynamicModel *);
    void ApplyUnembedImpulse(int index, const mVec3 &impulse);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    const cType *GetType(void) const;

    char _pad[0x24];
    int collisionMask;      // offset 0x34
    void *bodyEntries;      // offset 0x38
    char _pad2[0x114];      // offset 0x3C
    int contactCount;       // offset 0x150
};

#endif
```

## Matched Same-Class Neighbors

- `0x0006bd5c` 76B `eSimulatedController::Write(cFile &) const` — `src/eSimulatedController.cpp`
- `0x0006bda8` 188B `eSimulatedController::Read(cFile &, cMemPool *)` — `src/eSimulatedController.cpp`
- `0x0006be64` 172B `eSimulatedController::eSimulatedController(cBase *)` — `src/eSimulatedController.cpp`
- `0x0006bf10` 364B `eSimulatedController::~eSimulatedController(void)` — `src/eSimulatedController_dtor.cpp`
- `0x0006c07c` 156B `eSimulatedController::Activate(eDynamicModel *)` — `src/eSimulatedController.cpp`

## Matched Method Exemplars

### Exemplar 1: `eDynamicModel::GetSubObjectIndex(const cName &, int) const`

- Address: `0x00047b60`
- Size: `136` bytes
- Source: `src/eDynamicModel.cpp`

```cpp
int eDynamicModel::GetSubObjectIndex(const cName &name, int idx) const {
    void *mesh = *(void **)((char *)this + 0x60);
    eDynamicMesh *resolved = 0;
    if (mesh != 0) {
        int handle = *(int *)((char *)mesh + 0x48);
        if (handle == 0) {
            resolved = 0;
        } else {
            void *entry = D_00038890[handle & 0xFFFF];
            resolved = 0;
            if (entry != 0 && *(int *)((char *)entry + 0x30) == handle) {
                resolved = (eDynamicMesh *)entry;
            }
        }
    }
    if (resolved == 0) {
        return -1;
    }
    return resolved->GetNodeIndex(name, idx);
}
```

### Exemplar 2: `eDynamicGeom::GetSubObjectIndex(const cName &, int) const`

- Address: `0x001edb70`
- Size: `8` bytes
- Source: `src/eDynamicGeom.cpp`

```cpp
int eDynamicGeom::GetSubObjectIndex(const cName &, int) const {
    return -1;
}
```


## Prior Failure Notes

### Note 1

- session `6fb12182`; src `src/eSimulatedController.cpp`; snapshot `logs/failure_snapshots/match_20260505_232432/6fb12182/0006d12c__agent_self_reported_failure__src_eSimulatedController.cpp`

GetSubObjectIndex: 93/212 bytes differ. The first 12 instructions match (entry computation including `addu t0,a3,a0; lw t1,36(t0)`) using the IsSleeping-style asm hints (`__asm__("ori %0,$0,0")` for offset/idx initialization to defeat CSE that would hoist `entries+0x24`) and an inlined eSimulatedBodyEntry cast.

Three main sources of remaining divergence:

(1) Register allocation differences cascade through the inner cName comparison. Expected uses sub->t1, subLen->t0, argLen->t2; ours uses sub->t2, subLen->t1, argLen->t0. SNC chose to keep entry_p alive in t0 instead of overwriting it. Tried declaring `entry` as separate vs inlined, neither helped.

(2) Original emits `move t3,t0` in the delay slot of `beq t2,t3,equal_hash` to save subLen for later use as `addiu t1,t3,3`. Our compiler instead computes `addiu t0,t1,3` directly in the delay slot. This is a different scheduling decision.

(3) `bnel t1,t0,...` (0x51280003) appears at 0x3f4 in our output where expected has `bne t0,t2,...` (0x150a0005). Per docs/decisions/006-bnel-compiler-divergence.md this is a known SNC version divergence that cannot be fixed in source.

Approaches tried:
- if/else short-circuit in C (got OR-fused into `or t3,t1,t0; bnez t3`)
- Explicit goto for short-circuit (got separate bnez sequences, best result at 93 bytes)
- Goto-everywhere structure with labelled blocks (got worse at 104-127 bytes)
- Using struct member access for cache field vs explicit `*(char**)(entries+offset+0x24)` (struct version slightly better)
- Using helper variables `t0,t1,t2` named to match expected register flow (no improvement)

Permuter ran 180s, but its `find_function` picks the first 212B symbol (GetType) so its baseline of 155 is wrong. Permuter could not find improvements due to the size-collision bug (no --symbol flag exposed).

Root cause: this is a control-flow-heavy scalar function where SNC's register allocator and basic-block layout decisions differ from the original SNC version that built the binary, plus one bnel/bne divergence that's already documented as unfixable. Doesn't quite meet the unmatchable criteria (>8 byte diff in non-bnel/beql instructions) so left as failed.


## Disassembly

```asm
6d12c:	34020000 	li	v0,0x0
6d130:	8c870038 	lw	a3,56(a0)
6d134:	3c064000 	lui	a2,0x4000
6d138:	24c6ffff 	addiu	a2,a2,-1
6d13c:	34040000 	li	a0,0x0
6d140:	10e00003 	beqz	a3,0x6d150
6d144:	34080000 	li	t0,0x0
6d148:	8ce8fffc 	lw	t0,-4(a3)
6d14c:	01064024 	and	t0,t0,a2
6d150:	0048402a 	slt	t0,v0,t0
6d154:	11000028 	beqz	t0,0x6d1f8
6d158:	00e44021 	addu	t0,a3,a0
6d15c:	8d090024 	lw	t1,36(t0)
6d160:	8528001c 	lh	t0,28(t1)
6d164:	15000005 	bnez	t0,0x6d17c
6d168:	84aa0014 	lh	t2,20(a1)
6d16c:	15400003 	bnez	t2,0x6d17c
6d170:	00000000 	nop
6d174:	10000019 	b	0x6d1dc
6d178:	34080001 	li	t0,0x1
6d17c:	150a0005 	bne	t0,t2,0x6d194
6d180:	00000000 	nop
6d184:	952a001e 	lhu	t2,30(t1)
6d188:	94ab0016 	lhu	t3,22(a1)
6d18c:	114b0003 	beq	t2,t3,0x6d19c
6d190:	01005825 	move	t3,t0
6d194:	10000011 	b	0x6d1dc
6d198:	34080000 	li	t0,0x0
6d19c:	252a0008 	addiu	t2,t1,8
6d1a0:	25690003 	addiu	t1,t3,3
6d1a4:	00094883 	sra	t1,t1,0x2
6d1a8:	00094880 	sll	t1,t1,0x2
6d1ac:	01494821 	addu	t1,t2,t1
6d1b0:	11490009 	beq	t2,t1,0x6d1d8
6d1b4:	00a04025 	move	t0,a1
6d1b8:	8d4b0000 	lw	t3,0(t2)
6d1bc:	8d030000 	lw	v1,0(t0)
6d1c0:	11630003 	beq	t3,v1,0x6d1d0
6d1c4:	254a0004 	addiu	t2,t2,4
6d1c8:	10000004 	b	0x6d1dc
6d1cc:	34080000 	li	t0,0x0
6d1d0:	1549fff9 	bne	t2,t1,0x6d1b8
6d1d4:	25080004 	addiu	t0,t0,4
6d1d8:	34080001 	li	t0,0x1
6d1dc:	15000004 	bnez	t0,0x6d1f0
6d1e0:	00000000 	nop
6d1e4:	24420001 	addiu	v0,v0,1
6d1e8:	1000ffd5 	b	0x6d140
6d1ec:	24840030 	addiu	a0,a0,48
6d1f0:	03e00008 	jr	ra
6d1f4:	00000000 	nop
6d1f8:	03e00008 	jr	ra
6d1fc:	2402ffff 	li	v0,-1
```

## m2c Starting Point

```c
// Decompiled: eSimulatedController::GetSubObjectIndex(const cName &) const
// Address: 0x0006d12c, Size: 212B
// Obj: eAll_psp.obj

s32 eSimulatedController__GetSubObjectIndex_constcNamerefconst(void *arg0, s32 *arg1) {
    s16 temp_t0;
    s16 temp_t2;
    s32 *var_t0_3;
    s32 *var_t2;
    s32 var_a0;
    s32 var_t0;
    s32 var_t0_2;
    s32 var_v0;
    void *temp_a3;
    void *temp_t1;
    void *temp_t1_2;

    var_v0 = 0;
    temp_a3 = arg0->unk38;
    var_a0 = 0;
loop_1:
    var_t0 = 0;
    if (temp_a3 != NULL) {
        var_t0 = temp_a3->unk-4 & 0x3FFFFFFF;
    }
    if (var_v0 < var_t0) {
        temp_t1 = (temp_a3 + var_a0)->unk24;
        temp_t0 = temp_t1->unk1C;
        temp_t2 = arg1->unk14;
        if ((temp_t0 == 0) && (temp_t2 == 0)) {
            goto block_14;
        }
        if ((temp_t0 != temp_t2) || (temp_t1->unk1E != arg1->unk16)) {
            var_t0_2 = 0;
        } else {
            var_t2 = temp_t1 + 8;
            temp_t1_2 = var_t2 + (((s32) (temp_t0 + 3) >> 2) * 4);
            var_t0_3 = arg1;
            if (var_t2 != temp_t1_2) {
loop_11:
                var_t2 += 4;
                if (*var_t2 != *var_t0_3) {
                    var_t0_2 = 0;
                } else {
                    var_t0_3 += 4;
                    if (var_t2 == temp_t1_2) {
                        goto block_14;
                    }
                    goto loop_11;
                }
            } else {
block_14:
                var_t0_2 = 1;
            }
        }
        if (var_t0_2 == 0) {
            var_v0 += 1;
            var_a0 += 0x30;
            goto loop_1;
        }
        return var_v0;
    }
    return -1;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
