# Battle Packet: `nwConnection::nwMsgsBlock::nwMsgsBlock(void)`

## Target

- Address: `0x001a63e4`
- Size: `60` bytes
- Status: `failed`
- Object: `nwAll_psp.obj`
- Class: `nwConnection::nwMsgsBlock`
- Method family: `Constructor`
- Leaf: `True`

## Queue Metadata

- score: `154`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 2B; codegen-specific diagnosis; Constructor retry family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `2`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `constant_cse_codegen`, `needs_layout`
- Near miss: `2`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Placement

- Canonical source: `src/nwConnection_nwMsgsBlock.cpp`
- Header(s): `include/nwConnection_nwMsgsBlock.h`
- Allowed source(s): `src/nwConnection_nwMsgsBlock.cpp`
- Split-TU prefix: `src/nwConnection_nwMsgsBlock_*.cpp`

## Class Header: `include/nwConnection::nwMsgsBlock.h`

Not found.

## Prior Failure Notes

### Note 1

- session `ea37cfd0`

match wrote to out-of-scope path; reverted by Phase 1 ledger

### Note 2

- session `9c18e555`

Stuck at 2/60 bytes differ after exhaustive search. The 2 differing bytes are ONLY the destination register field in two adjacent instructions: offset 0x2 (0x05 vs target 0x06) and offset 0xA (0x85 vs target 0x86). Both are the register index for the register holding the 0x400 literal — my SNC picks $a1, the target picks $a2. All 15 instructions are otherwise byte-identical, including the extra 'li a1, 0x0' that's key to matching (achieved via `unsigned char zero = 0; m410 = zero;`). Every other field, operand, opcode, and schedule slot matches.

Approaches tried: (1) plain body assignment; (2) bool vs unsigned char for m410; (3) declaring 'zero' local before vs after the 0x400 assignment; (4) swapping mCursor/mCapacity assignment order; (5) using (char*)this instead of mBuffer; (6) sizeof(mBuffer) instead of 0x400 literal; (7) intermediate 'char *buf = mBuffer' pointer; (8) volatile (produces extra andi/sltu, rejected); (9) explicit initializer list (discards m410 byte-init, wrong size); (10) testing with -Xsched=1 and -Xmopt=0 manually (same result — register allocation is independent of these flags); (11) permuter for 90s over 3148 candidates (no improvement found).

Root cause appears to be SNC's register allocation strategy: when the compiler sees the first temp needed (for the 0x400 literal), my version picks $a1, but something in the target's source structure made SNC pick $a2. I suspect the target source either has an additional early declaration that reserves $a1, or uses a helper/inline function whose parameter conventions pre-claim $a1. The struct layout is confirmed correct (0x428 total, all 12 initialized fields accounted for). A next agent should experiment with (a) adding a dummy member function call that takes an arg in $a1 then gets inlined, (b) using a macro/template that might shift register allocation priorities, or (c) checking if nwAll_psp.obj was compiled with a non-default -Xvfpumatrix/vfpuscalar that affects nwConnection ctor register alloc.


## Disassembly

```asm
1a63e4:	34060400 	li	a2,0x400
1a63e8:	ac840400 	sw	a0,1024(a0)
1a63ec:	ac860404 	sw	a2,1028(a0)
1a63f0:	ac800408 	sw	zero,1032(a0)
1a63f4:	34050000 	li	a1,0x0
1a63f8:	ac80040c 	sw	zero,1036(a0)
1a63fc:	a0850410 	sb	a1,1040(a0)
1a6400:	a0800411 	sb	zero,1041(a0)
1a6404:	a0800412 	sb	zero,1042(a0)
1a6408:	ac800414 	sw	zero,1044(a0)
1a640c:	ac800418 	sw	zero,1048(a0)
1a6410:	ac80041c 	sw	zero,1052(a0)
1a6414:	ac800420 	sw	zero,1056(a0)
1a6418:	03e00008 	jr	ra
1a641c:	00801025 	move	v0,a0
```

## m2c Starting Point

```c
// Decompiled: nwConnection::nwMsgsBlock::nwMsgsBlock(void)
// Address: 0x001a63e4, Size: 60B
// Obj: nwAll_psp.obj

void *nwConnection__nwMsgsBlock__nwMsgsBlock_void(void *arg0) {
    arg0->unk400 = arg0;
    arg0->unk404 = 0x400;
    arg0->unk408 = 0;
    arg0->unk40C = 0;
    arg0->unk410 = 0;
    arg0->unk411 = 0;
    arg0->unk412 = 0;
    arg0->unk414 = 0;
    arg0->unk418 = 0;
    arg0->unk41C = 0;
    arg0->unk420 = 0;
    return arg0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
