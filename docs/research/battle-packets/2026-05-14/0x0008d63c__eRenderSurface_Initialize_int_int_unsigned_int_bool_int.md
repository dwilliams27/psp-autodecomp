# Battle Packet: `eRenderSurface::Initialize(int, int, unsigned int, bool, int)`

## Target

- Address: `0x0008d63c`
- Size: `272` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eRenderSurface`
- Method family: `Initialize`
- Leaf: `False`

## Queue Metadata

- score: `198`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 4B; correct function size; codegen-specific diagnosis; manageable size`
- failure_action: `quarantine`
- near_miss_bytes: `4`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `needs_layout`
- Near miss: `4`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x0008d74c` `matched` 156B `eRenderSurface::Uninitialize(void)`
- `0x0008df10` `untried` 540B `eVRAMMgr::AllocatePages(int) static`

Callers:
- `0x0008d7e8` `untried` 316B `eVRAMMgr::Initialize(int, int, int, int) static`
- `0x0009a444` `untried` 816B `eRenderTarget::CreateData(void)`

## Placement

- Canonical source: `src/eRenderSurface.cpp`
- Header(s): `include/eRenderSurface.h`
- Allowed source(s): `src/eRenderSurface.cpp`
- Split-TU prefix: `src/eRenderSurface_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.


## Class Header: `include/eRenderSurface.h`

```cpp
#ifndef ERENDERSURFACE_H
#define ERENDERSURFACE_H

class eRenderSurface {
public:
    unsigned int    field_0;
    int             mVRAMPage;
    int             mSizeBytes;
    short           mWidth;
    short           mHeight;
    unsigned short  mBytesPerPixel;
    short           mPixelFormat;
    eRenderSurface *mPrev;
    eRenderSurface *mNext;

    eRenderSurface(void);
    ~eRenderSurface(void);
    void Initialize(int, int, unsigned int, bool, int);
    void Uninitialize(void);
};

#endif
```

## Matched Same-Class Neighbors

- `0x0008d580` 52B `eRenderSurface::eRenderSurface(void)` — `src/eRenderSurface.cpp`
- `0x0008d5b4` 136B `eRenderSurface::~eRenderSurface(void)` — `src/eRenderSurface.cpp`
- `0x0008d74c` 156B `eRenderSurface::Uninitialize(void)` — `src/eRenderSurface.cpp`

## Matched Method Exemplars

### Exemplar 1: `eBodyWorldConstraint::Initialize(ePhysicsConstraintConfig *, eSimulatedController *)`

- Address: `0x0006b518`
- Size: `152` bytes
- Source: `src/eBodyWorldConstraint.cpp`

```cpp
//   eBodyWorldConstraint::Initialize(ePhysicsConstraintConfig *, eSimulatedController *) @ 0x0006b518 (eAll_psp.obj)
//   eBodyWorldConstraint::AssignCopy(const cBase *)      @ 0x0020992c  (eAll_psp.obj)
//   eBodyWorldConstraint::~eBodyWorldConstraint(void)    @ 0x0006b49c  (eAll_psp.obj)
//   eBodyWorldConstraint::OnPositionChanged(void)        @ 0x0006b5b0  (eAll_psp.obj)
//   eBodyWorldConstraint::New(cMemPool *, cBase *)       @ 0x0020998c  (eAll_psp.obj)

typedef int v4sf_t __attribute__((mode(V4SF)));

class cBase;
class cFile;
class cMemPool;
class cType;
class ePhysicsConstraintConfig;
class eSimulatedController;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};
```

### Exemplar 2: `eMoviePlatform::Initialize(void) static`

- Address: `0x000136c8`
- Size: `100` bytes
- Source: `src/eMoviePlatform.cpp`

```cpp
bool eMoviePlatform::Initialize(void) {
    int module = sceUtilityLoadModule(0x303);
    *(int *)0x37D060 = module;
    if (module < 0) {
        return false;
    }

    int sema = sceKernelCreateSema((const char *)0x36CFEC, 0, 1, 1, 0);
    *(int *)0x37D05C = sema;
    if (sema < 0) {
        return false;
    }

    return true;
}
```

### Exemplar 3: `eInput::Initialize(void) static`

- Address: `0x00030d14`
- Size: `96` bytes
- Source: `src/eInput.cpp`

```cpp
// ── eInput::Initialize(void) static @ 0x00030d14 ──
bool eInput::Initialize() {
    if (eInputJoystick::Initialize() &&
        eInputMouse::Initialize() &&
        eInputKeyboard::Initialize()) {
        Reset();
        return true;
    }
    return false;
}
```

### Exemplar 4: `eInputJoystick::Initialize(void) static`

- Address: `0x0003a718`
- Size: `80` bytes
- Source: `src/eInputJoystick.cpp`

```cpp
bool eInputJoystick::Initialize() {
    if (!PlatformInitialize()) {
        return false;
    }
    *(bool *)0x37D1ED = (cGetCurrentPlatform() == 8);
    return true;
}
```


## Prior Failure Notes

### Note 1

- session `a8b4314b`; src `src/eRenderSurface.cpp`; snapshot `logs/failure_snapshots/match_20260430_201648/a8b4314b/0008d63c__agent_self_reported_failure__src_eRenderSurface.cpp`

Restored the closest known source shape: local sched=1 for Initialize plus a memory barrier between assigning mNext and touching mPrev->mNext. This compiles to the correct 272-byte function and compare_func reports only 4/272 bytes differing. Direct objdump comparison shows the sole remaining difference is the prologue argument-save order: expected has move s1,a1 before move s0,a0, while this SNC build emits move s0,a0 before move s1,a1; all later instructions match after relocation masking.

Tried additional source restructurings targeted at the prologue: register parameter annotations, explicit width/self locals, split local assignment, explicit register variables, explicit this-> member access, qualified Uninitialize call syntax, pre-call and post-call empty asm register constraints, and sched=0/3 probes. The pre-call register constraint can force the desired move order, but it prevents the original jal delay-slot fill with andi s4,t0,0xff and regresses the function by shifting the body. Local/self variants changed saved-register allocation and regressed to 20-41 byte diffs; sched=0 regressed heavily, and sched=3 is rejected by SNC. A short -Xmopt=0 permuter probe could not locate the target in this multi-function source file; prior session 29e039dd already ran the sched=1 permuter for 301s with zero improvement from the same 4-byte baseline. This is not a bnel issue.

### Note 2

- session `29e039dd`

Improved eRenderSurface::Initialize from the prior 31/272 byte sched=2 mismatch to a 4/272 byte mismatch by keeping the method under #pragma control sched=1 and adding a memory barrier between assigning mNext and touching mPrev->mNext. The barrier fixed the linked-list insertion ordering so the tail block now matches; all remaining bytes are the prologue argument-save move order only: expected has move s1,a1 before move s0,a0, while SNC still emits move s0,a0 before move s1,a1. Tried an explicit self local, a width precompute before Uninitialize, and a register-pinned width local; all regressed by changing saved-register allocation or function size. Ran tools/permuter.py for 301s with --sched 1 from the 4-byte baseline: 8352 candidates generated, 5396 compiled, zero improvements. This is not a bnel issue; it is a tiny prologue scheduling/register-save ordering divergence.

### Note 3

- session `092de455`

eRenderSurface::Initialize(int, int, unsigned int, bool, int) — 31/272 bytes differ under Makefile default (sched=2). Manual -Xsched=1 test brings it down to 20/272 bytes diff (better but not matching).

Extensive source rewrites applied that got us from the initial 166/276 to 31/272 at sched=2:
  1. Initial version used direct expression 'mSizeBytes = ...*height*bpp;' — produced wrong-size (276B) function matching a different candidate.
  2. Added __asm__ volatile memory barriers to split the computation into discrete stages (pixfmt store before size calc, size calc before VRAM alloc logic).
  3. Used __asm__ volatile("":"+r"(prod)::) to anchor the intermediate product in a register, preventing the scheduler from moving the second mult across the first mflo.
  4. Inverted the if/else for the linked-list insert path (if(head!=0){...}else{...}) so SNC emits the correct beqzl variant with 'sw s0,gRenderSurfaceHead' in the delay slot.
  5. These changes yielded correct 272B function size and a proper match to the true eRenderSurface::Initialize symbol.

Remaining 31-byte diff at sched=2 has THREE clusters:
  (a) Prolog: sw s4/sw ra relocated after the callee-save moves (8 bytes). Same class of scheduler divergence as the destructor — not source-controllable.
  (b) Near the end: a 4-byte phase shift; our compile is 272B (matches expected size) but the overall layout of the 'head==NULL' tail block and the epilogue is shifted by one instruction. Unclear if this is tied to (a).
  (c) A couple of register choices (a2 vs a0 in the width rounding chain) — under sched=1 this cluster is resolved.

SCHED=1 REQUIRED (and further work): Even with sched=1 the function still has 20 bytes of divergence: the 'move s1,a1' vs 'move s0,a0' prolog order (8 bytes) and some residual reordering inside the linked-list branch. A retry agent with sched=1 active in the Makefile should focus on: (i) whether reordering the argument name order or the in-body first-use order affects the s0 vs s1 move allocation, (ii) whether splitting the linked-list insert into separate helper statements changes the beqzl delay-slot target. See docs/research/snc-delay-slots.md and snc-fpu-scheduling.md for background.

Current source is preserved with the barrier-based restructuring so the sched=1 build will get the 20-byte-diff state rather than regressing.


## Disassembly

```asm
8d63c:	27bdffe0 	addiu	sp,sp,-32
8d640:	afb00000 	sw	s0,0(sp)
8d644:	afb10004 	sw	s1,4(sp)
8d648:	afb20008 	sw	s2,8(sp)
8d64c:	afb3000c 	sw	s3,12(sp)
8d650:	afb40010 	sw	s4,16(sp)
8d654:	afb50014 	sw	s5,20(sp)
8d658:	afbf0018 	sw	ra,24(sp)
8d65c:	01209825 	move	s3,t1
8d660:	00e0a825 	move	s5,a3
8d664:	00c09025 	move	s2,a2
8d668:	00a08825 	move	s1,a1
8d66c:	00808025 	move	s0,a0
8d670:	0c0235d3 	jal	0x8d74c
8d674:	311400ff 	andi	s4,t0,0xff
8d678:	ae150000 	sw	s5,0(s0)
8d67c:	ae000008 	sw	zero,8(s0)
8d680:	a611000c 	sh	s1,12(s0)
8d684:	a612000e 	sh	s2,14(s0)
8d688:	34040002 	li	a0,0x2
8d68c:	3c050038 	lui	a1,0x38
8d690:	56800001 	bnezl	s4,0x8d698
8d694:	34040004 	li	a0,0x4
8d698:	a6040010 	sh	a0,16(s0)
8d69c:	34040001 	li	a0,0x1
8d6a0:	56800001 	bnezl	s4,0x8d6a8
8d6a4:	34040003 	li	a0,0x3
8d6a8:	a6040012 	sh	a0,18(s0)
8d6ac:	2624003f 	addiu	a0,s1,63
8d6b0:	00042182 	srl	a0,a0,0x6
8d6b4:	00042180 	sll	a0,a0,0x6
8d6b8:	00920018 	mult	a0,s2
8d6bc:	00002012 	mflo	a0
8d6c0:	96060010 	lhu	a2,16(s0)
8d6c4:	00000000 	nop
8d6c8:	00860018 	mult	a0,a2
8d6cc:	00002012 	mflo	a0
8d6d0:	ae040008 	sw	a0,8(s0)
8d6d4:	1e600005 	bgtz	s3,0x8d6ec
8d6d8:	02608825 	move	s1,s3
8d6dc:	0c0237c4 	jal	0x8df10
8d6e0:	8e040008 	lw	a0,8(s0)
8d6e4:	00408825 	move	s1,v0
8d6e8:	3c050038 	lui	a1,0x38
8d6ec:	ae110004 	sw	s1,4(s0)
8d6f0:	8cb1d338 	lw	s1,-11464(a1)
8d6f4:	5220000a 	beqzl	s1,0x8d720
8d6f8:	acb0d338 	sw	s0,-11464(a1)
8d6fc:	8e240014 	lw	a0,20(s1)
8d700:	ae040014 	sw	a0,20(s0)
8d704:	8ca4d338 	lw	a0,-11464(a1)
8d708:	ae040018 	sw	a0,24(s0)
8d70c:	8e040014 	lw	a0,20(s0)
8d710:	ac900018 	sw	s0,24(a0)
8d714:	8e040018 	lw	a0,24(s0)
8d718:	10000003 	b	0x8d728
8d71c:	ac900014 	sw	s0,20(a0)
8d720:	ae100018 	sw	s0,24(s0)
8d724:	ae100014 	sw	s0,20(s0)
8d728:	8fb00000 	lw	s0,0(sp)
8d72c:	8fb10004 	lw	s1,4(sp)
8d730:	8fb20008 	lw	s2,8(sp)
8d734:	8fb3000c 	lw	s3,12(sp)
8d738:	8fb40010 	lw	s4,16(sp)
8d73c:	8fb50014 	lw	s5,20(sp)
8d740:	8fbf0018 	lw	ra,24(sp)
8d744:	03e00008 	jr	ra
8d748:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: eRenderSurface::Initialize(int, int, unsigned int, bool, int)
// Address: 0x0008d63c, Size: 272B
// Obj: eAll_psp.obj

? eRenderSurface__Uninitialize_void();              /* extern */
s32 eVRAMMgr__AllocatePages_intstatic(s32, ?, u16); /* extern */

void eRenderSurface__Initialize_int_int_unsignedint_bool_int(void *arg0, s16 arg1, s16 arg2, s32 arg3) {
    s16 var_a0_2;
    s32 temp_s3;
    s32 temp_s4;
    s32 var_s1;
    u16 temp_a2;
    u16 var_a0;
    void *temp_s1;

    temp_s3 = M2C_ERROR(/* Read from unset register $t1 */);
    temp_s4 = M2C_ERROR(/* Read from unset register $t0 */) & 0xFF;
    eRenderSurface__Uninitialize_void();
    arg0->unk0 = arg3;
    arg0->unk8 = 0;
    arg0->unkC = arg1;
    arg0->unkE = arg2;
    var_a0 = 2;
    if (temp_s4 != 0) {
        var_a0 = 4;
    }
    arg0->unk10 = var_a0;
    var_a0_2 = 1;
    if (temp_s4 != 0) {
        var_a0_2 = 3;
    }
    arg0->unk12 = var_a0_2;
    temp_a2 = arg0->unk10;
    arg0->unk8 = (s32) ((((u32) (arg1 + 0x3F) >> 6) << 6) * arg2 * temp_a2);
    var_s1 = temp_s3;
    if (temp_s3 <= 0) {
        var_s1 = eVRAMMgr__AllocatePages_intstatic(arg0->unk8, 0x380000, temp_a2);
    }
    arg0->unk4 = var_s1;
    temp_s1 = *(void **)0x37D338;
    if (temp_s1 == NULL) {
        *(void **)0x37D338 = arg0;
        arg0->unk18 = arg0;
        arg0->unk14 = arg0;
        return;
    }
    arg0->unk14 = (void *) temp_s1->unk14;
    arg0->unk18 = (void *) *(void **)0x37D338;
    arg0->unk14->unk18 = arg0;
    arg0->unk18->unk14 = arg0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
