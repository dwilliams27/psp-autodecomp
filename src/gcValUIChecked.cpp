// gcValUIChecked — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x0036169c  New(cMemPool *, cBase *) static
//   0x00361cfc  Write(cFile &) const
//   0x00361d54  Read(cFile &, cMemPool *)
//   0x00361e1c  VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int)
//   0x00361e9c  ~gcValUIChecked(void)
//
// Class layout (20 bytes, alloc size 0x14):
//   [0x00] mParent (cBase *)
//   [0x04] mVtable
//   [0x08] mHelper (gcDesiredUIWidgetHelper, 12 bytes)

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    unsigned int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

struct gcDesiredUIWidgetHelper {
    int _a;
    int _b;
    int _c;
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

void gcDesiredUIWidgetHelper_ctor(void *, int);

extern char gcLValuevirtualtable[];
extern char gcValUICheckedvirtualtable[];
extern char cBaseclassdesc[];

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
};

class gcLValue {
public:
    cBase *mParent;
    void *mVtable;
    gcLValue(cBase *parent);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcValUIChecked : public gcLValue {
public:
    gcDesiredUIWidgetHelper mHelper;

    ~gcValUIChecked();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

inline gcLValue::gcLValue(cBase *parent) {
    mVtable = gcLValuevirtualtable;
    mParent = parent;
    __asm__ volatile("" ::: "memory");
}

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcValUIChecked::New(cMemPool *, cBase *) static @ 0x0036169c ──
cBase *gcValUIChecked::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcValUIChecked *result = 0;
    gcValUIChecked *obj = (gcValUIChecked *)e->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcLValuevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValUICheckedvirtualtable;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 8, 1);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValUIChecked::Write(cFile &) const @ 0x00361cfc ──
void gcValUIChecked::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcLValue::Write(file);
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->Write(wb);
    wb.End();
}

// ── gcValUIChecked::Read(cFile &, cMemPool *) @ 0x00361d54 ──
int gcValUIChecked::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 2, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 2 && this->gcLValue::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->Read(rb);
    return result;
}

// ── gcValUIChecked::~gcValUIChecked(void) @ 0x00361e9c ──
gcValUIChecked::~gcValUIChecked() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

// ── gcValUIChecked::VisitReferences @ 0x00361e1c ──
void gcValUIChecked::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->VisitReferences(flags, (cBase *)this, cb, user, mask);
}
