// gcValUIPosition — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x00363f18  Write(cFile &) const
//   0x00365200  ~gcValUIPosition(void)
//
// Class layout (32 bytes):
//   [0x00] mParent (cBase *)
//   [0x04] mVtable
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
    void GetText(char *) const;
};

void cStrAppend(char *, const char *, ...);
extern const char gcValUIPosition_fmt[];     // 0x0036DF34
extern const char gcValGetText_text[];       // 0x0036DAF0

extern char gcLValuevirtualtable[];
extern char gcValUIPositionvirtualtable[];
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

class gcValUIPosition : public gcLValue {
public:
    gcDesiredUIWidgetHelper mHelper;
    int   mField14;
    bool  mField18;
    float mField1C;

    ~gcValUIPosition();
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void GetText(char *) const;

    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

template <class T> T *dcast(const cBase *);

struct cHandle {
    int mId;
};

void gcDesiredUIWidgetHelper_ctor(void *, int);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcValUIPosition::AssignCopy(const cBase *) @ 0x00363cac ──
void gcValUIPosition::AssignCopy(const cBase *base) {
    gcValUIPosition *other = dcast<gcValUIPosition>(base);
    *(int *)((char *)this + 8) = *(const int *)((char *)other + 8);
    *(cHandle *)((char *)this + 12) = *(const cHandle *)((char *)other + 12);
    *(cHandle *)((char *)this + 16) = *(const cHandle *)((char *)other + 16);
    mField14 = other->mField14;
    mField18 = other->mField18;
    mField1C = other->mField1C;
}

// ── gcValUIPosition::New(cMemPool *, cBase *) static @ 0x00363d14 ──
cBase *gcValUIPosition::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *entry = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValUIPosition *result = 0;
    gcValUIPosition *obj = (gcValUIPosition *)entry->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcLValuevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValUIPositionvirtualtable;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 8, 1);
        obj->mField14 = 0;
        obj->mField18 = false;
        obj->mField1C = 0.85f;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValUIPosition::Write(cFile &) const @ 0x00363f18 ──
void gcValUIPosition::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcLValue::Write(file);
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->Write(wb);
    wb.Write(this->mField14);
    wb.Write(this->mField18);
    wb.Write(this->mField1C);
    wb.End();
}

// ── gcValUIPosition::GetText(char *) const @ 0x00365130 ──
void gcValUIPosition::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((gcDesiredUIWidgetHelper *)((char *)this + 8))->GetText(local);
    cStrAppend(buf, gcValUIPosition_fmt, local, gcValGetText_text);
}

// ── gcValUIPosition::~gcValUIPosition(void) @ 0x00365200 ──
// SNC auto-emits the deleting (D0) variant: (this != 0) guard, body
// resets classdesc at +4, then on (flag & 1) dispatches operator delete.
gcValUIPosition::~gcValUIPosition() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
