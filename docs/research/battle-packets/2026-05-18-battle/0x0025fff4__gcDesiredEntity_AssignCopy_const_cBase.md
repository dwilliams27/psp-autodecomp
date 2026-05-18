# Battle Packet: `gcDesiredEntity::AssignCopy(const cBase *)`

## Target

- Address: `0x0025fff4`
- Size: `360` bytes
- Status: `failed`
- Object: `gcAll_psp.obj`
- Class: `gcDesiredEntity`
- Method family: `AssignCopy`
- Leaf: `False`

## Queue Metadata

- score: `135`
- priority: `1`
- reason: `has failure_notes; rich notes; near miss 19B; codegen-specific diagnosis; AssignCopy retry family; manageable size`
- next_run_reason: `lane 3 <=64B regalloc/quarantine overflow target`
- failure_action: `quarantine`
- near_miss_bytes: `19`

## Failure Classification

- Primary: `regalloc_drift`
- Action: `quarantine`
- Confidence: `high`
- Tags: `regalloc_drift`, `branch_direction_codegen`, `needs_layout`
- Near miss: `19`
- Summary: Register naming/live-range drift after structure is correct.
- Guidance: do not put this in ordinary matching runs without new research leverage.

## Graph Context

Callees:
- `0x00007534` `untried` 264B `cType::InitializeType(const char *, const char *, unsigned int, const cType *, cBase * (*)(cMemPool *, cBase *), const char *, const char *, unsigned int) static`
- `0x0011d404` `untried` 632B `gcDesiredEntity::operator=(const gcDesiredEntity &)`

## Placement

- Canonical source: `src/gcDesiredEntity.cpp`
- Header(s): `include/gcDesiredEntity.h`
- Allowed source(s): `src/gcDesiredEntity.cpp`
- Split-TU prefix: `src/gcDesiredEntity_*.cpp`

## Compiler Guidance

Copy template: copy fields in target byte order, not semantic group order. Split pointer/word copies when register allocation differs, and use matched siblings for exact field offsets.

## Class Header: `include/gcDesiredEntity.h`

Not found.

## Matched Same-Class Neighbors

- `0x0011d67c` 120B `gcDesiredEntity::Write(cFile &) const` — `src/gcDesiredEntity.cpp`
- `0x0011d6f4` 248B `gcDesiredEntity::Read(cFile &, cMemPool *)` — `src/gcDesiredEntity.cpp`
- `0x0011d7ec` 212B `gcDesiredEntity::GetText(char *) const` — `src/gcDesiredEntity.cpp`
- `0x0011df9c` 80B `gcDesiredEntity::HasCategory(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &) const` — `src/gcDesiredEntity.cpp`
- `0x0026015c` 224B `gcDesiredEntity::New(cMemPool *, cBase *) static` — `src/gcDesiredEntity.cpp`

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

- session `42680821`

Implemented gcDesiredEntity::AssignCopy as the canonical reflective dcast plus operator= call, using the same type-cache chain as the matched GetType/New methods: D_000385DC -> D_0009F3F4 -> D_0009F3F8 with type IDs 1, 0x12C, and 0x12D. The generated body is structurally correct and compiles to the expected 360B, but remains at 19/360 bytes different. Objdump shows the mismatch is almost entirely saved-register allocation: my closest build keeps this/base in s1/s0 and assigns the intermediate type-cache globals to s5/s4, while the target uses this/base in s0/s1 and D_0009F3F4/D_000385DC in s4/s5. The dynamic-cast loop, branch-likely shape, cType::InitializeType argument setup, and final operator= call otherwise line up.

I tried several manual restructurings aimed specifically at that register swap: adding explicit this/base locals, using only a source/base local, using explicit register locals, inserting an empty asm barrier on a self local, splitting the parent-link layout into a separate cTypeNode, and replacing the reused write-slot type with a dedicated DispatchEntry matching known AssignCopy exemplars. All either stayed at 19 bytes or regressed to 28-39 bytes. I also ran `python3 tools/permuter.py src/gcDesiredEntity.cpp 0x0025fff4 --time 180 --save-best`; it generated 4344 candidates, compiled 1759, and found no improvement over the 19-byte baseline.


## Disassembly

```asm
25fff4:	27bdffe0 	addiu	sp,sp,-32
25fff8:	afb00000 	sw	s0,0(sp)
25fffc:	afb20008 	sw	s2,8(sp)
260000:	34120000 	li	s2,0x0
260004:	00808025 	move	s0,a0
260008:	afb10004 	sw	s1,4(sp)
26000c:	afb3000c 	sw	s3,12(sp)
260010:	afb40010 	sw	s4,16(sp)
260014:	afb50014 	sw	s5,20(sp)
260018:	afbf0018 	sw	ra,24(sp)
26001c:	10a00043 	beqz	a1,0x26012c
260020:	00a08825 	move	s1,a1
260024:	3c13000a 	lui	s3,0xa
260028:	8e64f3f8 	lw	a0,-3080(s3)
26002c:	14800028 	bnez	a0,0x2600d0
260030:	3c14000a 	lui	s4,0xa
260034:	8e84f3f4 	lw	a0,-3084(s4)
260038:	1480001b 	bnez	a0,0x2600a8
26003c:	3c080026 	lui	t0,0x26
260040:	3c150004 	lui	s5,0x4
260044:	8ea485dc 	lw	a0,-31268(s5)
260048:	1480000c 	bnez	a0,0x26007c
26004c:	3c040037 	lui	a0,0x37
260050:	3c050037 	lui	a1,0x37
260054:	34060001 	li	a2,0x1
260058:	00003825 	move	a3,zero
26005c:	00004025 	move	t0,zero
260060:	00004825 	move	t1,zero
260064:	00005025 	move	t2,zero
260068:	00005825 	move	t3,zero
26006c:	2484d894 	addiu	a0,a0,-10092
260070:	0c001d4d 	jal	0x7534
260074:	24a5d89c 	addiu	a1,a1,-10084
260078:	aea285dc 	sw	v0,-31268(s5)
26007c:	8ea785dc 	lw	a3,-31268(s5)
260080:	00002025 	move	a0,zero
260084:	00002825 	move	a1,zero
260088:	3406012c 	li	a2,0x12c
26008c:	00004025 	move	t0,zero
260090:	00004825 	move	t1,zero
260094:	00005025 	move	t2,zero
260098:	0c001d4d 	jal	0x7534
26009c:	00005825 	move	t3,zero
2600a0:	ae82f3f4 	sw	v0,-3084(s4)
2600a4:	3c080026 	lui	t0,0x26
2600a8:	8e87f3f4 	lw	a3,-3084(s4)
2600ac:	00002025 	move	a0,zero
2600b0:	00002825 	move	a1,zero
2600b4:	3406012d 	li	a2,0x12d
2600b8:	00004825 	move	t1,zero
2600bc:	00005025 	move	t2,zero
2600c0:	00005825 	move	t3,zero
2600c4:	0c001d4d 	jal	0x7534
2600c8:	2508015c 	addiu	t0,t0,348
2600cc:	ae62f3f8 	sw	v0,-3080(s3)
2600d0:	8e240004 	lw	a0,4(s1)
2600d4:	8e73f3f8 	lw	s3,-3080(s3)
2600d8:	24840008 	addiu	a0,a0,8
2600dc:	84850000 	lh	a1,0(a0)
2600e0:	8c860004 	lw	a2,4(a0)
2600e4:	00c0f809 	jalr	a2
2600e8:	02252021 	addu	a0,s1,a1
2600ec:	16600003 	bnez	s3,0x2600fc
2600f0:	00402025 	move	a0,v0
2600f4:	1000000a 	b	0x260120
2600f8:	34040000 	li	a0,0x0
2600fc:	50800008 	beqzl	a0,0x260120
260100:	34040000 	li	a0,0x0
260104:	54930003 	bnel	a0,s3,0x260114
260108:	8c84001c 	lw	a0,28(a0)
26010c:	10000004 	b	0x260120
260110:	34040001 	li	a0,0x1
260114:	1480fffb 	bnez	a0,0x260104
260118:	00000000 	nop
26011c:	34040000 	li	a0,0x0
260120:	10800003 	beqz	a0,0x260130
260124:	02002025 	move	a0,s0
260128:	02209025 	move	s2,s1
26012c:	02002025 	move	a0,s0
260130:	0c047501 	jal	0x11d404
260134:	02402825 	move	a1,s2
260138:	8fb00000 	lw	s0,0(sp)
26013c:	8fb10004 	lw	s1,4(sp)
260140:	8fb20008 	lw	s2,8(sp)
260144:	8fb3000c 	lw	s3,12(sp)
260148:	8fb40010 	lw	s4,16(sp)
26014c:	8fb50014 	lw	s5,20(sp)
260150:	8fbf0018 	lw	ra,24(sp)
260154:	03e00008 	jr	ra
260158:	27bd0020 	addiu	sp,sp,32
```

## m2c Starting Point

```c
// Decompiled: gcDesiredEntity::AssignCopy(const cBase *)
// Address: 0x0025fff4, Size: 360B
// Obj: gcAll_psp.obj

s32 cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(?, ?, ?, s32); /* extern */
? gcDesiredEntity__operator_eq_constgcDesiredEntityref(s32, void *); /* extern */

void gcDesiredEntity__AssignCopy_constcBaseptr(s32 arg0, void *arg1) {
    s16 temp_a1;
    s32 temp_s3;
    s32 var_a0_2;
    void *(*temp_a2)(void *, s16, ?);
    void *temp_a0;
    void *var_a0;
    void *var_s2;

    var_s2 = NULL;
    if (arg1 != NULL) {
        if (*(s32 *)0x9F3F8 == 0) {
            if (*(s32 *)0x9F3F4 == 0) {
                if (*(s32 *)0x385DC == 0) {
                    *(s32 *)0x385DC = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0x36D894, 0x36D89C, 1, 0);
                }
                *(s32 *)0x9F3F4 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x12C, *(s32 *)0x385DC);
            }
            *(s32 *)0x9F3F8 = cType__InitializeType_constcharptr_constcharptr_unsignedint_constcTypeptr_cBaseptr_ptr_cMemPoolptr_cBaseptr_constcharptr_constcharptr_unsignedintstatic(0, 0, 0x12D, *(s32 *)0x9F3F4);
        }
        temp_a0 = arg1->unk4;
        temp_s3 = *(s32 *)0x9F3F8;
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
    gcDesiredEntity__operator_eq_constgcDesiredEntityref(arg0, var_s2);
}
```

## Agent Checklist

- Read the classification before retrying old approaches.
- Prefer same-class and same-method matched exemplars over generic m2c structure.
- Keep failed notes specific: byte count, structural match status, and suspected blocker.
- Verify with `python3 tools/compare_func.py <src> --no-update-db`.
