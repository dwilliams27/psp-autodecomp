// gcUIControlString — gcStringLValue-derived UI control with a
// gcDesiredUIWidgetHelper at offset 8 and two trailing int fields.
//
// Functions matched here:
//   gcUIControlString::AssignCopy(const cBase *)        @ 0x0028f3d0  ( 96B)
//   gcUIControlString::Write(cFile &) const             @ 0x0028f5e8  (112B)
//   gcUIControlString::~gcUIControlString(void)         @ 0x00290188  (100B)

class cBase;
class cFile;
class cMemPool;

inline void *operator new(unsigned int, void *p) { return p; }

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

struct gcDesiredUIWidgetHelper {
    int mField0;
    cHandle mField4;
    cHandle mField8;
    void Write(cWriteBlock &) const;
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

class cMemPoolNS {
public:
    static cMemPoolNS *GetPoolFromPtr(const void *);
};

class gcStringValue {
public:
    cBase *mParent;       // 0x0
    void *mVtable;        // 0x4
    void Write(cFile &) const;
};

class gcStringLValue : public gcStringValue {
public:
    void Write(cFile &) const;
};

extern char gcStringLValuevirtualtable[];
extern char gcUIControlStringvirtualtable[];
extern char cBaseclassdesc[];

class gcUIControlString : public gcStringLValue {
public:
    gcDesiredUIWidgetHelper mHelper;   // 0x08
    int mField14;                      // 0x14
    int mField18;                      // 0x18

    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    ~gcUIControlString();

    static void operator delete(void *p) {
        cMemPoolNS *pool = cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        AllocEntry *rec = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void *(*fn)(void *, int, int, int, int) = rec->fn;
        ((void (*)(void *, void *))fn)(base, p);
    }
};

gcUIControlString *dcast(const cBase *);
void gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper(void *, int);

// ─────────────────────────────────────────────────────────────────────────
// gcUIControlString::New(cMemPool *, cBase *)  @ 0x0028f430, 160B
// ─────────────────────────────────────────────────────────────────────────
cBase *gcUIControlString::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *rec = (AllocEntry *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcUIControlString *result = 0;
    gcUIControlString *obj = (gcUIControlString *)rec->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)obj = parent;
        *(void **)((char *)obj + 4) = gcUIControlStringvirtualtable;
        gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper((char *)obj + 8, 1);
        obj->mField14 = 0;
        obj->mField18 = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ─────────────────────────────────────────────────────────────────────────
// gcUIControlString::AssignCopy(const cBase *)  @ 0x0028f3d0, 96B
// ─────────────────────────────────────────────────────────────────────────
void gcUIControlString::AssignCopy(const cBase *base) {
    gcUIControlString *other = dcast(base);
    this->mHelper.mField0 = other->mHelper.mField0;
    this->mHelper.mField4 = other->mHelper.mField4;
    this->mHelper.mField8 = other->mHelper.mField8;
    this->mField14 = other->mField14;
    this->mField18 = other->mField18;
}

// ─────────────────────────────────────────────────────────────────────────
// gcUIControlString::Write(cFile &) const  @ 0x0028f5e8, 112B
// ─────────────────────────────────────────────────────────────────────────
void gcUIControlString::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcStringLValue::Write(file);
    mHelper.Write(wb);
    wb.Write(mField14);
    wb.Write(mField18);
    wb.End();
}

// ─────────────────────────────────────────────────────────────────────────
// gcUIControlString::~gcUIControlString(void)  @ 0x00290188, 100B
// ─────────────────────────────────────────────────────────────────────────
gcUIControlString::~gcUIControlString() {
    *(void **)((char *)this + 4) = gcStringLValuevirtualtable;
}
