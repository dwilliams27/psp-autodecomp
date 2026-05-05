class cBase;
class cFile;
class cMemPool;
class cType;

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

class cFile {
public:
    void SetCurrentPos(unsigned int);
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
    void End();
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock();
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

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcStateMachine {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcStateMachineGroup : public cGroup {
public:
    gcStateMachineGroup(cBase *);
    ~gcStateMachineGroup();
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    bool IsManagedTypeExternal() const;
    const cType *GetType() const;
    const cType *GetManagedType() const;
    const char *GetDataDirectory() const;
    const char *GetFileExtension() const;
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

extern char gcStateMachineGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00040C94;
extern cType *D_000998C8;
extern cType *D_0009A3E8;

void gcStateMachineGroup::AssignCopy(const cBase *base) {
    gcStateMachineGroup *src = dcast<gcStateMachineGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void gcStateMachineGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

int gcStateMachineGroup::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if (rb._data[3] != 1) goto fail;
    if (cGroup::Read(file, pool)) goto succ;
fail:
    ((cFile *)rb._data[0])->SetCurrentPos((unsigned int)rb._data[1]);
    return 0;
succ:
    return result;
}

cBase *gcStateMachineGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcStateMachineGroup *result = 0;
    gcStateMachineGroup *obj = (gcStateMachineGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = gcStateMachineGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

gcStateMachineGroup::~gcStateMachineGroup() {
    ((void **)this)[1] = gcStateMachineGroupvirtualtable;
}

const cType *gcStateMachineGroup::GetType(void) const {
    if (D_000998C8 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_000998C8 = cType::InitializeType(0, 0, 0xB7, D_00040C94,
                                           &gcStateMachineGroup::New, 0, 0, 8);
    }
    return D_000998C8;
}

const cType *gcStateMachineGroup::GetManagedType(void) const {
    if (D_0009A3E8 == 0) {
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
        D_0009A3E8 = cType::InitializeType(
            0, 0, 0xB6, D_000385E4, &gcStateMachine::New,
            (const char *)0x36D954, (const char *)0x36D964, 5);
    }
    return D_0009A3E8;
}

const char *gcStateMachineGroup::GetFileExtension(void) const {
    if (D_0009A3E8 == 0) {
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
        D_0009A3E8 = cType::InitializeType(
            0, 0, 0xB6, D_000385E4, &gcStateMachine::New,
            (const char *)0x36D954, (const char *)0x36D964, 5);
    }
    return (const char *)((int *)D_0009A3E8)[6];
}

bool gcStateMachineGroup::IsManagedTypeExternalStatic() {
    if (D_0009A3E8 == 0) {
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
        D_0009A3E8 = cType::InitializeType(
            0, 0, 0xB6, D_000385E4, &gcStateMachine::New,
            (const char *)0x36D954, (const char *)0x36D964, 5);
    }
    int flags = *(int *)D_0009A3E8;
    bool result = false;
    if (flags & 1) result = true;
    return result;
}

const char *gcStateMachineGroup::GetDataDirectory(void) const {
    if (D_0009A3E8 == 0) {
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
        D_0009A3E8 = cType::InitializeType(
            0, 0, 0xB6, D_000385E4, &gcStateMachine::New,
            (const char *)0x36D954, (const char *)0x36D964, 5);
    }
    return (const char *)((int *)D_0009A3E8)[5];
}
