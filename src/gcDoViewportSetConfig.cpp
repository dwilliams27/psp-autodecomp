// gcDoViewportSetConfig.cpp -- decompiled from gcAll_psp.obj.
//
// Functions:
//   0x00317a98 gcDoViewportSetConfig::New(cMemPool *, cBase *) static
//   0x00317c4c gcDoViewportSetConfig::Write(cFile &) const
//   0x00238d70 gcEnumeration::New(cMemPool *, cBase *) static

class cBase;
class cFile;
class cMemPool;
class cType;

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
    unsigned int mValue;
    void Write(cWriteBlock &) const;
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;       // 0x00
    void *mVTable;       // 0x04
    unsigned int mNext;  // 0x08

    void Write(cFile &) const;
};

class cObject {
public:
    cObject(cBase *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct PoolDeleteSlot {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DtorSlot {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

extern "C" {
    void gcAction_gcAction(void *, cBase *);
    void cObject_cObject(void *, cBase *);
    void gcAction___dtor_gcAction_void(void *, int);
}

void *cMemPool_GetPoolFromPtr(const void *);

extern char gcDoViewportSetConfigvirtualtable[];
extern char gcEnumerationvirtualtable[];

class gcDoViewportSetConfig : public gcAction {
public:
    int mFieldC;             // 0x0C
    int mField10;            // 0x10
    cHandle mHandle;         // 0x14
    gcDesiredValue mDesired; // 0x18

    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *);
    const cType *GetType(void) const;
    ~gcDoViewportSetConfig(void);
    void Write(cFile &) const;
};

class gcEnumeration : public cObject {
public:
    char pad[0x44];
    void *mArrayData;        // 0x44
    void *mArrayOwner;       // 0x48
    unsigned char mField4C;  // 0x4C
    char pad4D[3];
    int mField50;            // 0x50
    int mHandle;             // 0x54

    static cBase *New(cMemPool *, cBase *);
};

inline void gcDoViewportSetConfig::operator delete(void *ptr) {
    void *pool = cMemPool_GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    PoolDeleteSlot *slot = (PoolDeleteSlot *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

cBase *gcDoViewportSetConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoViewportSetConfig *result = 0;
    gcDoViewportSetConfig *obj =
        (gcDoViewportSetConfig *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoViewportSetConfigvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 0;
        unsigned int tagged = ((unsigned int)obj) | 1;
        ((int *)obj)[5] = 0;
        ((unsigned int *)obj)[6] = tagged;
        result = obj;
    }
    return (cBase *)result;
}

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoViewportSetConfig;

const cType *gcDoViewportSetConfig::GetType(void) const {
    if (!type_gcDoViewportSetConfig) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoViewportSetConfig = cType::InitializeType(
            0, 0, 0xA9, type_action, gcDoViewportSetConfig::New, 0, 0, 0);
    }
    return type_gcDoViewportSetConfig;
}

void gcDoViewportSetConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction::Write(file);
    wb.Write(mFieldC);
    wb.Write(mField10);
    mHandle.Write(wb);
    mDesired.Write(wb);
    wb.End();
}

// Original object keeps this dead branch tail inside the destructor symbol.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

gcDoViewportSetConfig::~gcDoViewportSetConfig(void) {
    *(void **)((char *)this + 4) = gcDoViewportSetConfigvirtualtable;

    if ((void *)((char *)this + 0x18) != 0) {
        int owned = 1;
        int val = *(int *)((char *)this + 0x18);
        if (val & 1) {
            owned = 0;
        }
        if (owned != 0 && val != 0) {
            char *typeInfo = *(char **)(val + 4);
            DtorSlot *slot = (DtorSlot *)(typeInfo + 0x50);
            slot->fn((char *)val + slot->offset, 3);
            *(int *)((char *)this + 0x18) = 0;
        }
    }

    gcAction___dtor_gcAction_void(this, 0);
}

cBase *gcEnumeration::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEnumeration *result = 0;
    gcEnumeration *obj = (gcEnumeration *)entry->fn(base, 0x58, 4, 0, 0);
    if (obj != 0) {
        cObject_cObject(obj, parent);
        ((void **)obj)[1] = gcEnumerationvirtualtable;
        obj->mArrayData = 0;
        obj->mArrayOwner = obj;
        obj->mField4C = 0;
        obj->mField50 = 0;
        obj->mHandle = 0;
        result = obj;
    }
    return (cBase *)result;
}
