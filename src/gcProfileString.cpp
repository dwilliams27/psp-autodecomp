class cBase;
class cFile;
class cFileHandle;
class cMemPool;
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

class gcDesiredValue {
public:
    void Read(cReadBlock &);
};

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
};

class gcProfileString {
public:
    gcProfileString &operator=(const gcProfileString &);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *);
    ~gcProfileString(void);
};

struct TypeReadEntry {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F458;
extern cType *D_0009F558;

extern "C" int gcStringLValue_Read(void *, cFile &, cMemPool *)
    asm("__0fOgcStringLValueEReadR6FcFileP6IcMemPool");
extern "C" void *cMemPool_GetPoolFromPtr(const void *)
    asm("__0fP8cMemPoolLGetPoolFromPtrPCvT");
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern char gcProfileStringvirtualtable[];

inline void gcProfileString::operator delete(void *ptr) {
    void *pool = cMemPool_GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

void gcProfileString::AssignCopy(const cBase *base) {
    const gcProfileString *other = 0;

    if (base != 0) {
        if (D_0009F558 == 0) {
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
            D_0009F558 = cType::InitializeType(0, 0, 0x1EE, D_0009F458,
                                               &gcProfileString::New,
                                               0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F558;
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
                type = (cType *)type->mParent;
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
            other = (const gcProfileString *)base;
        }
    }
    operator=(*other);
}

const cType *gcProfileString::GetType(void) const {
    if (D_0009F558 == 0) {
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
        D_0009F558 = cType::InitializeType(0, 0, 0x1EE, D_0009F458,
                                           &gcProfileString::New,
                                           0, 0, 0);
    }
    return D_0009F558;
}

int gcProfileString::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if (rb._data[3] != 1 || gcStringLValue_Read(this, file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }

    ((gcDesiredValue *)((char *)this + 8))->Read(rb);
    cFileSystem::Read((cFileHandle *)*(void **)rb._data[0], (char *)this + 0x0C, 4);
    {
        char *typeInfo = *(char **)((char *)this + 0x14);
        char *base = (char *)this + 0x10;
        TypeReadEntry *slot = (TypeReadEntry *)(typeInfo + 0x30);
        slot->fn(base + slot->offset,
                 *(cFile **)&rb._data[0],
                 (cMemPool *)cMemPool_GetPoolFromPtr(base));
    }
    return result;
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oPgcProfileStringdtv, 0x148\n");

gcProfileString::~gcProfileString(void) {
    *(void **)((char *)this + 4) = gcProfileStringvirtualtable;
    void *baseVtable = (void *)0x37E6A8;
    char *first = (char *)this + 0x10;
    char *desired = (char *)this + 8;

    if ((void *)first != 0) {
        *(void **)((char *)this + 0x14) = (void *)0x388568;
        if ((void *)((char *)this + 0x24) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x24);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0 && val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x24) = 0;
            }
        }
        *(void **)((char *)this + 0x14) = baseVtable;
    }

    if ((void *)desired != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 8);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 8) = 0;
        }
    }

    *(void **)((char *)this + 4) = baseVtable;
}
