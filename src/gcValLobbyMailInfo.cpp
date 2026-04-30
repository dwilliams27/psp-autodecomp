// gcValLobbyMailInfo — decompiled from gcAll_psp.obj
// Methods in this file:
//   0x0034a140  Write(cFile &) const
//
// Class layout (partial):
//   [0x00] gcValue base (parent + vtable)
//   [0x08] embedded gcDesiredValue
//   [0x0C] mField (int written between base and gcDesiredValue::Write)

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cObject;
class cType;

template <class T> T dcast(const cBase *);

extern char cBaseclassdesc[];
extern char gcValLobbyMailInfovirtualtable[];
extern char gcValObjectComparevirtualtable[];
extern char eSurfacePropertyTablevirtualtable[];

struct AllocBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct CloneEntry {
    short offset;
    short pad;
    cBase *(*fn)(void *, cMemPool *, cBase *);
};

struct ReleaseEntry {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class gcDesiredValue {
public:
    int mValue;

    void Write(cWriteBlock &) const;
};

class gcValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
};

class gcValLobbyMailInfo : public gcValue {
public:
    gcDesiredValue mDesired;   // 0x08 — start of embedded gcDesiredValue
    int mFieldC;   // 0x0C — int field written by wb.Write before gcDesiredValue::Write

    ~gcValLobbyMailInfo();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    void Write(cFile &) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec =
            (DtorDeleteRecord *)(((AllocBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

class gcValObjectCompare : public gcValue {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cObject {
public:
    cObject(cBase *);
};

extern "C" void cObject_ctor(void *, cBase *) __asm__("__0oHcObjectctP6FcBase");

class eSurfacePropertyTable {
public:
    static cBase *New(cMemPool *, cBase *);
};

static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValLobbyMailInfo asm("D_0009F888");

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oSgcValLobbyMailInfodtv, 0xd4\n");

// ── gcValLobbyMailInfo::~gcValLobbyMailInfo(void) @ 0x0034a8f0 ──
gcValLobbyMailInfo::~gcValLobbyMailInfo() {
    *(void **)((char *)this + 4) = gcValLobbyMailInfovirtualtable;
    char *slot = (char *)this + 0x08;
    if (slot != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 0x08);
        if (val & 1) {
            keep = 0;
        }
        if (keep != 0 && val != 0) {
            char *obj = (char *)val;
            char *type = ((char **)obj)[1];
            DtorDeleteRecord *rec = (DtorDeleteRecord *)(type + 0x50);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(obj + off, (void *)3);
            *(int *)((char *)this + 0x08) = 0;
        }
    }
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

// ── gcValLobbyMailInfo::AssignCopy(const cBase *) @ 0x00349e4c ──
void gcValLobbyMailInfo::AssignCopy(const cBase *base) {
    gcValLobbyMailInfo *other = dcast<gcValLobbyMailInfo *>(base);
    int finalField;

    if (&other->mDesired != &mDesired) {
        goto copyDesired;
    }
    finalField = other->mFieldC;
    goto done;

copyDesired:
    {
        int value = mDesired.mValue;
        int flag = 1;
        int tag = value & 1;
        if (tag != 0) {
            flag = 0;
        }
        if (flag != 0) {
            int old = value;
            int flag2 = 0;
            if (tag != 0) {
                flag2 = 1;
            }
            if (flag2 != 0) {
                value &= ~1;
                value |= 1;
            } else {
                value = *(int *)value;
                value |= 1;
            }
            mDesired.mValue = value;
            if (old != 0) {
                ReleaseEntry *entry =
                    (ReleaseEntry *)(*(char **)(old + 4) + 0x50);
                entry->fn((char *)old + entry->offset, 3);
            }
        }

        __asm__ volatile("" ::: "memory");
        int srcValue = other->mDesired.mValue;
        int srcFlag = 1;
        int srcTag = srcValue & 1;
        if (srcTag != 0) {
            srcFlag = 0;
        }
        if (srcFlag != 0) {
            int source = srcValue;
            CloneEntry *entry =
                (CloneEntry *)(*(char **)(source + 4) + 0x10);
            short offset = entry->offset;
            void *target = (char *)source + offset;
            cMemPool *pool = cMemPool::GetPoolFromPtr(&mDesired);
            int current = mDesired.mValue;
            int flag2 = 0;
            if (current & 1) {
                flag2 = 1;
            }
            if (flag2 != 0) {
                current &= ~1;
            } else {
                current = *(int *)current;
            }
            mDesired.mValue = (int)entry->fn(target, pool, (cBase *)current);
        }
        finalField = other->mFieldC;
    }
done:
    mFieldC = finalField;
}

// ── gcValLobbyMailInfo::New(cMemPool *, cBase *) static @ 0x00349f98 ──
cBase *gcValLobbyMailInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((AllocBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValLobbyMailInfo *result = 0;
    gcValLobbyMailInfo *obj = (gcValLobbyMailInfo *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcValLobbyMailInfovirtualtable;
        *(int *)((char *)obj + 8) = (int)obj | 1;
        *(int *)((char *)obj + 0xC) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValLobbyMailInfo::GetType(void) const @ 0x0034a028 ──
const cType *gcValLobbyMailInfo::GetType(void) const {
    if (!type_gcValLobbyMailInfo) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValLobbyMailInfo = cType::InitializeType(0, 0, 0x1F4,
                                                        type_value,
                                                        gcValLobbyMailInfo::New,
                                                        0, 0, 0);
    }
    return type_gcValLobbyMailInfo;
}

// ── gcValObjectCompare::New(cMemPool *, cBase *) static @ 0x00353978 ──
cBase *gcValObjectCompare::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((AllocBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValObjectCompare *result = 0;
    gcValObjectCompare *obj = (gcValObjectCompare *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcValObjectComparevirtualtable;
        int flagged = (int)obj | 1;
        *(int *)((char *)obj + 8) = flagged;
        *(int *)((char *)obj + 0xC) = flagged;
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=1

// ── eSurfacePropertyTable::New(cMemPool *, cBase *) static @ 0x001f6f80 ──
cBase *eSurfacePropertyTable::New(cMemPool *pool, cBase *parent) {
    eSurfacePropertyTable *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((AllocBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    eSurfacePropertyTable *obj = (eSurfacePropertyTable *)entry->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        cObject_ctor(obj, parent);
        *(void **)((char *)obj + 4) = eSurfacePropertyTablevirtualtable;
        *(int *)((char *)obj + 0x44) = 0;
        *(eSurfacePropertyTable **)((char *)obj + 0x48) = obj;
        *(int *)((char *)obj + 0x4C) = 0;
        result = obj;
    }
    return (cBase *)result;
}

#pragma control sched=2

// ── gcValLobbyMailInfo::Write(cFile &) const @ 0x0034a140 ──
void gcValLobbyMailInfo::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcValue *)this)->Write(file);
    wb.Write(this->mFieldC);
    ((gcDesiredValue *)((char *)this + 8))->Write(wb);
    wb.End();
}
