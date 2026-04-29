// gcExternalVariableGroup — inherits cGroup. Mirrors gcTableTemplateGroup.cpp.
// Decompiled functions:
//   0x000d1510 gcExternalVariableGroup::Write(cFile &) const
//   0x002379fc gcExternalVariableGroup::New(cMemPool *, cBase *) static
//   0x00237b94 gcExternalVariableGroup::IsManagedTypeExternal(void) const
//   0x00237bb0 gcExternalVariableGroup::~gcExternalVariableGroup(void)

class cBase;
class cFile;
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

class cGroup {
public:
    cBase *m_parent;       // 0x00
    void *m_classdesc;     // 0x04

    ~cGroup();
    void Write(cFile &) const;
};

class gcExternalVariableGroup : public cGroup {
public:
    unsigned char mFlag;   // 0x08
    char _pad1[3];         // 0x09..0x0B
    int mField;            // 0x0C

    void Write(cFile &) const;
    bool IsManagedTypeExternal() const;
    static bool IsManagedTypeExternalStatic();
    static cBase *New(cMemPool *, cBase *);
    ~gcExternalVariableGroup();

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

extern char gcExternalVariableGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

// ============================================================
// 0x000d1510 — Write(cFile &) const
// ============================================================
void gcExternalVariableGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ============================================================
// 0x002379fc — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcExternalVariableGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcExternalVariableGroup *result = 0;
    gcExternalVariableGroup *obj =
        (gcExternalVariableGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = gcExternalVariableGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x00237b94 — IsManagedTypeExternal(void) const
// ============================================================
bool gcExternalVariableGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ============================================================
// 0x00237bb0 — ~gcExternalVariableGroup(void)
// ============================================================
gcExternalVariableGroup::~gcExternalVariableGroup() {
    *(void **)((char *)this + 4) = (void *)0x386750;
}
