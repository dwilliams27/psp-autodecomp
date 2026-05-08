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

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(void *, void *, unsigned int);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

class cBaseArray {
public:
    void RemoveAll(void);
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
};

class gcDoSetEventObjectArray {
public:
    float Evaluate(void) const;
    void GetText(char *) const;
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
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

typedef float (*EvaluateFn)(const void *);
struct EvaluateSlot {
    short offset;
    short pad;
    EvaluateFn fn;
};

typedef int (*EvaluateObjectFn)(void *, int);
struct EvaluateObjectSlot {
    short offset;
    short pad;
    EvaluateObjectFn fn;
};

struct TextSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

void *cMemPool_GetPoolFromPtr(const void *);
void cStrAppend(char *, const char *, ...);
void cStrCat(char *, const char *);

extern void *g_expressionEvalStack asm("D_0037D7B4");

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
// gcDoSetEventObjectArray::Read(cFile &, cMemPool *) @ 0x002fdd0c
// ----------------------------------------------------------------
int gcDoSetEventObjectArray::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 &&
        ((gcAction *)this)->gcAction::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    {
        void *h = *(void **)rb._data[0];
        cFileSystem::Read(h, (char *)this + 0x0C, 4);
    }
    ((cBaseArray *)((char *)this + 0x10))->Read(rb);
    ((gcDesiredValue *)((char *)this + 0x18))->Read(rb);
    return result;
}

// ----------------------------------------------------------------
// gcDoSetEventObjectArray::Evaluate(void) const @ 0x002fddf4
// ----------------------------------------------------------------
float gcDoSetEventObjectArray::Evaluate(void) const {
    int val = *(int *)((const char *)this + 0x18);
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    if (flag != 0) {
        val = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }
    const void *ptr = (const void *)val;

    float value;
    if (ptr != 0) {
        EvaluateSlot *slot = (EvaluateSlot *)(*(char **)((const char *)ptr + 4) + 0x70);
        value = slot->fn((const char *)ptr + slot->offset);
    } else {
        value = 0.0f;
    }
    register int index __asm__("$4") = (int)value;

    if (index >= 0) {
        int *items = *(int **)((const char *)this + 0x10);
        int count = 0;
        if (items != 0) {
            count = items[-1];
        }
        if (index < count) {
            register int stackBase __asm__("$17") = 0;
            int *itemSlot = items + index;
            void *stack = g_expressionEvalStack;
            int *item = (int *)*itemSlot;
            if (stack != 0) {
                stackBase = *(int *)((char *)stack + 0x10);
            } else {
                __asm__ volatile("" ::: "memory");
            }

            int result = 0;
            if (item != 0) {
                EvaluateObjectSlot *slot =
                    (EvaluateObjectSlot *)(*(char **)((char *)item + 4) + 0x70);
                result = slot->fn((char *)item + slot->offset, 0);
            }
            *(int *)(stackBase + (*(int *)((const char *)this + 0x0C) * 4) + 0x40) = result;
            return 1.0f;
        }
    }
    return 0.0f;
}

// ----------------------------------------------------------------
// gcDoSetEventObjectArray::GetText(char *) const @ 0x002fdf1c
// ----------------------------------------------------------------
void gcDoSetEventObjectArray::GetText(char *buf) const {
    int val = *(int *)((const char *)this + 0x18);
    char text[256];
    text[0] = '\0';

    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    if (flag != 0) {
        val = 0;
    } else {
        __asm__ volatile("" ::: "memory");
    }
    void *ptr = (void *)val;

    if (ptr != 0) {
        TextSlot *slot = (TextSlot *)(*(char **)((char *)ptr + 4) + 0xD0);
        slot->fn((char *)ptr + slot->offset, text);
    } else {
        cStrCat(text, (const char *)0x36DB24);
    }
    cStrAppend(buf, (const char *)0x36EF38, (const char *)0x36DAF0, text);
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
