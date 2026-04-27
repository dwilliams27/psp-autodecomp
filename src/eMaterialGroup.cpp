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

class eMaterialGroup : public cGroup {
public:
    eMaterialGroup(cBase *);
    ~eMaterialGroup();
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

extern char eMaterialGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern const char eMaterialGroup_cBase_name[];   // "cBase"
extern const char eMaterialGroup_cBase_desc[];   // "Base"

static cType *type_cBase;
static cType *type_cGroup;
static cType *type_eMaterialGroup;

// ── eMaterialGroup::Write(cFile &) const @ 0x00013CE0 ──
void eMaterialGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── eMaterialGroup::Read(cFile &, cMemPool *) @ 0x00013D2C ──
int eMaterialGroup::Read(cFile &file, cMemPool *pool) {
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

// ── eMaterialGroup::New(cMemPool *, cBase *) static @ 0x001DBA64 ──
cBase *eMaterialGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    eMaterialGroup *result = 0;
    eMaterialGroup *obj = (eMaterialGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = eMaterialGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── eMaterialGroup::GetType(void) const @ 0x001DBB20 ──
const cType *eMaterialGroup::GetType() const {
    if (!type_eMaterialGroup) {
        if (!type_cGroup) {
            if (!type_cBase) {
                type_cBase = cType::InitializeType(
                    eMaterialGroup_cBase_name, eMaterialGroup_cBase_desc,
                    1, 0, 0, 0, 0, 0);
            }
            type_cGroup = cType::InitializeType(
                0, 0, 4, type_cBase, 0, 0, 0, 0);
        }
        type_eMaterialGroup = cType::InitializeType(
            0, 0, 0x11, type_cGroup,
            (cBase *(*)(cMemPool *, cBase *))&eMaterialGroup::New,
            0, 0, 8);
    }
    return type_eMaterialGroup;
}

// ── eMaterialGroup::~eMaterialGroup(void) @ 0x001DBC18 ──
eMaterialGroup::~eMaterialGroup() {
    ((void **)this)[1] = eMaterialGroupvirtualtable;
}
