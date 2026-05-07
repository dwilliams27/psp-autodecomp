// gcLobbyScoreboardRowStrings — gcAll_psp.obj
//   Class hierarchy: cBase -> gcStringValue -> gcLobbyScoreboardRowStrings
//   Total size 0x10. Layout:
//     0x00: cBase (mOwner, mClassDesc)
//     0x08: gcDesiredValue (tagged self-pointer)
//     0x0C: int mField

class cBase {
public:
    int _b0;
    int _b4;
};

class cFile;
class cType;

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

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcStringValue : public cBase {
public:
    void Write(cFile &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

extern char gcLobbyScoreboardRowStringsvirtualtable[];

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
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

class gcLobbyScoreboardRowStrings : public gcStringValue {
public:
    int _b8;    // 0x08 - gcDesiredValue tagged self-ptr
    int mField; // 0x0C

    ~gcLobbyScoreboardRowStrings();
    void AssignCopy(const cBase *);
    void GetName(char *) const;
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static gcLobbyScoreboardRowStrings *New(cMemPool *, cBase *);

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
extern cType *D_0009F500;

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

// ── gcLobbyScoreboardRowStrings::GetType(void) const  @ 0x00283d68, 220B ──
const cType *gcLobbyScoreboardRowStrings::GetType(void) const {
    if (D_0009F500 == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F500 = cType::InitializeType(
            0, 0, 0x128, D_0009F454,
            (cBase *(*)(cMemPool *, cBase *))&gcLobbyScoreboardRowStrings::New,
            0, 0, 0);
    }
    return D_0009F500;
}

// ── gcLobbyScoreboardRowStrings::Write(cFile &) const  @ 0x00283e44, 100B ──
void gcLobbyScoreboardRowStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcStringValue::Write(file);
    wb.Write(this->mField);
    ((const gcDesiredValue *)((const char *)this + 8))->Write(wb);
    wb.End();
}

// ── gcLobbyScoreboardRowStrings::GetName(char *) const @ 0x00284070, 184B ──
void gcLobbyScoreboardRowStrings::GetName(char *buf) const {
    cStrAppend(buf, (const char *)0x36E0BC);

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

// ── gcLobbyScoreboardRowStrings::New(cMemPool *, cBase *) static  @ 0x00283cd8, 144B ──
gcLobbyScoreboardRowStrings *gcLobbyScoreboardRowStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcLobbyScoreboardRowStrings *result = 0;
    int *p = (int *)entry->fn(base, 0x10, 4, 0, 0);
    if (p != 0) {
        *(int *)((char *)p + 4) = 0x37E6A8;
        *(cBase **)((char *)p + 0) = parent;
        *(int *)((char *)p + 4) = (int)gcLobbyScoreboardRowStringsvirtualtable;
        *(int *)((char *)p + 8) = (int)((unsigned int)p | 1);
        *(int *)((char *)p + 12) = 0;
        result = (gcLobbyScoreboardRowStrings *)p;
    }
    return result;
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0obgcLobbyScoreboardRowStringsdtv, 0xd4\n");

// ── gcLobbyScoreboardRowStrings::~gcLobbyScoreboardRowStrings(void)  @ 0x002843d8, 212B ──
gcLobbyScoreboardRowStrings::~gcLobbyScoreboardRowStrings() {
    *(char **)((char *)this + 4) = gcLobbyScoreboardRowStringsvirtualtable;
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
    *(int *)((char *)this + 4) = 0x37E6A8;
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0fbgcLobbyScoreboardRowStringsKAssignCopyPC6FcBase, 0x270\n");

// ── gcLobbyScoreboardRowStrings::AssignCopy(const cBase *) @ 0x00283a68, 624B ──
void gcLobbyScoreboardRowStrings::AssignCopy(const cBase *base) {
    const gcLobbyScoreboardRowStrings *other = 0;
    char *slot = (char *)this + 0x08;

    if (base != 0) {
        if (D_0009F500 == 0) {
            if (D_0009F454 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_0009F454 = cType::InitializeType(
                    0, 0, 0x170, D_000385DC, 0, 0, 0, 0);
            }
            D_0009F500 = cType::InitializeType(
                0, 0, 0x128, D_0009F454,
                (cBase *(*)(cMemPool *, cBase *))&gcLobbyScoreboardRowStrings::New,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F500;
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
            other = (const gcLobbyScoreboardRowStrings *)base;
        }
    }

    int finalField;

    if ((char *)other + 0x08 != slot) {
        goto copy_desired;
    }
    finalField = other->mField;
    goto done;

copy_desired:
    {
        int flag = 1;
        int value = *(int *)((char *)this + 0x08);
        int tag = value & 1;
        if (tag != 0) {
            flag = 0;
        }
        if (flag != 0) {
            int flag2 = 0;
            int old = value;
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

        int srcValue = *(int *)((char *)other + 0x08);
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
        finalField = other->mField;
    }
done:
    mField = finalField;
}
