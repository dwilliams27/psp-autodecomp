// gcTableString.cpp - gcAll_psp.obj
//
// Functions in this file:
//   0x0028ad40 gcTableString::New(cMemPool *, cBase *) static  200B
//   0x0028af20 gcTableString::Write(cFile &) const             132B
//   0x0035f188 gcValTableEntry::New(cMemPool *, cBase *) static 200B

class cBase;
class cFileHandle;

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcStringLValue {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class cReadBlock {
public:
    int _data[5];
};

class cHandle {
public:
    void Read(cReadBlock &, cMemPool *);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

struct DispatchEntry {
    short offset;
    short _pad;
    cType *(*fn)(void *, short, void *);
};

typedef void (*DesiredWriteFn)(cBase *, cFile *);
typedef void (*DesiredReadFn)(void *, cFile *, cMemPool *);

struct DesiredWriteSlot {
    short mOffset;
    short _pad;
    DesiredWriteFn mFn;
};

struct DesiredReadSlot {
    short mOffset;
    short _pad;
    DesiredReadFn mRead;
};

struct GetNameSlot {
    short offset;
    short _pad;
    void (*fn)(void *, char *);
};

struct DesiredTypeInfoWrite {
    char _pad[0x28];
    DesiredWriteSlot mSlot;
};

struct PoolBlock {
    char _pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcTableString : public gcStringLValue {
public:
    static cBase *New(cMemPool *, cBase *);
    gcTableString &operator=(const gcTableString &);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void GetName(char *) const;
};

class gcValTableEntry : public gcStringLValue {
public:
    static cBase *New(cMemPool *, cBase *);
};

extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern char D_00000838[];
extern char D_000015A8[];
extern char D_0000A198[];
extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F458;
extern cType *D_0009F574;

void cStrCat(char *, const char *);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

cBase *gcTableString::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcTableString *result = 0;
    gcTableString *obj =
        (gcTableString *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = 0x37E6A8;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = D_000015A8;
        gcDesiredObject_gcDesiredObject((char *)obj + 8, (cBase *)obj);
        ((void **)obj)[3] = D_00000838;
        ((int *)obj)[5] = 1;
        ((int *)obj)[6] = 0;
        ((int *)obj)[3] = 0x38A000;
        int self = (int)((unsigned int)obj | 1);
        ((int *)obj)[7] = self;
        ((int *)obj)[8] = self;
        result = obj;
    }
    return (cBase *)result;
}

void gcTableString::AssignCopy(const cBase *base) {
    const gcTableString *other = 0;

    if (base != 0) {
        if (D_0009F574 == 0) {
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
            D_0009F574 = cType::InitializeType(0, 0, 0x219, D_0009F458,
                                               &gcTableString::New,
                                               0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F574;
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
            other = (const gcTableString *)base;
        }
    }
    operator=(*other);
}

const cType *gcTableString::GetType(void) const {
    if (D_0009F574 == 0) {
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
        D_0009F574 = cType::InitializeType(0, 0, 0x219, D_0009F458,
                                           &gcTableString::New,
                                           0, 0, 0);
    }
    return D_0009F574;
}

void gcTableString::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcStringLValue::Write(file);

    DesiredTypeInfoWrite *ti = *(DesiredTypeInfoWrite **)((char *)this + 0x0C);
    DesiredWriteSlot *slot = &ti->mSlot;
    cBase *embedded = (cBase *)((char *)this + 0x08);
    slot->mFn((cBase *)((char *)embedded + slot->mOffset), wb._file);

    ((const gcDesiredValue *)((const char *)this + 0x1C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x20))->Write(wb);
    wb.End();
}

int gcTableString::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);

    if ((unsigned int)rb[3] >= 3 || (unsigned int)rb[3] < 1 ||
        gcStringLValue::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    if ((unsigned int)rb[3] >= 2) {
        char *desired = (char *)this + 0x08;
        char *type = ((char **)desired)[1];
        __asm__ volatile("" ::: "memory");
        cFile *f = *(cFile **)&rb[0];
        DesiredReadSlot *slot = (DesiredReadSlot *)(type + 0x30);
        slot->mRead(desired + slot->mOffset, f,
                    cMemPool::GetPoolFromPtr(desired));
    } else {
        *(int *)((char *)this + 0x14) = 7;
        *(int *)((char *)this + 0x18) = 0;
        cHandle *handle = (cHandle *)((char *)this + 0x18);
        handle->Read(*(cReadBlock *)rb, cMemPool::GetPoolFromPtr(handle));
    }

    ((gcDesiredValue *)((char *)this + 0x1C))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x20))->Read(*(cReadBlock *)rb);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

void gcTableString::GetName(char *buf) const {
    void *typeInfo0 = *(void **)((const char *)this + 0x0C);
    GetNameSlot *slot0 = (GetNameSlot *)((char *)typeInfo0 + 0x78);
    char *embedded = (char *)this + 0x08;
    slot0->fn(embedded + slot0->offset, buf);

    cStrCat(buf, (const char *)0x36E300);

    int val = *(const int *)((const char *)this + 0x1C);
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

    cStrCat(buf, (const char *)0x36DAD8);

    int val2 = *(const int *)((const char *)this + 0x20);
    int flag2 = 0;
    if (val2 & 1) {
        flag2 = 1;
    }
    if (flag2 != 0) {
        val2 = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }

    int check2 = val2;
    if (check2 != 0) {
        char *typeInfo = *(char **)(check2 + 4);
        GetNameSlot *slot = (GetNameSlot *)(typeInfo + 0xD0);
        slot->fn((char *)check2 + slot->offset, buf);
    } else {
        cStrCat(buf, (const char *)0x36DB24);
    }

    cStrCat(buf, (const char *)0x36E2E8);
}

cBase *gcValTableEntry::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValTableEntry *result = 0;
    gcValTableEntry *obj =
        (gcValTableEntry *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        ((int *)obj)[1] = 0x37E6A8;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = D_0000A198;
        gcDesiredObject_gcDesiredObject((char *)obj + 8, (cBase *)obj);
        ((void **)obj)[3] = D_00000838;
        ((int *)obj)[5] = 1;
        ((int *)obj)[6] = 0;
        ((int *)obj)[3] = 0x38A000;
        int self = (int)((unsigned int)obj | 1);
        ((int *)obj)[7] = self;
        ((int *)obj)[8] = self;
        result = obj;
    }
    return (cBase *)result;
}
