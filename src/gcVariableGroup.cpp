// gcVariableGroup: Write/Read serialization + destructor.
//
// Parent class is cGroup (same shape as eSkinGroup, gcStringTableGroup, etc.).
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

class gcVariableGroup : public cGroup {
public:
    gcVariableGroup(cBase *);
    ~gcVariableGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static bool IsManagedTypeExternalStatic();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType() const;
    const cType *GetManagedType() const;
    const char *GetDataDirectory() const;
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char gcVariableGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040C94;
extern cType *D_000998A8;
extern cType *D_0009F4D4;

class gcVariable {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

// ── gcVariableGroup::New(cMemPool *, cBase *) static @ 0x002361EC ──
cBase *gcVariableGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcVariableGroup *result = 0;
    gcVariableGroup *obj = (gcVariableGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = gcVariableGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcVariableGroup::GetType(void) const @ 0x002362A8 ──
const cType *gcVariableGroup::GetType() const {
    if (D_000998A8 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_000998A8 = cType::InitializeType(
            0, 0, 0x7A, D_00040C94,
            (cBase *(*)(cMemPool *, cBase *))&gcVariableGroup::New,
            0, 0, 8);
    }
    return D_000998A8;
}

// ── gcVariableGroup::Write(cFile &) const @ 0x000CDB68 ──
void gcVariableGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── gcVariableGroup::Read(cFile &, cMemPool *) @ 0x000CDBB4 ──
int gcVariableGroup::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->cGroup::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcVariableGroup::GetManagedType(void) const @ 0x000CDC70 ──
const cType *gcVariableGroup::GetManagedType() const {
    if (D_0009F4D4 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(
                0, 0, 3, D_000385E0, 0, 0, 0, 0);
        }
        D_0009F4D4 = cType::InitializeType(
            0, 0, 0x79, D_000385E4, &gcVariable::New,
            (const char *)0x36DA24, (const char *)0x36DA30, 4);
    }
    return D_0009F4D4;
}

// ── gcVariableGroup::GetDataDirectory(void) const @ 0x000CDD98 ──
const char *gcVariableGroup::GetDataDirectory() const {
    if (D_0009F4D4 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC, &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(
                0, 0, 3, D_000385E0, 0, 0, 0, 0);
        }
        D_0009F4D4 = cType::InitializeType(
            0, 0, 0x79, D_000385E4, &gcVariable::New,
            (const char *)0x36DA24, (const char *)0x36DA30, 4);
    }
    return (const char *)((int *)D_0009F4D4)[5];
}

// ── gcVariableGroup::~gcVariableGroup(void) @ 0x002363A0 ──
gcVariableGroup::~gcVariableGroup() {
    *(void **)((char *)this + 4) = gcVariableGroupvirtualtable;
}
