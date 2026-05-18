# Battle Packet: `gcMessageHandler::Write(cFile &) const`

## Target

- Address: `0x000d7290`
- Size: `256` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcMessageHandler`
- Method family: `Write`
- Leaf: `False`

## Queue Metadata

- score: `110`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 51B; codegen-specific diagnosis; Write retry family; manageable size`
- next_run_reason: `lane 1 focused research/codegen target`
- failure_action: `research`
- near_miss_bytes: `51`

## Failure Classification

- Primary: `branch_direction_codegen`
- Action: `research`
- Confidence: `high`
- Tags: `branch_direction_codegen`, `constant_cse_codegen`, `tagged_pointer_shape`, `needs_layout`
- Near miss: `51`
- Summary: Branch-likely or jump-in/skip-over codegen divergence.
- Guidance: use as a focused research target, not a broad retry target.

## Graph Context

Callees:
- `0x00005458` `failed` 136B `cWriteBlock::cWriteBlock(cFile &, unsigned int)`
- `0x00005690` `matched` 116B `cWriteBlock::Write(int)`
- `0x00005704` `matched` 116B `cWriteBlock::Write(unsigned int)`
- `0x00005c10` `matched` 128B `cWriteBlock::WriteBase(const cBase *)`
- `0x00005c90` `matched` 112B `cWriteBlock::End(void)`
- `0x00009030` `failed` 664B `cHandle::Write(cWriteBlock &) const`

## Placement

- Canonical source: `src/gcMessageHandler.cpp`
- Header(s): `include/gcMessageHandler.h`
- Allowed source(s): `src/gcMessageHandler.cpp`
- Split-TU prefix: `src/gcMessageHandler_*.cpp`

## Compiler Guidance

Write template: preserve cWriteBlock/cOutStream construction, field write order, and End() placement. SNC is sensitive to temporary bool/cast shapes and to independent store ordering.

## Class Header: `include/gcMessageHandler.h`

Not found.

## Matched Same-Class Neighbors

- `0x000d75a4` 48B `gcMessageHandler::gcMessageHandler(cBase *)` — `src/gcMessageHandler.cpp`
- `0x0023ba60` 124B `gcMessageHandler::New(cMemPool *, cBase *) static` — `src/gcMessageHandler.cpp`
- `0x0023badc` 160B `gcMessageHandler::GetType(void) const` — `src/gcMessageHandler.cpp`
- `0x0023bb7c` 36B `gcMessageHandler::GetName(char *) const` — `src/gcMessageHandler.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcMsgPlayMovie::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const`

- Address: `0x0013661c`
- Size: `252` bytes
- Source: `src/gcMsgPlayMovie.cpp`

```cpp
void gcMsgPlayMovie::Write(cOutStream &s, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
    ((const cName *)((const char *)this + 4))->Write(s);
    ((cOutStreamBit *)&s)->WriteBit(*((const unsigned char *)this + 0x1C));
    ((const cHandle *)((const char *)this + 0x20))->Write(s);
    ((const cHandle *)((const char *)this + 0x24))->Write(s);
    ((cOutStreamFloat *)&s)->Write(*(const float *)((const char *)this + 0x28), true);
    ((const cTimeValueRef *)((const char *)this + 0x2C))->Write(s);
    ((const cHandle *)((const char *)this + 0x30))->Write(s);
}
```

### Exemplar 2: `gcString::Write(cFile &) const`

- Address: `0x000d6214`
- Size: `264` bytes
- Source: `src/gcString.cpp`

```cpp
// ── gcString::Write(cFile &) const @ 0x000d6214 ──
void gcString::Write(cFile &file) const {
    cWriteBlock wb(file, 5);

    const wchar_t *text = *(const wchar_t * const *)((const char *)this + 0x18);
    wb.Write(text != 0 ? (*((const int *)text - 1) & 0x3FFFFFFF) : 0);

    text = *(const wchar_t * const *)((const char *)this + 0x18);
    int textCount = 0;
    if (text != 0) {
        textCount = *((const int *)text - 1) & 0x3FFFFFFF;
    }
    wb.Write(textCount, text);

    wb.Write((unsigned int)mField20);
    ((const cHandle *)&mField08)->Write(wb);
    wb.Write((unsigned int)mField0C);
    ((const cHandle *)&mField10.a)->Write(wb);
    wb.Write((unsigned int)mField10.b);

    text = (const wchar_t *)mName.mData;
    wb.Write(text != 0 ? (*((const int *)text - 1) & 0x3FFFFFFF) : 0);

    text = (const wchar_t *)mName.mData;
    textCount = 0;
    if (text != 0) {
        textCount = *((const int *)text - 1) & 0x3FFFFFFF;
    }
    wb.Write(textCount, text);

    wb.End();
}
```

### Exemplar 3: `gcSurface::Write(cFile &) const`

- Address: `0x00137ce4`
- Size: `236` bytes
- Source: `src/gcSurface.cpp`

```cpp
void gcSurface::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    ((const eSurface *)this)->Write(file);

    {
        char *typePtr = *(char **)((char *)this + 0x70);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0x6C;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    {
        char *typePtr = *(char **)((char *)this + 0x8C);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0x88;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    {
        char *typePtr = *(char **)((char *)this + 0xA8);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0xA4;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    {
        char *typePtr = *(char **)((char *)this + 0xC4);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0xC0;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    {
        char *typePtr = *(char **)((char *)this + 0xE0);
        TypeDispatchEntry *entry = (TypeDispatchEntry *)(typePtr + 0x28);
        char *base = (char *)this + 0xDC;
        entry->fn(base + entry->offset, wb._data[0]);
    }

    wb.End();
}
```

### Exemplar 4: `gcEntityAttack::Write(cFile &) const`

- Address: `0x0010eed8`
- Size: `232` bytes
- Source: `src/gcEntityAttack.cpp`

```cpp
void gcEntityAttack::Write(cFile &file) const {
    cFile *out = &file;
    const gcEntityAttack *self = this;
    cWriteBlock wb(file, 6);

    {
        char *typePtr = *(char **)((const char *)self + 0x0C);
        WriteEntry *entry = (WriteEntry *)(typePtr + 0x28);
        char *base = (char *)self + 8;
        entry->fn(base + entry->offset, out);
    }

    wb.Write(*(const int *)((const char *)self + 0x2C));
    wb.Write(*(const int *)((const char *)self + 0x30));
    ((const cBaseArray *)((const char *)self + 0x70))->Write(wb);

    {
        char *typePtr = *(char **)((const char *)self + 0x3C);
        WriteEntry *entry = (WriteEntry *)(typePtr + 0x28);
        char *base = (char *)self + 0x38;
        entry->fn(base + entry->offset, wb.mFile);
    }

    wb.Write(*(const unsigned int *)((const char *)self + 0x34));
    wb.Write(*(const bool *)((const char *)self + 0x54));
    wb.Write(*(const bool *)((const char *)self + 0x55));

    {
        char *typePtr = *(char **)((const char *)self + 0x5C);
        WriteEntry *entry = (WriteEntry *)(typePtr + 0x28);
        char *base = (char *)self + 0x58;
        entry->fn(base + entry->offset, wb.mFile);
    }

    wb.End();
}
```


## Prior Failure Notes

### Note 1

- session `f2337a3e`

Implemented gcMessageHandler::Write in its class file despite the batch prompt's repeated file hint, to satisfy the repo class-method rule. The body writes cWriteBlock(file, 2), handle at +8, uint +0xC, int +0x10, handle +0x14, then the tagged base pointer at +0x18 with the same null/self-owned checks as the disassembly. I tried structured if/else, literal m2c temporaries, shared goto null block, and cBase* temporaries for WriteBase; the permuter then added a memory barrier in the var_a2_2 false path and improved the diff from 61 to 51 bytes.

Best result is 51/256 bytes different after a 60s permuter run (1608 generated / 767 compiled / one improvement). The remaining mismatch is the middle tagged-pointer control-flow scheduling: target keeps the main value in a0, temp tag in a1, flag in a2 and uses the exact bnez/beqz layout leading to one shared WriteBase(0) block; our closest source still introduces a3 moves and different branch layout around the null/self-owned tests. The high-level behavior and call order are correct, but source shape has not yet produced the target allocator/branch schedule. This is not a bnel divergence; the diff is too large and is not branch-likely vs branch+nop.


## Disassembly

```asm
d7290:	27bdfff0 	addiu	sp,sp,-16
d7294:	afb00008 	sw	s0,8(sp)
d7298:	00808025 	move	s0,a0
d729c:	03a02025 	move	a0,sp
d72a0:	afbf000c 	sw	ra,12(sp)
d72a4:	0c001516 	jal	0x5458
d72a8:	34060002 	li	a2,0x2
d72ac:	26040008 	addiu	a0,s0,8
d72b0:	0c00240c 	jal	0x9030
d72b4:	03a02825 	move	a1,sp
d72b8:	8e05000c 	lw	a1,12(s0)
d72bc:	0c0015c1 	jal	0x5704
d72c0:	03a02025 	move	a0,sp
d72c4:	8e050010 	lw	a1,16(s0)
d72c8:	0c0015a4 	jal	0x5690
d72cc:	03a02025 	move	a0,sp
d72d0:	26040014 	addiu	a0,s0,20
d72d4:	0c00240c 	jal	0x9030
d72d8:	03a02825 	move	a1,sp
d72dc:	8e040018 	lw	a0,24(s0)
d72e0:	34060000 	li	a2,0x0
d72e4:	30850001 	andi	a1,a0,0x1
d72e8:	54a00001 	bnezl	a1,0xd72f0
d72ec:	34060001 	li	a2,0x1
d72f0:	10c00003 	beqz	a2,0xd7300
d72f4:	2c860001 	sltiu	a2,a0,1
d72f8:	10000003 	b	0xd7308
d72fc:	34060001 	li	a2,0x1
d7300:	30c600ff 	andi	a2,a2,0xff
d7304:	0006302b 	sltu	a2,zero,a2
d7308:	14c0000c 	bnez	a2,0xd733c
d730c:	34060000 	li	a2,0x0
d7310:	54a00001 	bnezl	a1,0xd7318
d7314:	34060001 	li	a2,0x1
d7318:	10c00003 	beqz	a2,0xd7328
d731c:	00803025 	move	a2,a0
d7320:	10000001 	b	0xd7328
d7324:	34060000 	li	a2,0x0
d7328:	8cc60008 	lw	a2,8(a2)
d732c:	2cc60001 	sltiu	a2,a2,1
d7330:	30c600ff 	andi	a2,a2,0xff
d7334:	10c00006 	beqz	a2,0xd7350
d7338:	00a03025 	move	a2,a1
d733c:	03a02025 	move	a0,sp
d7340:	0c001704 	jal	0x5c10
d7344:	00002825 	move	a1,zero
d7348:	1000000b 	b	0xd7378
d734c:	00000000 	nop
d7350:	34050000 	li	a1,0x0
d7354:	54c00001 	bnezl	a2,0xd735c
d7358:	34050001 	li	a1,0x1
d735c:	10a00003 	beqz	a1,0xd736c
d7360:	00000000 	nop
d7364:	10000001 	b	0xd736c
d7368:	34040000 	li	a0,0x0
d736c:	00802825 	move	a1,a0
d7370:	0c001704 	jal	0x5c10
d7374:	03a02025 	move	a0,sp
d7378:	0c001724 	jal	0x5c90
d737c:	03a02025 	move	a0,sp
d7380:	8fb00008 	lw	s0,8(sp)
d7384:	8fbf000c 	lw	ra,12(sp)
d7388:	03e00008 	jr	ra
d738c:	27bd0010 	addiu	sp,sp,16
```

## m2c Starting Point

```c
// Decompiled: gcMessageHandler::Write(cFile &) const
// Address: 0x000d7290, Size: 256B
// Obj: gcAll_psp.obj

? cHandle__Write_cWriteBlockrefconst(void *, void *); /* extern */
? cWriteBlock__End_void(void *);                    /* extern */
? cWriteBlock__WriteBase_constcBaseptr(void *, s32, s32); /* extern */
? cWriteBlock__Write_int(void *, s32);              /* extern */
? cWriteBlock__Write_unsignedint(void *, s32);      /* extern */
? cWriteBlock__cWriteBlock_cFileref_unsignedint(void *, ?); /* extern */

void gcMessageHandler__Write_cFilerefconst(void *arg0) {
    s32 temp_a1;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_a2_2;
    s32 var_a2_3;
    s32 var_a2_4;

    cWriteBlock__cWriteBlock_cFileref_unsignedint(sp, 2);
    cHandle__Write_cWriteBlockrefconst(arg0 + 8, sp);
    cWriteBlock__Write_unsignedint(sp, arg0->unkC);
    cWriteBlock__Write_int(sp, arg0->unk10);
    cHandle__Write_cWriteBlockrefconst(arg0 + 0x14, sp);
    var_a0 = arg0->unk18;
    var_a2 = 0;
    temp_a1 = var_a0 & 1;
    if (temp_a1 != 0) {
        var_a2 = 1;
    }
    if (var_a2 != 0) {
        var_a2_2 = 1;
    } else {
        var_a2_2 = ((var_a0 == 0) & 0xFF) != 0;
    }
    var_a2_3 = 0;
    if (var_a2_2 == 0) {
        if (temp_a1 != 0) {
            var_a2_3 = 1;
        }
        var_a2_4 = var_a0;
        if (var_a2_3 != 0) {
            var_a2_4 = 0;
        }
        var_a2_3 = temp_a1;
        if ((var_a2_4->unk8 == 0) & 0xFF) {
            goto block_11;
        }
        var_a1 = 0;
        if (var_a2_3 != 0) {
            var_a1 = 1;
        }
        if (var_a1 != 0) {
            var_a0 = 0;
        }
        cWriteBlock__WriteBase_constcBaseptr(sp, var_a0, var_a2_3);
    } else {
block_11:
        cWriteBlock__WriteBase_constcBaseptr(sp, 0, var_a2_3);
    }
    cWriteBlock__End_void(sp);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
