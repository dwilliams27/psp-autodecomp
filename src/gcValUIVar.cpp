// gcValUIVar — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x00368484  Write(cFile &) const
//   0x003699c0  GetText(char *) const
//   0x00369a10  VisitReferences(...)
//   0x00369a90  ~gcValUIVar(void)
//
// Class layout:
//   [0x00] mParent (cBase *)              ─┐ gcLValue base
//   [0x04] mVtable                         ┘
//   [0x08] mHelper (gcDesiredUIWidgetHelper, 12 bytes)
//   [0x14] mField14 (int)

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

struct gcDesiredUIWidgetHelper {
    int _a;
    int _b;
    int _c;
    void Write(cWriteBlock &) const;
    void GetText(char *) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

void cStrAppend(char *, const char *, ...);
extern const char gcValUIVar_fmt[];
extern const char gcValUIVar_str2[];

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
    void Write(cFile &) const;
};

class gcValUIVar : public gcLValue {
public:
    gcDesiredUIWidgetHelper mHelper;
    int mField14;

    void Write(cFile &) const;
    void GetText(char *) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    ~gcValUIVar();

    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

// ── gcValUIVar::Write @ 0x00368484 ──
void gcValUIVar::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcLValue::Write(file);
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->Write(wb);
    wb.Write(mField14);
    wb.End();
}

// ── gcValUIVar::GetText @ 0x003699c0 ──
void gcValUIVar::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->GetText(local);
    cStrAppend(buf, gcValUIVar_fmt, local, gcValUIVar_str2);
}

// ── gcValUIVar::VisitReferences @ 0x00369a10 ──
void gcValUIVar::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    this->mHelper.VisitReferences(flags, (cBase *)this, cb, user, mask);
}

// ── gcValUIVar::~gcValUIVar @ 0x00369a90 ──
gcValUIVar::~gcValUIVar() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
