# Battle Packet: `ePoint::ePoint(cBase *)`

## Target

- Address: `0x0005c2f4`
- Size: `304` bytes
- Status: `failed`
- Object: `eAll_psp.obj`
- Class: `ePoint`
- Method family: `Constructor`
- Leaf: `False`

## Queue Metadata

- score: `146`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 6B; Constructor retry family; manageable size`
- failure_action: `prep`
- near_miss_bytes: `6`

## Failure Classification

- Primary: `needs_layout`
- Action: `prep`
- Confidence: `medium`
- Tags: `needs_layout`, `retryable_nearmiss`
- Near miss: `6`
- Summary: Likely blocked by missing/uncertain class layout or field offsets.
- Guidance: improve layout/context first, then retry.

## Graph Context

Callees:
- `0x00009d28` `matched` 72B `cObject::cObject(cBase *)`

Callers:
- `0x00203b88` `matched` 124B `ePoint::New(cMemPool *, cBase *) static`

## Placement

- Canonical source: `src/ePoint.cpp`
- Header(s): `include/ePoint.h`
- Allowed source(s): `src/ePoint.cpp`
- Split-TU prefix: `src/ePoint_*.cpp`

## Compiler Guidance

SCHED HINT: This function is in the transition zone (0x040000-0x06e000) where the sched flag may be either 1 or 2. Try sched=2 first (default). If bytes don't match, this class may need sched=1 — report failure with a note and a human will update the Makefile.

Constructor template: base constructor call order, vtable store, field initialization order, and vec_new/helper calls drive the bytes. Prefer already matched constructors from the same family.

## Class Header: `include/ePoint.h`

Not found.

## Matched Same-Class Neighbors

- `0x0005c13c` 152B `ePoint::Write(cFile &) const` — `src/ePoint.cpp`
- `0x0005c1d4` 288B `ePoint::Read(cFile &, cMemPool *)` — `src/ePoint.cpp`
- `0x00203b18` 112B `ePoint::AssignCopy(const cBase *)` — `src/ePoint.cpp`
- `0x00203b88` 124B `ePoint::New(cMemPool *, cBase *) static` — `src/ePoint.cpp`
- `0x00203c04` 288B `ePoint::GetType(void) const` — `src/ePoint.cpp`

## Prior Failure Notes

### Note 1

- session `f132c206`

Implemented ePoint::ePoint(cBase *) as a canonical C++ constructor in src/ePoint.cpp, reusing the existing cObject base construction and ePoint vtable store. A high-level mOCS/v4sf_t identity-matrix version compiled but was too small (244B vs 304B) because SNC emitted scalar swc1 setup instead of the original mtc1/mfc1/mtv/sv.q stack construction. Reworked the identity initialization and post-copy to inline VFPU asm under .set noreorder, leaving C++ for the constructor shell and color field; this reduced the diff to 6/304 bytes.

The remaining mismatch is entirely the compiler-generated prologue/epilogue frame layout: our object uses a 16-byte frame with s0/ra at 0/4, while the original has a 192-byte frame with s0/ra at 176/180 and matching stack restore. I tried volatile scratch arrays, memory/register asm operands, address-taken local pads, comment asm operands, and sched=1 scoping; SNC optimized the dummy frame pressure away unless real C vector temporaries were used, and those reintroduced large scalar-code diffs. Likely needs a source pattern that forces a dead 0xB0 local frame without emitted code, or a compiler/pragma-specific frame-size mechanism.


## Disassembly

```asm
5c2f4:	27bdff40 	addiu	sp,sp,-192
5c2f8:	afb000b0 	sw	s0,176(sp)
5c2fc:	afbf00b4 	sw	ra,180(sp)
5c300:	0c00274a 	jal	0x9d28
5c304:	00808025 	move	s0,a0
5c308:	3c040038 	lui	a0,0x38
5c30c:	24842010 	addiu	a0,a0,8208
5c310:	ae040004 	sw	a0,4(s0)
5c314:	3c060004 	lui	a2,0x4
5c318:	8cc785b8 	lw	a3,-31304(a2)
5c31c:	3c050004 	lui	a1,0x4
5c320:	14e00029 	bnez	a3,0x5c3c8
5c324:	24a48578 	addiu	a0,a1,-31368
5c328:	34070001 	li	a3,0x1
5c32c:	acc785b8 	sw	a3,-31304(a2)
5c330:	44806000 	mtc1	zero,$f12
5c334:	44066000 	mfc1	a2,$f12
5c338:	44076000 	mfc1	a3,$f12
5c33c:	44086000 	mfc1	t0,$f12
5c340:	48e60006 	mtv         $a2, S120
5c344:	48e70026 	mtv         $a3, S121
5c348:	48e80046 	mtv         $t0, S122
5c34c:	fba60040 	sv.q        C120, 0x40($sp)
5c350:	3c063f80 	lui	a2,0x3f80
5c354:	44866800 	mtc1	a2,$f13
5c358:	44066800 	mfc1	a2,$f13
5c35c:	44076000 	mfc1	a3,$f12
5c360:	44086000 	mfc1	t0,$f12
5c364:	48e60007 	mtv         $a2, S130
5c368:	48e70027 	mtv         $a3, S131
5c36c:	48e80047 	mtv         $t0, S132
5c370:	fba70080 	sv.q        C130, 0x80($sp)
5c374:	44066000 	mfc1	a2,$f12
5c378:	44076800 	mfc1	a3,$f13
5c37c:	44086000 	mfc1	t0,$f12
5c380:	48e60008 	mtv         $a2, S200
5c384:	48e70028 	mtv         $a3, S201
5c388:	48e80048 	mtv         $t0, S202
5c38c:	fba80090 	sv.q        C200, 0x90($sp)
5c390:	44066000 	mfc1	a2,$f12
5c394:	44076000 	mfc1	a3,$f12
5c398:	44086800 	mfc1	t0,$f13
5c39c:	48e60009 	mtv         $a2, S210
5c3a0:	48e70029 	mtv         $a3, S211
5c3a4:	48e80049 	mtv         $t0, S212
5c3a8:	fba900a0 	sv.q        C210, 0xA0($sp)
5c3ac:	fba70050 	sv.q        C130, 0x50($sp)
5c3b0:	fba80060 	sv.q        C200, 0x60($sp)
5c3b4:	fba90070 	sv.q        C210, 0x70($sp)
5c3b8:	f8a78578 	sv.q        C130, -0x7A88($a1)
5c3bc:	f8880010 	sv.q        C200, 0x10($a0)
5c3c0:	f8890020 	sv.q        C210, 0x20($a0)
5c3c4:	f8860030 	sv.q        C120, 0x30($a0)
5c3c8:	d8a68578 	lv.q        C120, -0x7A88($a1)
5c3cc:	fba60000 	sv.q        C120, 0x0($sp)
5c3d0:	d8870010 	lv.q        C130, 0x10($a0)
5c3d4:	fba70010 	sv.q        C130, 0x10($sp)
5c3d8:	d8870020 	lv.q        C130, 0x20($a0)
5c3dc:	fba70020 	sv.q        C130, 0x20($sp)
5c3e0:	d8870030 	lv.q        C130, 0x30($a0)
5c3e4:	fba70030 	sv.q        C130, 0x30($sp)
5c3e8:	fa060050 	sv.q        C120, 0x50($s0)
5c3ec:	dba60010 	lv.q        C120, 0x10($sp)
5c3f0:	fa060060 	sv.q        C120, 0x60($s0)
5c3f4:	dba60020 	lv.q        C120, 0x20($sp)
5c3f8:	fa060070 	sv.q        C120, 0x70($s0)
5c3fc:	dba60030 	lv.q        C120, 0x30($sp)
5c400:	fa060080 	sv.q        C120, 0x80($s0)
5c404:	3c0400ff 	lui	a0,0xff
5c408:	24844000 	addiu	a0,a0,16384
5c40c:	ae040090 	sw	a0,144(s0)
5c410:	02001025 	move	v0,s0
5c414:	8fb000b0 	lw	s0,176(sp)
5c418:	8fbf00b4 	lw	ra,180(sp)
5c41c:	03e00008 	jr	ra
5c420:	27bd00c0 	addiu	sp,sp,192
```

## m2c Starting Point

```c
// Decompiled: ePoint::ePoint(cBase *)
// Address: 0x0005c2f4, Size: 304B
// Obj: eAll_psp.obj

? cObject__cObject_cBaseptr();                      /* extern */

void *ePoint__ePoint_cBaseptr(void *arg0) {
    cObject__cObject_cBaseptr();
    arg0->unk4 = 0x382010;
    if (*(s32 *)0x385B8 == 0) {
        *(s32 *)0x385B8 = 1;
        __asm__ volatile("mtv $a2, S120");
        __asm__ volatile("mtv $a3, S121");
        __asm__ volatile("mtv $t0, S122");
        __asm__ volatile("sv.q C120, 0x40($sp)");
        __asm__ volatile("mtv $a2, S130");
        __asm__ volatile("mtv $a3, S131");
        __asm__ volatile("mtv $t0, S132");
        __asm__ volatile("sv.q C130, 0x80($sp)");
        __asm__ volatile("mtv $a2, S200");
        __asm__ volatile("mtv $a3, S201");
        __asm__ volatile("mtv $t0, S202");
        __asm__ volatile("sv.q C200, 0x90($sp)");
        __asm__ volatile("mtv $a2, S210");
        __asm__ volatile("mtv $a3, S211");
        __asm__ volatile("mtv $t0, S212");
        __asm__ volatile("sv.q C210, 0xa0($sp)");
        __asm__ volatile("sv.q C130, 0x50($sp)");
        __asm__ volatile("sv.q C200, 0x60($sp)");
        __asm__ volatile("sv.q C210, 0x70($sp)");
        __asm__ volatile("sv.q C130, -0x7a88($a1)");
        __asm__ volatile("sv.q C200, 0x10($a0)");
        __asm__ volatile("sv.q C210, 0x20($a0)");
        __asm__ volatile("sv.q C120, 0x30($a0)");
    }
    __asm__ volatile("lv.q C120, -0x7a88($a1)");
    __asm__ volatile("sv.q C120, ($sp)");
    __asm__ volatile("lv.q C130, 0x10($a0)");
    __asm__ volatile("sv.q C130, 0x10($sp)");
    __asm__ volatile("lv.q C130, 0x20($a0)");
    __asm__ volatile("sv.q C130, 0x20($sp)");
    __asm__ volatile("lv.q C130, 0x30($a0)");
    __asm__ volatile("sv.q C130, 0x30($sp)");
    __asm__ volatile("sv.q C120, 0x50($s0)");
    __asm__ volatile("lv.q C120, 0x10($sp)");
    __asm__ volatile("sv.q C120, 0x60($s0)");
    __asm__ volatile("lv.q C120, 0x20($sp)");
    __asm__ volatile("sv.q C120, 0x70($s0)");
    __asm__ volatile("lv.q C120, 0x30($sp)");
    __asm__ volatile("sv.q C120, 0x80($s0)");
    arg0->unk90 = 0xFF4000;
    return arg0;
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
