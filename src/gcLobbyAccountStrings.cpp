// gcLobbyAccountStrings — string-table value with extra fixed flag.
//
// Functions matched here:
//   gcLobbyAccountStrings::Write(cFile &) const            @ 0x0027fcc4  (gcAll_psp.obj)
//   gcLobbyAccountStrings::New(cMemPool *, cBase *) static @ 0x0027fb50  (gcAll_psp.obj)
//   gcLobbyAccountStrings::~gcLobbyAccountStrings(void)    @ 0x002802e0  (gcAll_psp.obj)

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cBase;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

extern char cBaseclassdesc[];
extern char gcLobbyAccountStringsclassdesc[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
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

struct GetNameSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

struct LobbyDispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *, int, void *);
};

struct AccountDispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct FloatDispatchEntry {
    short offset;
    short pad;
    float (*fn)(void *);
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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcLobbyAccountStrings : public gcStringValue {
public:
    int mField08;       // 0x08 — constant 1
    int mField0C;       // 0x0C — gcDesiredValue first slot, holds (this | 1)
    int mField10;       // 0x10

    gcLobbyAccountStrings(cBase *parent) : gcStringValue(parent) {
        mClassDesc = gcLobbyAccountStringsclassdesc;
        mField08 = 1;
        mField0C = (int)this | 1;
        mField10 = 0;
    }
    ~gcLobbyAccountStrings();
    void AssignCopy(const cBase *);
    void Get(wchar_t *, int) const;
    void GetName(char *) const;
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static gcLobbyAccountStrings *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec = (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F4E8;

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
void cStrCopy(wchar_t *, const char *, int);

class nwNetwork {
public:
    static void *GetLobby(void);
};

// ── GetType ──  @ 0x0027fbe8, 220B
const cType *gcLobbyAccountStrings::GetType(void) const {
    if (D_0009F4E8 == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F4E8 = cType::InitializeType(
            0, 0, 0xFF, D_0009F454,
            (cBase *(*)(cMemPool *, cBase *))&gcLobbyAccountStrings::New, 0, 0, 0);
    }
    return D_0009F4E8;
}

// ── Write ──  @ 0x0027fcc4, 112B
void gcLobbyAccountStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcStringValue::Write(file);
    wb.Write(mField08);
    ((const gcDesiredValue *)((const char *)this + 12))->Write(wb);
    wb.Write(mField10);
    wb.End();
}

// ── New ──  @ 0x0027fb50, 152B
gcLobbyAccountStrings *gcLobbyAccountStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcLobbyAccountStrings *result = 0;
    gcLobbyAccountStrings *obj = (gcLobbyAccountStrings *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcLobbyAccountStrings(parent);
        result = obj;
    }
    return result;
}

// ── gcLobbyAccountStrings::Get(wchar_t *, int) const @ 0x0027fe24 ──
void gcLobbyAccountStrings::Get(wchar_t *buf, int size) const {
    void *lobby = nwNetwork::GetLobby();
    if (lobby != 0) {
        char tmp[24];
        int mode = mField08;
        if (mode <= 0) {
            if (mode < 0) {
                return;
            }
            AccountDispatchEntry *entry =
                (AccountDispatchEntry *)(*(char **)lobby + 0x100);
            entry->fn((char *)lobby + entry->offset, tmp);
        } else if (mode < 2) {
            LobbyDispatchEntry *entry =
                (LobbyDispatchEntry *)(*(char **)lobby + 0xF8);
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

            entry->fn(self, idx, tmp);
        } else {
            return;
        }

        if (mField10 == 0) {
            cStrCopy(buf, tmp + 4, size);
        }
    }
}

// ── gcLobbyAccountStrings::GetName(char *) const @ 0x0027ff58, 216B ──
void gcLobbyAccountStrings::GetName(char *buf) const {
    cStrAppend(buf, (const char *)0x36E048);

    if (mField08 == 0) {
        cStrAppend(buf, (const char *)0x36E058);
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

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oVgcLobbyAccountStringsdtv, 0xd4\n");

// 0x002802e0 - gcLobbyAccountStrings::~gcLobbyAccountStrings(void)
gcLobbyAccountStrings::~gcLobbyAccountStrings() {
    *(char **)((char *)this + 4) = gcLobbyAccountStringsclassdesc;
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

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0fVgcLobbyAccountStringsKAssignCopyPC6FcBase, 0x278\n");

// ── gcLobbyAccountStrings::AssignCopy(const cBase *) @ 0x0027f8d8, 632B ──
void gcLobbyAccountStrings::AssignCopy(const cBase *base) {
    const gcLobbyAccountStrings *other = 0;

    if (base != 0) {
        if (D_0009F4E8 == 0) {
            if (D_0009F454 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_0009F454 = cType::InitializeType(
                    0, 0, 0x170, D_000385DC, 0, 0, 0, 0);
            }
            D_0009F4E8 = cType::InitializeType(
                0, 0, 0xFF, D_0009F454,
                (cBase *(*)(cMemPool *, cBase *))&gcLobbyAccountStrings::New,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F4E8;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, fn);
        int isValid;

        if (target != 0) {
            goto have_target;
        }
        isValid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                isValid = 1;
            } else {
                type = (cType *)*((void **)((char *)type + 0x1C));
                if (type != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            isValid = 0;
        }

cast_done:
        if (isValid != 0) {
            other = (const gcLobbyAccountStrings *)base;
        }
    }

    mField08 = other->mField08;
    int finalField;

    if ((char *)other + 0x0C != (char *)this + 0x0C) {
        goto copy_desired;
    }
    finalField = other->mField10;
    goto done;

copy_desired:
    {
        int value = mField0C;
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
            mField0C = value;
            if (old != 0) {
                ReleaseEntry *release =
                    (ReleaseEntry *)(*(char **)(old + 4) + 0x50);
                release->fn((char *)old + release->offset, 3);
            }
        }

        int srcValue = other->mField0C;
        int srcFlag = 1;
        int srcTag = srcValue & 1;
        if (srcTag != 0) {
            srcFlag = 0;
        }
        if (srcFlag != 0) {
            int source = srcValue;
            CloneEntry *clone =
                (CloneEntry *)(*(char **)(source + 4) + 0x10);
            short cloneOffset = clone->offset;
            void *target = (char *)source + cloneOffset;
            int current = mField0C;
            cMemPool *pool = cMemPool::GetPoolFromPtr((char *)this + 0x0C);
            int currentFlag = 0;
            if (current & 1) {
                currentFlag = 1;
            }
            if (currentFlag != 0) {
                current &= ~1;
            } else {
                current = *(int *)current;
            }
            mField0C = (int)clone->fn(target, pool, (cBase *)current);
        }
        finalField = other->mField10;
    }
done:
    mField10 = finalField;
}
