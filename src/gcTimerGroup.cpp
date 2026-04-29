// gcTimerGroup serialization, allocation, and small methods.

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

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class cGroup {
public:
    cBase *m_parent;
    void *m_vtbl;
    unsigned char mFlag;
    char _pad[3];
    int mField;
    cGroup(cBase *);
    ~cGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcTimerGroup : public cGroup {
public:
    gcTimerGroup(cBase *);
    ~gcTimerGroup();
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
    void AssignCopy(const cBase *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
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

template <class T> T *dcast(const cBase *);

extern char gcTimerGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_000998CC;

void gcTimerGroup::AssignCopy(const cBase *base) {
    gcTimerGroup *src = dcast<gcTimerGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool gcTimerGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

void gcTimerGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

int gcTimerGroup::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->cGroup::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

cBase *gcTimerGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcTimerGroup *result = 0;
    gcTimerGroup *obj = (gcTimerGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = gcTimerGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcTimerGroup::GetType() const {
    if (D_000998CC == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_000998CC = cType::InitializeType(0, 0, 0xDE, D_00040C94,
                                           &gcTimerGroup::New,
                                           0, 0, 8);
    }
    return D_000998CC;
}

gcTimerGroup::~gcTimerGroup() {
    *(void **)((char *)this + 4) = gcTimerGroupvirtualtable;
}
