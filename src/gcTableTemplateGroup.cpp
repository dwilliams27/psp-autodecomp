// gcTableTemplateGroup — inherits cGroup, holds a bool flag and an int.
// Decompiled functions:
//   0x000d31e4 gcTableTemplateGroup::Write(cFile &) const
//   0x000d3230 gcTableTemplateGroup::Read(cFile &, cMemPool *)
//   0x002385cc gcTableTemplateGroup::AssignCopy(const cBase *)
//   0x00238604 gcTableTemplateGroup::New(cMemPool *, cBase *) static
//   0x002387b8 gcTableTemplateGroup::~gcTableTemplateGroup(void)

class cBase;
class cFile {
public:
    void SetCurrentPos(unsigned int);
};
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

template <class T> T *dcast(const cBase *);

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

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cGroup {
public:
    cBase *m_parent;       // 0x00
    void *m_classdesc;     // 0x04

    ~cGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcTableTemplateGroup : public cGroup {
public:
    unsigned char mFlag;   // 0x08
    char _pad1[3];         // 0x09..0x0B
    int mField;            // 0x0C

    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    bool IsManagedTypeExternal() const;
    static bool IsManagedTypeExternalStatic();
    ~gcTableTemplateGroup();

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

// ============================================================
// 0x000d31e4 — Write(cFile &) const
// ============================================================
void gcTableTemplateGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ============================================================
// 0x000d3230 — Read(cFile &, cMemPool *)
// ============================================================
int gcTableTemplateGroup::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1 || cGroup::Read(file, pool) == 0) {
        ((cFile *)rb._data[0])->SetCurrentPos(rb._data[1]);
        return 0;
    }
    return result;
}

// ============================================================
// 0x002385cc — AssignCopy(const cBase *)
// ============================================================
void gcTableTemplateGroup::AssignCopy(const cBase *base) {
    gcTableTemplateGroup *src = dcast<gcTableTemplateGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

// ============================================================
// 0x00238604 — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcTableTemplateGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    int flag = 0;
    gcTableTemplateGroup *obj = (gcTableTemplateGroup *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        if (gcTableTemplateGroup::IsManagedTypeExternalStatic() == 0) {
            flag = 1;
        }
        ((void **)obj)[1] = (void *)0x37E6A8;
        ((void **)obj)[0] = parent;
        ((void **)obj)[1] = (void *)0x37EA80;
        ((unsigned char *)obj)[8] = (unsigned char)flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = (void *)0x386A20;
        flag = (int)obj;
    }
    return (cBase *)flag;
}

// ============================================================
// 0x002387b8 — ~gcTableTemplateGroup(void)
// ============================================================
gcTableTemplateGroup::~gcTableTemplateGroup() {
    *(void **)((char *)this + 4) = (void *)0x386A20;
}
