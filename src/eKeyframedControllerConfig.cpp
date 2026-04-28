// eKeyframedControllerConfig — 32-byte config object that extends
// ePhysicsControllerConfig (which itself adds only vtable wiring on top of
// cBase). The config's own 24 bytes of data live at offset 0x08..0x20 and are
// what AssignCopy splat-copies from a sibling instance.
//
// Functions matched here:
//   eKeyframedControllerConfig::Write(cFile &) const                @ 0x00075558  (eAll_psp.obj, sched=1)
//   eKeyframedControllerConfig::Read(cFile &, cMemPool *)           @ 0x000755a4  (eAll_psp.obj, sched=1)
//   eKeyframedControllerConfig::AssignCopy(const cBase *)           @ 0x0020dfb0  (eAll_psp.obj, sched=2)
//   eKeyframedControllerConfig::~eKeyframedControllerConfig(void)   @ 0x00075694  (eAll_psp.obj, sched=1)
//   eKeyframedControllerConfig::New(cMemPool *, cBase *) static     @ 0x0020e010  (eAll_psp.obj, sched=1)
//   eKeyframedControllerConfig::GetType(void) const                 @ 0x0020e08c  (eAll_psp.obj, sched=1)
//
// The destructor follows the canonical deleting-destructor shape used
// throughout the engine (see src/eKeyframedController_dtor.cpp,
// src/eSphereShape_dtor.cpp): SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~ePhysicsControllerConfig(this, 0), and the deleting tail
// dispatch via operator delete (inlined here).

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class ePhysicsControllerConfig {
public:
    char _pad[0x8];
    ePhysicsControllerConfig(cBase *);
    ~ePhysicsControllerConfig();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

void cFile_SetCurrentPos(void *, unsigned int);

struct eKCCData { int _w[6]; };

inline void *operator new(unsigned int, void *p) { return p; }

class eKeyframedControllerConfig : public ePhysicsControllerConfig {
public:
    eKCCData mData;  // offset 0x08..0x20

    eKeyframedControllerConfig(cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    ~eKeyframedControllerConfig();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

eKeyframedControllerConfig *dcast(const cBase *);

static cType *type_cBase;
static cType *type_ePhysicsControllerConfig;
static cType *type_eKeyframedControllerConfig;

// ── Write ──  @ 0x00075558, 76B
#pragma control sched=1
void eKeyframedControllerConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const ePhysicsControllerConfig *)this)->Write(file);
    wb.End();
}

// ── Read ──  @ 0x000755a4, 188B
int eKeyframedControllerConfig::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 &&
        ((ePhysicsControllerConfig *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}
#pragma control sched=2

// ── AssignCopy ──  @ 0x0020dfb0, 96B
//
// dcast()s the source cBase* to the concrete type, then splat-copies the
// 24-byte config payload at offset +8. SNC schedules the copy as two
// 3-load + 3-store batches (a2/a3/t0 → a2/a3/a0).
#pragma control sched=1
void eKeyframedControllerConfig::AssignCopy(const cBase *base) {
    eKeyframedControllerConfig *other = dcast(base);
    *(eKCCData *)((char *)this + 8) = *(eKCCData *)((char *)other + 8);
}
#pragma control sched=2

// ── Destructor ──  @ 0x00075694, 124B
#pragma control sched=1
eKeyframedControllerConfig::~eKeyframedControllerConfig() {
    *(void **)((char *)this + 4) = (void *)0x383DA8;
}
#pragma control sched=2

#pragma control sched=1

// ── New ──  @ 0x0020e010, 124B
cBase *eKeyframedControllerConfig::New(cMemPool *pool, cBase *parent) {
    eKeyframedControllerConfig *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eKeyframedControllerConfig *obj =
        (eKeyframedControllerConfig *)rec->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        new (obj) eKeyframedControllerConfig(parent);
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=2

#pragma control sched=1

// ── GetType ──  @ 0x0020e08c, 216B
const cType *eKeyframedControllerConfig::GetType(void) const {
    if (!type_eKeyframedControllerConfig) {
        if (!type_ePhysicsControllerConfig) {
            if (!type_cBase) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                type_cBase = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
            }
            type_ePhysicsControllerConfig =
                cType::InitializeType(0, 0, 0x23D, type_cBase, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = type_ePhysicsControllerConfig;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x20E010;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        type_eKeyframedControllerConfig =
            cType::InitializeType(0, 0, 0x265, parentType, factory, 0, 0, 0);
    }
    return type_eKeyframedControllerConfig;
}

#pragma control sched=2
