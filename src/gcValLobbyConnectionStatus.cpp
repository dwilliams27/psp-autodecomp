// gcValLobbyConnectionStatus — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x003478F8  AssignCopy(const cBase *)
//   0x00347928  New(cMemPool *, cBase *) static
//   0x00347ACC  Write(cFile &) const
//   0x00347B24  Read(cFile &, cMemPool *)
//   0x00347BF4  Evaluate(void) const
//   0x00347CCC  ~gcValLobbyConnectionStatus(void)
//
// Class layout (12 bytes, alloc size 0xC):
//   [0x00] parent (cBase *)
//   [0x04] vtable
//   [0x08] f8 (lobby-connection-status int, defaults to 1 in New)

class cBase;
class cFile;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

class gcValue {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcValLobbyConnectionStatus : public gcValue {
public:
    int pad0;
    int f8;
    int pad4;

    ~gcValLobbyConnectionStatus();
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    float Evaluate(void) const;
    const cType *GetType(void) const;
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

gcValLobbyConnectionStatus *dcast(const cBase *);

extern char gcValLobbyConnectionStatusvirtualtable[];
extern char gcValLobbyConnectionStatus_cBase_vtable[];
extern const char gcValLobbyConnectionStatus_base_name[];
extern const char gcValLobbyConnectionStatus_base_desc[];

// ── gcValLobbyConnectionStatus::AssignCopy(const cBase *) @ 0x003478F8 ──
void gcValLobbyConnectionStatus::AssignCopy(const cBase *base) {
    gcValLobbyConnectionStatus *other = dcast(base);
    *(int *)((char *)this + 8) = *(int *)((char *)other + 8);
}

// ── gcValLobbyConnectionStatus::New(cMemPool *, cBase *) static @ 0x00347928 ──
cBase *gcValLobbyConnectionStatus::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValLobbyConnectionStatus *result = 0;
    gcValLobbyConnectionStatus *obj =
        (gcValLobbyConnectionStatus *)entry->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = gcValLobbyConnectionStatus_cBase_vtable;
        ((cBase **)obj)[0] = parent;
        ((char **)obj)[1] = gcValLobbyConnectionStatusvirtualtable;
        ((int *)obj)[2] = 1;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValLobbyConnectionStatus::Write(cFile &) const @ 0x00347ACC ──
void gcValLobbyConnectionStatus::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcValue *)this)->Write(file);
    wb.Write(*(int *)((char *)this + 8));
    wb.End();
}

// ── gcValLobbyConnectionStatus::GetType(void) const @ 0x003479B4 ──
static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValLobbyConnectionStatus;

const cType *gcValLobbyConnectionStatus::GetType(void) const {
    if (!type_gcValLobbyConnectionStatus) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcValLobbyConnectionStatus_base_name, gcValLobbyConnectionStatus_base_desc, 1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
        }
        type_gcValLobbyConnectionStatus = cType::InitializeType(0, 0, 0x182, type_value, gcValLobbyConnectionStatus::New, 0, 0, 0);
    }
    return type_gcValLobbyConnectionStatus;
}

// ── gcValLobbyConnectionStatus::Read(cFile &, cMemPool *) @ 0x00347B24 ──
int gcValLobbyConnectionStatus::Read(cFile &file, cMemPool *pool) {
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

// ── gcValLobbyConnectionStatus::Evaluate(void) const @ 0x00347BF4 ──
//
// Returns 1.0f when the queried lobby connection status matches the
// expected value stored at offset 8. Special-case: when the expected
// value is 1, both lobby states 1 and 3 satisfy the predicate. The
// goto labels at the bottom force SNC's block-layout pass to emit the
// "lobby == 0" cold block before the "field8 != 1" cold block — that
// matches the original codegen order (zero return at +0x74, neq
// return at +0x88).
float gcValLobbyConnectionStatus::Evaluate(void) const {
    void *lobby = nwNetwork::GetLobby();
    DispatchEntry *e;
    short off;
    int (*fn)(void *);
    int status;
    int field8;
    if (lobby == 0) goto retZero;
    e = (DispatchEntry *)(*(char **)lobby + 0x78);
    off = e->offset;
    fn = e->fn;
    status = fn((char *)lobby + off);
    field8 = ((const int *)this)[2];
    if (field8 != 1) goto retNeq;
    return (float)((status == 1) | (status == 3));
retZero:
    return 0.0f;
retNeq:
    return (float)(field8 == status);
}

// ── gcValLobbyConnectionStatus::~gcValLobbyConnectionStatus(void) @ 0x00347CCC ──
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard,
// the (in_chrg & 1) deleting-tail dispatch through the inline operator
// delete, and the classdesc reset. Body just reverts the classdesc
// pointer at offset 4 to cBase (no parent-class destructor in the chain
// — gcValue has no destructor of its own).
gcValLobbyConnectionStatus::~gcValLobbyConnectionStatus() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
