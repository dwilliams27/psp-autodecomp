#include "cBase.h"

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
    void Write(bool);
    void Write(int);
    void End(void);
};

class cGUID {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcDoLoadMap {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    gcDoLoadMap &operator=(const gcDoLoadMap &);
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

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern char gcDoLoadMapvirtualtable[];
void gcAction_gcAction(gcDoLoadMap *, cBase *);
void gcAction_Write(const gcDoLoadMap *, cFile &);

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoLoadMap asm("D_0009F684");

cBase *gcDoLoadMap::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoLoadMap *result = 0;
    gcDoLoadMap *obj = (gcDoLoadMap *)entry->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoLoadMapvirtualtable;
        ((int *)obj)[3] = 0;
        *((unsigned char *)obj + 0x10) = 0;
        ((int *)obj)[5] = (int)obj | 1;
        ((int *)obj)[6] = 0;
        ((int *)obj)[7] = 0;
        *((unsigned char *)obj + 0x20) = 0;
        ((int *)obj)[9] = (int)obj | 1;
        ((int *)obj)[10] = 0;
        ((int *)obj)[11] = 0;
        ((int *)obj)[12] = (int)obj | 1;
        ((int *)obj)[13] = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoLoadMap::GetType(void) const {
    if (!type_gcDoLoadMap) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A,
                                                        type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoLoadMap = cType::InitializeType(
            0, 0, 0xB2, type_action, gcDoLoadMap::New, 0, 0, 0);
    }
    return type_gcDoLoadMap;
}

void gcDoLoadMap::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoLoadMap) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoLoadMap = cType::InitializeType(
                0, 0, 0xB2, type_action, gcDoLoadMap::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoLoadMap;
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
    *this = *(const gcDoLoadMap *)copy;
}

void gcDoLoadMap::Write(cFile &file) const {
    cWriteBlock wb(file, 8);
    gcAction_Write(this, file);
    wb.Write(*(bool *)((char *)this + 0x10));
    ((gcDesiredValue *)((char *)this + 0x14))->Write(wb);
    ((cGUID *)((char *)this + 0x18))->Write(wb);
    wb.Write(*(bool *)((char *)this + 0x20));
    ((gcDesiredValue *)((char *)this + 0x24))->Write(wb);
    ((cGUID *)((char *)this + 0x28))->Write(wb);
    wb.Write(((int *)this)[3]);
    ((gcDesiredValue *)((char *)this + 0x30))->Write(wb);
    ((gcDesiredValue *)((char *)this + 0x34))->Write(wb);
    wb.End();
}
