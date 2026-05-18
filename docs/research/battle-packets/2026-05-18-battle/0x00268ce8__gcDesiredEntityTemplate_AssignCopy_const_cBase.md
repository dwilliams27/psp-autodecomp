# Battle Packet: `gcDesiredEntityTemplate::AssignCopy(const cBase *)`

## Target

- Address: `0x00268ce8`
- Size: `360` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcDesiredEntityTemplate`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `156`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 16B; codegen-specific diagnosis; AssignCopy retry family; manageable size`
- next_run_reason: `lane 2 <=16B regalloc/quarantine tail target`
- failure_action: `quarantine`
- near_miss_bytes: `16`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`
- Near miss: `16`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00007534` `untried` 264B `cType::InitializeType(const char *, const char *, unsigned int, const cType *, cBase * (*)(cMemPool *, cBase *), const char *, const char *, unsigned int) static`
- `0x0012775c` `untried` 588B `gcDesiredEntityTemplate::operator=(const gcDesiredEntityTemplate &)`

## Placement

- Canonical source: `src/gcDesiredEntityTemplate.cpp`
- Header(s): `include/gcDesiredEntityTemplate.h`
- Allowed source(s): `src/gcDesiredEntityTemplate.cpp`
- Split-TU prefix: `src/gcDesiredEntityTemplate_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/gcDesiredEntityTemplate.h`

Not found.

## Matched Same-Class Neighbors

- `0x001279a8` 176B `gcDesiredEntityTemplate::Write(cFile &) const` — `src/gcDesiredEntityTemplate.cpp`
- `0x00127bf0` 28B `gcDesiredEntityTemplate::GetObject(bool) const` — `src/gcDesiredEntityTemplate.cpp`
- `0x001281c0` 296B `gcDesiredEntityTemplate::GetDesiredType(void) const` — `src/gcDesiredEntityTemplate.cpp`
- `0x00128488` 80B `gcDesiredEntityTemplate::HasCategory(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &) const` — `src/gcDesiredEntityTemplate.cpp`
- `0x00268e50` 156B `gcDesiredEntityTemplate::New(cMemPool *, cBase *) static` — `src/gcValSaveGame.cpp`

## Matched Method Exemplars

### Exemplar 1: `gcProjectileController::AssignCopy(const cBase *)`

- Address: `0x0031ee30`
- Size: `400` bytes
- Source: `src/gcProjectileController.cpp`

```cpp
// ── gcProjectileController::AssignCopy(const cBase *) @ 0x0031ee30 ──
void gcProjectileController::AssignCopy(const cBase *src) {
    int var_a3_2;
    unsigned int var_a3;
    gcProjectileController *temp_v0;
    gcProjectileControllerBlock24 *var_a1;
    gcProjectileControllerBlock24 *var_a2;
    gcProjectileControllerHalf3 *var_a1_2;
    gcProjectileControllerHalf3 *var_a2_2;
    int *temp_a0;
    int *temp_a1_3;

    temp_v0 = (gcProjectileController *)
        dcastdcast_gcProjectileControllerptr__constcBaseptr(src);

    int *src8 = (int *)((char *)temp_v0 + 8);
    int *dst8 = (int *)((char *)this + 8);
    int word8 = src8[0];
    int wordC = src8[1];
    int word10 = src8[2];
    dst8[0] = word8;
    int word14 = src8[3];
    dst8[1] = wordC;
    int word18 = src8[4];
    dst8[2] = word10;
    int word1C = src8[5];
    dst8[3] = word14;
    dst8[4] = word18;
    dst8[5] = word1C;

    *(int *)((char *)this + 0x20) = *(int *)((char *)temp_v0 + 0x20);
    *(unsigned char *)((char *)this + 0x24) =
        *(unsigned char *)((char *)temp_v0 + 0x24);
    *(unsigned char *)((char *)this + 0x25) =
        *(unsigned char *)((char *)temp_v0 + 0x25);

    var_a3 = 0;
    var_a2 = (gcProjectileControllerBlock24 *)((char *)this + 0x28);
    var_a1 = (gcProjectileControllerBlock24 *)((char *)temp_v0 + 0x28);
    do {
        int *src_words;
        int *dst_words;
        int *src_words_2;
        int *dst_words_2;
        int *dst_words_3;
        int *src_words_3;
        int *src_words_4;
        int *src_words_5;
        int *src_words_6;

        var_a2->unk0 = (int)var_a1->unk0;
        var_a2->unk4 = (unsigned char)var_a1->unk4;
        var_a2->unk5 = (unsigned char)var_a1->unk5;
        src_words = (int *)((char *)var_a1 + 8);
        dst_words = (int *)((unsigned char *)var_a2 + 8);
        dst_words[0] = src_words[0];
        dst_words_2 = (int *)((char *)var_a2 + 12);
        src_words_2 = (int *)((char *)var_a1 + 12);
        dst_words_2[0] = src_words_2[0];
        src_words_3 = (int *)((char *)var_a1 + 16);
        dst_words_3 = (int *)((char *)var_a2 + 16);
        src_words_4 = (int *)((char *)var_a1 + 20);
        dst_words_3[0] = src_words_3[0];
        dst_words_2 = (int *)((char *)var_a2 + 20);
        src_words_5 = (int *)((char *)var_a1 + 24);
        dst_words_2[0] = src_words_4[0];
        dst_words = (int *)((char *)var_a2 + 24);
        src_words_6 = (int *)((char *)var_a1 + 28);
        dst_words[0] = src_words_5[0];
        dst_words = (int *)((char *)var_a2 + 28);
        var_a3 += 1;
        dst_words[0] = src_words_6[0];
        var_a2->unk20 = (int)var_a1->unk20;
        var_a1 = (gcProjectileControllerBlock24 *)((char *)var_a1 + 0x24);
        var_a2 = (gcProjectileControllerBlock24 *)((char *)var_a2 + 0x24);
    } while (var_a3 < 2U);

    *(int *)((char *)this + 0x70) = *(int *)((char *)temp_v0 + 0x70);
    *(int *)((char *)this + 0x74) = *(int *)((char *)temp_v0 + 0x74);
    {
        int *src78 = (int *)((char *)temp_v0 + 0x78);
        int *dst78 = (int *)((char *)this + 0x78);

        *dst78 = *src78;
    }
    var_a3_2 = 0;
    var_a2_2 = (gcProjectileControllerHalf3 *)((char *)this + 0x80);
    *(int *)((char *)this + 0x7C) = *(int *)((char *)temp_v0 + 0x7C);
    var_a1_2 = (gcProjectileControllerHalf3 *)((char *)temp_v0 + 0x80);
    do {
        short a = var_a1_2->unk0;
        short b = var_a1_2->unk2;
        short c = var_a1_2->unk4;
        var_a2_2->unk0 = a;
        var_a2_2->unk2 = b;
        var_a2_2->unk4 = c;
        var_a3_2 += 1;
        var_a2_2 = (gcProjectileControllerHalf3 *)((char *)var_a2_2 + 6);
        var_a1_2 = (gcProjectileControllerHalf3 *)((char *)var_a1_2 + 6);
    } while (var_a3_2 <= 0);

    temp_a0 = (int *)((char *)temp_v0 + 0x8C);
    *(float *)((char *)this + 0x88) = *(float *)((char *)temp_v0 + 0x88);
    temp_a1_3 = (int *)((char *)this + 0x8C);
    *temp_a1_3 = *temp_a0;
}
```

### Exemplar 2: `gcRigidBodyController::AssignCopy(const cBase *)`

- Address: `0x0031f488`
- Size: `400` bytes
- Source: `src/gcRigidBodyController.cpp`

```cpp
// gcRigidBodyController::AssignCopy(const cBase *) @ 0x0031f488
// ─────────────────────────────────────────────────────────────────────────

void gcRigidBodyController::AssignCopy(const cBase *base) {
    gcRigidBodyControllerLayout *arg0 = (gcRigidBodyControllerLayout *)this;
    gcRigidBodyControllerLayout *temp_v0 =
        (gcRigidBodyControllerLayout *)dcastdcast_gcRigidBodyControllerptr__constcBaseptr(base);
    int *temp_a1 = &temp_v0->unk8;
    int *temp_a3 = &arg0->unk8;

    int temp_unk8 = temp_a1[0];
    int temp_unkC = temp_a1[1];
    int temp_unk10 = temp_a1[2];
    temp_a3[0] = temp_unk8;
    int temp_unk14 = temp_a1[3];
    temp_a3[1] = temp_unkC;
    int temp_unk18 = temp_a1[4];
    temp_a3[2] = temp_unk10;
    int temp_unk1C = temp_a1[5];
    temp_a3[3] = temp_unk14;
    temp_a3[4] = temp_unk18;
    temp_a3[5] = temp_unk1C;

    arg0->unk20 = temp_v0->unk20;
    arg0->unk24 = temp_v0->unk24;
    arg0->unk25 = temp_v0->unk25;

    unsigned int var_a3 = 0;
    gcRigidBodyControllerBlock36 *var_a2 = arg0->unk28;
    gcRigidBodyControllerBlock36 *var_a1 = temp_v0->unk28;
    do {
        int temp_unk0 = var_a1->unk0;
        var_a2->unk0 = temp_unk0;
        unsigned char temp_unk4 = var_a1->unk4;
        var_a2->unk4 = temp_unk4;
        unsigned char temp_unk5 = var_a1->unk5;
        int *temp_t1 = &var_a1->unk8;
        var_a2->unk5 = temp_unk5;
        int *temp_t2 = &var_a2->unk8;
        int temp_unk8_2 = *temp_t1;
        *temp_t2 = temp_unk8_2;
        int *temp_t0 = (int *)((char *)var_a2 + 12);
        int *temp_t1_2 = (int *)((char *)var_a1 + 12);
        int temp_unkC_2 = *temp_t1_2;
        int *temp_t2_2 = (int *)((char *)var_a1 + 16);
        *temp_t0 = temp_unkC_2;
        int *temp_t0_2 = (int *)((char *)var_a2 + 16);
        int *temp_t2_3 = (int *)((char *)var_a1 + 20);
        int temp_unk10_2 = *temp_t2_2;
        int *temp_t0_3 = (int *)((char *)var_a2 + 20);
        *temp_t0_2 = temp_unk10_2;
        int *temp_t2_4 = (int *)((char *)var_a1 + 24);
        int temp_unk14_2 = *temp_t2_3;
        int *temp_t0_4 = (int *)((char *)var_a2 + 24);
        int *temp_t2_5 = (int *)((char *)var_a1 + 28);
        *temp_t0_3 = temp_unk14_2;
        int temp_unk18_2 = *temp_t2_4;
        int *temp_t0_5 = (int *)((char *)var_a2 + 28);
        *temp_t0_4 = temp_unk18_2;
        int temp_unk1C_2 = *temp_t2_5;
        *temp_t0_5 = temp_unk1C_2;
        int temp_unk20_2 = var_a1->unk20;
        var_a3 += 1;
        var_a2->unk20 = temp_unk20_2;
        var_a1 = (gcRigidBodyControllerBlock36 *)((char *)var_a1 + 0x24);
        var_a2 = (gcRigidBodyControllerBlock36 *)((char *)var_a2 + 0x24);
    } while (var_a3 < 2U);

    arg0->unk70 = temp_v0->unk70;
    arg0->unk74 = temp_v0->unk74;

    int *var_a2_2 = &temp_v0->unk78;
    int *var_t0 = &arg0->unk78;
    *var_t0 = *var_a2_2;
    arg0->unk7C = temp_v0->unk7C;

    int var_a3_2 = 0;
    gcRigidBodyControllerHalf3 *var_a2_3 = arg0->unk80;
    gcRigidBodyControllerHalf3 *var_a1_2 = temp_v0->unk80;
    do {
        short temp_unk0_2 = var_a1_2->unk0;
        short temp_unk2_2 = var_a1_2->unk2;
        short temp_unk4_2 = var_a1_2->unk4;
        var_a2_3->unk0 = temp_unk0_2;
        var_a2_3->unk2 = temp_unk2_2;
        var_a2_3->unk4 = temp_unk4_2;
        var_a3_2 += 1;
        var_a2_3 = (gcRigidBodyControllerHalf3 *)((char *)var_a2_3 + 6);
        var_a1_2 = (gcRigidBodyControllerHalf3 *)((char *)var_a1_2 + 6);
    } while (var_a3_2 <= 0);

    float temp_f12 = temp_v0->unk88;
    int *temp_a0_2 = (int *)((char *)temp_v0 + 0x8C);
    arg0->unk88 = temp_f12;
    int *temp_a1_2 = (int *)((char *)arg0 + 0x8C);
    *temp_a1_2 = *temp_a0_2;
}
```

### Exemplar 3: `gcSimpleController::AssignCopy(const cBase *)`

- Address: `0x0031fff4`
- Size: `400` bytes
- Source: `src/gcSimpleController.cpp`

```cpp
void gcSimpleController::AssignCopy(const cBase *base) {
    gcSimpleControllerLayout *arg0 = (gcSimpleControllerLayout *)this;
    gcSimpleControllerLayout *temp_v0 =
        (gcSimpleControllerLayout *)dcastdcast_gcSimpleControllerptr__constcBaseptr(base);
    int *temp_a1 = &temp_v0->unk8;
    int *temp_a3 = &arg0->unk8;

    int temp_unk8 = temp_a1[0];
    int temp_unkC = temp_a1[1];
    int temp_unk10 = temp_a1[2];
    temp_a3[0] = temp_unk8;
    int temp_unk14 = temp_a1[3];
    temp_a3[1] = temp_unkC;
    int temp_unk18 = temp_a1[4];
    temp_a3[2] = temp_unk10;
    int temp_unk1C = temp_a1[5];
    temp_a3[3] = temp_unk14;
    temp_a3[4] = temp_unk18;
    temp_a3[5] = temp_unk1C;

    arg0->unk20 = temp_v0->unk20;
    arg0->unk24 = temp_v0->unk24;
    arg0->unk25 = temp_v0->unk25;

    unsigned int var_a3 = 0;
    gcSimpleControllerBlock36 *var_a2 = arg0->unk28;
    gcSimpleControllerBlock36 *var_a1 = temp_v0->unk28;
    do {
        int temp_unk0 = var_a1->unk0;
        var_a2->unk0 = temp_unk0;
        unsigned char temp_unk4 = var_a1->unk4;
        var_a2->unk4 = temp_unk4;
        unsigned char temp_unk5 = var_a1->unk5;
        int *temp_t1 = &var_a1->unk8;
        var_a2->unk5 = temp_unk5;
        int *temp_t2 = &var_a2->unk8;
        int temp_unk8_2 = *temp_t1;
        *temp_t2 = temp_unk8_2;
        int *temp_t0 = (int *)((char *)var_a2 + 12);
        int *temp_t1_2 = (int *)((char *)var_a1 + 12);
        int temp_unkC_2 = *temp_t1_2;
        int *temp_t2_2 = (int *)((char *)var_a1 + 16);
        *temp_t0 = temp_unkC_2;
        int *temp_t0_2 = (int *)((char *)var_a2 + 16);
        int *temp_t2_3 = (int *)((char *)var_a1 + 20);
        int temp_unk10_2 = *temp_t2_2;
        int *temp_t0_3 = (int *)((char *)var_a2 + 20);
        *temp_t0_2 = temp_unk10_2;
        int *temp_t2_4 = (int *)((char *)var_a1 + 24);
        int temp_unk14_2 = *temp_t2_3;
        int *temp_t0_4 = (int *)((char *)var_a2 + 24);
        int *temp_t2_5 = (int *)((char *)var_a1 + 28);
        *temp_t0_3 = temp_unk14_2;
        int temp_unk18_2 = *temp_t2_4;
        int *temp_t0_5 = (int *)((char *)var_a2 + 28);
        *temp_t0_4 = temp_unk18_2;
        int temp_unk1C_2 = *temp_t2_5;
        *temp_t0_5 = temp_unk1C_2;
        int temp_unk20_2 = var_a1->unk20;
        var_a3 += 1;
        var_a2->unk20 = temp_unk20_2;
        var_a1 = (gcSimpleControllerBlock36 *)((char *)var_a1 + 0x24);
        var_a2 = (gcSimpleControllerBlock36 *)((char *)var_a2 + 0x24);
    } while (var_a3 < 2U);

    arg0->unk70 = temp_v0->unk70;
    arg0->unk74 = temp_v0->unk74;

    int *var_a2_2 = &temp_v0->unk78;
    int *var_t0 = &arg0->unk78;
    *var_t0 = *var_a2_2;
    arg0->unk7C = temp_v0->unk7C;

    int var_a3_2 = 0;
    gcSimpleControllerHalf3 *var_a2_3 = arg0->unk80;
    gcSimpleControllerHalf3 *var_a1_2 = temp_v0->unk80;
    do {
        short temp_unk0_2 = var_a1_2->unk0;
        short temp_unk2_2 = var_a1_2->unk2;
        short temp_unk4_2 = var_a1_2->unk4;
        var_a2_3->unk0 = temp_unk0_2;
        var_a2_3->unk2 = temp_unk2_2;
        var_a2_3->unk4 = temp_unk4_2;
        var_a3_2 += 1;
        var_a2_3 = (gcSimpleControllerHalf3 *)((char *)var_a2_3 + 6);
        var_a1_2 = (gcSimpleControllerHalf3 *)((char *)var_a1_2 + 6);
    } while (var_a3_2 <= 0);

    float temp_f12 = temp_v0->unk88;
    int *temp_a0_2 = (int *)((char *)temp_v0 + 0x8C);
    arg0->unk88 = temp_f12;
    int *temp_a1_2 = (int *)((char *)arg0 + 0x8C);
    *temp_a1_2 = *temp_a0_2;
}
```

### Exemplar 4: `gcStationaryController::AssignCopy(const cBase *)`

- Address: `0x0032064c`
- Size: `400` bytes
- Source: `src/gcStationaryController.cpp`

```cpp
void gcStationaryController::AssignCopy(const cBase *base) {
    int var_a3_2;
    unsigned int var_a3;
    gcStationaryControllerLayout *temp_v0;
    gcStationaryControllerBlock36 *var_a1;
    gcStationaryControllerBlock36 *var_a2;
    gcStationaryControllerHalf3 *var_a1_2;
    gcStationaryControllerHalf3 *var_a2_2;
    int *temp_a0;
    int *temp_a1_3;

    temp_v0 = (gcStationaryControllerLayout *)
        dcastdcast_gcStationaryControllerptr__constcBaseptr(base);

    gcStationaryControllerWords6 *src8 =
        (gcStationaryControllerWords6 *)((char *)temp_v0 + 8);
    gcStationaryControllerWords6 *dst8 =
        (gcStationaryControllerWords6 *)((char *)this + 8);
    int word8 = src8->unk0;
    int wordC = src8->unk4;
    int word10 = src8->unk8;
    dst8->unk0 = word8;
    int word14 = src8->unkC;
    dst8->unk4 = wordC;
    int word18 = src8->unk10;
    dst8->unk8 = word10;
    int word1C = src8->unk14;
    dst8->unkC = word14;
    dst8->unk10 = word18;
    dst8->unk14 = word1C;

    *(int *)((char *)this + 0x20) = *(int *)((char *)temp_v0 + 0x20);
    *(unsigned char *)((char *)this + 0x24) =
        *(unsigned char *)((char *)temp_v0 + 0x24);
    *(unsigned char *)((char *)this + 0x25) =
        *(unsigned char *)((char *)temp_v0 + 0x25);

    var_a3 = 0;
    var_a2 = (gcStationaryControllerBlock36 *)((char *)this + 0x28);
    var_a1 = (gcStationaryControllerBlock36 *)((char *)temp_v0 + 0x28);
    do {
        var_a2->unk0 = var_a1->unk0;
        var_a2->unk4 = var_a1->unk4;
        var_a2->unk5 = var_a1->unk5;
        int *temp_t1 = &var_a1->unk8;
        int *temp_t2 = &var_a2->unk8;
        int temp_unk8_2 = *temp_t1;
        *temp_t2 = temp_unk8_2;
        int *temp_t0 = (int *)((char *)var_a2 + 12);
        int *temp_t1_2 = (int *)((char *)var_a1 + 12);
        int temp_unkC_2 = *temp_t1_2;
        int *temp_t2_2 = (int *)((char *)var_a1 + 16);
        *temp_t0 = temp_unkC_2;
        int *temp_t0_2 = (int *)((char *)var_a2 + 16);
        int *temp_t2_3 = (int *)((char *)var_a1 + 20);
        int temp_unk10_2 = *temp_t2_2;
        int *temp_t0_3 = (int *)((char *)var_a2 + 20);
        *temp_t0_2 = temp_unk10_2;
        int *temp_t2_4 = (int *)((char *)var_a1 + 24);
        int temp_unk14_2 = *temp_t2_3;
        int *temp_t0_4 = (int *)((char *)var_a2 + 24);
        int *temp_t2_5 = (int *)((char *)var_a1 + 28);
        *temp_t0_3 = temp_unk14_2;
        int temp_unk18_2 = *temp_t2_4;
        int *temp_t0_5 = (int *)((char *)var_a2 + 28);
        *temp_t0_4 = temp_unk18_2;
        int temp_unk1C_2 = *temp_t2_5;
        *temp_t0_5 = temp_unk1C_2;
        var_a3 += 1;
        var_a2->unk20 = var_a1->unk20;
        var_a1 = (gcStationaryControllerBlock36 *)((char *)var_a1 + 0x24);
        var_a2 = (gcStationaryControllerBlock36 *)((char *)var_a2 + 0x24);
    } while (var_a3 < 2U);

    *(int *)((char *)this + 0x70) = *(int *)((char *)temp_v0 + 0x70);
    *(int *)((char *)this + 0x74) = *(int *)((char *)temp_v0 + 0x74);
    {
        int *src78 = (int *)((char *)temp_v0 + 0x78);
        int *dst78 = (int *)((char *)this + 0x78);

        *dst78 = *src78;
    }
    var_a3_2 = 0;
    var_a2_2 = (gcStationaryControllerHalf3 *)((char *)this + 0x80);
    *(int *)((char *)this + 0x7C) = *(int *)((char *)temp_v0 + 0x7C);
    var_a1_2 = (gcStationaryControllerHalf3 *)((char *)temp_v0 + 0x80);
    do {
        short a = var_a1_2->unk0;
        short b = var_a1_2->unk2;
        short c = var_a1_2->unk4;
        var_a2_2->unk0 = a;
        var_a2_2->unk2 = b;
        var_a2_2->unk4 = c;
        var_a3_2 += 1;
        var_a2_2 = (gcStationaryControllerHalf3 *)((char *)var_a2_2 + 6);
        var_a1_2 = (gcStationaryControllerHalf3 *)((char *)var_a1_2 + 6);
    } while (var_a3_2 <= 0);

    temp_a0 = (int *)((char *)temp_v0 + 0x8C);
    *(float *)((char *)this + 0x88) = *(float *)((char *)temp_v0 + 0x88);
    temp_a1_3 = (int *)((char *)this + 0x8C);
    *temp_a1_3 = *temp_a0;
}
```


## Prior Failure Notes

### Note 1

- session `90183265`

Implemented the full cType initialization, dynamic-cast parent walk, and final operator= call using the matched gcProfileString/gcTableString pattern adapted to D_0009F44C, D_0009F3F4, D_000385DC, type id 0x1A6, and gcDesiredEntityTemplate::New. The generated control flow is structurally identical, including the bnel parent-walk loop, but the best result remains 16/360 bytes different after relocation masking.

The residual diff is register allocation/scheduling only: SNC keeps the same operations but assigns the long-lived locals/cache globals in a different saved-register order around the prologue/type-cache block than the target. I tried the straight exemplar form, explicit self/src locals in multiple declaration orders, moving cType::mParent onto cType vs a separate cTypeNode, class declaration ordering, fixed-register local syntax, empty asm scheduling barriers, and explicit cache-pointer locals. A 300s permuter run generated 8376 candidates, compiled 3205, and found zero improvement from the 16-byte baseline.


## Disassembly

```asm
268ce8:	27bdffe0 	addiu	sp,sp,-32
268cec:	afb00000 	sw	s0,0(sp)
268cf0:	afb20008 	sw	s2,8(sp)
268cf4:	34120000 	li	s2,0x0
268cf8:	00808025 	move	s0,a0
268cfc:	afb10004 	sw	s1,4(sp)
268d00:	afb3000c 	sw	s3,12(sp)
268d04:	afb40010 	sw	s4,16(sp)
268d08:	afb50014 	sw	s5,20(sp)
268d0c:	afbf0018 	sw	ra,24(sp)
268d10:	10a00043 	beqz	a1,0x268e20
268d14:	00a08825 	move	s1,a1
268d18:	3c13000a 	lui	s3,0xa
268d1c:	8e64f44c 	lw	a0,-2996(s3)
268d20:	14800028 	bnez	a0,0x268dc4
268d24:	3c14000a 	lui	s4,0xa
268d28:	8e84f3f4 	lw	a0,-3084(s4)
268d2c:	1480001b 	bnez	a0,0x268d9c
268d30:	3c080027 	lui	t0,0x27
268d34:	3c150004 	lui	s5,0x4
268d38:	8ea485dc 	lw	a0,-31268(s5)
268d3c:	1480000c 	bnez	a0,0x268d70
268d40:	3c040037 	lui	a0,0x37
268d44:	3c050037 	lui	a1,0x37
268d48:	34060001 	li	a2,0x1
268d4c:	00003825 	move	a3,zero
268d50:	00004025 	move	t0,zero
268d54:	00004825 	move	t1,zero
268d58:	00005025 	move	t2,zero
268d5c:	00005825 	move	t3,zero
268d60:	2484d894 	addiu	a0,a0,-10092
268d64:	0c001d4d 	jal	0x7534
268d68:	24a5d89c 	addiu	a1,a1,-10084
268d6c:	aea285dc 	sw	v0,-31268(s5)
268d70:	8ea785dc 	lw	a3,-31268(s5)
268d74:	00002025 	move	a0,zero
268d78:	00002825 	move	a1,zero
268d7c:	3406012c 	li	a2,0x12c
268d80:	00004025 	move	t0,zero
268d84:	00004825 	move	t1,zero
268d88:	00005025 	move	t2,zero
268d8c:	0c001d4d 	jal	0x7534
268d90:	00005825 	move	t3,zero
268d94:	ae82f3f4 	sw	v0,-3084(s4)
268d98:	3c080027 	lui	t0,0x27
268d9c:	8e87f3f4 	lw	a3,-3084(s4)
268da0:	00002025 	move	a0,zero
268da4:	00002825 	move	a1,zero
268da8:	340601a6 	li	a2,0x1a6
268dac:	00004825 	move	t1,zero
268db0:	00005025 	move	t2,zero
268db4:	00005825 	move	t3,zero
268db8:	0c001d4d 	jal	0x7534
268dbc:	25088e50 	addiu	t0,t0,-29104
268dc0:	ae62f44c 	sw	v0,-2996(s3)
268dc4:	8e240004 	lw	a0,4(s1)
268dc8:	8e73f44c 	lw	s3,-2996(s3)
268dcc:	24840008 	addiu	a0,a0,8
268dd0:	84850000 	lh	a1,0(a0)
268dd4:	8c860004 	lw	a2,4(a0)
268dd8:	00c0f809 	jalr	a2
268ddc:	02252021 	addu	a0,s1,a1
268de0:	16600003 	bnez	s3,0x268df0
268de4:	00402025 	move	a0,v0
268de8:	1000000a 	b	0x268e14
268dec:	34040000 	li	a0,0x0
268df0:	50800008 	beqzl	a0,0x268e14
268df4:	34040000 	li	a0,0x0
268df8:	54930003 	bnel	a0,s3,0x268e08
268dfc:	8c84001c 	lw	a0,28(a0)
268e00:	10000004 	b	0x268e14
268e04:	34040001 	li	a0,0x1
268e08:	1480fffb 	bnez	a0,0x268df8
268e0c:	00000000 	nop
268e10:	34040000 	li	a0,0x0
268e14:	10800003 	beqz	a0,0x268e24
268e18:	02002025 	move	a0,s0
268e1c:	02209025 	move	s2,s1
268e20:	02002025 	move	a0,s0
268e24:	0c049dd7 	jal	0x12775c
268e28:	02402825 	move	a1,s2
268e2c:	8fb00000 	lw	s0,0(sp)
268e30:	8fb10004 	lw	s1,4(sp)
268e34:	8fb20008 	lw	s2,8(sp)
268e38:	8fb3000c 	lw	s3,12(sp)
268e3c:	8fb40010 	lw	s4,16(sp)
268e40:	8fb50014 	lw	s5,20(sp)
268e44:	8fbf0018 	lw	ra,24(sp)
268e48:	03e00008 	jr	ra
268e4c:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcDesiredEntityTemplate::AssignCopy(const cBase *)
// Address: 0x00268ce8, Size: 360B
// Obj: gcAll_psp.obj

s32 cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(?, ?, ?, s32); /* extern */
? gcDesiredEntityTemplate__operator_eq_constgcDesiredEntityTemplateref(s32, void *); /* extern */

void gcDesiredEntityTemplate__AssignCopy_constcBaseptr(s32 arg0, void *arg1) {
    s16 temp_a1;
    s32 temp_s3;
    s32 var_a0_2;
    void *(*temp_a2)(void *, s16, ?);
    void *temp_a0;
    void *var_a0;
    void *var_s2;

    var_s2 = NULL;
    if (arg1 != NULL) {
        if (*(s32 *)0x9F44C == 0) {
            if (*(s32 *)0x9F3F4 == 0) {
                if (*(s32 *)0x385DC == 0) {
                    *(s32 *)0x385DC = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0x36D894, 0x36D89C, 1, 0);
                }
                *(s32 *)0x9F3F4 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x12C, *(s32 *)0x385DC);
            }
            *(s32 *)0x9F44C = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x1A6, *(s32 *)0x9F3F4);
        }
        temp_a0 = arg1->unk4;
        temp_s3 = *(s32 *)0x9F44C;
        temp_a1 = temp_a0->unk8;
        temp_a2 = (temp_a0 + 8)->unk4;
        var_a0 = temp_a2(arg1 + temp_a1, temp_a1, temp_a2);
        if (temp_s3 == 0) {
            goto block_15;
        }
        if (var_a0 != NULL) {
loop_10:
            if (var_a0 != temp_s3) {
                var_a0 = var_a0->unk1C;
                if (var_a0 == NULL) {
                    goto block_15;
                }
                goto loop_10;
            }
            var_a0_2 = 1;
        } else {
block_15:
            var_a0_2 = 0;
        }
        if (var_a0_2 != 0) {
            var_s2 = arg1;
        }
    }
    gcDesiredEntityTemplate__operator_eq_constgcDesiredEntityTemplateref(arg0, var_s2);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
