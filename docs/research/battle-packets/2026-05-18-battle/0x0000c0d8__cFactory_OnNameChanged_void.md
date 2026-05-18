# Battle Packet: `cFactory::OnNameChanged(void)`

## Target

- Address: `0x0000c0d8`
- Size: `180` bytes
- Status: `failed`
- Object: `cAll_psp.obj`
- Class: `cFactory`
- Method family: `OnNameChanged`
- Leaf: `False`

## Queue Metadata

- score: `166`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 3B; codegen-specific diagnosis; manageable size`
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
- `0x000012d0` `untried` 68B `cStrLength(const char *)`
- `0x00001588` `untried` 68B `cStrCopy(char *, const char *, int)`
- `0x000047a0` `untried` 368B `cMemPool::GetPoolFromPtr(const void *) static`

## Placement

- Canonical source: `src/cFactory.cpp`
- Header(s): `include/cFactory.h`
- Allowed source(s): `src/cFactory.cpp`, `src/cFactory_OnNameChanged.cpp`
- Split-TU prefix: `src/cFactory_*.cpp`

## Class Header: `include/cFactory.h`

Not found.

## Matched Same-Class Neighbors

- `0x0000ab4c` 76B `cFactory::Write(cFile &) const` — `src/cFactory.cpp`
- `0x0000ab98` 188B `cFactory::Read(cFile &, cMemPool *)` — `src/cFactory.cpp`
- `0x0000ac54` 152B `cFactory::Load(void)` — `src/cFactory.cpp`
- `0x0000b114` 296B `cFactory::CreateObject(const cType *, const cGUID &, bool, unsigned int, bool)` — `src/cFactory.cpp`
- `0x0000b23c` 160B `cFactory::CopyObject(const cObject *, const cGUID &)` — `src/cFactory.cpp`

## Matched Method Exemplars

### Exemplar 1: `cNamed::OnNameChanged(void)`

- Address: `0x001c6ef8`
- Size: `8` bytes
- Source: `src/cMemAllocator.cpp`

```cpp
void cNamed::OnNameChanged(void) {
}
```


## Prior Failure Notes

### Note 1

- session `5d7878fd`; src `src/cFactory_OnNameChanged.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/5d7878fd/0000c0d8__agent_self_reported_failure__src_cFactory_OnNameChanged.cpp`

REG_ALLOC drift — 3-byte register-encoding diff, source has no leverage. Closest attempt is in a split-TU file (src/cFactory_OnNameChanged.cpp) because adding `void OnNameChanged(void);` to cFactory.cpp's local class regressed FindObject (16B diff) and DeleteGroups (size mismatch) — exactly the perturbation documented in docs/direction/005-regalloc-drift-guards.md. The split-TU isolates OnNameChanged's allocator context.

The entire function body matches except for the pre-loop counter init at offset 0x40/0x50: expected emits `addiu a0,v0,-1; ... move a3,a0` (counter primary → a0, shadow → a3); mine emits `addiu a3,v0,-1; ... move a0,a3` (primary → a3, shadow → a0). Loop body, dance pattern, CRC table access, final xor — all match. Diff is purely in 3 register-field bytes of the two init instructions (the addiu's rd field and the move's rd/rs fields).

Approaches attempted in THIS session beyond prior:
1. register-binding `__asm__("a0")` / `__asm__("a3")` on the two counter vars — SNC honored bindings for crc/p (a2/a1) but swapped a0↔a3 for the counter, ignoring the binding for those two vars specifically.
2. Swapping the C role: `old_a0 = var_a0; var_a0 = var_a3 - 1; var_a3 = var_a0;` (so var_a0 is captured first instead of var_a3). Compiler still chose to put the captured-first variable in physical a3, not a0 — confirming the choice is independent of variable name and loop-body role.
3. Inline-asm forced `addiu` and `move` writing into register-bound vars. This destabilized the whole prologue (24-byte diff) because SNC reallocated everything around the forced bindings.
4. Single-counter simpler loop (`captured = i; i--;`) — produced 172 bytes vs expected 180, dropping the dance entirely. Confirms the dual-counter dance is what expected wants but the byte count constrains us to it.
5. Permuter --time 300 --save-best: 10176 candidates generated, 6239 compiled, zero improvements past 3 bytes baseline.

Matches the REG_ALLOC failure signature in docs/direction/005-regalloc-drift-guards.md: ≤8 byte diff, all in register encoding fields, permuter ≥5min no improvement, ≥3 source restructurings tested. Per docs/decisions/006, this is `failed` (NOT unmatchable_bnel — diff is not in bnel/beql opcodes).

### Note 2

- session `287a7542`; src `src/cFactory.cpp`; snapshot `logs/failure_snapshots/match_20260505_232432/287a7542/0000c0d8__agent_self_reported_failure__src_cFactory.cpp`

cFactory::OnNameChanged(void) — 3 byte diff remains. CRC32 hash of the name string copied into pool+4. Reduced from 15 → 12 → 11 → 3 bytes through these key fixes:

1) Use cMemBlockSuspend's dual-counter `var_a3 = var_a0` dance pattern: declare `int var_a3; int var_a0 = len-1;` then assign `var_a3 = var_a0;` before the loop. Loop body reads var_a3 first (`old_a0 = var_a3`), then `var_a3 = var_a0 - 1` and `var_a0 = var_a3` in delay slot.
2) Add `__asm__ volatile("" ::: "memory");` between `crc = t1 ^ (crc>>8)` and `var_a0 = var_a3` in the loop body. WITHOUT this barrier the compiler reorders `addiu (counter dec)` BEFORE `srl (crc shift)`, causing 8 bytes of mismatch. Cribbed from cMemBlockSuspend cMemPool ctor pattern.
3) Write the table access as `*(unsigned int *)(((crc^*p)&0xff)*4 + 0x37BC50)` instead of `table[idx]` to flip the addu operand order from `addu t1,t0,t1` to `addu t1,t1,t0` (matches expected).

Remaining 3 bytes are dual-counter init register swap: expected emits `addiu a0,v0,-1; move a3,a0` (var_a0 → physical a0); mine emits `addiu a3,v0,-1; move a0,a3` (var_a0 → physical a3). The roles in the loop body (a0=current, a3=shadow) match expected exactly — only the pre-loop initialization writes to opposite registers.

Experiments tried: variable name swaps (var_current/var_shadow), declaration-order swaps, both-with-initializer vs uninit-then-assign patterns, declarations inside vs outside the if-block (the inside variant exploded to 97 bytes diff — bad). Permuter ran 200s, 2336 candidates compiled, zero improvement past 3 bytes. The compiler's choice of which dance variable gets a0 vs a3 appears deterministic but not source-controllable through any lever I could find. Curiously, sibling matched function cMemBlockSuspend::cMemPool::cMemPool uses the same dance pattern but lands on physical registers a1/a3 instead of a0/a3 — register pressure from the surrounding context (more locals in the ctor) likely shifts the allocator. For OnNameChanged with very few locals, the allocator picked an order I cannot flip from C source.


## Disassembly

```asm
c0d8:	27bdfff0 	addiu	sp,sp,-16
c0dc:	afb00000 	sw	s0,0(sp)
c0e0:	afb10004 	sw	s1,4(sp)
c0e4:	afb20008 	sw	s2,8(sp)
c0e8:	afbf000c 	sw	ra,12(sp)
c0ec:	0c0011e8 	jal	0x47a0
c0f0:	00808825 	move	s1,a0
c0f4:	00408025 	move	s0,v0
c0f8:	26120004 	addiu	s2,s0,4
c0fc:	26250008 	addiu	a1,s1,8
c100:	02402025 	move	a0,s2
c104:	0c000562 	jal	0x1588
c108:	34060020 	li	a2,0x20
c10c:	a2000023 	sb	zero,35(s0)
c110:	0c0004b4 	jal	0x12d0
c114:	02402025 	move	a0,s2
c118:	2444ffff 	addiu	a0,v0,-1
c11c:	2406ffff 	li	a2,-1
c120:	02402825 	move	a1,s2
c124:	10400010 	beqz	v0,0xc168
c128:	00803825 	move	a3,a0
c12c:	3c080038 	lui	t0,0x38
c130:	2508bc50 	addiu	t0,t0,-17328
c134:	90a90000 	lbu	t1,0(a1)
c138:	00805025 	move	t2,a0
c13c:	00c94826 	xor	t1,a2,t1
c140:	312900ff 	andi	t1,t1,0xff
c144:	00094880 	sll	t1,t1,0x2
c148:	01284821 	addu	t1,t1,t0
c14c:	8d290000 	lw	t1,0(t1)
c150:	00063202 	srl	a2,a2,0x8
c154:	24e4ffff 	addiu	a0,a3,-1
c158:	24a50001 	addiu	a1,a1,1
c15c:	01263026 	xor	a2,t1,a2
c160:	1540fff4 	bnez	t2,0xc134
c164:	00803825 	move	a3,a0
c168:	2404ffff 	li	a0,-1
c16c:	00c42026 	xor	a0,a2,a0
c170:	ae040000 	sw	a0,0(s0)
c174:	8fb00000 	lw	s0,0(sp)
c178:	8fb10004 	lw	s1,4(sp)
c17c:	8fb20008 	lw	s2,8(sp)
c180:	8fbf000c 	lw	ra,12(sp)
c184:	03e00008 	jr	ra
c188:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: cFactory::OnNameChanged(void)
// Address: 0x0000c0d8, Size: 180B
// Obj: cAll_psp.obj

void *cMemPool__GetPoolFromPtr_constvoidptrstatic(); /* extern */
? cStrCopy_charptr_constcharptr_int(u8 *, s32, ?);  /* extern */
s32 cStrLength_constcharptr(u8 *);                  /* extern */

void cFactory__OnNameChanged_void(s32 arg0) {
    s32 temp_t1;
    s32 temp_t2;
    s32 temp_v0_2;
    s32 var_a0;
    s32 var_a3;
    u32 var_a2;
    u8 *temp_s2;
    u8 *var_a1;
    void *temp_v0;

    temp_v0 = cMemPool__GetPoolFromPtr_constvoidptrstatic();
    temp_s2 = temp_v0 + 4;
    cStrCopy_charptr_constcharptr_int(temp_s2, arg0 + 8, 0x20);
    temp_v0->unk23 = 0;
    temp_v0_2 = cStrLength_constcharptr(temp_s2);
    var_a0 = temp_v0_2 - 1;
    var_a2 = -1U;
    var_a1 = temp_s2;
    var_a3 = var_a0;
    if (temp_v0_2 != 0) {
        do {
            temp_t2 = var_a0;
            temp_t1 = (((var_a2 ^ *var_a1) & 0xFF) * 4)->unk37BC50;
            var_a0 = var_a3 - 1;
            var_a1 += 1;
            var_a2 = temp_t1 ^ (var_a2 >> 8);
            var_a3 = var_a0;
        } while (temp_t2 != 0);
    }
    temp_v0->unk0 = (s32) (var_a2 ^ -1);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
