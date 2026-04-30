class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcDesiredValue {
public:
    int mOwner;
    void Write(cWriteBlock &) const;
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
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

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
