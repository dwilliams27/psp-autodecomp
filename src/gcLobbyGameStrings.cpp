// gcLobbyGameStrings — string-lvalue with constant flag and desired-value slot.
//
// Functions matched here:
//   gcLobbyGameStrings::Write(cFile &) const            @ 0x002815b0  (gcAll_psp.obj)
//   gcLobbyGameStrings::New(cMemPool *, cBase *) static @ 0x00281400  (gcAll_psp.obj)

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cFileHandle;
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

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

class gcDesiredValue {
public:
    int mValue;
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
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

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
void cStrCopy(char *, const wchar_t *, int);
void cStrCopy(wchar_t *, const char *, int);
void *memset(void *, int, unsigned int);

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

struct GetNameSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

struct LobbyDispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *, int);
};

struct LobbyIndexDispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *, int, void *);
};

struct FloatDispatchEntry {
    short offset;
    short pad;
    float (*fn)(void *);
};

struct LobbyGameInfo {
    char pad04[4];
    char text0[20];
    char text1[8];
    char pad20[8];
    int field28;
    char field2C[16];
    int field3C;
    char pad40[24];
};

class nwNetwork {
public:
    static void *GetLobby(void);
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
    int Read(cFile &, cMemPool *);
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
    int Read(cFile &, cMemPool *);
    void AssignCopy(const cBase *);
    void GetName(char *) const;
    void Get(wchar_t *, int) const;
    void Set(const wchar_t *) const;
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

// ── gcLobbyGameStrings::Read(cFile &, cMemPool *) @ 0x00281620, 240B ──
int gcLobbyGameStrings::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);
    if ((unsigned int)rb[3] == 3 && this->gcStringLValue::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
    __0oKcReadBlockdtv(rb, 2);
    return 0;
success:
    cFileSystem::Read((cFileHandle *)*(void **)rb[0], &mField08, 4);
    ((gcDesiredValue *)((char *)this + 0x0C))->Read(*(cReadBlock *)rb);
    cFileSystem::Read((cFileHandle *)*(void **)rb[0], &mField10, 4);
    __0oKcReadBlockdtv(rb, 2);
    return result;
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

// ── gcLobbyGameStrings::Get(wchar_t *, int) const @ 0x00281710 ──
void gcLobbyGameStrings::Get(wchar_t *buf, int size) const {
    LobbyGameInfo info;
    void *lobby = nwNetwork::GetLobby();
    if (lobby != 0) {
        info.field28 = 0;
        memset(info.field2C, 0, 0x10);
        info.field3C = 0;

        switch (mField08) {
        case 0: {
            LobbyDispatchEntry *entry =
                (LobbyDispatchEntry *)(*(char **)lobby + 0x258);
            entry->fn((char *)lobby + entry->offset, &info, entry->offset);
            break;
        }
        case 1: {
            LobbyIndexDispatchEntry *entry =
                (LobbyIndexDispatchEntry *)(*(char **)lobby + 0x250);
            int val = mField0C;
            int flag = 0;
            void *self = (char *)lobby + entry->offset;
            if (val & 1) {
                flag = 1;
            }
            if (flag != 0) {
                val = 0;
            } else {
                __asm__ volatile("" ::: "memory");
            }

            int desiredPtr = val;
            float f;
            if (desiredPtr != 0) {
                FloatDispatchEntry *fe =
                    (FloatDispatchEntry *)(*(char **)(desiredPtr + 4) + 0x70);
                f = fe->fn((char *)desiredPtr + fe->offset);
            } else {
                f = 0.0f;
            }
            int idx = (int)f;

            entry->fn(self, idx, &info);
            break;
        }
        case 2: {
            LobbyDispatchEntry *entry =
                (LobbyDispatchEntry *)(*(char **)lobby + 0x258);
            entry->fn((char *)lobby + entry->offset, &info, entry->offset);
            break;
        }
        default:
            return;
        }

        int field = mField10;
        if (field <= 0) {
            if (field >= 0) {
                cStrCopy(buf, info.text0, size);
            }
        } else if (field < 2) {
            cStrCopy(buf, info.text1, size);
        }
    }
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

// ── gcLobbyGameStrings::GetName(char *) const @ 0x0028199c, 252B ──
void gcLobbyGameStrings::GetName(char *buf) const {
    cStrAppend(buf, (const char *)0x36E084);

    if (mField08 == 0) {
        cStrAppend(buf, (const char *)0x36E058);
    } else if (mField08 == 2) {
        cStrAppend(buf, (const char *)0x36E090);
    } else {
        int val = mField0C;
        int flag = 0;
        if (val & 1) {
            flag = 1;
        }
        if (flag != 0) {
            val = 0;
        } else {
            __asm__ volatile("" ::: "memory");
        }

        int check = val;
        if (check != 0) {
            char *typeInfo = *(char **)(check + 4);
            GetNameSlot *slot = (GetNameSlot *)(typeInfo + 0xD0);
            slot->fn((char *)val + slot->offset, buf);
        } else {
            cStrCat(buf, (const char *)0x36DB24);
        }
    }

    cStrAppend(buf, (const char *)0x36E060, (const char *)0x36DAF0);
}

// ── gcLobbyGameStrings::Set(const wchar_t *) const @ 0x002818b8, 228B ──
void gcLobbyGameStrings::Set(const wchar_t *src) const {
    LobbyGameInfo info;
    void *lobby = nwNetwork::GetLobby();
    if (lobby != 0 && mField08 == 2) {
        info.field28 = 0;
        memset(info.field2C, 0, 0x10);
        info.field3C = 0;

        LobbyDispatchEntry *get =
            (LobbyDispatchEntry *)(*(char **)lobby + 0x258);
        get->fn((char *)lobby + get->offset, &info, get->offset);

        int index = mField10;
        switch (index) {
        case 0:
            cStrCopy(info.text0, src, 0x14);
            break;
        case 1:
            cStrCopy(info.text1, src, 8);
            break;
        }

        LobbyDispatchEntry *set =
            (LobbyDispatchEntry *)(*(char **)lobby + 0x260);
        set->fn((char *)lobby + set->offset, &info, set->offset);
    }
}
