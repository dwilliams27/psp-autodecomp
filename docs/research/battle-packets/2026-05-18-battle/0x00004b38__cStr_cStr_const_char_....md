# Battle Packet: `cStr::cStr(const char *, ...)`

## Target

- Address: `0x00004b38`
- Size: `116` bytes
- Status: `failed`
- Object: `cAll_psp.obj`
- Class: `cStr`
- Method family: `Constructor`
- Leaf: `False`

## Queue Metadata

- score: `154`
- priority: `1`
- reason: `has failure_notes; rich notes; has failure snapshot; near miss 24B; correct function size; codegen-specific diagnosis; Constructor retry family; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `24`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `needs_layout`
- Near miss: `24`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x001ac408` `untried` 104B `vsnprintf`

Callers:
- `0x00002ac0` `untried` 448B `cMemAllocator::PlatformMalloc(unsigned int, unsigned int) static`
- `0x000086b4` `matched` 136B `cConfigBase::cConfigBase(cBase *)`
- `0x0000fb14` `untried` 1028B `cObject::Save(void)`
- `0x0001f180` `untried` 488B `eAudio::PlayChannel(const eAudioChannel *, float, float, int, const mOCS *) static`
- `0x00055ff0` `untried` 1412B `eMemCard::FindNextSave(const char *, cStr *, eMemCard::eFileTime *) static`
- `0x00056574` `untried` 1052B `eMemCard::GetRequiredFreeSpace(const char *, const char *, int, int, int) static`
- `0x0008d144` `failed` 140B `eAudioPlatform::eStreamThread::Initialize(int)`
- `0x00090acc` `untried` 1284B `eMoviePlatform::Open(const char *)`
- `0x000f2dc8` `failed` 272B `gcMap::GetRegionSetName(unsigned int, char *) const`
- `0x000f43a0` `untried` 472B `gcMap::DeleteRegion(int)`
- `0x001038d8` `untried` 1032B `gcGameSettings::SaveGameSaveFile(int, int, bool)`
- `0x00103ce0` `untried` 568B `gcGameSettings::SaveGameLoadFile(int, int)`
- `0x00103f34` `untried` 1084B `gcGameSettings::SaveGameListGames(void)`
- `0x001054b4` `untried` 940B `gcGameSettings::OnProfileSaved(gcProfile *, bool)`
- `0x0010904c` `untried` 832B `gcNetGame::HandleCommandLine(void) static`
- `0x0010e958` `failed` 372B `gcDesiredEnumerationEntryHelper::GetText(char *, bool) const`
- `0x0012f16c` `matched` 88B `gcValNumber::GetText(char *) const`
- `0x00165458` `untried` 6840B `gcMap::gcMap(cBase *, gcConfig::gcMapRegionPreset)`
- `0x0016ccbc` `untried` 7036B `gcGame::gcGame(cBase *)`
- `0x0016f4bc` `untried` 2192B `gcGame::Loop(void)`
- `0x0017cf5c` `failed` 1496B `gcDoUIShowDialog::GetText(char *) const`
- `0x00296a08` `failed` 340B `gcDoCameraLookFromEntityNode::GetText(char *) const`
- `0x0029980c` `untried` 2608B `gcDoCameraOp::GetText(char *) const`
- `0x0029e10c` `matched` 188B `gcDoDecalSpawn::GetText(char *) const`

## Placement

- Canonical source: `src/cStr.cpp`
- Header(s): `include/cStr.h`
- Allowed source(s): `src/cStr.cpp`
- Split-TU prefix: `src/cStr_*.cpp`

## Compiler Guidance

Constructor template: base constructor call order, vtable store, field initialization order, and vec_new/helper calls drive the bytes. Prefer already matched constructors from the same family.

## Class Header: `include/cStr.h`

Not found.

## Matched Same-Class Neighbors

- `0x00004cbc` 36B `cStr::Write(cWriteBlock &) const` — `src/cBasegcDesiredObjectTgcDesiredCustomAttackgcDesiredCustomAttackHelpergcEntityCustomAttack.cpp`
- `0x00004ce0` 44B `cStr::Read(cReadBlock &)` — `src/cStr.cpp`

## Prior Failure Notes

### Note 1

- session `11a032a2`; src `src/cStr.cpp`; snapshot `logs/failure_snapshots/match_20260514_022603/11a032a2/00004b38__agent_self_reported_failure__src_cStr.cpp`

Failed near-miss, 43-byte diff (out of 116 bytes total). Same root cause as prior session: SNC's automatic vararg save-area emission for a variadic CONSTRUCTOR does not produce the interleaved prologue shape the target uses. The target interleaves the 6 vararg saves (sw a2..t3 at sp+0x18..0x2c) with the callee-save sw s0,4(sp)/sw ra,8(sp) and the move s0,a0/move a0,a1 — and lands sw t3,44(sp) in the delay slot of the bnez. The current source emits all 6 vararg saves correctly via explicit inline asm but the compiler places them as a contiguous block AFTER the (incorrectly-offset) callee saves at sp+0/sp+4 instead of sp+4/sp+8.

New investigation this session: confirmed via experiments in src/test_va_experiment.cpp (now deleted) that SNC's __builtin_stdarg_start does exist (strings extern/snc/pspcfe.exe lists `__builtin_va_list`, `__builtin_stdarg_start`, `__builtin_varargs_start`, `__builtin_va_arg`, `__builtin_va_end`) but the SDK is missing <stdarg.h> (and include_snc/) so we can't directly use them. Key discovery: a `char buf[0x28]` local in the same function shape DOES make SNC auto-emit the 6 vararg saves and interleave them with the prologue — proved by replicating cName::Set's pattern. But that pattern requires extra callee-save register usage (s1 etc.) and makes the frame 96 bytes, not the 48 bytes target needs.

For the 48-byte-frame variadic constructor in cFilename::cFilename (same shape, 120B target) we also fail with 24-byte diff — no auto-emit at all. So this isn't just a cStr-specific problem; it's a constructor-vs-method asymmetry in SNC's varargs handling under our compiler config. To match: would likely need either (a) the missing SNC stdarg.h header that triggers proper __builtin_stdarg_start lowering, (b) a different compile flag we haven't tried, or (c) source contortion forcing both the right frame layout AND the interleaved saves.

Current source compiles to size-correct 116 bytes via manual sw a2/a3/t0-t3 inline asm. Vararg manipulation via lui+addiu+andi+addu+addiu pattern (matches cName::Set's matched form) computes va_list at sp+0x18. 43 differing bytes are all in the prologue interleaving (s0/ra offsets and instruction ordering).

### Note 2

- session `a2a9606d`

The body and va_list computation are straightforward and compile to the expected call/branch shape, but I could not make SNC emit the original constructor vararg prologue. The current best source explicitly saves a2/a3/t0/t1/t2/t3 to the correct stack offsets and has the correct 116-byte size, but SNC schedules those saves after its normal constructor prologue: sw s0/move s0/move a0/sw ra appear before the vararg saves. The target interleaves compiler-generated vararg saves before and between the s0/ra saves, so 43/116 bytes differ even though the stack offsets and later vsnprintf sequence are right.

Tried the existing cFilename-style manual va_list setup, volatile register locals for a2-a3/t0-t3, address-of-fmt stdarg-style expressions, an inert stack local to mimic cName::Set, parameter self-assignment/local aliases, optimization-level checks, and a temporary labeled-method experiment to isolate constructor lowering. The only way to trigger SNC's automatic vararg save area for this constructor was taking &fmt, but that adds an unwanted sw a1,0(sp) and produces a shorter va_list calculation (sp+4 instead of the target sp+0x18 sequence). This looks like a missing stdarg/compiler intrinsic issue for variadic constructors in our source environment rather than a control-flow problem.


## Disassembly

```asm
4b38:	27bdffd0 	addiu	sp,sp,-48
4b3c:	afa60018 	sw	a2,24(sp)
4b40:	afa7001c 	sw	a3,28(sp)
4b44:	afa80020 	sw	t0,32(sp)
4b48:	afb00004 	sw	s0,4(sp)
4b4c:	afa90024 	sw	t1,36(sp)
4b50:	00808025 	move	s0,a0
4b54:	afaa0028 	sw	t2,40(sp)
4b58:	00a02025 	move	a0,a1
4b5c:	afbf0008 	sw	ra,8(sp)
4b60:	14800003 	bnez	a0,0x4b70
4b64:	afab002c 	sw	t3,44(sp)
4b68:	1000000b 	b	0x4b98
4b6c:	a2000000 	sb	zero,0(s0)
4b70:	00803025 	move	a2,a0
4b74:	3c040000 	lui	a0,0x0
4b78:	24840010 	addiu	a0,a0,16
4b7c:	3084ffff 	andi	a0,a0,0xffff
4b80:	03a43821 	addu	a3,sp,a0
4b84:	24e70008 	addiu	a3,a3,8
4b88:	02002025 	move	a0,s0
4b8c:	0c06b102 	jal	0x1ac408
4b90:	34050100 	li	a1,0x100
4b94:	a20000ff 	sb	zero,255(s0)
4b98:	02001025 	move	v0,s0
4b9c:	8fb00004 	lw	s0,4(sp)
4ba0:	8fbf0008 	lw	ra,8(sp)
4ba4:	03e00008 	jr	ra
4ba8:	27bd0030 	addiu	sp,sp,48
```

## m2c Starting Point

```c
// Decompiled: cStr::cStr(const char *, ...)
// Address: 0x00004b38, Size: 116B
// Obj: cAll_psp.obj

? vsnprintf(void *, ?, s32, void *);                /* extern */

void *cStr__cStr_constcharptr_...(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp18;
    s32 sp1C;
    s32 sp20;
    s32 sp24;
    s32 sp28;
    s32 sp2C;

    sp18 = arg2;
    sp1C = arg3;
    sp20 = M2C_ERROR(/* Read from unset register $t0 */);
    sp24 = M2C_ERROR(/* Read from unset register $t1 */);
    sp28 = M2C_ERROR(/* Read from unset register $t2 */);
    sp2C = M2C_ERROR(/* Read from unset register $t3 */);
    if (arg1 == 0) {
        arg0->unk0 = 0;
    } else {
        vsnprintf(arg0, 0x100, arg1, sp + (0x10 & 0xFFFF) + 8);
        arg0->unkFF = 0;
    }
    return arg0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
