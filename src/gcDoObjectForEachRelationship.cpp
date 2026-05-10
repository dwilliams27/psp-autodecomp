class gcExpression;
class cBase;
class cFile;
class cFileHandle;
class cType;
class cReadBlock;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcDoObjectForEachRelationship {
public:
    char _pad[0x28];
    gcExpression *branch;

    ~gcDoObjectForEachRelationship();
    int GetMaxBranches(void) const;
    gcExpression *GetBranch(int) const;
    void SetBranch(int, gcExpression *);
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    gcDoObjectForEachRelationship &
    operator=(const gcDoObjectForEachRelationship &);
    static cBase *New(cMemPool *, cBase *);
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

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cWriteBlock {
public:
    cFile *_file;
    int _pos;

    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcExpressionList {
public:
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
};

class gcAction {
public:
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class cHandle {
public:
    int mIndex;
    void Read(cReadBlock &, cMemPool *);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short pad;
    const cType *(*getType)(void *);
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

struct WriteRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

struct ReadRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

void gcAction_gcAction(void *, cBase *);
void gcExpressionList_gcExpressionList(void *, void *);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern "C" void gcExpressionList___dtor_gcExpressionList_void(void *, int);
extern "C" void gcAction___dtor_gcAction_void(void *, int);

extern char gcDoObjectForEachRelationshipvirtualtable[];
extern char gcDesiredEnumerationEntryvirtualtable[];
extern const char gcDoObjectForEachRelationship_base_name[] asm("D_0036D894");
extern const char gcDoObjectForEachRelationship_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoObjectForEachRelationship asm("D_0009F6B0");

// ── gcDoObjectForEachRelationship::~gcDoObjectForEachRelationship @ 0x002ea2bc ──
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0odgcDoObjectForEachRelationshipdtv, 0x158\n");

gcDoObjectForEachRelationship::~gcDoObjectForEachRelationship(void) {
    *(void **)((char *)this + 4) = gcDoObjectForEachRelationshipvirtualtable;
    gcExpressionList___dtor_gcExpressionList_void((char *)this + 0x28, 2);
    char *second = (char *)this + 0xC;

    if ((void *)((char *)this + 0x10) != 0) {
        *(void **)((char *)this + 0x14) = (void *)0x388568;
        if ((void *)((char *)this + 0x24) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x24);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0) {
                if (val != 0) {
                    char *typeInfo = *(char **)(val + 4);
                    DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
                    slot->fn((char *)val + slot->offset, (void *)3);
                    *(int *)((char *)this + 0x24) = 0;
                }
            }
        }
        *(void **)((char *)this + 0x14) = (void *)0x37E6A8;
    }

    if ((void *)second != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0xC);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorDeleteRecord *slot = (DtorDeleteRecord *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, (void *)3);
            *(int *)((char *)this + 0xC) = 0;
        }
    }

    gcAction___dtor_gcAction_void(this, 0);
}

int gcDoObjectForEachRelationship::GetMaxBranches(void) const {
    return 1;
}

cBase *gcDoObjectForEachRelationship::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoObjectForEachRelationship *result = 0;
    gcDoObjectForEachRelationship *obj =
        (gcDoObjectForEachRelationship *)entry->fn(base, 0x30, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoObjectForEachRelationshipvirtualtable;
        int self = (int)((unsigned int)obj | 1);
        ((int *)obj)[3] = self;
        ((int *)obj)[5] = 0x37E6A8;
        ((gcDoObjectForEachRelationship **)obj)[4] = obj;
        ((void **)obj)[5] = gcDesiredEnumerationEntryvirtualtable;
        *((char *)obj + 0x18) = 1;
        *((char *)obj + 0x19) = 0;
        ((int *)obj)[7] = 0;
        ((int *)obj)[8] = 0;
        ((int *)obj)[9] = self;
        gcExpressionList_gcExpressionList((char *)obj + 0x28, obj);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoObjectForEachRelationship::GetType(void) const {
    if (!type_gcDoObjectForEachRelationship) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoObjectForEachRelationship_base_name,
                        gcDoObjectForEachRelationship_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoObjectForEachRelationship = cType::InitializeType(
            0, 0, 0x1A4, type_action,
            gcDoObjectForEachRelationship::New, 0, 0, 0);
    }
    return type_gcDoObjectForEachRelationship;
}

void gcDoObjectForEachRelationship::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcAction *)this)->Write(file);

    int value = ((const int *)this)[3];
    int flag = 0;
    int tagged = value & 1;
    if (tagged != 0) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)value;
    }
    wb.WriteBase(ptr);

    char *typeInfo = *(char **)((const char *)this + 0x14);
    WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
    short off = rec->offset;
    void *base = (char *)this + 0x10;
    rec->fn((char *)base + off, wb._file);

    ((const gcExpressionList *)((const char *)this + 0x28))->Write(wb);
    wb.End();
}

int gcDoObjectForEachRelationship::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if ((unsigned int)rb[3] >= 3 || (unsigned int)rb[3] < 1 ||
        ((gcAction *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    int value = *(int *)((char *)this + 0x0C);
    int tag = value & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    int outValue;
    if (flag != 0) {
        outValue = 0;
        goto out_done0;
    }
    outValue = value;
out_done0:
    int sp14 = outValue;

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

    ((cReadBlock *)rb)->ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x0C),
                                 (cBase *)base, *(cBase **)&sp14);

    register int newValue __asm__("$5");
    if (sp14 != 0) {
        newValue = sp14;
    } else {
        newValue = base | 1;
    }
    register unsigned int version __asm__("$4") = rb[3];
    *(int *)((char *)this + 0x0C) = newValue;

    if (version >= 2) {
        char *typeInfo = *(char **)((char *)this + 0x14);
        char *basePtr = (char *)this + 0x10;
        ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
        short off = rec->offset;
        cFile *f = *(cFile **)&rb[0];
        rec->fn(basePtr + off, f, cMemPool::GetPoolFromPtr(basePtr));
    } else {
        cHandle *handle = (cHandle *)((char *)this + 0x1C);
        *(int *)((char *)this + 0x1C) = 0;
        handle->Read(*(cReadBlock *)rb, cMemPool::GetPoolFromPtr(handle));
        *(char *)((char *)this + 0x18) = 1;
    }

    ((gcExpressionList *)((char *)this + 0x28))->Read(*(cReadBlock *)rb);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

void gcDoObjectForEachRelationship::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoObjectForEachRelationship) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoObjectForEachRelationship_base_name,
                            gcDoObjectForEachRelationship_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoObjectForEachRelationship = cType::InitializeType(
                0, 0, 0x1A4, type_action,
                gcDoObjectForEachRelationship::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoObjectForEachRelationship;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short voff = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        const cType *type = getType((char *)other + voff);
        int ok;

        if (myType == 0) {
            ok = 0;
            goto done;
        }
        if (type != 0) {
        loop:
            if (type == myType) {
                ok = 1;
                goto done;
            }
            type = (const cType *)((cTypeNode *)type)->parent;
            if (type != 0) {
                goto loop;
            }
        }
        ok = 0;
    done:
        if (ok != 0) {
            copy = other;
        }
    }
    *this = *(const gcDoObjectForEachRelationship *)copy;
}
