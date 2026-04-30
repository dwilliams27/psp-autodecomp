#include "cBase.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cFile;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

class gcDoEntityAttack : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    gcDoEntityAttack &operator=(const gcDoEntityAttack &);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

extern const char gcDoEntityAttack_base_name[];
extern const char gcDoEntityAttack_base_desc[];

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char D_00000478[];
extern char D_00002268[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityAttack;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct cTypeNode {
    char pad[0x1C];
    const cType *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

cBase *gcDoEntityAttack::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntityAttack *result = 0;
    gcDoEntityAttack *obj =
        (gcDoEntityAttack *)entry->fn(base, 0x70, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);

        int *obj_i = (int *)obj;
        obj_i[1] = (int)D_00002268;

        void *desired0 = (char *)obj + 0x0C;
        gcDesiredObject_gcDesiredObject(desired0, (cBase *)obj);

        obj_i[4] = (int)D_00000338;

        gcDesiredEntityHelper_ctor((char *)obj + 0x18, 1, 0, 0);

        obj_i[4] = 0x388A48;
        obj_i[9] = 0x37E6A8;
        obj_i[8] = (int)desired0;
        obj_i[9] = 0x388568;
        ((char *)obj)[0x28] = 1;
        ((char *)obj)[0x29] = 0;
        obj_i[11] = 0;
        int desired0Encoded = (int)desired0 | 1;
        __asm__ volatile("" : "+r"(desired0Encoded));
        obj_i[12] = 0;
        obj_i[13] = desired0Encoded;
        obj_i[16] = 0x37E6A8;
        obj_i[14] = 0;
        obj_i[15] = (int)obj;
        obj_i[16] = 0x388568;
        ((char *)obj)[0x44] = 1;
        ((char *)obj)[0x45] = 0;
        obj_i[18] = 0;
        int encoded = (int)obj | 1;
        obj_i[19] = 0;
        obj_i[20] = encoded;

        gcDesiredObject_gcDesiredObject((char *)obj + 0x54, (cBase *)obj);

        obj_i[22] = (int)D_00000478;
        obj_i[24] = 7;
        obj_i[25] = 0;
        obj_i[22] = 0x389270;
        obj_i[26] = encoded;
        obj_i[27] = 0;

        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoEntityAttack::GetType(void) const {
    if (!type_gcDoEntityAttack) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoEntityAttack_base_name, gcDoEntityAttack_base_desc, 1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntityAttack = cType::InitializeType(0, 0, 0x148, type_action, gcDoEntityAttack::New, 0, 0, 0x80);
    }
    return type_gcDoEntityAttack;
}

void gcDoEntityAttack::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntityAttack) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoEntityAttack_base_name,
                            gcDoEntityAttack_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoEntityAttack = cType::InitializeType(
                0, 0, 0x148, type_action,
                gcDoEntityAttack::New, 0, 0, 0x80);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntityAttack;
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
            type = ((cTypeNode *)type)->parent;
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
    *this = *(const gcDoEntityAttack *)copy;
}

void gcDoEntityAttack::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcAction::Write(file);

    char *typeInfo0 = *(char **)((const char *)this + 0x10);
    WriteRec *rec0 = (WriteRec *)(typeInfo0 + 0x28);
    short off0 = rec0->offset;
    void *base0 = (char *)this + 0x0C;
    rec0->fn((char *)base0 + off0, wb._file);

    ((const gcDesiredValue *)((const char *)this + 0x68))->Write(wb);

    char *typeInfo1 = *(char **)((const char *)this + 0x40);
    WriteRec *rec1 = (WriteRec *)(typeInfo1 + 0x28);
    short off1 = rec1->offset;
    void *base1 = (char *)this + 0x3C;
    rec1->fn((char *)base1 + off1, wb._file);

    wb.Write(*(int *)((const char *)this + 0x6C));
    wb.Write(*(int *)((const char *)this + 0x38));

    char *typeInfo2 = *(char **)((const char *)this + 0x58);
    WriteRec *rec2 = (WriteRec *)(typeInfo2 + 0x28);
    short off2 = rec2->offset;
    void *base2 = (char *)this + 0x54;
    rec2->fn((char *)base2 + off2, wb._file);

    wb.End();
}
