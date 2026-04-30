class cBase;
class cMemPool;
class cType;

class gcStringLValue {};

class cType {
public:
    char _pad[0x1C];
    cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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

struct DispatchEntry {
    short offset;
    short _pad;
    cType *(*fn)(void *, short, void *);
};

class gcValTableEntry : public gcStringLValue {
public:
    gcValTableEntry &operator=(const gcValTableEntry &);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
};

extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern char D_00000838[];
extern char D_0000A198[];

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_variable asm("D_0009F3EC");
static cType *type_gcValTableEntry asm("D_0009F8E8");

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

const cType *gcValTableEntry::GetType(void) const {
    if (!type_gcValTableEntry) {
        if (!type_variable) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_variable = cType::InitializeType(
                0, 0, 0x6D, type_value, 0, 0, 0, 0);
        }
        type_gcValTableEntry = cType::InitializeType(
            0, 0, 0x218, type_variable, gcValTableEntry::New, 0, 0, 0);
    }
    return type_gcValTableEntry;
}

void gcValTableEntry::AssignCopy(const cBase *base) {
    const gcValTableEntry *other = 0;

    if (base != 0) {
        if (!type_gcValTableEntry) {
            if (!type_variable) {
                if (!type_value) {
                    if (!type_expression) {
                        if (!type_base) {
                            type_base = cType::InitializeType(
                                (const char *)0x36D894, (const char *)0x36D89C,
                                1, 0, 0, 0, 0, 0);
                        }
                        type_expression = cType::InitializeType(
                            0, 0, 0x6A, type_base, 0, 0, 0, 0);
                    }
                    type_value = cType::InitializeType(
                        0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
                }
                type_variable = cType::InitializeType(
                    0, 0, 0x6D, type_value, 0, 0, 0, 0);
            }
            type_gcValTableEntry = cType::InitializeType(
                0, 0, 0x218, type_variable, gcValTableEntry::New, 0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *wanted = type_gcValTableEntry;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, fn);
        int ok;

        if (wanted == 0) {
            ok = 0;
        } else if (type != 0) {
        loop:
            if (type == wanted) {
                ok = 1;
            } else {
                type = type->mParent;
                if (type != 0) {
                    goto loop;
                }
                goto fail;
            }
        } else {
fail:
            ok = 0;
        }
        if (ok != 0) {
            other = (const gcValTableEntry *)base;
        }
    }

    operator=(*other);
}
