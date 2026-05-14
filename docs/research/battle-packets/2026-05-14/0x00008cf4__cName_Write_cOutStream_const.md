# Battle Packet: `cName::Write(cOutStream &) const`

## Target

- Address: `0x00008cf4`
- Size: `152` bytes
- Status: `failed`
- Object: `cAll_psp.obj`
- Class: `cName`
- Method family: `Write`
- Leaf: `False`

## Queue Metadata

- score: `175`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 10B; codegen-specific diagnosis; Write retry family; manageable size`
- failure_action: `research`
- near_miss_bytes: `10`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `constant_cse_codegen`, `needs_layout`
- Near miss: `10`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x000012d0` `untried` 68B `cStrLength(const char *)`
- `0x0000616c` `failed` 372B `cOutStream::Write(int, int, bool)`
- `0x000062e0` `failed` 372B `cOutStream::Write(unsigned int, int, bool)`

Callers:
- `0x0013661c` `matched` 252B `gcMsgPlayMovie::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const`

## Placement

- Canonical source: `src/cName.cpp`
- Header(s): `include/cName.h`
- Allowed source(s): `src/cName.cpp`
- Split-TU prefix: `src/cName_*.cpp`

## Compiler Guidance

Write template: preserve cWriteBlock/cOutStream construction, field write order, and End() placement. SNC is sensitive to temporary bool/cast shapes and to independent store ordering.

## Class Header: `include/cName.h`

Not found.

## Matched Same-Class Neighbors

- `0x00008afc` 284B `cName::Set(const char *, ...)` — `src/cName.cpp`
- `0x00008c18` 96B `cName::Write(cWriteBlock &) const` — `src/cName.cpp`
- `0x00008c78` 124B `cName::Read(cReadBlock &)` — `src/cName.cpp`
- `0x00008d8c` 216B `cName::Read(cInStream &)` — `src/cName.cpp`

## Matched Method Exemplars

### Exemplar 1: `cBaseArray::Write(cWriteBlock &) const`

- Address: `0x00007244`
- Size: `148` bytes
- Source: `src/cBaseArray.cpp`

```cpp
void cBaseArray::Write(cWriteBlock &wb) const {
    int count = 0;
    if (mData != 0) {
        count = mData[-1];
    }
    wb.Write(count);

    int index = 0;
    int *tmp = mData;
    __asm__ volatile("" : "+r"(tmp));
    int offset = 0;
loop:
    {
        count = 0;
        if (mData != 0) {
            count = mData[-1];
        }
        if (index < count) {
            wb.WriteBase(*(cBase **)((char *)mData + offset));
            index += 1;
            offset += 4;
            goto loop;
        }
    }
}
```

### Exemplar 2: `cGroup::Write(cFile &) const`

- Address: `0x0000c18c`
- Size: `160` bytes
- Source: `src/cGroup.cpp`

```cpp
void cGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    cGroupNode *node = mList;
    if (node != 0) {
        do {
            int write = node->flags & 4;
            write = write != 0;
            write = (unsigned char)write;
            if (write != 0) {
                wb.Write(true);
                node->handle.Write(wb);
            }
            cGroupNode *nextNode = 0;
            cGroupNode *next = node->next;
            if (next != mList) {
                nextNode = next;
            }
            node = nextNode;
        } while (node != 0);
    }
    wb.Write(false);
    wb.End();
}
```

### Exemplar 3: `cWriteBlock::Write(int, const short *)`

- Address: `0x00005a1c`
- Size: `172` bytes
- Source: `src/cWriteBlock.cpp`

```cpp
void cWriteBlock::Write(bool data) {
    unsigned char tmp = (unsigned int)data;
    cFileSystem::Write(mFile->mHandle, &tmp, 1);
}
```

### Exemplar 4: `cHandle::Write(cOutStream &) const`

- Address: `0x000092f0`
- Size: `128` bytes
- Source: `src/cHandle.cpp`

```cpp
//   cHandle::Write(cOutStream &) const  @ 0x000092f0  128B
//   cHandle::Read(cInStream &)          @ 0x00009370  124B

class cInStream;
class cOutStream;
class cWriteBlock;

class cObjectKey {
public:
    unsigned int mKey;
    void Read(cInStream &);
    void Write(cOutStream &) const;
};
```


## Prior Failure Notes

### Note 1

- session `ecc06b50`; src `src/cName.cpp`; snapshot `logs/failure_snapshots/match_20260503_231848/ecc06b50/00008cf4__agent_self_reported_failure__src_cName.cpp`

Same 8-byte scheduler divergence the prior agent (8788b6b2) hit. Expected at 0x8d18-0x8d2c is `andi s3,v0,0xffff; andi a1,s3,0xffff; li a3,0x1; move a0,s0; jal; li a2,0x10`. Current is `andi s3; li a3,0x1; andi a1,s3,0xffff; move a0,s0; jal; li a2,0x10` — the second andi and the `li a3,0x1` are swapped. Both orderings are functionally identical (4 independent setup instructions before a call); SNC's list-scheduler picks one based on internal heuristics not influenced by source.

New approaches tried this session beyond prior agent's 7 (cast/promotion/double-mask/block bool/literal-true/separate-pointer): (A) `__asm__ volatile("" : "+r"(arg))` to anchor the truncated arg value — moved second andi to right place but pushed `li a2,0x10` out of the delay slot and `li a3,1` INTO the delay slot, raising diff to 10 bytes. (B) Pure memory barrier `__asm__ volatile("" ::: "memory")` between cStrLength and out.Write — same effect as (A), 10-byte diff. (C) `#pragma control sched=1` over the function — exploded to 24-byte diff (loop body unscheduled too).

The scheduler-induced swap is a 2-instruction position swap that any anchor either makes worse or doesn't move. The matched sibling gcMsgAssignPlayerId::Write produces yet another schedule (`andi, li a3, andi a1`) from the same source pattern, confirming context-dependence outside source control. Permuter (5min, 5544 candidates) produced zero improvement. Cannot mark unmatchable_bnel — diff is `andi/li` swap, not bnel/beql. Source on disk reverted to canonical 8-byte-diff version.

For next agent: the only remaining option I can think of is a pspcor.exe scheduler heuristic patch (out of scope for src/ agents) or finding some surrounding-context manipulation (e.g., changing prior call signatures or struct layouts) that nudges SNC's heuristic state. Worth checking whether the gcMsgAssignPlayerId match used a slightly different `Write` signature/order that influenced the scheduler — if it did, replicating that here is a long-shot avenue.

### Note 2

- session `8788b6b2`

cName::Write(cOutStream &) const. Got to within 8 bytes (2 instructions swapped) — pure SNC scheduler-ordering divergence, NOT a bnel pattern. The diff: expected has `andi a1,s3,0xffff; li a3,0x1; move a0,s0` (instructions at offsets 0x28, 0x2c, 0x30 of expected). Mine produces `li a3,0x1; andi a1,s3,0xffff; move a0,s0` — same three instructions, scheduler chose different position for li a3,1. Both placements are functionally equivalent and produce the same delay-slot filler (li a2,0x10).

Approaches tried: (1) explicit (unsigned int) cast, (2) implicit promotion from unsigned short, (3) (unsigned short) cast at call site, (4) double mask `len & 0xFFFF` on both sides, (5) bool sign2 = true local variable wrapped in block (this matched the gcMsgAssignPlayerId.cpp Write pattern that successfully matched), (6) literal true with bare call, (7) separate base/cur pointer like m2c output suggested.

The loop body matches PERFECTLY (lb a0; li a3=0; sll/sra a1,a0,0x18; move a0,s0; jal; li a2=8 in delay slot) — fixed by adopting the `signed char v = mName[i]; int iv = v; out.Write((signed char)iv, 8, sign);` pattern from gcMsgAssignPlayerId.cpp. The redundant `andi a1,s3,0xffff` is also present (fixed by using `int len = cStrLength(mName) & 0xFFFF; out.Write((unsigned int)(len & 0xFFFF), 0x10, true);`).

Permuter ran for 5 minutes (3393 candidates compiled, 5544 generated) with zero improvement. Best stayed at 8 bytes throughout. Cannot mark unmatchable_bnel — diff is li/andi swap, not bnel/beql. The matched sibling gcMsgAssignPlayerId::Write(cOutStream&,…) uses the same source pattern but SNC produced a slightly different schedule there too (`andi, li a3, andi a1` instead of cName's `andi, andi, li a3`) — suggesting SNC's scheduler is influenced by surrounding context (number of prior calls, register state) in a way that's not controllable from source alone for a bare-prologue function with only one preceding call.

Next agent: try compiling with -Xxopt or -Xmopt=0 variants (current uses -Xsched=2 default), or experiment with making the bool argument come from a struct field or non-trivially-computed expression that anchors its scheduling position.


## Disassembly

```asm
8cf4:	27bdffe0 	addiu	sp,sp,-32
8cf8:	afb10004 	sw	s1,4(sp)
8cfc:	00808825 	move	s1,a0
8d00:	afb00000 	sw	s0,0(sp)
8d04:	afb20008 	sw	s2,8(sp)
8d08:	afb3000c 	sw	s3,12(sp)
8d0c:	afbf0010 	sw	ra,16(sp)
8d10:	0c0004b4 	jal	0x12d0
8d14:	00a08025 	move	s0,a1
8d18:	3053ffff 	andi	s3,v0,0xffff
8d1c:	3265ffff 	andi	a1,s3,0xffff
8d20:	34070001 	li	a3,0x1
8d24:	02002025 	move	a0,s0
8d28:	0c0018b8 	jal	0x62e0
8d2c:	34060010 	li	a2,0x10
8d30:	34120000 	li	s2,0x0
8d34:	0253202a 	slt	a0,s2,s3
8d38:	1080000d 	beqz	a0,0x8d70
8d3c:	00000000 	nop
8d40:	02322021 	addu	a0,s1,s2
8d44:	80840000 	lb	a0,0(a0)
8d48:	34070000 	li	a3,0x0
8d4c:	00042e00 	sll	a1,a0,0x18
8d50:	00052e03 	sra	a1,a1,0x18
8d54:	02002025 	move	a0,s0
8d58:	0c00185b 	jal	0x616c
8d5c:	34060008 	li	a2,0x8
8d60:	26520001 	addiu	s2,s2,1
8d64:	0253202a 	slt	a0,s2,s3
8d68:	1480fff6 	bnez	a0,0x8d44
8d6c:	02322021 	addu	a0,s1,s2
8d70:	8fb00000 	lw	s0,0(sp)
8d74:	8fb10004 	lw	s1,4(sp)
8d78:	8fb20008 	lw	s2,8(sp)
8d7c:	8fb3000c 	lw	s3,12(sp)
8d80:	8fbf0010 	lw	ra,16(sp)
8d84:	03e00008 	jr	ra
8d88:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: cName::Write(cOutStream &) const
// Address: 0x00008cf4, Size: 152B
// Obj: cAll_psp.obj

? cOutStream__Write_int_int_bool(?, s8, ?, ?);      /* extern */
? cOutStream__Write_unsignedint_int_bool(?, s32, ?, ?); /* extern */
s32 cStrLength_constcharptr();                      /* extern */

void cName__Write_cOutStreamrefconst(s8 *arg0, ? arg1) {
    s32 temp_s3;
    s32 var_s2;
    s8 *var_a0;

    temp_s3 = cStrLength_constcharptr() & 0xFFFF;
    cOutStream__Write_unsignedint_int_bool(arg1, temp_s3 & 0xFFFF, 0x10, 1);
    var_s2 = 0;
    if (temp_s3 > 0) {
        var_a0 = arg0;
        do {
            cOutStream__Write_int_int_bool(arg1, (s8) *var_a0, 8, 0);
            var_s2 += 1;
            var_a0 = arg0 + var_s2;
        } while (var_s2 < temp_s3);
    }
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
