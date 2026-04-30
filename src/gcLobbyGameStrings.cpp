// gcLobbyGameStrings — string-lvalue with constant flag and desired-value slot.
//
// Functions matched here:
//   gcLobbyGameStrings::Write(cFile &) const            @ 0x002815b0  (gcAll_psp.obj)
//   gcLobbyGameStrings::New(cMemPool *, cBase *) static @ 0x00281400  (gcAll_psp.obj)

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cMemPool;
class cBase;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcDesiredValue {
public:
    int mValue;
    void Write(cWriteBlock &) const;
    gcDesiredValue &operator=(const gcDesiredValue &);
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

template <class T> T dcast(const cBase *);

extern char cBaseclassdesc[];
extern char gcLobbyGameStringsclassdesc[];
extern char gcLobbyGameStringsvirtualtable[];
extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F458;
extern cType *D_0009F4F4;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cBase {
public:
    cBase *mOwner;          // 0
    void *mClassDesc;       // 4

    cBase(cBase *owner) {
        mClassDesc = cBaseclassdesc;
        mOwner = owner;
    }
};

class gcStringValue : public cBase {
public:
    gcStringValue(cBase *parent) : cBase(parent) {}
    void Write(cFile &) const;
};

class gcStringLValue : public gcStringValue {
public:
    gcStringLValue(cBase *parent) : gcStringValue(parent) {}
    void Write(cFile &) const;
};

class gcLobbyGameStrings : public gcStringLValue {
public:
    int mField08;       // 0x08 — constant 1
    int mField0C;       // 0x0C — gcDesiredValue first slot, holds (this | 1)
    int mField10;       // 0x10

    gcLobbyGameStrings(cBase *parent) : gcStringLValue(parent) {
        mClassDesc = gcLobbyGameStringsclassdesc;
        mField08 = 1;
        mField0C = (int)this | 1;
        mField10 = 0;
    }
    ~gcLobbyGameStrings();
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static gcLobbyGameStrings *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec = (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

struct gcLobbyGameStringsDispatch {
    short offset;
    short pad;
    int (*fn)(void *, cMemPool *, int);
};

struct gcLobbyGameStringsDestroyDispatch {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

inline gcDesiredValue &gcDesiredValue::operator=(const gcDesiredValue &other) {
    if (&other != this) {
        int oldValue = mValue;
        int release = 1;
        int oldTagged = oldValue & 1;
        if (oldTagged != 0) {
            release = 0;
        }
        if (release != 0) {
            int releaseTagged = 0;
            if (oldTagged != 0) {
                releaseTagged = 1;
            }
            int stored;
            if (releaseTagged != 0) {
                stored = oldValue & -2;
            } else {
                stored = *(int *)oldValue;
            }
            mValue = stored | 1;
            if (oldValue != 0) {
                void *classDesc = *(void **)(oldValue + 4);
                gcLobbyGameStringsDestroyDispatch *entry =
                    (gcLobbyGameStringsDestroyDispatch *)((char *)classDesc + 0x50);
                short offset = entry->offset;
                void (*fn)(void *, int) = entry->fn;
                fn((char *)oldValue + offset, 3);
            }
        }
        int sourceValue = other.mValue;
        int copy = 1;
        if ((sourceValue & 1) != 0) {
            copy = 0;
        }
        if (copy != 0) {
            void *classDesc = *(void **)(sourceValue + 4);
            gcLobbyGameStringsDispatch *entry =
                (gcLobbyGameStringsDispatch *)((char *)classDesc + 0x10);
            short offset = entry->offset;
            void *sourceObject = (char *)sourceValue + offset;
            cMemPool *pool = cMemPool::GetPoolFromPtr(this);
            int currentValue = mValue;
            int currentTagged = 0;
            if ((currentValue & 1) != 0) {
                currentTagged = 1;
            }
            int currentObject;
            if (currentTagged != 0) {
                currentObject = currentValue & -2;
            } else {
                currentObject = *(int *)currentValue;
            }
            int (*fn)(void *, cMemPool *, int) = entry->fn;
            mValue = fn(sourceObject, pool, currentObject);
        }
    }
    return *this;
}

// ── Write ──  @ 0x002815b0, 112B
void gcLobbyGameStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcStringLValue::Write(file);
    wb.Write(mField08);
    ((const gcDesiredValue *)((const char *)this + 12))->Write(wb);
    wb.Write(mField10);
    wb.End();
}

void gcLobbyGameStrings::AssignCopy(const cBase *base) {
    gcLobbyGameStrings *other = dcast<gcLobbyGameStrings *>(base);
    mField08 = other->mField08;
    *(gcDesiredValue *)((char *)this + 0xC) =
        *(const gcDesiredValue *)((char *)other + 0xC);
    mField10 = other->mField10;
}

const cType *gcLobbyGameStrings::GetType(void) const {
    if (D_0009F4F4 == 0) {
        if (D_0009F458 == 0) {
            if (D_0009F454 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F458 = cType::InitializeType(0, 0, 0x171, D_0009F454,
                                               0, 0, 0, 0);
        }
        D_0009F4F4 = cType::InitializeType(
            0, 0, 0x184, D_0009F458,
            (cBase *(*)(cMemPool *, cBase *))&gcLobbyGameStrings::New,
            0, 0, 0);
    }
    return D_0009F4F4;
}

// ── New ──  @ 0x00281400, 152B
gcLobbyGameStrings *gcLobbyGameStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcLobbyGameStrings *result = 0;
    gcLobbyGameStrings *obj = (gcLobbyGameStrings *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcLobbyGameStrings(parent);
        result = obj;
    }
    return result;
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oSgcLobbyGameStringsdtv, 0xd4\n");

// ── gcLobbyGameStrings::~gcLobbyGameStrings(void)  @ 0x00281d48, 212B ──
gcLobbyGameStrings::~gcLobbyGameStrings() {
    *(char **)((char *)this + 4) = gcLobbyGameStringsvirtualtable;
    char *slot = (char *)this + 0x0C;
    if (slot != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 0x0C);
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
            *(int *)((char *)this + 0x0C) = 0;
        }
    }
    *(char **)((char *)this + 4) = cBaseclassdesc;
}
