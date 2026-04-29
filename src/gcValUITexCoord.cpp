// gcValUITexCoord — gcAll_psp.obj
// Decompiled functions:
//   0x0036655c  gcValUITexCoord::AssignCopy(const cBase *)            (112B)
//   0x003665cc  gcValUITexCoord::New(cMemPool *, cBase *) static      (180B)
//   0x00367070  gcValUITexCoord::VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int) (128B)
//   0x003670f0  gcValUITexCoord::~gcValUITexCoord(void)               (100B)
//
// Class layout (36 bytes, alloc size 0x24):
//   [0x00] mParent (cBase *)             ─┐ gcLValue base
//   [0x04] mVtable                        ┘
//   [0x08] mHelper (gcDesiredUIWidgetHelper, 12 bytes)
//   [0x14] mField14 (int)
//   [0x18] mField18 (int)
//   [0x1C] mField1C (bool)
//   [0x20] mField20 (float)

class cBase;
class cMemPool;

template <class T> T *dcast(const cBase *);

extern char cBaseclassdesc[];   // @ 0x37E6A8
extern char gcLValuevirtualtable[];
extern char gcValUITexCoordvirtualtable[];

void gcDesiredUIWidgetHelper_ctor(void *, int);

struct gcDesiredUIWidgetHelper {
    int _a;
    int _b;
    int _c;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
};

class gcValUITexCoord {
public:
    cBase *m_parent;     // 0x00
    void  *m_vtable;     // 0x04
    int    m_helper[3];  // 0x08, 0x0C, 0x10
    int    mField14;     // 0x14
    int    mField18;     // 0x18
    bool   mField1C;     // 0x1C
    float  mField20;     // 0x20

    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    ~gcValUITexCoord();

    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec =
            (DeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

// ============================================================
// 0x0036655c — AssignCopy(const cBase *)
// ============================================================
struct cHandle {
    int mId;
};

struct gcDesiredUIWidgetHelperData {
    int mField0;
    cHandle mField4;
    unsigned int mField8;
};

void gcValUITexCoord::AssignCopy(const cBase *base) {
    gcValUITexCoord *other = dcast<gcValUITexCoord>(base);
    __asm__ volatile("" ::: "memory");
    gcDesiredUIWidgetHelperData *dst_h = (gcDesiredUIWidgetHelperData *)((char *)this + 8);
    const gcDesiredUIWidgetHelperData *src_h = (const gcDesiredUIWidgetHelperData *)((const char *)other + 8);
    dst_h->mField4 = src_h->mField4;
    dst_h->mField0 = src_h->mField0;
    dst_h->mField8 = src_h->mField8;
    *(int *)((char *)this + 0x14) = *(const int *)((char *)other + 0x14);
    *(int *)((char *)this + 0x18) = *(const int *)((char *)other + 0x18);
    *(unsigned char *)((char *)this + 0x1C) = *(const unsigned char *)((char *)other + 0x1C);
    *(float *)((char *)this + 0x20) = *(const float *)((char *)other + 0x20);
}

// ============================================================
// 0x003665cc — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcValUITexCoord::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *entry = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValUITexCoord *result = 0;
    gcValUITexCoord *obj = (gcValUITexCoord *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcLValuevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValUITexCoordvirtualtable;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 8, 1);
        obj->mField14 = 0;
        obj->mField18 = 0;
        obj->mField1C = false;
        obj->mField20 = 0.85f;
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x00367070 — VisitReferences
// ============================================================
void gcValUITexCoord::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->VisitReferences(flags, (cBase *)this, cb, user, mask);
}

// ============================================================
// 0x003670f0 — ~gcValUITexCoord(void)  (deleting destructor)
// ============================================================
gcValUITexCoord::~gcValUITexCoord() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
