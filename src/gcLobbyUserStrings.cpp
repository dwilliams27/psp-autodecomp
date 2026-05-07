// gcLobbyUserStrings — string-table value bound to a desired-value lvalue.
//
// Functions matched here:
//   gcLobbyUserStrings::Write(cFile &) const            @ 0x00284888  (gcAll_psp.obj)
//   gcLobbyUserStrings::New(cMemPool *, cBase *) static @ 0x0028471c  (gcAll_psp.obj)

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
class cBase;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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
extern char gcLobbyUserStringsclassdesc[];

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

struct GetNameSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
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

class gcLobbyUserStrings : public gcStringValue {
public:
    int mField08;       // 0x08 — gcDesiredValue first slot, holds (this | 1)
    int mField0C;       // 0x0C

    gcLobbyUserStrings(cBase *parent) : gcStringValue(parent) {
        mClassDesc = gcLobbyUserStringsclassdesc;
        mField08 = (int)this | 1;
        mField0C = 0;
    }
    ~gcLobbyUserStrings();
    void AssignCopy(const cBase *);
    void GetName(char *) const;
    void Write(cFile &) const;
    static gcLobbyUserStrings *New(cMemPool *, cBase *);
    const cType *GetType(void) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec =
            (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

class gcNetworkConfigStrings {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

class gcPartialBodyControllerTemplate {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F504;
extern cType *D_0009F554;
extern cType *D_0009F5DC;
extern cType *D_0009F5F0;

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oSgcLobbyUserStringsdtv, 0xd4\n");

// ── Destructor ──  @ 0x00284e24, 212B
gcLobbyUserStrings::~gcLobbyUserStrings() {
    *(char **)((char *)this + 4) = gcLobbyUserStringsclassdesc;
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
    *(char **)((char *)this + 4) = cBaseclassdesc;
}

// ── GetType ──  @ 0x002847ac, 220B
const cType *gcLobbyUserStrings::GetType(void) const {
    if (D_0009F504 == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F504 = cType::InitializeType(
            0, 0, 0x18C, D_0009F454,
            (cBase *(*)(cMemPool *, cBase *))&gcLobbyUserStrings::New,
            0, 0, 0);
    }
    return D_0009F504;
}

// ── GetType ──  @ 0x00285e78, 220B
const cType *gcNetworkConfigStrings::GetType(void) const {
    if (D_0009F554 == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F554 = cType::InitializeType(0, 0, 0xF8, D_0009F454,
                                           &gcNetworkConfigStrings::New,
                                           0, 0, 0);
    }
    return D_0009F554;
}

// ── GetType ──  @ 0x002a55fc, 220B
const cType *gcPartialBodyControllerTemplate::GetType(void) const {
    if (D_0009F5F0 == 0) {
        if (D_0009F5DC == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F5DC = cType::InitializeType(0, 0, 0x104, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F5F0 = cType::InitializeType(0, 0, 0x106, D_0009F5DC,
                                           &gcPartialBodyControllerTemplate::New,
                                           0, 0, 0);
    }
    return D_0009F5F0;
}

// ── Write ──  @ 0x00284888, 100B
void gcLobbyUserStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcStringValue::Write(file);
    ((const gcDesiredValue *)((const char *)this + 8))->Write(wb);
    wb.Write(mField0C);
    wb.End();
}

// ── New ──  @ 0x0028471c, 144B
gcLobbyUserStrings *gcLobbyUserStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcLobbyUserStrings *result = 0;
    gcLobbyUserStrings *obj = (gcLobbyUserStrings *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcLobbyUserStrings(parent);
        result = obj;
    }
    return result;
}

// ── gcLobbyUserStrings::GetName(char *) const @ 0x00284abc, 184B ──
void gcLobbyUserStrings::GetName(char *buf) const {
    cStrAppend(buf, (const char *)0x36E0D0);

    int val = *(int *)((const char *)this + 0x08);
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

    cStrAppend(buf, (const char *)0x36E060, (const char *)0x36DAF0);
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0fSgcLobbyUserStringsKAssignCopyPC6FcBase, 0x270\n");

// ── gcLobbyUserStrings::AssignCopy(const cBase *) @ 0x002844ac, 624B ──
void gcLobbyUserStrings::AssignCopy(const cBase *base) {
    const gcLobbyUserStrings *other = 0;
    char *slot = (char *)this + 0x08;

    if (base != 0) {
        if (D_0009F504 == 0) {
            if (D_0009F454 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_0009F454 = cType::InitializeType(
                    0, 0, 0x170, D_000385DC, 0, 0, 0, 0);
            }
            D_0009F504 = cType::InitializeType(
                0, 0, 0x18C, D_0009F454,
                (cBase *(*)(cMemPool *, cBase *))&gcLobbyUserStrings::New,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F504;
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
            other = (const gcLobbyUserStrings *)base;
        }
    }

    int finalField;

    if ((char *)other + 0x08 != slot) {
        goto copy_desired;
    }
    finalField = other->mField0C;
    goto done;

copy_desired:
    {
        int tag = *(int *)((char *)this + 0x08) & 1;
        int flag = 1;
        int _tmp_value = *(int *)((char *)this + 0x08);
        int value = _tmp_value;
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
            *(int *)((char *)this + 0x08) = value;
            if (old != 0) {
                ReleaseEntry *release =
                    (ReleaseEntry *)(*(char **)(old + 4) + 0x50);
                release->fn((char *)old + release->offset, 3);
            }
        }

        int srcTag = *(int *)((char *)other + 0x08) & 1;
        int srcFlag = 1;
        int _tmp_846 = *(int *)((char *)other + 0x08);
        int srcValue = _tmp_846;
        if (srcTag != 0) {
            srcFlag = 0;
        }
        if (srcFlag != 0) {
            int source = srcValue;
            CloneEntry *clone =
                (CloneEntry *)(*(char **)(source + 4) + 0x10);
            short cloneOffset = clone->offset;
            void *target = (char *)source + cloneOffset;
            cMemPool *pool = cMemPool::GetPoolFromPtr(slot);
            int current = *(int *)((char *)this + 0x08);
            int currentFlag = 0;
            if (current & 1) {
                currentFlag = 1;
            }
            if (currentFlag != 0) {
                current &= ~1;
            } else {
                current = *(int *)current;
            }
            *(int *)((char *)this + 0x08) =
                (int)clone->fn(target, pool, (cBase *)current);
        }
        finalField = other->mField0C;
    }
done:
    mField0C = finalField;
}
