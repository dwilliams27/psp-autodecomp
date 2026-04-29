// gcFunctionGroup — Write, Read, ~gcFunctionGroup
//
// Modeled directly on the matched gcStaticInstanceGroup family in
// src/gcStaticInstanceGroup.cpp (same .obj, same shape, same calls).

class cBase;
class cFile;
class cMemPool;
class cType;

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cGroup {
public:
    cBase *m_parent;        // 0x00
    void *m_vtbl;           // 0x04
    unsigned char mFlag;    // 0x08
    char _pad[3];
    int mField;             // 0x0C
    cGroup(cBase *);
    ~cGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcFunctionGroup : public cGroup {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static bool IsManagedTypeExternalStatic();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType() const;
    ~gcFunctionGroup();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

class gcProfileString {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos_Group(void *, unsigned int);

extern char gcFunctionGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_000998A4;

// ── gcFunctionGroup::New(cMemPool *, cBase *) static @ 0x00235f84 ──
cBase *gcFunctionGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcFunctionGroup *result = 0;
    gcFunctionGroup *obj = (gcFunctionGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = gcFunctionGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcFunctionGroup::GetType(void) const @ 0x00236040 ──
const cType *gcFunctionGroup::GetType() const {
    if (D_000998A4 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_000998A4 = cType::InitializeType(
            0, 0, 0x75, D_00040C94,
            (cBase *(*)(cMemPool *, cBase *))&gcFunctionGroup::New,
            0, 0, 8);
    }
    return D_000998A4;
}

// ── gcProfileString::New(cMemPool *, cBase *) static @ 0x002867a4 ──
cBase *gcProfileString::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcProfileString *result = 0;
    gcProfileString *obj = (gcProfileString *)e->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        int mid;
        int tagged;
        ((void **)obj)[1] = (void *)0x37E6A8;
        __asm__ volatile("lui %0, 0" : "=r"(mid));
        ((cBase **)obj)[0] = parent;
        __asm__ volatile("addiu %0, %0, 0x12a8" : "+r"(mid));
        tagged = (int)((unsigned int)obj | 1);
        ((int *)obj)[1] = mid;
        ((int *)obj)[2] = tagged;
        ((void **)obj)[5] = (void *)0x37E6A8;
        ((int *)obj)[3] = 0;
        ((gcProfileString **)obj)[4] = obj;
        void *vt = (void *)0x388568;
        int one = 1;
        __asm__ volatile("" : "+r"(vt), "+r"(one));
        ((void **)obj)[5] = vt;
        ((unsigned char *)obj)[0x18] = one;
        ((unsigned char *)obj)[0x19] = 0;
        ((int *)obj)[7] = 0;
        ((int *)obj)[8] = 0;
        ((int *)obj)[9] = tagged;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcFunctionGroup::Write(cFile &) const @ 0x000cd5a4 ──
// Same shape as cFactory::Write — chain to cGroup::Write under a cWriteBlock.
void gcFunctionGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->cGroup::Write(file);
    wb.End();
}

// ── gcFunctionGroup::Read(cFile &, cMemPool *) @ 0x000cd5f0 ──
// Same shape as eWeatherEffect::Read / gcStaticInstanceGroup::Read.
#pragma control sched=1
int gcFunctionGroup::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->cGroup::Read(file, pool)) goto success;
    cFile_SetCurrentPos_Group(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}
#pragma control sched=2

// ── gcFunctionGroup::~gcFunctionGroup(void) @ 0x00236138 ──
// Canonical C++ destructor; SNC ABI auto-generates the (this != 0) guard,
// the chain call to ~cGroup(), and the deleting-tail `if (flags & 1)`
// dispatch via operator delete.
gcFunctionGroup::~gcFunctionGroup() {
    *(void **)((char *)this + 4) = gcFunctionGroupvirtualtable;
}
