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
class mOCS;

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

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

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
    void OnSnappedTo(const mOCS &, unsigned int)
        __asm__("__0fRgcBipedControllerLOnSnappedToRC6EmOCSb");
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcPCBipedController : public gcBipedController {
public:
    gcPCBipedController(cBase *);
    ~gcPCBipedController();
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void OnSnappedTo(const mOCS &, unsigned int)
        __asm__("__0fTgcPCBipedControllerLOnSnappedToRC6EmOCSb");
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern "C" {
    void gcPCBipedController__gcPCBipedController_cBaseptr(void *self, cBase *parent);
    void gcEntityController___dtor_gcEntityController_void(void *self, int flags);
    void *cMemPool_GetPoolFromPtr(const void *);
    void gcBipedController_OnSnappedTo_raw(gcBipedController *, const mOCS &, unsigned int)
        __asm__("__0fRgcBipedControllerLOnSnappedToRC6EmOCSb");
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

// ── gcPCBipedController::gcPCBipedController(cBase *) @ 0x0015285c ──
gcPCBipedController::gcPCBipedController(cBase *parent)
    : gcBipedController(parent) {
    void *vtable = gcPCBipedControllervirtualtable;
    __asm__ volatile("mtc1 $0, $f12" ::: "$f12", "memory");
    *(void **)((char *)this + 4) = vtable;
    __asm__ volatile(
        "mfc1 $5, $f12\n"
        "mfc1 $6, $f12\n"
        "mfc1 $4, $f12\n"
        "mtv $5, S120\n"
        "mtv $4, S121\n"
        "mtv $6, S122\n"
        "sv.q C120, 0xC0(%0)\n"
        :
        : "r"(this)
        : "$4", "$5", "$6", "memory");
}

// ── gcPCBipedController::Write(cFile &) const @ 0x00152754 ──
void gcPCBipedController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcBipedController *)this)->Write(file);
    wb.End();
}

// ── gcPCBipedController::OnSnappedTo(const mOCS &, bool) @ 0x00152f88 ──
void gcPCBipedController::OnSnappedTo(const mOCS &ocs, unsigned int snapped) {
    unsigned int doReset = snapped & 0xFF;
    gcBipedController_OnSnappedTo_raw(this, ocs, doReset);
    if (doReset != 0) {
        __asm__ volatile(
            "mtc1 $0, $f12\n"
            "mfc1 $4, $f12\n"
            "mfc1 $5, $f12\n"
            "mfc1 $6, $f12\n"
            "mtv $4, S120\n"
            "mtv $5, S121\n"
            "mtv $6, S122\n"
            "sv.q C120, 0xC0(%0)\n"
            :
            : "r"(this)
            : "$4", "$5", "$6", "$f12", "memory");
    }
}

// ── gcPCBipedController::Read(cFile &, cMemPool *) @ 0x001527a0 ──
int gcPCBipedController::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcBipedController::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
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
        register unsigned char *srcNext __asm__("$9");
        register unsigned char *dstNext __asm__("$10");
        int word0 = *(int *)(src1 + 0x00);
        srcNext = src1 + 0x08;
        dstNext = dst1 + 0x08;
        *(int *)(dst1 + 0x00) = word0;
        *(unsigned char *)(dst1 + 0x04) = *(unsigned char *)(src1 + 0x04);
        *(unsigned char *)(dst1 + 0x05) = *(unsigned char *)(src1 + 0x05);
        __asm__ volatile("" : "+r"(srcNext), "+r"(dstNext) :: "memory");
        int word8 = *(int *)srcNext;
        srcNext = src1 + 0x0C;
        *(int *)dstNext = word8;
        register int wordNext __asm__("$9");
        register unsigned char *dstAfter __asm__("$8");
        register unsigned char *srcAfter __asm__("$10");
        wordNext = *(int *)srcNext;
        dstAfter = dst1 + 0x0C;
        srcAfter = src1 + 0x10;
        *(int *)dstAfter = wordNext;
        wordNext = *(int *)srcAfter;
        dstAfter = dst1 + 0x10;
        srcAfter = src1 + 0x14;
        *(int *)dstAfter = wordNext;
        wordNext = *(int *)srcAfter;
        dstAfter = dst1 + 0x14;
        srcAfter = src1 + 0x18;
        *(int *)dstAfter = wordNext;
        wordNext = *(int *)srcAfter;
        dstAfter = dst1 + 0x18;
        srcAfter = src1 + 0x1C;
        *(int *)dstAfter = wordNext;
        wordNext = *(int *)srcAfter;
        dstAfter = dst1 + 0x1C;
        *(int *)dstAfter = wordNext;
        int word20 = ((int *)src1)[8];
        i += 1;
        ((int *)dst1)[8] = word20;
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
