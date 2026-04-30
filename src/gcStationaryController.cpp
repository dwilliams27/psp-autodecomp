// gcStationaryController — methods from gcAll_psp.obj.

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

class cTimeValue {
public:
    int mTime;
};

class gcEntityController {
public:
    char _pad[0x80];
    gcEntityController(cBase *);
    ~gcEntityController();
    void HandleNextAnimation(cTimeValue);
    void Write(cFile &) const;
};

class gcStationaryController : public gcEntityController {
public:
    gcStationaryController(cBase *);
    ~gcStationaryController();
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Move(cTimeValue);
    void Write(cFile &) const;
    static gcStationaryController *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char gcStationaryControllervirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009A404;
extern cType *D_0009F7CC;

extern "C" void gcEntityController_gcEntityController(void *, cBase *);
extern "C" void *dcastdcast_gcStationaryControllerptr__constcBaseptr(
    const cBase *);

struct gcStationaryControllerWords6 {
    int unk0;
    int unk4;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
};

struct gcStationaryControllerBlock36 {
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

struct gcStationaryControllerHalf3 {
    short unk0;
    short unk2;
    short unk4;
};

struct gcStationaryControllerLayout {
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
    gcStationaryControllerBlock36 unk28[2];
    int unk70;
    int unk74;
    int unk78;
    int unk7C;
    gcStationaryControllerHalf3 unk80[1];
    float unk88;
    int unk8C;
};

// ── Move (0x00158504) ──
void gcStationaryController::Move(cTimeValue t) {
    HandleNextAnimation(t);
}

// ── Write (0x001583fc) ──
void gcStationaryController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcEntityController::Write(file);
    wb.End();
}

// ── ~gcStationaryController (0x00320984) ──
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~gcEntityController(this, 0), and the deleting-tail
// dispatch through operator delete (which inlines the pool/block lookup).
gcStationaryController::~gcStationaryController() {
    *(void **)((char *)this + 4) = gcStationaryControllervirtualtable;
}

// ── New (0x003207dc) ──
gcStationaryController *gcStationaryController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcStationaryController *result = 0;
    gcStationaryController *obj = (gcStationaryController *)e->fn(base, 0x90, 4, 0, 0);
    if (obj != 0) {
        gcEntityController_gcEntityController(obj, parent);
        *(void **)((char *)obj + 4) = gcStationaryControllervirtualtable;
        result = obj;
    }
    return result;
}

const cType *gcStationaryController::GetType(void) const {
    if (D_0009F7CC == 0) {
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
        D_0009F7CC = cType::InitializeType(
            0, 0, 0x103, D_0009A404,
            (cBase *(*)(cMemPool *, cBase *))&gcStationaryController::New,
            0, 0, 0);
    }
    return D_0009F7CC;
}

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
        int *src_words = (int *)((char *)var_a1 + 8);
        int *dst_words = (int *)((unsigned char *)var_a2 + 8);
        dst_words[0] = src_words[0];
        int *dst_words_2 = (int *)((char *)var_a2 + 12);
        int *src_words_2 = (int *)((char *)var_a1 + 12);
        dst_words_2[0] = src_words_2[0];
        int *src_words_3 = (int *)((char *)var_a1 + 16);
        __asm__ volatile("" ::: "memory");
        dst_words = (int *)((char *)var_a2 + 16);
        dst_words[0] = src_words_3[0];
        dst_words_2 = (int *)((char *)var_a2 + 20);
        int *src_words_4 = (int *)((char *)var_a1 + 20);
        int *src_words_5 = (int *)((char *)var_a1 + 24);
        dst_words = (int *)((char *)var_a2 + 24);
        int *src_words_6 = (int *)((char *)var_a1 + 28);
        dst_words_2[0] = src_words_4[0];
        dst_words_2 = (int *)((char *)var_a2 + 28);
        dst_words[0] = src_words_5[0];
        dst_words_2[0] = src_words_6[0];
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
