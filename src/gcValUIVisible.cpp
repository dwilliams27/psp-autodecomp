// Days of Thunder decompilation: gcValUIVisible methods
// Obj: gcAll_psp.obj. Vtable at 0x398658.
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
    int _data[5];
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
    void GetText(char *) const;
};

void gcDesiredUIWidgetHelper_ctor(void *, int);

extern char gcLValuevirtualtable[];
extern char gcValUIVisiblevirtualtable[];
extern char cBaseclassdesc[];

class gcLValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
};

class gcValUIVisible : public gcLValue {
public:
    gcDesiredUIWidgetHelper mHelper;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    static cBase *New(cMemPool *, cBase *);
    ~gcValUIVisible();
    void AssignCopy(const cBase *);
    void GetText(char *) const;

    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec =
            (DeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

// ── gcValUIVisible::New(cMemPool *, cBase *) static @ 0x00369B44 ──
cBase *gcValUIVisible::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcValUIVisible *result = 0;
    gcValUIVisible *obj = (gcValUIVisible *)e->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcLValuevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValUIVisiblevirtualtable;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 8, 1);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValUIVisible::Write(cFile &) const @ 0x00369D30 ──
void gcValUIVisible::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcLValue::Write(file);
    ((gcDesiredUIWidgetHelper *)((unsigned char *)this + 8))->Write(wb);
    wb.End();
}

// ── gcValUIVisible::VisitReferences @ 0x0036A124 ──
void gcValUIVisible::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->VisitReferences(flags, (cBase *)this, cb, user, mask);
}

// ── gcValUIVisible::Read(cFile &, cMemPool *) @ 0x00369D88 ──
int gcValUIVisible::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 2, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 2 && ((gcLValue *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->Read(rb);
    return result;
}

// ── gcValUIVisible::~gcValUIVisible(void) @ 0x0036A1A4 ──
gcValUIVisible::~gcValUIVisible() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

template <class T> T *dcast(const cBase *);

extern void cStrCat(char *, const char *);

struct cHandle {
    int mId;
};

// ── gcValUIVisible::AssignCopy(const cBase *) @ 0x00369AF4 ──
void gcValUIVisible::AssignCopy(const cBase *base) {
    gcValUIVisible *other = dcast<gcValUIVisible>(base);
    *(int *)((char *)this + 8) = *(const int *)((char *)other + 8);
    *(cHandle *)((char *)this + 12) = *(const cHandle *)((char *)other + 12);
    *(cHandle *)((char *)this + 16) = *(const cHandle *)((char *)other + 16);
}

// ── gcValUIVisible::GetText(char *) const @ 0x0036A0D4 ──
void gcValUIVisible::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->GetText(local);
    cStrCat(buf, local);
    cStrCat(buf, (const char *)0x36F810);
}
