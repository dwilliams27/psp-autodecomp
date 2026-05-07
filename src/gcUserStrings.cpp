// gcUserStrings.cpp — gcAll_psp.obj
//
// Functions matched here:
//   0x00291bb0  gcUserStrings::New(cMemPool *, cBase *) static   144B
//   0x00291d1c  gcUserStrings::Write(cFile &) const              100B
//
// Layout: cBase (8) + gcDesiredValue (4) + int (4) = 16 bytes.
// gcStringValue is a base with no extra members and no ctor body.

inline void *operator new(unsigned int, void *p) { return p; }

class cFile;
class cBase;
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
    void Write(unsigned int);
    void End(void);
};

extern char cBaseclassdesc[];
extern char gcUserStringsclassdesc[];

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

class gcDesiredValue {
public:
    int _data;
    void Write(cWriteBlock &) const;
};

class gcUserStrings : public gcStringValue {
public:
    gcDesiredValue mDesired;   // 0x08
    int mField0C;              // 0x0C

    gcUserStrings(cBase *parent) : gcStringValue(parent) {
        mClassDesc = gcUserStringsclassdesc;
        mDesired._data = (int)this | 1;
        mField0C = 0;
    }

    ~gcUserStrings();
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    void GetName(char *) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);

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
extern cType *D_0009F598;

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oNgcUserStringsdtv, 0xd4\n");

// -- gcUserStrings::~gcUserStrings @ 0x00292280 --
gcUserStrings::~gcUserStrings() {
    *(char **)((char *)this + 4) = gcUserStringsclassdesc;
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

// ── gcUserStrings::Write @ 0x00291d1c ──
void gcUserStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcStringValue::Write(file);
    mDesired.Write(wb);
    wb.Write(mField0C);
    wb.End();
}

// ── gcUserStrings::New @ 0x00291bb0 ──
cBase *gcUserStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcUserStrings *result = 0;
    gcUserStrings *obj =
        (gcUserStrings *)entry->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcUserStrings(parent);
        result = obj;
    }
    return (cBase *)result;
}

// -- gcUserStrings::GetType @ 0x00291c40 --
const cType *gcUserStrings::GetType(void) const {
    if (D_0009F598 == 0) {
        if (D_0009F454 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F598 = cType::InitializeType(0, 0, 0xA3, D_0009F454,
                                           &gcUserStrings::New, 0, 0, 0);
    }
    return D_0009F598;
}

// -- gcUserStrings::GetName @ 0x00291f18 --
void gcUserStrings::GetName(char *buf) const {
    cStrAppend(buf, (const char *)0x36E380);

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
__asm__(".size __0fNgcUserStringsKAssignCopyPC6FcBase, 0x270\n");

// -- gcUserStrings::AssignCopy @ 0x00291940 --
void gcUserStrings::AssignCopy(const cBase *base) {
    const gcUserStrings *other = 0;
    char *slot = (char *)this + 0x08;

    if (base != 0) {
        if (D_0009F598 == 0) {
            if (D_0009F454 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                D_0009F454 = cType::InitializeType(
                    0, 0, 0x170, D_000385DC, 0, 0, 0, 0);
            }
            D_0009F598 = cType::InitializeType(
                0, 0, 0xA3, D_0009F454, &gcUserStrings::New,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F598;
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
            other = (const gcUserStrings *)base;
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
        int value = *(int *)slot;
        int flag = 1;
        int tag = value & 1;
        if (tag != 0) {
            flag = 0;
        }
        if (flag != 0) {
            int old = value;
            __asm__ volatile("" ::: "memory");
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
            *(int *)slot = value;
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
            __asm__ volatile("" ::: "memory");
            int current = *(int *)slot;
            cMemPool *pool = cMemPool::GetPoolFromPtr(slot);
            short cloneOffset = clone->offset;
            int currentFlag = 0;
            void *target = (unsigned char *)source + cloneOffset;
            if (current & 1) {
                currentFlag = 1;
            }
            if (currentFlag != 0) {
                current &= ~1;
            } else {
                current = *(int *)current;
            }
            *(int *)slot = (int)clone->fn(target, pool, (cBase *)current);
        }
        finalField = other->mField0C;
    }
done:
    mField0C = finalField;
}
