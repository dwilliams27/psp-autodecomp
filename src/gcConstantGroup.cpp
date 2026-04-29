// gcConstantGroup: AssignCopy + Write/Read serialization + destructor.
//
// Same shape as gcVariableGroup (parent cGroup; cBase header 8 bytes; vtable
// at offset 4). Patterns mirror cFactory::Write / eWeatherEffect::Read /
// eSphereShape::~eSphereShape — SNC's ABI auto-generates the dtor's
// (this!=0) guard, the chain call to ~cGroup, and the deleting-tail
// dispatch through operator delete.

class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

template <class T> T *dcast(const cBase *);

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
    char _pad[8];
    cGroup(cBase *);
    ~cGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcConstantGroup : public cGroup {
public:
    unsigned char mFlag;
    char _pad1[3];
    int mField;

    gcConstantGroup(cBase *);
    ~gcConstantGroup();
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static bool IsManagedTypeExternalStatic();
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char gcConstantGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_000998AC;

// ── gcConstantGroup::AssignCopy(const cBase *) @ 0x0023641C ──
void gcConstantGroup::AssignCopy(const cBase *base) {
    gcConstantGroup *src = dcast<gcConstantGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

// ── gcConstantGroup::Write(cFile &) const @ 0x000CE12C ──
void gcConstantGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── gcConstantGroup::Read(cFile &, cMemPool *) @ 0x000CE178 ──
int gcConstantGroup::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->cGroup::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcConstantGroup::New(cMemPool *, cBase *) static @ 0x00236454 ──
cBase *gcConstantGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcConstantGroup *result = 0;
    gcConstantGroup *obj = (gcConstantGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = gcConstantGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcConstantGroup::GetType(void) const @ 0x00236510 ──
const cType *gcConstantGroup::GetType(void) const {
    if (D_000998AC == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_000998AC = cType::InitializeType(0, 0, 0x7C, D_00040C94,
                                           &gcConstantGroup::New,
                                           0, 0, 8);
    }
    return D_000998AC;
}

// ── gcConstantGroup::~gcConstantGroup(void) @ 0x00236608 ──
gcConstantGroup::~gcConstantGroup() {
    *(void **)((char *)this + 4) = gcConstantGroupvirtualtable;
}
