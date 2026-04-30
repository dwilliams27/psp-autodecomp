// gcNPCFlyingController — methods from gcAll_psp.obj.
//
// gcNPCFlyingController extends gcFlyingController. Write forwards to its
// gcFlyingController base via static dispatch (cFactory/gcFlyingController
// pattern). New() and the destructor follow the SNC ABI patterns documented
// in eConfigBase.cpp / eSphereShape_dtor.cpp: pool slot lookup at +0x28
// (alloc) / +0x30 (delete), and operator delete generates the deleting tail.

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

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short _pad;
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
    void Write(cFile &) const;
};

class gcCreatureController : public gcEntityController {
public:
    void Write(cFile &) const;
};

class gcFlyingController : public gcCreatureController {
public:
    gcFlyingController(cBase *);
    ~gcFlyingController();
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcNPCFlyingController : public gcFlyingController {
public:
    gcNPCFlyingController(cBase *);
    ~gcNPCFlyingController();
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        rec->fn(block + off, p);
    }
};

extern char gcNPCFlyingControllervirtualtable[];
static cType *type_cBase asm("D_000385DC");
static cType *type_cNamed asm("D_000385E0");
static cType *type_gcEntityController asm("D_0009A404");
static cType *type_gcCreatureController asm("D_0009F5A8");
static cType *type_gcFlyingController asm("D_0009F780");
static cType *type_gcNPCFlyingController asm("D_0009F79C");

typedef int v4sf_t __attribute__((mode(V4SF)));

struct CopyChunk36 {
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

struct Half3 {
    short unk0;
    short unk2;
    short unk4;
};

extern "C" {
    void gcNPCFlyingController__gcNPCFlyingController_cBaseptr(void *self, cBase *parent);
}

extern "C" gcNPCFlyingController *dcast_gcNPCFlyingController(const cBase *)
    asm("__0FFdcast7P6VgcNPCFlyingController_PC6FcBase_9BA");

// ── gcNPCFlyingController::Read(cFile &, cMemPool *) @ 0x00155380 ──
int gcNPCFlyingController::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcFlyingController::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcNPCFlyingController::Write(cFile &) const @ 0x00155334 ──
void gcNPCFlyingController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcFlyingController::Write(file);
    wb.End();
}

// ── gcNPCFlyingController::gcNPCFlyingController(cBase *) @ 0x0015543c ──
gcNPCFlyingController::gcNPCFlyingController(cBase *parent)
    : gcFlyingController(parent) {
    void *vtable = gcNPCFlyingControllervirtualtable;
    __asm__ volatile("mtc1 $0, $f12" ::: "$f12", "memory");
    *(void **)((char *)this + 4) = vtable;
    __asm__ volatile(
        "mfc1 $5, $f12\n"
        "mfc1 $6, $f12\n"
        "mfc1 $4, $f12\n"
        "mtv $5, S120\n"
        "mtv $4, S121\n"
        "mtv $6, S122\n"
        "sv.q C120, 0xA0(%0)\n"
        :
        : "r"(this)
        : "$4", "$5", "$6", "memory");
}

// ── gcNPCFlyingController::AssignCopy(const cBase *) @ 0x0031d810 ──
void gcNPCFlyingController::AssignCopy(const cBase *src) {
    gcNPCFlyingController *other = dcast_gcNPCFlyingController(src);

    int *src8 = (int *)((char *)other + 8);
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

    *(int *)((char *)this + 0x20) = *(int *)((char *)other + 0x20);
    *(unsigned char *)((char *)this + 0x24) =
        *(unsigned char *)((char *)other + 0x24);
    *(unsigned char *)((char *)this + 0x25) =
        *(unsigned char *)((char *)other + 0x25);

    unsigned int i = 0;
    char *dst = (char *)this + 0x28;
    char *srcp = (char *)other + 0x28;
    do {
        CopyChunk36 *dstc = (CopyChunk36 *)dst;
        CopyChunk36 *srcc = (CopyChunk36 *)srcp;
        int word0 = srcc->unk0;
        char *src8 = srcp + 0x08;
        dstc->unk0 = word0;
        dstc->unk4 = srcc->unk4;
        char *dst8 = dst + 0x08;
        dstc->unk5 = srcc->unk5;
        int word8 = *(int *)(src8 + 0x00);
        *(int *)(dst8 + 0x00) = word8;
        char *srcC = srcp + 0x0C;
        int wordC = *(int *)(srcC + 0x00);
        char *dstC = dst + 0x0C;
        *(int *)(dstC + 0x00) = wordC;
        char *src10 = srcp + 0x10;
        int word10 = *(int *)(src10 + 0x00);
        char *dst10 = dst + 0x10;
        *(int *)(dst10 + 0x00) = word10;
        char *src14 = srcp + 0x14;
        int word14 = *(int *)(src14 + 0x00);
        char *dst14 = dst + 0x14;
        *(int *)(dst14 + 0x00) = word14;
        char *src18 = srcp + 0x18;
        int word18 = *(int *)(src18 + 0x00);
        char *dst18 = dst + 0x18;
        *(int *)(dst18 + 0x00) = word18;
        char *src1C = srcp + 0x1C;
        int word1C = *(int *)(src1C + 0x00);
        char *dst1C = dst + 0x1C;
        int word20 = *(int *)(srcp + 0x20);
        i += 1;
        *(int *)(dst1C + 0x00) = word1C;
        *(int *)(dst + 0x20) = word20;
        srcp += 0x24;
        dst += 0x24;
    } while (i < 2);

    *(int *)((char *)this + 0x70) = *(int *)((char *)other + 0x70);
    *(int *)((char *)this + 0x74) = *(int *)((char *)other + 0x74);
    int *src78 = (int *)((char *)other + 0x78);
    *(int *)((char *)this + 0x78) = *src78;
    int j = 0;
    *(int *)((char *)this + 0x7C) = *(int *)((char *)other + 0x7C);
    Half3 *dsth = (Half3 *)((char *)this + 0x80);
    Half3 *srch = (Half3 *)((char *)other + 0x80);
    do {
        short half0 = srch->unk0;
        short half2 = srch->unk2;
        short half4 = srch->unk4;
        dsth->unk0 = half0;
        dsth->unk2 = half2;
        dsth->unk4 = half4;
        j += 1;
        dsth = (Half3 *)((char *)dsth + 6);
        srch = (Half3 *)((char *)srch + 6);
    } while (j <= 0);

    *(float *)((char *)this + 0x88) = *(float *)((char *)other + 0x88);
    int *src8C = (int *)((char *)other + 0x8C);
    int *dst8C = (int *)((char *)this + 0x8C);
    *dst8C = *src8C;
    int k = 0;
    *(unsigned short *)((char *)this + 0x90) =
        *(unsigned short *)((char *)other + 0x90);
    Half3 *dsth2 = (Half3 *)((char *)this + 0x94);
    *(short *)((char *)this + 0x92) = *(short *)((char *)other + 0x92);
    Half3 *srch2 = (Half3 *)((char *)other + 0x94);
    do {
        short half0 = srch2->unk0;
        short half2 = srch2->unk2;
        short half4 = srch2->unk4;
        dsth2->unk0 = half0;
        dsth2->unk2 = half2;
        dsth2->unk4 = half4;
        k += 1;
        dsth2 = (Half3 *)((char *)dsth2 + 6);
        srch2 = (Half3 *)((char *)srch2 + 6);
    } while (k <= 0);

    *(v4sf_t *)((char *)this + 0xA0) = *(v4sf_t *)((char *)other + 0xA0);
}

// ── gcNPCFlyingController::New(cMemPool *, cBase *) static @ 0x0031d9ec ──
cBase *gcNPCFlyingController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcNPCFlyingController *result = 0;
    gcNPCFlyingController *obj = (gcNPCFlyingController *)rec->fn(base, 0xB0, 0x10, 0, 0);
    if (obj != 0) {
        gcNPCFlyingController__gcNPCFlyingController_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcNPCFlyingController::GetType(void) const @ 0x0031da68 ──
const cType *gcNPCFlyingController::GetType(void) const {
    if (type_gcNPCFlyingController == 0) {
        if (type_gcFlyingController == 0) {
            if (type_gcCreatureController == 0) {
                if (type_gcEntityController == 0) {
                    if (type_cNamed == 0) {
                        if (type_cBase == 0) {
                            type_cBase = cType::InitializeType(
                                (const char *)0x36D894, (const char *)0x36D89C,
                                1, 0, 0, 0, 0, 0);
                        }
                        type_cNamed = cType::InitializeType(
                            0, 0, 2, type_cBase,
                            (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
                    }
                    type_gcEntityController = cType::InitializeType(
                        0, 0, 0x99, type_cNamed, 0, 0, 0, 0);
                }
                type_gcCreatureController = cType::InitializeType(
                    0, 0, 0xB9, type_gcEntityController, 0, 0, 0, 0);
            }
            type_gcFlyingController = cType::InitializeType(
                0, 0, 0x1AD, type_gcCreatureController, 0, 0, 0, 0);
        }
        type_gcNPCFlyingController = cType::InitializeType(
            0, 0, 0x1AF, type_gcFlyingController, &gcNPCFlyingController::New,
            0, 0, 0);
    }
    return type_gcNPCFlyingController;
}

// ── gcNPCFlyingController::~gcNPCFlyingController(void) @ 0x0031dc00 ──
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~gcFlyingController (which is inlined: it just resets
// the vtable, then chains to ~gcEntityController at 0x00110958), and the
// deleting tail dispatch through operator delete. After dead-store elim,
// only the gcFlyingController vtable set survives in the emitted asm
// (the gcNPCFlyingController vtable store is dead-stored away).
gcNPCFlyingController::~gcNPCFlyingController() {
    *(void **)((char *)this + 4) = gcNPCFlyingControllervirtualtable;
}
