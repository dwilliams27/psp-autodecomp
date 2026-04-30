#include "cBase.h"

class cType {
public:
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
    void Write(bool);
    void Write(int);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

struct WriteRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

class gcDoLoadRegionSet : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    gcDoLoadRegionSet &operator=(const gcDoLoadRegionSet &);
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

extern char D_00003938[];
extern const char gcDoLoadRegionSet_base_name[];
extern const char gcDoLoadRegionSet_base_desc[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoLoadRegionSet;

void gcAction_gcAction(void *, cBase *);
extern "C" void gcEvent_ctor(void *, cBase *, const char *)
    __asm__("__0oHgcEventctP6FcBasePCc");

cBase *gcDoLoadRegionSet::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoLoadRegionSet *result = 0;
    gcDoLoadRegionSet *obj =
        (gcDoLoadRegionSet *)entry->fn(base, 0x54, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = D_00003938;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = (int)((unsigned int)obj | 1);
        ((int *)obj)[5] = ((int *)obj)[4];
        gcEvent_ctor((char *)obj + 0x18, (cBase *)obj, (const char *)0x36EBE8);
        gcEvent_ctor((char *)obj + 0x34, (cBase *)obj, (const char *)0x36EBF8);
        ((char *)obj)[0x50] = 0;
        ((char *)obj)[0x51] = 0;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoLoadRegionSet::GetType(void) const {
    if (!type_gcDoLoadRegionSet) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoLoadRegionSet_base_name,
                        gcDoLoadRegionSet_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoLoadRegionSet = cType::InitializeType(
            0, 0, 0xB4, type_action, gcDoLoadRegionSet::New, 0, 0, 0);
    }
    return type_gcDoLoadRegionSet;
}

void gcDoLoadRegionSet::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoLoadRegionSet) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoLoadRegionSet_base_name,
                            gcDoLoadRegionSet_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoLoadRegionSet = cType::InitializeType(
                0, 0, 0xB4, type_action, gcDoLoadRegionSet::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoLoadRegionSet;
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
    *this = *(const gcDoLoadRegionSet *)copy;
}

void gcDoLoadRegionSet::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    gcAction::Write(file);
    wb.Write(*(const int *)((const char *)this + 0x0C));
    ((const gcDesiredValue *)((const char *)this + 0x10))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x14))->Write(wb);

    {
        char *typeInfo = *(char **)((const char *)this + 0x1C);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (char *)this + 0x18;
        rec->fn((char *)base + off, wb._file);
    }

    {
        char *typeInfo = *(char **)((const char *)this + 0x38);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        short off = rec->offset;
        void *base = (char *)this + 0x34;
        rec->fn((char *)base + off, wb._file);
    }

    wb.Write(*(const bool *)((const char *)this + 0x50));
    wb.Write(*(const bool *)((const char *)this + 0x51));
    wb.End();
}
