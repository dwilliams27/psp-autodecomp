# Battle Packet: `gcConfig::Reset(cMemPool *, bool)`

## Target

- Address: `0x000f00c8`
- Size: `136` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcConfig`
- Method family: `Reset`
- Leaf: `False`

## Queue Metadata

- score: `154`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 28B; correct function size; codegen-specific diagnosis; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `28`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`
- Near miss: `28`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x000f0150` `untried` 416B `gcConfig::CalcPresetSizes(void)`

## Placement

- Canonical source: `src/gcConfig.cpp`
- Header(s): `include/gcConfig.h`
- Allowed source(s): `src/gcConfig.cpp`
- Split-TU prefix: `src/gcConfig_*.cpp`

## Class Header: `include/gcConfig.h`

Not found.

## Matched Same-Class Neighbors

- `0x000efd00` 220B `gcConfig::Write(cFile &) const` — `src/gcConfig.cpp`
- `0x000efddc` 460B `gcConfig::Read(cFile &, cMemPool *)` — `src/gcConfig.cpp`
- `0x000effa8` 168B `gcConfig::gcConfig(cBase *)` — `src/gcConfig.cpp`
- `0x000f0050` 120B `gcConfig::~gcConfig(void)` — `src/gcConfig.cpp`
- `0x0024631c` 536B `gcConfig::AssignCopy(const cBase *)` — `src/gcConfig.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcRoomInstance::Reset(cMemPool *, bool)`

- Address: `0x0013c6e8`
- Size: `176` bytes
- Source: `src/gcRoomInstance.cpp`

```cpp
void gcRoomInstance::Reset(cMemPool *, bool) {
    int value = *(int *)((char *)this + 0x44);
    int valid;
    if (value == 0) {
        valid = 0;
    } else {
        HandleEntry *entry = D_00038890[value & 0xFFFF];
        HandleEntry *found = 0;
        if (entry != 0) {
            if (entry->handle == value) {
                found = entry;
            }
        }
        valid = found != 0;
    }

    if ((valid & 0xFF) != 0) {
        HandleEntry *entry = 0;
        if (value != 0) {
            entry = D_00038890[value & 0xFFFF];
        }
        int *nextRoom = (int *)((char *)this + 0x48);
        int next = *nextRoom;
        __asm__ volatile(
            "addiu $a3, %1, 0x00e8\n"
            "sw %0, 0($a3)"
            :
            : "r"(next), "r"(entry)
            : "a3", "memory");

        value = *(volatile int *)((char *)this + 0x44);
        entry = 0;
        if (value != 0) {
            entry = D_00038890[value & 0xFFFF];
        }
        entry->roomInstance = (int)this;
    }
}
```

### Exemplar 2: `gcExternalCinematic::Reset(cMemPool *, bool)`

- Address: `0x000eaee0`
- Size: `72` bytes
- Source: `src/gcExternalCinematic.cpp`

```cpp
// gcExternalCinematic::Reset(cMemPool *, bool) @ 0x000eaee0
// gcExternalCinematic::Write(cFile &) const @ 0x000eace0
// gcExternalCinematic::New(cMemPool *, cBase *) static @ 0x00243958
// cObject base, gcCinematic embedded member at +0x44.

class cBase;
class cMemPool;
class cFile;
class cType;

template <class T> T *dcast(const cBase *);

struct copy_word { int v; };
```

### Exemplar 3: `gcEntityControllerTemplate::Reset(cMemPool *, bool)`

- Address: `0x0010fed8`
- Size: `64` bytes
- Source: `src/gcEntityControllerTemplate.cpp`

```cpp
//   0x0010fed8 gcEntityControllerTemplate::Reset(cMemPool *, bool)
//   0x00110030 gcEntityControllerTemplate::FindAnimationSet(cHandleT<gcEnumeration>) const
//   0x001101a8 gcEntityControllerTemplate::FindAttackSet(cHandleT<gcEnumeration>) const
//   0x0025cbac gcEntityControllerTemplate::GetType(void) const
//   0x0025d4c8 gcEntityControllerTemplate::~gcEntityControllerTemplate(void)

class cBase;
class cFile;
class cMemPool;
class gcEnumeration;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
```

### Exemplar 4: `gcPartialBodyControllerTemplate::Reset(cMemPool *, bool)`

- Address: `0x0013fe38`
- Size: `64` bytes
- Source: `src/gcPartialBodyControllerTemplate_Reset.cpp`

```cpp
// gcPartialBodyControllerTemplate::Reset(cMemPool *, bool) @ 0x0013fe38
// Two embedded cBaseArray members at +0x14 and +0x20; both get Reset(pool).

class cBase;
class cMemPool;

class cBaseArray {
public:
    void Reset(cMemPool *);
};
```


## Prior Failure Notes

### Note 1

- session `1c8a5471`; src `src/gcConfig.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/1c8a5471/000f00c8__agent_self_reported_failure__src_gcConfig.cpp`

Implemented gcConfig::Reset in src/gcConfig.cpp with CalcPresetSizes(), the 0x40E7C preset-size load, global 1.0f store to 0x37D7E0, sign-aware +/-0.5f truncation, stack spills matching the visible sw to 0(sp)/4(sp), and final store to 0x99AF4. The half constant is declared before the call so SNC saves it in $f20, giving the correct 32-byte frame with f20 at 12(sp) and ra at 16(sp). All previously matched gcConfig functions still compare as MATCH.

Closest result is 28/136 bytes different. The residual differences are FPU scheduling/register allocation: generated code converts the preset size through $f12 and holds 1.0f in $f14, with mul.s/store ordering and positive add operand order differing from the target, which uses $f14 for the converted int, $f12 for 1.0f, stores 1.0f before mul.s, and emits add.s $f12,$f12,$f20. Tried variants with an explicit presetFloat temporary, memory barrier after the global float store, volatile/nonvolatile spill arrays, sched=1, and expression-order changes. tools/permuter.py ran for 300s (6096 generated, 3100 compiled); best non-saved candidate remained 28 bytes different.


## Disassembly

```asm
f00c8:	27bdffe0 	addiu	sp,sp,-32
f00cc:	3c053f00 	lui	a1,0x3f00
f00d0:	e7b4000c 	swc1	$f20,12(sp)
f00d4:	afbf0010 	sw	ra,16(sp)
f00d8:	0c03c054 	jal	0xf0150
f00dc:	4485a000 	mtc1	a1,$f20
f00e0:	3c040004 	lui	a0,0x4
f00e4:	8c840e7c 	lw	a0,3708(a0)
f00e8:	44847000 	mtc1	a0,$f14
f00ec:	3c053f80 	lui	a1,0x3f80
f00f0:	468073a0 	cvt.s.w	$f14,$f14
f00f4:	44856000 	mtc1	a1,$f12
f00f8:	3c050038 	lui	a1,0x38
f00fc:	e4acd7e0 	swc1	$f12,-10272(a1)
f0100:	460e6302 	mul.s	$f12,$f12,$f14
f0104:	44806800 	mtc1	zero,$f13
f0108:	460d603c 	c.lt.s	$f12,$f13
f010c:	00000000 	nop
f0110:	45000005 	bc1f	0xf0128
f0114:	afa40004 	sw	a0,4(sp)
f0118:	46146301 	sub.s	$f12,$f12,$f20
f011c:	4600630d 	trunc.w.s	$f12,$f12
f0120:	10000004 	b	0xf0134
f0124:	44046000 	mfc1	a0,$f12
f0128:	46146300 	add.s	$f12,$f12,$f20
f012c:	4600630d 	trunc.w.s	$f12,$f12
f0130:	44046000 	mfc1	a0,$f12
f0134:	afa40000 	sw	a0,0(sp)
f0138:	3c05000a 	lui	a1,0xa
f013c:	aca49af4 	sw	a0,-25868(a1)
f0140:	c7b4000c 	lwc1	$f20,12(sp)
f0144:	8fbf0010 	lw	ra,16(sp)
f0148:	03e00008 	jr	ra
f014c:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcConfig::Reset(cMemPool *, bool)
// Address: 0x000f00c8, Size: 136B
// Obj: gcAll_psp.obj

? gcConfig__CalcPresetSizes_void(?);                /* extern */

void gcConfig__Reset_cMemPoolptr_bool(void) {
    f32 temp_f12;
    s32 var_f12;

    gcConfig__CalcPresetSizes_void(0x3F000000);
    *(f32 *)0x37D7E0 = 1.0f;
    temp_f12 = 1.0f * (f32) *(s32 *)0x40E7C;
    if (temp_f12 < 0.0f) {
        var_f12 = (s32) (temp_f12 - 0.5f);
    } else {
        var_f12 = (s32) (temp_f12 + 0.5f);
    }
    *(s32 *)0x99AF4 = var_f12;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
