// gcPCBipedController — gcAll_psp.obj.
//
// Functions:
//   gcPCBipedController::Write(cFile &) const               @ 0x00152754  (76B)
//   gcPCBipedController::New(cMemPool *, cBase *) static    @ 0x0031a4c8 (124B)
//   gcPCBipedController::~gcPCBipedController(void)         @ 0x0031a6dc (124B)
//
// gcPCBipedController inherits from gcBipedController which inherits from
// gcCreatureController -> gcEntityController.

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

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcEntityController {
public:
    gcEntityController(cBase *);
    ~gcEntityController();
};

class gcCreatureController : public gcEntityController {
public:
    gcCreatureController(cBase *);
    ~gcCreatureController();
};

class gcBipedController : public gcCreatureController {
public:
    gcBipedController(cBase *);
    ~gcBipedController();
    void Write(cFile &) const;
};

class gcPCBipedController : public gcBipedController {
public:
    gcPCBipedController(cBase *);
    ~gcPCBipedController();
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern "C" {
    void gcPCBipedController__gcPCBipedController_cBaseptr(void *self, cBase *parent);
    void gcEntityController___dtor_gcEntityController_void(void *self, int flags);
    void *cMemPool_GetPoolFromPtr(const void *);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

template <class T> T *dcast(const cBase *);

extern char gcPCBipedControllervirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009A404;
extern cType *D_0009F5A8;
extern cType *D_0009F600;
extern cType *D_0009F770;

// ── gcPCBipedController::Write(cFile &) const @ 0x00152754 ──
void gcPCBipedController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcBipedController *)this)->Write(file);
    wb.End();
}

// ── gcPCBipedController::New(cMemPool *, cBase *) static @ 0x0031a4c8 ──
cBase *gcPCBipedController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcPCBipedController *result = 0;
    gcPCBipedController *obj = (gcPCBipedController *)rec->fn(base, 0xD0, 0x10, 0, 0);
    if (obj != 0) {
        gcPCBipedController__gcPCBipedController_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcPCBipedController::GetType(void) const @ 0x0031a544 ──
const cType *gcPCBipedController::GetType(void) const {
    if (D_0009F770 == 0) {
        if (D_0009F600 == 0) {
            if (D_0009F5A8 == 0) {
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
                D_0009F5A8 = cType::InitializeType(
                    0, 0, 0xB9, D_0009A404, 0, 0, 0, 0);
            }
            D_0009F600 = cType::InitializeType(
                0, 0, 0xBB, D_0009F5A8, 0, 0, 0, 0);
        }
        D_0009F770 = cType::InitializeType(
            0, 0, 0xBD, D_0009F600, &gcPCBipedController::New, 0, 0, 0);
    }
    return D_0009F770;
}

struct gcPCBipedController_half3 {
    short x;
    short y;
    short z;
};

struct gcPCBipedController_block36 {
    int unk0;
    unsigned char unk4;
    unsigned char unk5;
    unsigned short pad6;
    int unk8;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
};

typedef int gcPCBipedController_v4sf_t __attribute__((mode(V4SF)));

struct gcPCBipedController_layout {
    char pad00[8];
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    unsigned char unk24;
    unsigned char unk25;
    unsigned short pad26;
    gcPCBipedController_block36 unk28[2];
    int unk70;
    int unk74;
    int unk78;
    int unk7C;
    gcPCBipedController_half3 unk80[1];
    float unk88;
    int unk8C;
    unsigned short unk90;
    char pad92[0xE];
    gcPCBipedController_v4sf_t unkA0;
    int unkB0;
    float unkB4;
    float unkB8;
    char padBC[4];
    gcPCBipedController_v4sf_t unkC0;
};

// ── gcPCBipedController::AssignCopy(const cBase *) @ 0x0031a300 ──
void gcPCBipedController::AssignCopy(const cBase *base) {
    gcPCBipedController_layout *self = (gcPCBipedController_layout *)this;
    gcPCBipedController_layout *other =
        (gcPCBipedController_layout *)dcast<gcPCBipedController>(base);

    int *src0 = &other->unk08;
    int *dst0 = &self->unk08;
    int word08 = src0[0];
    int word0C = src0[1];
    int word10 = src0[2];
    dst0[0] = word08;
    int word14 = src0[3];
    dst0[1] = word0C;
    int word18 = src0[4];
    dst0[2] = word10;
    int word1C = src0[5];
    dst0[3] = word14;
    dst0[4] = word18;
    dst0[5] = word1C;

    self->unk20 = other->unk20;
    self->unk24 = other->unk24;
    self->unk25 = other->unk25;

    unsigned int i = 0;
    unsigned char *dst1 = (unsigned char *)this + 0x28;
    unsigned char *src1 = (unsigned char *)other + 0x28;
    do {
        int word0 = *(int *)(src1 + 0x00);
        unsigned char *src8 = src1 + 0x08;
        unsigned char *dst8 = dst1 + 0x08;
        *(int *)(dst1 + 0x00) = word0;
        *(unsigned char *)(dst1 + 0x04) = *(unsigned char *)(src1 + 0x04);
        *(unsigned char *)(dst1 + 0x05) = *(unsigned char *)(src1 + 0x05);
        int word8 = *(int *)(src8 + 0x00);
        *(int *)(dst8 + 0x00) = word8;
        unsigned char *srcC = src1 + 0x0C;
        int wordC = *(int *)(srcC + 0x00);
        unsigned char *src10 = src1 + 0x10;
        unsigned char *dstC = dst1 + 0x0C;
        *(int *)(dstC + 0x00) = wordC;
        int word10 = *(int *)(src10 + 0x00);
        unsigned char *src14 = src1 + 0x14;
        unsigned char *dst10 = dst1 + 0x10;
        *(int *)(dst10 + 0x00) = word10;
        int word14 = *(int *)(src14 + 0x00);
        unsigned char *src18 = src1 + 0x18;
        unsigned char *dst14 = dst1 + 0x14;
        *(int *)(dst14 + 0x00) = word14;
        int word18 = *(int *)(src18 + 0x00);
        unsigned char *src1C = src1 + 0x1C;
        unsigned char *dst18 = dst1 + 0x18;
        *(int *)(dst18 + 0x00) = word18;
        int word1C = *(int *)(src1C + 0x00);
        unsigned char *dst1C = dst1 + 0x1C;
        *(int *)(dst1C + 0x00) = word1C;
        unsigned char *dst20 = dst1 + 0x20;
        int word20 = *(int *)(src1 + 0x20);
        i += 1;
        *(int *)(dst20 + 0x00) = word20;
        src1 += 0x24;
        dst1 += 0x24;
    } while (i < 2U);

    *(int *)((char *)this + 0x70) = *(int *)((char *)other + 0x70);
    int *src78 = (int *)((char *)other + 0x78);
    *(int *)((char *)this + 0x74) = *(int *)((char *)other + 0x74);
    int *dst78 = (int *)((char *)this + 0x78);
    *dst78 = src78[0];
    int j = 0;
    gcPCBipedController_half3 *dst2 = (gcPCBipedController_half3 *)((char *)this + 0x80);
    *(int *)((char *)this + 0x7C) = *(int *)((char *)other + 0x7C);
    gcPCBipedController_half3 *src2 = (gcPCBipedController_half3 *)((char *)other + 0x80);

    do {
        short x = src2->x;
        short y = src2->y;
        short z = src2->z;
        dst2->x = x;
        dst2->y = y;
        dst2->z = z;
        j += 1;
        dst2 += 1;
        src2 += 1;
    } while (j <= 0);

    float f88 = *(float *)((char *)other + 0x88);
    int *src8C = (int *)((char *)other + 0x8C);
    *(float *)((char *)this + 0x88) = f88;
    int *dst8C = (int *)((char *)this + 0x8C);
    *dst8C = src8C[0];
    *(unsigned short *)((char *)this + 0x90) = *(unsigned short *)((char *)other + 0x90);
    *(gcPCBipedController_v4sf_t *)((char *)this + 0xA0) =
        *(gcPCBipedController_v4sf_t *)((char *)other + 0xA0);
    int *srcB0 = (int *)((char *)other + 0xB0);
    int *dstB0 = (int *)((char *)this + 0xB0);
    *dstB0 = srcB0[0];
    *(float *)((char *)this + 0xB4) = *(float *)((char *)other + 0xB4);
    *(float *)((char *)this + 0xB8) = *(float *)((char *)other + 0xB8);
    *(gcPCBipedController_v4sf_t *)((char *)this + 0xC0) =
        *(gcPCBipedController_v4sf_t *)((char *)other + 0xC0);
}

// ── gcPCBipedController::~gcPCBipedController(void) @ 0x0031a6dc ──
extern "C" {

void gcPCBipedController___dtor_gcPCBipedController_void(void *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = gcPCBipedControllervirtualtable;
        gcEntityController___dtor_gcEntityController_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            char *allocTable = *(char **)((char *)block + 0x1C);
            DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

}
