// gcValKeyboardState.cpp — decompiled from gcAll_psp.obj.
// Parent: gcValue.
//
// Functions:
//   gcValKeyboardState::AssignCopy(const cBase *)         @ 0x0034735c    56B
//   gcValKeyboardState::New(cMemPool *, cBase *) static   @ 0x00347394   144B
//   gcValKeyboardState::Write(cFile &) const              @ 0x0034753c   100B
//   gcValKeyboardState::Read(cFile &, cMemPool *)         @ 0x003475a0   228B
//   gcValKeyboardState::VisitReferences(...)              @ 0x00347858    60B
//   gcValKeyboardState::~gcValKeyboardState(void)         @ 0x00347894   100B

class cBase;
class cFile;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern char gcValuevirtualtable[];
extern char gcValKeyboardStatevirtualtable[];
extern char cBaseclassdesc[];                       // @ 0x37E6A8

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

struct PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcValue {
public:
    cBase *mParent;       // 0x0
    void *mVtable;        // 0x4
    gcValue(cBase *parent);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcValKeyboardState : public gcValue {
public:
    int mField8;          // 0x8
    int mFieldC;          // 0xC

    gcValKeyboardState(cBase *parent);
    ~gcValKeyboardState();
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    void VisitReferences(unsigned int, cBase *,
        void (*)(cBase *, unsigned int, void *),
        void *, unsigned int);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        AllocEntry *rec = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void *(*fn)(void *, int, int, int, int) = rec->fn;
        ((void (*)(void *, void *))fn)(base, p);
    }
};

gcValKeyboardState *dcast(const cBase *);

// ─────────────────────────────────────────────────────────────────────────
// gcValKeyboardState::AssignCopy(const cBase *) @ 0x0034735c
// ─────────────────────────────────────────────────────────────────────────
void gcValKeyboardState::AssignCopy(const cBase *base) {
    gcValKeyboardState *other = dcast(base);
    this->mField8 = other->mField8;
    this->mFieldC = other->mFieldC;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValKeyboardState::New(cMemPool *, cBase *) static @ 0x00347394
// ─────────────────────────────────────────────────────────────────────────
cBase *gcValKeyboardState::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *entry =
        (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValKeyboardState *result = 0;
    gcValKeyboardState *obj =
        (gcValKeyboardState *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = (int)cBaseclassdesc;
        ((int *)obj)[0] = (int)parent;
        ((int *)obj)[1] = (int)gcValKeyboardStatevirtualtable;
        ((int *)obj)[2] = 0;
        ((int *)obj)[3] = 0x48;
        result = obj;
    }
    return (cBase *)result;
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValKeyboardState;

const cType *gcValKeyboardState::GetType(void) const {
    if (!type_gcValKeyboardState) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValKeyboardState = cType::InitializeType(
            0, 0, 0x20D, type_value, gcValKeyboardState::New, 0, 0, 0);
    }
    return type_gcValKeyboardState;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValKeyboardState::Write(cFile &) const @ 0x0034753c
// ─────────────────────────────────────────────────────────────────────────
void gcValKeyboardState::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    wb.Write(this->mField8);
    wb.Write(this->mFieldC);
    wb.End();
}

// ─────────────────────────────────────────────────────────────────────────
// gcValKeyboardState::Read(cFile &, cMemPool *) @ 0x003475a0
// ─────────────────────────────────────────────────────────────────────────
int gcValKeyboardState::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && ((gcValue *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 8, 4);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0xC, 4);
    return result;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValKeyboardState::VisitReferences(...) @ 0x00347858
// ─────────────────────────────────────────────────────────────────────────
void gcValKeyboardState::VisitReferences(unsigned int arg0, cBase *arg1,
    void (*arg2)(cBase *, unsigned int, void *),
    void *arg3, unsigned int arg4) {
    if (arg2 != 0) {
        arg2(arg1, (unsigned int)this, arg3);
    }
}

// ─────────────────────────────────────────────────────────────────────────
// gcValKeyboardState::~gcValKeyboardState() @ 0x00347894
// ─────────────────────────────────────────────────────────────────────────
gcValKeyboardState::~gcValKeyboardState() {
    mVtable = cBaseclassdesc;
}
