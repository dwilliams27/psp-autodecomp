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
    void Write(int);
    void End(void);
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
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

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short pad;
    const cType *(*getType)(void *);
};

extern const char gcDoEntityAttach_base_name[];
extern const char gcDoEntityAttach_base_desc[];
extern char gcDoEntityAttachvirtualtable[];
extern char D_00000338[];

extern "C" {
void gcAction_ctor_cBase(void *, cBase *);
void gcDesiredObject_ctor_cBase(void *, cBase *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);
}

class gcDoEntityAttach : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    gcDoEntityAttach &operator=(const gcDoEntityAttach &);
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityAttach;

cBase *gcDoEntityAttach::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntityAttach *result = 0;
    gcDoEntityAttach *obj =
        (gcDoEntityAttach *)entry->fn(base, 0x88, 4, 0, 0);
    if (obj != 0) {
        gcAction_ctor_cBase(obj, parent);

        int *obj_i = (int *)obj;
        obj_i[1] = (int)gcDoEntityAttachvirtualtable;

        void *desired = (char *)obj + 0x10;
        obj_i[3] = 0;
        gcDesiredObject_ctor_cBase(desired, (cBase *)obj);

        obj_i[5] = (int)D_00000338;

        gcDesiredEntityHelper_ctor((char *)obj + 0x1C, 1, 0, 0);

        obj_i[5] = 0x388A48;
        obj_i[10] = 0x37E6A8;
        obj_i[9] = (int)desired;
        obj_i[10] = 0x388568;
        ((char *)obj)[0x2C] = 1;
        ((char *)obj)[0x2D] = 0;
        obj_i[12] = 0;
        obj_i[13] = 0;
        obj_i[14] = (int)desired | 1;

        desired = (char *)obj + 0x3C;
        gcDesiredObject_ctor_cBase(desired, (cBase *)obj);

        obj_i[16] = (int)D_00000338;

        gcDesiredEntityHelper_ctor((char *)obj + 0x48, 0, 0, 0);

        obj_i[21] = 0x37E6A8;
        obj_i[16] = 0x388A48;
        obj_i[20] = (int)desired;
        obj_i[21] = 0x388568;
        ((char *)obj)[0x58] = 1;
        ((char *)obj)[0x59] = 0;
        obj_i[23] = 0;
        obj_i[24] = 0;
        obj_i[25] = (int)desired | 1;
        *(short *)((char *)obj + 0x7C) = 0;
        *(short *)((char *)obj + 0x7E) = 0;
        int encoded = (int)obj | 1;
        ((char *)obj)[0x68] = 0;
        obj_i[32] = encoded;
        obj_i[33] = encoded;

        result = obj;
    }
    return (cBase *)result;
}

void gcDoEntityAttach::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntityAttach) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C, 1,
                            0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoEntityAttach = cType::InitializeType(
                0, 0, 0x156, type_action, gcDoEntityAttach::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntityAttach;
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
    *this = *(const gcDoEntityAttach *)copy;
}

const cType *gcDoEntityAttach::GetType(void) const {
    if (!type_gcDoEntityAttach) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntityAttach_base_name,
                        gcDoEntityAttach_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntityAttach = cType::InitializeType(
            0, 0, 0x156, type_action, gcDoEntityAttach::New, 0, 0, 0);
    }
    return type_gcDoEntityAttach;
}

void gcDoEntityAttach::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    gcAction::Write(file);
    wb.Write(*(const int *)((const char *)this + 0x0C));

    {
        char *typeInfo = *(char **)((const char *)this + 0x14);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x10;
        rec->fn(base + rec->offset, wb._file);
    }

    {
        char *typeInfo = *(char **)((const char *)this + 0x40);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x3C;
        rec->fn(base + rec->offset, wb._file);
    }

    ((const cName *)((const char *)this + 0x68))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x80))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x84))->Write(wb);
    wb.End();
}
