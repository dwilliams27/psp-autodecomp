// gcProjectileController.cpp — decompiled from gcAll_psp.obj
// gcProjectileController inherits from gcEntityController. Vtable at 0x38CED8.

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

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

extern "C" void gcEntityController_gcEntityController(void *, cBase *);

class gcEntityController {
public:
    char _pad[0x90];
    gcEntityController(cBase *);
    ~gcEntityController();
    void Write(cFile &) const;
};

class gcProjectileController : public gcEntityController {
public:
    gcProjectileController(cBase *);
    ~gcProjectileController();
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static gcProjectileController *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char gcProjectileControllervirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009A404;
extern cType *D_0009F7AC;

extern "C" void *dcastdcast_gcProjectileControllerptr__constcBaseptr(
    const cBase *);

struct gcProjectileControllerWord6 {
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
};

struct gcProjectileControllerBlock24 {
    int unk0;
    unsigned char unk4;
    unsigned char unk5;
    char pad6[2];
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
};

struct gcProjectileControllerHalf3 {
    short unk0;
    short unk2;
    short unk4;
};

// ── gcProjectileController::Write(cFile &) const @ 0x00155ca0 ──
void gcProjectileController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcEntityController::Write(file);
    wb.End();
}

// ── gcProjectileController::~gcProjectileController(void) @ 0x0031f168 ──
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the ~gcEntityController() base-chain call (with flags=0), and the
// deleting-tail `if (flags & 1) operator delete(this)` epilogue.
gcProjectileController::~gcProjectileController() {
    *(void **)((char *)this + 4) = gcProjectileControllervirtualtable;
}

// ── gcProjectileController::New(cMemPool *, cBase *) static @ 0x0031efc0 ──
gcProjectileController *gcProjectileController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcProjectileController *result = 0;
    gcProjectileController *obj = (gcProjectileController *)e->fn(base, 0x90, 4, 0, 0);
    if (obj != 0) {
        gcEntityController_gcEntityController(obj, parent);
        *(void **)((char *)obj + 4) = gcProjectileControllervirtualtable;
        result = obj;
    }
    return result;
}

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
        dst_words_3 = (int *)((char *)var_a2 + 28);
        dst_words_3[0] = src_words_6[0];
        var_a3 += 1;
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

// ── gcProjectileController::GetType(void) const @ 0x0031f048 ──
const cType *gcProjectileController::GetType(void) const {
    if (D_0009F7AC == 0) {
        if (D_0009A404 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_0009A404 = cType::InitializeType(0, 0, 0x99, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_0009F7AC = cType::InitializeType(
            0, 0, 0xF2, D_0009A404,
            (cBase *(*)(cMemPool *, cBase *))&gcProjectileController::New,
            0, 0, 0);
    }
    return D_0009F7AC;
}
