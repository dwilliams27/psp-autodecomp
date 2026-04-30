// gcValLobbyStatus — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x0034eaa0  Write(cFile &) const
//   0x0034ed50  ~gcValLobbyStatus(void)
//
// Class layout (16 bytes, alloc size 0x10):
//   [0x00] parent (cBase *)
//   [0x04] vtable
//   [0x08] f8
//   [0x0C] fC

class cBase;
class cFile;

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

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
};

class gcValLobbyStatus : public gcValue {
public:
    int f8;
    int fC;

    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    ~gcValLobbyStatus();
    void Write(cFile &) const;

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

extern char gcValLobbyStatusvirtualtable[];

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValLobbyStatus asm("D_0009F89C");

// -- gcValLobbyStatus::New(cMemPool *, cBase *) static @ 0x0034e8fc --
cBase *gcValLobbyStatus::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValLobbyStatus *result = 0;
    gcValLobbyStatus *obj = (gcValLobbyStatus *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcValLobbyStatusvirtualtable;
        *(int *)((char *)obj + 8) = 0;
        *(int *)((char *)obj + 0xC) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValLobbyStatus::GetType(void) const @ 0x0034e988 ──
const cType *gcValLobbyStatus::GetType(void) const {
    if (!type_gcValLobbyStatus) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A,
                                                        type_base, 0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValLobbyStatus = cType::InitializeType(0, 0, 0x1F0,
                                                      type_value,
                                                      gcValLobbyStatus::New,
                                                      0, 0, 0);
    }
    return type_gcValLobbyStatus;
}

// ── gcValLobbyStatus::Write(cFile &) const @ 0x0034eaa0 ──
void gcValLobbyStatus::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcValue *)this)->Write(file);
    wb.Write(this->f8);
    wb.Write(this->fC);
    wb.End();
}

// ── gcValLobbyStatus::~gcValLobbyStatus(void) @ 0x0034ed50 ──
gcValLobbyStatus::~gcValLobbyStatus() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
