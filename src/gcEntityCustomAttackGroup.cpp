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

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

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
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cGroup {
public:
    int _pad[2];
    ~cGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcEntityCustomAttackGroup : public cGroup {
public:
    static bool IsManagedTypeExternalStatic();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    ~gcEntityCustomAttackGroup();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

class gcEntityGroup : public cGroup {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

class gcObjectRelationship {
};

class gcEntityRelationship : public gcObjectRelationship {
public:
    static gcEntityRelationship *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

extern char gcEntityCustomAttackGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_000998B8;
extern cType *D_000998DC;
extern cType *D_0009F3F0;
extern cType *D_0009F774;

cBase *gcEntityCustomAttackGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcEntityCustomAttackGroup *result = 0;
    gcEntityCustomAttackGroup *obj =
        (gcEntityCustomAttackGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = gcEntityCustomAttackGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcEntityCustomAttackGroup::GetType(void) const {
    if (D_000998DC == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_000998DC = cType::InitializeType(0, 0, 0x1CE, D_00040C94,
                                           &gcEntityCustomAttackGroup::New,
                                           0, 0, 8);
    }
    return D_000998DC;
}

const cType *gcEntityGroup::GetType(void) const {
    if (D_000998B8 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_000998B8 = cType::InitializeType(0, 0, 0x8D, D_00040C94,
                                           &gcEntityGroup::New,
                                           0, 0, 8);
    }
    return D_000998B8;
}

const cType *gcEntityRelationship::GetType(void) const {
    if (D_0009F774 == 0) {
        if (D_0009F3F0 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F3F0 = cType::InitializeType(0, 0, 0x131, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F774 = cType::InitializeType(
            0, 0, 0x132, D_0009F3F0,
            (cBase *(*)(cMemPool *, cBase *))&gcEntityRelationship::New,
            0, 0, 0);
    }
    return D_0009F774;
}

// gcEntityCustomAttackGroup::Write(cFile &) const @ 0x000d265c
void gcEntityCustomAttackGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->cGroup::Write(file);
    wb.End();
}

// gcEntityCustomAttackGroup::Read(cFile &, cMemPool *) @ 0x000d26a8
int gcEntityCustomAttackGroup::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->cGroup::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// gcEntityCustomAttackGroup::~gcEntityCustomAttackGroup(void) @ 0x002382e8
gcEntityCustomAttackGroup::~gcEntityCustomAttackGroup() {
    *(void **)((char *)this + 4) = gcEntityCustomAttackGroupvirtualtable;
}
