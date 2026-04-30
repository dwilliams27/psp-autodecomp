// gcValLobbyInfo — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x003496e4  New(cMemPool *, cBase *) static
//   0x00349884  Write(cFile &) const
//   0x003498dc  Read(cFile &, cMemPool *)
//   0x00349de8  ~gcValLobbyInfo(void)
//
// Class layout (12 bytes, alloc size 0xC):
//   [0x00] mParent (cBase *)
//   [0x04] mVtable
//   [0x08] mField8 (defaults to 0 in New)

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

extern char gcLValuevirtualtable[];
extern char gcValLobbyInfovirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcLValue {
public:
    cBase *mParent;
    void *mVtable;
    gcLValue(cBase *parent);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcValLobbyInfo : public gcLValue {
public:
    int mField8;

    gcValLobbyInfo(cBase *parent);
    ~gcValLobbyInfo();
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static cBase *New(cMemPool *, cBase *);

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

inline gcLValue::gcLValue(cBase *parent) {
    mVtable = gcLValuevirtualtable;
    mParent = parent;
}

inline gcValLobbyInfo::gcValLobbyInfo(cBase *parent) : gcLValue(parent) {
    mVtable = gcValLobbyInfovirtualtable;
    mField8 = 0;
}

static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValLobbyInfo asm("D_0009F884");

inline void *operator new(unsigned, void *p) { return p; }

// ── gcValLobbyInfo::New(cMemPool *, cBase *) static @ 0x003496e4 ──
cBase *gcValLobbyInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcValLobbyInfo *result = 0;
    gcValLobbyInfo *obj = (gcValLobbyInfo *)e->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValLobbyInfo(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValLobbyInfo::GetType(void) const @ 0x0034976c ──
const cType *gcValLobbyInfo::GetType(void) const {
    if (!type_gcValLobbyInfo) {
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
        type_gcValLobbyInfo = cType::InitializeType(0, 0, 0x185, type_value,
                                                    gcValLobbyInfo::New,
                                                    0, 0, 0);
    }
    return type_gcValLobbyInfo;
}

// ── gcValLobbyInfo::Write(cFile &) const @ 0x00349884 ──
void gcValLobbyInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue::Write(file);
    wb.Write(mField8);
    wb.End();
}

// ── gcValLobbyInfo::Read(cFile &, cMemPool *) @ 0x003498dc ──
int gcValLobbyInfo::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && ((gcLValue *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    {
        char *bufp = (char *)this + 8;
        void *h = *(void **)rb._data[0];
        cFileSystem::Read(h, bufp, 4);
    }
    return result;
}

// ── gcValLobbyInfo::~gcValLobbyInfo(void) @ 0x00349de8 ──
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard,
// the (in_chrg & 1) deleting-tail dispatch through the inline operator
// delete, and the classdesc reset. Body just reverts the classdesc
// pointer at offset 4 to cBase (no parent destructor — gcLValue has no
// destructor of its own).
gcValLobbyInfo::~gcValLobbyInfo() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
