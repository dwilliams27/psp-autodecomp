// gcTableString.cpp - gcAll_psp.obj
//
// Functions in this file:
//   0x0028ad40 gcTableString::New(cMemPool *, cBase *) static  200B
//   0x0028af20 gcTableString::Write(cFile &) const             132B
//   0x0035f188 gcValTableEntry::New(cMemPool *, cBase *) static 200B

class cBase;
class cFile;
class cMemPool;

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
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

struct DispatchEntry {
    short offset;
    short _pad;
    cType *(*fn)(void *, short, void *);
};

typedef void (*DesiredWriteFn)(cBase *, cFile *);

struct DesiredWriteSlot {
    short mOffset;
    short _pad;
    DesiredWriteFn mFn;
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
