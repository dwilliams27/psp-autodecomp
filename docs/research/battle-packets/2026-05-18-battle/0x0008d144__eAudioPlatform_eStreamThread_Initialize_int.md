# Battle Packet: `eAudioPlatform::eStreamThread::Initialize(int)`

## Target

- Address: `0x0008d144`
- Size: `140` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `eAudioPlatform::eStreamThread`
- Method family: `Initialize`
- Leaf: `False`

## Queue Metadata

- score: `143`
- priority: `1`
- reason: `has failure_notes; has failure snapshot; near miss 3B; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `3`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `3`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00004b38` `failed` 116B `cStr::cStr(const char *, ...)`
- `0x0036bb5c` `untried` 0B `sceKernelStartThread`
- `0x0036bbac` `untried` 0B `sceKernelCreateThread`

Callers:
- `0x0008c870` `untried` 440B `eAudioPlatform::Initialize(void) static`

## Placement

- Canonical source: `src/eAudioPlatform_eStreamThread.cpp`
- Header(s): `include/eAudioPlatform_eStreamThread.h`
- Allowed source(s): `src/eAudioPlatform_eStreamThread.cpp`
- Split-TU prefix: `src/eAudioPlatform_eStreamThread_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the confirmed sched=1 zone (0x06e000-0x0bab28). The Makefile should already apply -Xsched=1 for known classes. If bytes don't match and you suspect a missing sched override, report failure — a human will update the Makefile.


## Class Header: `include/eAudioPlatform::eStreamThread.h`

Not found.

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

- session `f5d1bf53`; src `src/eAudioPlatform_eStreamThread.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/f5d1bf53/0008d144__agent_self_reported_failure__src_eAudioPlatform_eStreamThread.cpp`

Down to 3/140 bytes diff after permuter run (150s/2887 candidates). Remaining diff is pure regalloc: at 0x34 original has 'move a0,v0' (uses cStr ctor's return-this in v0), mine has 'move a0,sp' (SNC's optimizer folded &name==sp); at 0x50 original 'sw v0,0(s0)' vs mine 'sw a0,0(s0)' from same regalloc choice cascading. Tried: memory barriers between cStr ctor and sceKernelCreateThread, chained assignment *(int*)this=sceKernelCreateThread(...) (worsened to 14), extern-C cStr_ctor declaration with mangled name (worsened to 34). Confirmed sched=1 is correct (sched=2 worsens to 22). Diff exceeds ≤8 byte threshold for unmatchable classification but is fundamentally a SNC optimizer choice not controllable from C source.


## Disassembly

```asm
8d144:	27bdfef0 	addiu	sp,sp,-272
8d148:	afb00104 	sw	s0,260(sp)
8d14c:	afbf0108 	sw	ra,264(sp)
8d150:	00808025 	move	s0,a0
8d154:	24a60001 	addiu	a2,a1,1
8d158:	ae060008 	sw	a2,8(s0)
8d15c:	3c050037 	lui	a1,0x37
8d160:	24a5d158 	addiu	a1,a1,-11944
8d164:	0c0012ce 	jal	0x4b38
8d168:	03a02025 	move	a0,sp
8d16c:	3c050009 	lui	a1,0x9
8d170:	24a5d1d0 	addiu	a1,a1,-11824
8d174:	34078000 	li	a3,0x8000
8d178:	00402025 	move	a0,v0
8d17c:	34060010 	li	a2,0x10
8d180:	00004025 	move	t0,zero
8d184:	0c0daeeb 	jal	0x36bbac
8d188:	00004825 	move	t1,zero
8d18c:	00402025 	move	a0,v0
8d190:	0480000a 	bltz	a0,0x8d1bc
8d194:	ae020000 	sw	v0,0(s0)
8d198:	afb00100 	sw	s0,256(sp)
8d19c:	27a60100 	addiu	a2,sp,256
8d1a0:	0c0daed7 	jal	0x36bb5c
8d1a4:	34050004 	li	a1,0x4
8d1a8:	34020001 	li	v0,0x1
8d1ac:	8fb00104 	lw	s0,260(sp)
8d1b0:	8fbf0108 	lw	ra,264(sp)
8d1b4:	03e00008 	jr	ra
8d1b8:	27bd0110 	addiu	sp,sp,272
8d1bc:	00001025 	move	v0,zero
8d1c0:	8fb00104 	lw	s0,260(sp)
8d1c4:	8fbf0108 	lw	ra,264(sp)
8d1c8:	03e00008 	jr	ra
8d1cc:	27bd0110 	addiu	sp,sp,272
```

## m2c Starting Point

```c
// Decompiled: eAudioPlatform::eStreamThread::Initialize(int)
// Address: 0x0008d144, Size: 140B
// Obj: eAll_psp.obj

s32 cStr__cStr_constcharptr_...(void *, ?, s32);    /* extern */
s32 sceKernelCreateThread(s32, ?, ?, ?);            /* extern */
? sceKernelStartThread(s32, ?, void **);            /* extern */

s32 eAudioPlatform__eStreamThread__Initialize_int(void *arg0, s32 arg1) {
    void *sp100;
    s32 temp_a2;
    s32 temp_v0;

    temp_a2 = arg1 + 1;
    arg0->unk8 = temp_a2;
    temp_v0 = sceKernelCreateThread(cStr__cStr_constcharptr_...(sp, 0x36D158, temp_a2), 0x8D1D0, 0x10, 0x8000);
    arg0->unk0 = temp_v0;
    if (temp_v0 >= 0) {
        sp100 = arg0;
        sceKernelStartThread(temp_v0, 4, &sp100);
        return 1;
    }
    return 0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
