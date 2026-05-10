class cBase;
class cFile;
class cFileHandle;
class cMemPool;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class gcDesiredValue {
public:
    int mOwner;
    void Write(cWriteBlock &) const;
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

class gcStringLValue {
public:
    cBase *mOwner;
    void *mVTable;

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
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

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
};

struct GetNameSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

extern char cBaseclassdesc[];
extern char gcStatsTrackingStringsvirtualtable[];
extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F458;
extern cType *D_0009F568;

class gcStatsTrackingStrings : public gcStringLValue {
public:
    int mField8;
    gcDesiredValue mDesiredC;
    int mField10;

    static cBase *New(cMemPool *, cBase *);
    gcStatsTrackingStrings &operator=(const gcStatsTrackingStrings &);
    void AssignCopy(const cBase *);
    void GetName(char *) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
    static void operator delete(void *);
    ~gcStatsTrackingStrings(void);
};

inline void gcStatsTrackingStrings::operator delete(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DtorDeleteRecord *slot = (DtorDeleteRecord *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

cBase *gcStatsTrackingStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcStatsTrackingStrings *result = 0;
    gcStatsTrackingStrings *obj =
        (gcStatsTrackingStrings *)entry->fn(base, 0x14, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)((char *)obj + 0) = parent;
        *(void **)((char *)obj + 4) = gcStatsTrackingStringsvirtualtable;
        *(int *)((char *)obj + 8) = 0;
        int self = (int)obj | 1;
        *(int *)((char *)obj + 0xC) = self;
        *(int *)((char *)obj + 0x10) = self;
        result = obj;
    }
    return (cBase *)result;
}

void gcStatsTrackingStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcStringLValue::Write(file);
    wb.Write(mField8);
    mDesiredC.Write(wb);
    int value = mField10;
    int flag = 0;
    if (value & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)value;
    }
    wb.WriteBase(ptr);
    wb.End();
}

int gcStatsTrackingStrings::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);

    if (rb[3] != 1 || gcStringLValue::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x08, 4);
    ((gcDesiredValue *)((char *)this + 0x0C))->Read(*(cReadBlock *)rb);

    int sp14;
    int value = *(int *)((char *)this + 0x10);
    int tag = value & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    int outValue;
    if (flag != 0) {
        outValue = 0;
        goto out_done;
    }
    outValue = value;
out_done:
    sp14 = outValue;

    int flag2 = 0;
    if (tag != 0) {
        flag2 = 1;
    }

    int base;
    if (flag2 != 0) {
        base = value & ~1;
    } else {
        base = *(int *)value;
    }

    cMemPool *childPool = cMemPool::GetPoolFromPtr((char *)this + 0x10);
    ((cReadBlock *)rb)->ReadBase(childPool, (cBase *)base, *(cBase **)&sp14);

    int newValue;
    if (sp14 == 0) {
        newValue = base | 1;
    } else {
        newValue = sp14;
    }
    *(int *)((char *)this + 0x10) = newValue;
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

const cType *gcStatsTrackingStrings::GetType(void) const {
    if (D_0009F568 == 0) {
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
        D_0009F568 = cType::InitializeType(0, 0, 0xE9, D_0009F458,
                                           &gcStatsTrackingStrings::New,
                                           0, 0, 0);
    }
    return D_0009F568;
}

void gcStatsTrackingStrings::AssignCopy(const cBase *base) {
    const gcStatsTrackingStrings *other = 0;

    if (base != 0) {
        if (D_0009F568 == 0) {
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
            D_0009F568 = cType::InitializeType(0, 0, 0xE9, D_0009F458,
                                               &gcStatsTrackingStrings::New,
                                               0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F568;
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
            other = (const gcStatsTrackingStrings *)base;
        }
    }
    operator=(*other);
}

void gcStatsTrackingStrings::GetName(char *buf) const {
    cStrAppend(buf, (const char *)0x36E2EC, (const char *)0x36DAF0);

    int val = *(const int *)((const char *)this + 0x10);
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }

    if (flag != 0) {
        val = 0;
    } else {
        val = (val != 0);
        val &= 0xFF;
        val = (val != 0);
    }

    if (val != 0) {
        cStrCat(buf, (const char *)0x36E300);

        register int val2 __asm__("$4") =
            *(const int *)((const char *)this + 0x10);
        register int check __asm__("$5") = 0;
        register int bit2 __asm__("$6") = val2 & 1;
        if (bit2) {
            check = 1;
        }

        char *typeInfo;
        if (check != 0) {
            check = 0;
            typeInfo = *(char **)(check + 4);
        } else {
            check = val2;
            typeInfo = *(char **)(check + 4);
        }
        GetNameSlot *entry = (GetNameSlot *)(typeInfo + 0x40);
        short off = entry->offset;
        void (*fn)(void *, char *) = entry->fn;
        fn((char *)check + off, buf);

        cStrCat(buf, (const char *)0x36E2E8);
    }
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

gcStatsTrackingStrings::~gcStatsTrackingStrings(void) {
    *(void **)((char *)this + 4) = gcStatsTrackingStringsvirtualtable;
    char *second = (char *)this + 0x0C;

    if ((void *)((char *)this + 0x10) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x10);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0) {
            if (val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, (void *)3);
                *(int *)((char *)this + 0x10) = 0;
            }
        }
    }

    if ((void *)second != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x0C);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0x0C) = 0;
        }
    }

    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}
