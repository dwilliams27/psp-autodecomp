// gcSimpleController.cpp — decompiled from gcAll_psp.obj
// gcSimpleController inherits from gcEntityController. Vtable at 0x38D260.
// All three methods follow the canonical pattern shared with eSoundDataMap
// (Write/dtor/New): same allocation slot layout, same delete record, and the
// canonical C++ destructor body that lets SNC's ABI auto-generate the
// (this != 0) guard, the base-chain call to ~gcEntityController(), and the
// deleting-tail dispatch via operator delete.

class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct gcSimpleControllerWords6 {
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
};

struct gcSimpleControllerBlock36 {
    int unk0;
    unsigned char unk4;
    unsigned char unk5;
    short _pad;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
};

struct gcSimpleControllerHalf3 {
    short unk0;
    short unk2;
    short unk4;
};

struct gcSimpleControllerLayout {
    char _pad0[8];
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    unsigned char unk24;
    unsigned char unk25;
    short _pad26;
    gcSimpleControllerBlock36 unk28[2];
    int unk70;
    int unk74;
    int unk78;
    int unk7C;
    gcSimpleControllerHalf3 unk80[1];
    float unk88;
    int unk8C;
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

extern "C" void gcEntityController__gcEntityController_cBaseptr(void *, cBase *);

class gcEntityController {
public:
    char _pad[0x90];
    gcEntityController(cBase *);
    ~gcEntityController();
    void Write(cFile &) const;
};

class gcSimpleController : public gcEntityController {
public:
    gcSimpleController(cBase *);
    ~gcSimpleController();
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static gcSimpleController *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char gcSimpleControllervirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009A404;
extern cType *D_0009F7C4;

extern "C" void *dcastdcast_gcSimpleControllerptr__constcBaseptr(const cBase *);

// ── gcSimpleController::Write(cFile &) const @ 0x00157f64 ──
void gcSimpleController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcEntityController::Write(file);
    wb.End();
}

// ── gcSimpleController::~gcSimpleController(void) @ 0x0032032c ──
gcSimpleController::~gcSimpleController() {
    *(void **)((char *)this + 4) = gcSimpleControllervirtualtable;
}

// ── gcSimpleController::New(cMemPool *, cBase *) static @ 0x00320184 ──
gcSimpleController *gcSimpleController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcSimpleController *result = 0;
    gcSimpleController *obj = (gcSimpleController *)e->fn(base, 0x90, 4, 0, 0);
    if (obj != 0) {
        gcEntityController__gcEntityController_cBaseptr(obj, parent);
        *(void **)((char *)obj + 4) = gcSimpleControllervirtualtable;
        result = obj;
    }
    return result;
}

const cType *gcSimpleController::GetType(void) const {
    if (D_0009F7C4 == 0) {
        if (D_0009A404 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC,
                    (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
            }
            D_0009A404 = cType::InitializeType(
                0, 0, 0x99, D_000385E0, 0, 0, 0, 0);
        }
        D_0009F7C4 = cType::InitializeType(
            0, 0, 0xF4, D_0009A404,
            (cBase *(*)(cMemPool *, cBase *))&gcSimpleController::New,
            0, 0, 0);
    }
    return D_0009F7C4;
}

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
