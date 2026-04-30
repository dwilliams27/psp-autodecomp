#include "cBase.h"

class cFile;
class cMemPool;
class cBase;
class cType;
class gcDoSetEventObjectArray;

extern "C" void gcAction_gcAction(gcDoSetEventObjectArray *, cBase *);
extern "C" void gcAction_Write(const gcDoSetEventObjectArray *, cFile &);
extern "C" void gcAction___dtor_gcAction_void(void *, int);

extern char gcDoSetEventObjectArrayvirtualtable[];

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

class cBaseArray {
public:
    void RemoveAll(void);
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcDoSetEventObjectArray {
public:
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *);
    ~gcDoSetEventObjectArray(void);
};

extern const char gcDoSetEventObjectArray_base_name[] asm("D_0036D894");
extern const char gcDoSetEventObjectArray_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoSetEventObjectArray asm("D_0009F6EC");

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

struct PoolDeleteSlot {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct DtorSlot {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

void *cMemPool_GetPoolFromPtr(const void *);

inline void gcDoSetEventObjectArray::operator delete(void *ptr) {
    void *pool = cMemPool_GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    PoolDeleteSlot *slot = (PoolDeleteSlot *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

const cType *gcDoSetEventObjectArray::GetType(void) const {
    if (!type_gcDoSetEventObjectArray) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoSetEventObjectArray_base_name,
                        gcDoSetEventObjectArray_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoSetEventObjectArray = cType::InitializeType(
            0, 0, 0x1FD, type_action, gcDoSetEventObjectArray::New,
            0, 0, 0);
    }
    return type_gcDoSetEventObjectArray;
}

// ----------------------------------------------------------------
// gcDoSetEventObjectArray::Write(cFile &) const @ 0x002fdc9c
// ----------------------------------------------------------------
void gcDoSetEventObjectArray::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    wb.Write(((int *)this)[3]);
    ((cBaseArray *)((char *)this + 16))->Write(wb);
    ((gcDesiredValue *)((char *)this + 24))->Write(wb);
    wb.End();
}

// ----------------------------------------------------------------
// gcDoSetEventObjectArray::New(cMemPool *, cBase *) static @ 0x002fdae8
// ----------------------------------------------------------------
cBase *gcDoSetEventObjectArray::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoSetEventObjectArray *result = 0;
    gcDoSetEventObjectArray *obj =
        (gcDoSetEventObjectArray *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoSetEventObjectArrayvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 0;
        ((int **)obj)[5] = (int *)obj;
        ((int *)obj)[6] = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

// Original object keeps this dead branch tail inside the destructor symbol.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

gcDoSetEventObjectArray::~gcDoSetEventObjectArray(void) {
    *(void **)((char *)this + 4) = gcDoSetEventObjectArrayvirtualtable;

    cBaseArray *items = (cBaseArray *)((char *)this + 0x10);

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

    if (items != 0) {
        items->RemoveAll();
    }

    gcAction___dtor_gcAction_void(this, 0);
}
