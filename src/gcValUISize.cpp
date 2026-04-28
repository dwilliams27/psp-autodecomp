// gcValUISize — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x003654d0  Write(cFile &) const                       (124B)
//   0x003664f8  ~gcValUISize(void)                         (100B)
//
// Class layout (32 bytes, alloc size 0x20):
//   [0x00] mParent (cBase *)            ─┐ gcLValue base
//   [0x04] mVtable                       ┘
//   [0x08] mHelper (gcDesiredUIWidgetHelper, 12 bytes)
//   [0x14] mField14 (int)
//   [0x18] mField18 (bool)
//   [0x1C] mField1C (float)

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(bool);
    void Write(float);
    void End(void);
};

struct gcDesiredUIWidgetHelper {
    int _a;
    int _b;
    int _c;
    void Write(cWriteBlock &) const;
};

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

class gcValUISize : public gcLValue {
public:
    gcDesiredUIWidgetHelper mHelper;
    int mField14;
    bool mField18;
    float mField1C;

    ~gcValUISize();
    void Write(cFile &) const;

    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

// ── gcValUISize::Write(cFile &) const @ 0x003654d0 ──
void gcValUISize::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcLValue::Write(file);
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->Write(wb);
    wb.Write(mField14);
    wb.Write(mField18);
    wb.Write(mField1C);
    wb.End();
}

// ── gcValUISize::~gcValUISize(void) @ 0x003664f8 ──
gcValUISize::~gcValUISize() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
