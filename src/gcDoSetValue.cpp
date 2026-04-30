#include "gcDoReturn.h"
#include "cBase.h"

void gcAction_gcAction(gcDoSetValue *, cBase *);
void gcAction_Write(const gcDoSetValue *, cFile &);
void gcDoSetValue_gcDoSetValue(gcDoSetValue *, cBase *);

extern char gcDoSetValuevirtualtable[];
extern const char gcDoSetValue_base_name[];
extern const char gcDoSetValue_base_desc[];

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void WriteBase(const cBase *);
    void Write(unsigned int);
    void End(void);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

// ----------------------------------------------------------------
// gcDoSetValue::gcDoSetValue(cBase *) @ 0x0014f65c
// ----------------------------------------------------------------
gcDoSetValue::gcDoSetValue(cBase *parent) {
    gcAction_gcAction(this, parent);
    ((void **)this)[1] = gcDoSetValuevirtualtable;
    ((int *)this)[3] = 0;
    int encoded = (int)this | 1;
    ((int *)this)[4] = encoded;
    ((int *)this)[5] = encoded;
}

// ----------------------------------------------------------------
// gcDoSetValue::GetChild(int) const @ 0x00300748
// ----------------------------------------------------------------
gcExpression *gcDoSetValue::GetChild(int index) const {
    if (index == 0) {
        int val = ((int *)this)[4];
        int flag = 0;
        if (val & 1) {
            flag = 1;
        }
        gcExpression *result;
        if (flag != 0) {
            result = 0;
        } else {
            result = (gcExpression *)val;
        }
        return result;
    }
    int val = ((int *)this)[5];
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    if (flag != 0) {
        val = 0;
    }
    return (gcExpression *)val;
}

// ----------------------------------------------------------------
// gcDoSetValue::New(cMemPool *, cBase *) @ 0x003005ac
// ----------------------------------------------------------------
cBase *gcDoSetValue::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoSetValue *result = 0;
    gcDoSetValue *obj = (gcDoSetValue *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcDoSetValue_gcDoSetValue(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ----------------------------------------------------------------
// gcDoSetValue::Write(cFile &) const @ 0x0014f3d4
// ----------------------------------------------------------------
void gcDoSetValue::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction_Write(this, file);
    wb.Write(((unsigned int *)this)[3]);

    int val10 = ((int *)this)[4];
    int flag1 = 0;
    if (val10 & 1) {
        flag1 = 1;
    }
    cBase *ptr1;
    if (flag1 != 0) {
        ptr1 = 0;
    } else {
        ptr1 = (cBase *)val10;
    }
    wb.WriteBase(ptr1);

    int val14 = ((int *)this)[5];
    int flag2 = 0;
    if (val14 & 1) {
        flag2 = 1;
    }
    cBase *ptr2;
    if (flag2 != 0) {
        ptr2 = 0;
    } else {
        ptr2 = (cBase *)val14;
    }
    wb.WriteBase(ptr2);

    wb.End();
}

// ----------------------------------------------------------------
// gcDoSetValue::GetType(void) const @ 0x00300628
// ----------------------------------------------------------------
static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoSetValue;

// ----------------------------------------------------------------
// gcDoSetValue::AssignCopy(const cBase *) @ 0x00300408
// ----------------------------------------------------------------
void gcDoSetValue::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoSetValue) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoSetValue_base_name, gcDoSetValue_base_desc, 1,
                            0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoSetValue = cType::InitializeType(
                0, 0, 0x78, type_action, gcDoSetValue::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoSetValue;
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
    *this = *(const gcDoSetValue *)copy;
}

const cType *gcDoSetValue::GetType(void) const {
    if (!type_gcDoSetValue) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoSetValue_base_name, gcDoSetValue_base_desc, 1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoSetValue = cType::InitializeType(0, 0, 0x78, type_action, gcDoSetValue::New, 0, 0, 0);
    }
    return type_gcDoSetValue;
}
