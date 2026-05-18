# Battle Packet: `gcTableTemplate::Reset(cMemPool *, bool)`

## Target

- Address: `0x0012bc18`
- Size: `216` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcTableTemplate`
- Method family: `Reset`
- Leaf: `False`

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

Callees:
- `0x00006f60` `failed` 348B `cBaseArray::SetSize(int)`
- `0x00238fcc` `untried` 200B `gcEnumeration::GetSize(void) const`

## Placement

- Canonical source: `src/gcTableTemplate.cpp`
- Header(s): `include/gcTableTemplate.h`
- Allowed source(s): `src/gcTableTemplate.cpp`
- Split-TU prefix: `src/gcTableTemplate_*.cpp`

## Class Header: `include/gcTableTemplate.h`

Not found.

## Matched Same-Class Neighbors

- `0x0012b98c` 276B `gcTableTemplate::Read(cFile &, cMemPool *)` — `src/gcTableTemplate.cpp`
- `0x0012baa0` 72B `gcTableTemplate::gcTableTemplate(cBase *)` — `src/gcTableTemplate.cpp`
- `0x0012bcf0` 36B `gcTableTemplate::GetNumRows(void) const` — `src/gcTableTemplate.cpp`
- `0x00273aa0` 112B `gcTableTemplate::AssignCopy(const cBase *)` — `src/gcTableTemplate.cpp`
- `0x00273b10` 124B `gcTableTemplate::New(cMemPool *, cBase *) static` — `src/gcTableTemplate.cpp`

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

- session `e3dede71`

Reset remains at the prior best 8/216 byte diff. The only remaining mismatch is still the first handle lookup register allocation: expected keeps the validated enum result in a1 and the raw D_00038890 load in a2, while SNC continues to emit the swap for the C source shape that preserves the surrounding schedule. I tried register-variable hints, initializing enum_ptr before the branch, changing declaration order and pointer types, splitting the slot load, inverting the nested validity checks, using the loaded candidate as the result variable, and adding/removing empty asm constraints for the unused parameters. Variants that got the a1/a2 roles right changed the top branch shape to beqz or added extra control-flow instructions, producing larger mismatches; the best scheduled version stayed at the same 8-byte register-field diff.

Permuter was run for 300 seconds against 0x0012bc18. It generated 6960 candidates, compiled 2876, and found zero improvements: best stayed 8 bytes different from the 8-byte baseline. This is not classifiable as unmatchable_bnel because the differing instructions are register allocation in li/lw/beqz/lw/move/beqzl, not bnel/beql versus bne/beq+nop.

### Note 2

- session `36cb8d44`

Achieved 8/216 byte diff after extensive iteration. Remaining diff is pure register allocation in the first handle-lookup block: expected uses a1 for enum_ptr (validity-checked result) and a2 for e (the raw load from D_00038890); my compilation produces the swap (a2 for enum_ptr, a1 for e). The 7-8 differing bytes are all register-field bits in li/lw/beqz/lw 0x30/move/beqzl instructions in that block; the rest of the function (second lookup, GetSize call, sz/data tail block) matches exactly. CRITICAL FIX during iteration: swapping the declaration order of `int sz = 0;` and `int *data = ...;` so that sz is declared FIRST produced a 38-byte improvement (46 → 8 bytes) — SNC then allocates sz to a0 and data to s1 instead of the reverse with an extra `move a0, s1`. Attempts to fix the remaining 8 bytes (variable declaration order swaps, nested if vs short-circuit &&, ternary forms, single-variable forms, gcEnumeration* typing, scope changes) did not change SNC's register choice in the first lookup. Permuter could not run due to a multi-function .o (564B .text vs 216B target) and permuter.py not exposing a target_symbol CLI argument — reported as a tooling limitation, not bypassed. NOT classifiable as unmatchable_bnel: the differing instructions are not bnel/beql variants (criterion 1 of docs/decisions/006 not met). Marking failed for retry per project norms — register-allocation diffs <10 bytes are source/flag problems, not compiler-version divergence.


## Disassembly

```asm
12bc18:	27bdfff0 	addiu	sp,sp,-16
12bc1c:	afb00000 	sw	s0,0(sp)
12bc20:	00808025 	move	s0,a0
12bc24:	8e040044 	lw	a0,68(s0)
12bc28:	afb10004 	sw	s1,4(sp)
12bc2c:	afbf0008 	sw	ra,8(sp)
12bc30:	14800003 	bnez	a0,0x12bc40
12bc34:	26110048 	addiu	s1,s0,72
12bc38:	1000000d 	b	0x12bc70
12bc3c:	34050000 	li	a1,0x0
12bc40:	3085ffff 	andi	a1,a0,0xffff
12bc44:	3c060004 	lui	a2,0x4
12bc48:	00052880 	sll	a1,a1,0x2
12bc4c:	24c68890 	addiu	a2,a2,-30576
12bc50:	00a62821 	addu	a1,a1,a2
12bc54:	8ca60000 	lw	a2,0(a1)
12bc58:	10c00005 	beqz	a2,0x12bc70
12bc5c:	34050000 	li	a1,0x0
12bc60:	8cc70030 	lw	a3,48(a2)
12bc64:	14e40002 	bne	a3,a0,0x12bc70
12bc68:	00000000 	nop
12bc6c:	00c02825 	move	a1,a2
12bc70:	50a00010 	beqzl	a1,0x12bcb4
12bc74:	02202025 	move	a0,s1
12bc78:	10800007 	beqz	a0,0x12bc98
12bc7c:	34050000 	li	a1,0x0
12bc80:	3084ffff 	andi	a0,a0,0xffff
12bc84:	3c050004 	lui	a1,0x4
12bc88:	00042080 	sll	a0,a0,0x2
12bc8c:	24a58890 	addiu	a1,a1,-30576
12bc90:	00852021 	addu	a0,a0,a1
12bc94:	8c850000 	lw	a1,0(a0)
12bc98:	0c08e3f3 	jal	0x238fcc
12bc9c:	00a02025 	move	a0,a1
12bca0:	02202025 	move	a0,s1
12bca4:	0c001bd8 	jal	0x6f60
12bca8:	00402825 	move	a1,v0
12bcac:	10000004 	b	0x12bcc0
12bcb0:	8e110050 	lw	s1,80(s0)
12bcb4:	0c001bd8 	jal	0x6f60
12bcb8:	00002825 	move	a1,zero
12bcbc:	8e110050 	lw	s1,80(s0)
12bcc0:	12200005 	beqz	s1,0x12bcd8
12bcc4:	34040000 	li	a0,0x0
12bcc8:	8e24fffc 	lw	a0,-4(s1)
12bccc:	3c054000 	lui	a1,0x4000
12bcd0:	24a5ffff 	addiu	a1,a1,-1
12bcd4:	00852024 	and	a0,a0,a1
12bcd8:	ae040054 	sw	a0,84(s0)
12bcdc:	8fb00000 	lw	s0,0(sp)
12bce0:	8fb10004 	lw	s1,4(sp)
12bce4:	8fbf0008 	lw	ra,8(sp)
12bce8:	03e00008 	jr	ra
12bcec:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcTableTemplate::Reset(cMemPool *, bool)
// Address: 0x0012bc18, Size: 216B
// Obj: gcAll_psp.obj

? cBaseArray__SetSize_int(void *, s32, void *, s32); /* extern */
s32 gcEnumeration__GetSize_voidconst(s32, s32, void *, s32); /* extern */

void gcTableTemplate__Reset_cMemPoolptr_bool(void *arg0, void *arg2, s32 arg3) {
    s32 temp_a0;
    s32 var_a0;
    s32 var_a1_2;
    s32 var_a3;
    void *temp_s1;
    void *temp_s1_2;
    void *var_a1;
    void *var_a2;

    var_a2 = arg2;
    var_a3 = arg3;
    temp_a0 = arg0->unk44;
    temp_s1 = arg0 + 0x48;
    if (temp_a0 == 0) {
        var_a1 = NULL;
    } else {
        var_a2 = ((temp_a0 & 0xFFFF) * 4)->unk38890;
        var_a1 = NULL;
        if (var_a2 != NULL) {
            var_a3 = var_a2->unk30;
            if (var_a3 == temp_a0) {
                var_a1 = var_a2;
            }
        }
    }
    if (var_a1 == NULL) {
        cBaseArray__SetSize_int(temp_s1, 0, var_a2, var_a3);
    } else {
        var_a1_2 = 0;
        if (temp_a0 != 0) {
            var_a1_2 = ((temp_a0 & 0xFFFF) * 4)->unk38890;
        }
        cBaseArray__SetSize_int(temp_s1, gcEnumeration__GetSize_voidconst(var_a1_2, var_a1_2, var_a2, var_a3));
    }
    temp_s1_2 = arg0->unk50;
    var_a0 = 0;
    if (temp_s1_2 != NULL) {
        var_a0 = temp_s1_2->unk-4 & 0x3FFFFFFF;
    }
    arg0->unk54 = var_a0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
