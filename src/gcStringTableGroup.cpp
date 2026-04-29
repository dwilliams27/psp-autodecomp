// gcStringTableGroup: Write/Read serialization + destructor.
//
// Parent class is cGroup (same shape as gcVariableGroup, eSkinGroup, etc.).
// Patterns mirror cFactory::Write / eWeatherEffect::Read / eSphereShape::~eSphereShape
// — SNC's ABI auto-generates the dtor's (this!=0) guard, the chain call to
// ~cGroup, and the deleting-tail dispatch through operator delete.

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

void cFile_SetCurrentPos(void *, unsigned int);

class cGroup {
public:
    int base;

    cGroup(cBase *);
    ~cGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcStringTableGroup : public cGroup {
public:
    gcStringTableGroup(cBase *);
    ~gcStringTableGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static bool IsManagedTypeExternalStatic();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType() const;
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char gcStringTableGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_000998B0;

// ── gcStringTableGroup::New(cMemPool *, cBase *) static @ 0x002366BC ──
cBase *gcStringTableGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcStringTableGroup *result = 0;
    gcStringTableGroup *obj =
        (gcStringTableGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = gcStringTableGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcStringTableGroup::GetType(void) const @ 0x00236778 ──
const cType *gcStringTableGroup::GetType() const {
    if (D_000998B0 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_000998B0 = cType::InitializeType(0, 0, 0x88, D_00040C94,
                                           &gcStringTableGroup::New,
                                           0, 0, 8);
    }
    return D_000998B0;
}

// ── gcStringTableGroup::Write(cFile &) const @ 0x000CE6F0 ──
void gcStringTableGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── gcStringTableGroup::Read(cFile &, cMemPool *) @ 0x000CE73C ──
int gcStringTableGroup::Read(cFile &file, cMemPool *pool) {
    int ok;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(ok));
    if ((unsigned int)rb._data[3] != 1 || !this->cGroup::Read(file, pool)) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    return ok;
}

// ── gcStringTableGroup::~gcStringTableGroup(void) @ 0x00236870 ──
gcStringTableGroup::~gcStringTableGroup() {
    *(void **)((char *)this + 4) = gcStringTableGroupvirtualtable;
}
