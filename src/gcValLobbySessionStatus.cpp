// gcValLobbySessionStatus — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x0034e4fc  New(cMemPool *, cBase *) static
//   0x0034e69c  Write(cFile &) const
//   0x0034e6f4  Read(cFile &, cMemPool *)
//   0x0034e7c4  Evaluate(void) const
//   0x0034e860  ~gcValLobbySessionStatus(void)
//
// Class layout (12 bytes, alloc size 0xC):
//   [0x00] mParent (cBase *)
//   [0x04] mVtable
//   [0x08] mField8 (defaults to 0 in New)

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

class nwNetwork {
public:
    static void *GetLobby(void);
};

extern char cBaseclassdesc[];                       // @ 0x37E6A8

struct DispatchEntry {
    short offset;
    short pad;
    int (*fn)(void *);
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

class gcValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

extern char gcValLobbySessionStatus_gcValue_vtable[];
extern char gcValLobbySessionStatusvirtualtable[];

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValLobbySessionStatus asm("D_0009F898");

class gcValLobbySessionStatus : public gcValue {
public:
    int mField8;

    ~gcValLobbySessionStatus();
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    float Evaluate(void) const;
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

// ── gcValLobbySessionStatus::New(cMemPool *, cBase *) static @ 0x0034e4fc ──
cBase *gcValLobbySessionStatus::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValLobbySessionStatus *result = 0;
    gcValLobbySessionStatus *obj =
        (gcValLobbySessionStatus *)entry->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = gcValLobbySessionStatus_gcValue_vtable;
        ((cBase **)obj)[0] = parent;
        ((char **)obj)[1] = gcValLobbySessionStatusvirtualtable;
        ((int *)obj)[2] = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValLobbySessionStatus::GetType(void) const @ 0x0034e584 ──
const cType *gcValLobbySessionStatus::GetType(void) const {
    if (!type_gcValLobbySessionStatus) {
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
        type_gcValLobbySessionStatus = cType::InitializeType(
            0, 0, 0x19E, type_value, gcValLobbySessionStatus::New, 0, 0, 0);
    }
    return type_gcValLobbySessionStatus;
}

// ── gcValLobbySessionStatus::Write(cFile &) const @ 0x0034e69c ──
void gcValLobbySessionStatus::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcValue *)this)->Write(file);
    wb.Write(this->mField8);
    wb.End();
}

// ── gcValLobbySessionStatus::Read(cFile &, cMemPool *) @ 0x0034e6f4 ──
int gcValLobbySessionStatus::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && ((gcValue *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    {
        void *h = *(void **)rb._data[0];
        cFileSystem::Read(h, (char *)this + 8, 4);
    }
    return result;
}

// ── gcValLobbySessionStatus::Evaluate(void) const @ 0x0034e7c4 ──
float gcValLobbySessionStatus::Evaluate(void) const {
    void *lobby = nwNetwork::GetLobby();
    if (lobby == 0) {
        return 0.0f;
    }
    DispatchEntry *e = (DispatchEntry *)(*(char **)lobby + 0x80);
    short off = e->offset;
    int (*fn)(void *) = e->fn;
    int status = fn((char *)lobby + off);
    return (float)(this->mField8 == status);
}

// ── gcValLobbySessionStatus::~gcValLobbySessionStatus(void) @ 0x0034e860 ──
gcValLobbySessionStatus::~gcValLobbySessionStatus() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
