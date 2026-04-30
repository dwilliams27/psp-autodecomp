// gcNPCBipedController — gcAll_psp.obj.
//
// Functions:
//   gcNPCBipedController::Write(cFile &) const            @ 0x00154d70  (76B)
//   gcNPCBipedController::New(cMemPool *, cBase *) static @ 0x0031d25c (124B)
//   gcNPCBipedController::~gcNPCBipedController(void)     @ 0x0031d470 (124B)
//
// The constructor, Write, and Read forward through gcBipedController, but the
// destructor chain at 0x31d49c jumps to ~gcEntityController.  The local
// gcBipedController declaration below aliases its destructor accordingly so the
// canonical C++ methods produce the observed mixed chain.

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
};

class gcBipedController : public gcEntityController {
public:
    gcBipedController(cBase *);
    ~gcBipedController() __asm__("gcEntityController___dtor_gcEntityController_void");
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcNPCBipedController : public gcBipedController {
public:
    gcNPCBipedController(cBase *);
    ~gcNPCBipedController();
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
        void (*fn)(void *, void *) = rec->fn;
        char *base = block + off;
        fn(base, p);
    }
};

extern char gcNPCBipedControllervirtualtable[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009A404;
extern cType *D_0009F5A8;
extern cType *D_0009F600;
extern cType *D_0009F794;

extern "C" void gcNPCBipedController__gcNPCBipedController_cBaseptr(void *self, cBase *parent);
extern "C" void *dcastdcast_gcNPCBipedControllerptr__constcBaseptr(const cBase *);

// ── gcNPCBipedController::gcNPCBipedController(cBase *) @ 0x00154e78 ──
gcNPCBipedController::gcNPCBipedController(cBase *parent)
    : gcBipedController(parent) {
    *(void **)((char *)this + 4) = gcNPCBipedControllervirtualtable;
}

// ── gcNPCBipedController::Write(cFile &) const @ 0x00154d70 ──
void gcNPCBipedController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcBipedController *)this)->Write(file);
    wb.End();
}

// ── gcNPCBipedController::Read(cFile &, cMemPool *) @ 0x00154dbc ──
int gcNPCBipedController::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcBipedController::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcNPCBipedController::New(cMemPool *, cBase *) static @ 0x0031d25c ──
cBase *gcNPCBipedController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcNPCBipedController *result = 0;
    gcNPCBipedController *obj = (gcNPCBipedController *)rec->fn(base, 0xC0, 0x10, 0, 0);
    if (obj != 0) {
        gcNPCBipedController__gcNPCBipedController_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

typedef int v4sf_t __attribute__((mode(V4SF)));

// ── gcNPCBipedController::AssignCopy(const cBase *) @ 0x0031d09c ──
void gcNPCBipedController::AssignCopy(const cBase *src) {
    gcNPCBipedController *other =
        (gcNPCBipedController *)dcastdcast_gcNPCBipedControllerptr__constcBaseptr(src);

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

    int i = 0;
    char *dst = (char *)this + 0x28;
    char *srcp = (char *)other + 0x28;
    do {
        unsigned char *dstb = (unsigned char *)dst;
        unsigned char *srcb = (unsigned char *)srcp;
        *(int *)(dst + 0x00) = *(int *)(srcp + 0x00);
        dstb[0x04] = srcb[0x04];
        dstb[0x05] = srcb[0x05];
        *(int *)(dst + 0x08) = *(int *)(srcp + 0x08);
        *(int *)(dst + 0x0C) = *(int *)(srcp + 0x0C);
        *(int *)(dst + 0x10) = *(int *)(srcp + 0x10);
        *(int *)(dst + 0x14) = *(int *)(srcp + 0x14);
        *(int *)(dst + 0x18) = *(int *)(srcp + 0x18);
        *(int *)(dst + 0x1C) = *(int *)(srcp + 0x1C);
        i += 1;
        *(int *)(dst + 0x20) = *(int *)(srcp + 0x20);
        srcp += 0x24;
        dst += 0x24;
    } while (i < 2U);

    int *src78 = (int *)((char *)other + 0x78);
    int *dst78 = (int *)((char *)this + 0x78);
    *(int *)((char *)this + 0x70) = *(int *)((char *)other + 0x70);
    *(int *)((char *)this + 0x74) = *(int *)((char *)other + 0x74);
    *dst78 = *src78;
    *(int *)((char *)this + 0x7C) = *(int *)((char *)other + 0x7C);

    int j = 0;
    short *dsth = (short *)((char *)this + 0x80);
    short *srch = (short *)((char *)other + 0x80);
    do {
        short half0 = srch[0];
        short half2 = srch[1];
        short half4 = srch[2];
        dsth[0] = half0;
        dsth[1] = half2;
        dsth[2] = half4;
        j += 1;
        dsth = (short *)((char *)dsth + 6);
        srch = (short *)((char *)srch + 6);
    } while (j <= 0);

    *(float *)((char *)this + 0x88) = *(float *)((char *)other + 0x88);
    int *src8C = (int *)((char *)other + 0x8C);
    int value8C = *src8C;
    int *dst8C = (int *)((char *)this + 0x8C);
    *dst8C = value8C;
    *(unsigned short *)((char *)this + 0x90) =
        *(unsigned short *)((char *)other + 0x90);
    *(v4sf_t *)((char *)this + 0xA0) = *(v4sf_t *)((char *)other + 0xA0);
    *(int *)((char *)this + 0xB0) = *(int *)((char *)other + 0xB0);
    *(float *)((char *)this + 0xB4) = *(float *)((char *)other + 0xB4);
    *(float *)((char *)this + 0xB8) = *(float *)((char *)other + 0xB8);
}

// ── gcNPCBipedController::GetType(void) const @ 0x0031d2d8 ──
const cType *gcNPCBipedController::GetType(void) const {
    if (D_0009F794 == 0) {
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
        D_0009F794 = cType::InitializeType(
            0, 0, 0x101, D_0009F600, &gcNPCBipedController::New, 0, 0, 0);
    }
    return D_0009F794;
}

// ── gcNPCBipedController::~gcNPCBipedController(void) @ 0x0031d470 ──
gcNPCBipedController::~gcNPCBipedController() {
    *(void **)((char *)this + 4) = gcNPCBipedControllervirtualtable;
}
