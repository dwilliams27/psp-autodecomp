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

struct cTypeNode {
    char pad[0x1C];
    const cType *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

class gcDoEntityPartialBodyAttack : public gcAction {
public:
    gcDoEntityPartialBodyAttack(cBase *);
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    gcDoEntityPartialBodyAttack &
    operator=(const gcDoEntityPartialBodyAttack &);
};

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char D_00002978[];

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoEntityPartialBodyAttack asm("D_0009F624");

void gcDoEntityPartialBodyAttack::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntityPartialBodyAttack) {
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
            type_gcDoEntityPartialBodyAttack = cType::InitializeType(
                0, 0, 0x149, type_action,
                gcDoEntityPartialBodyAttack::New, 0, 0, 0x80);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntityPartialBodyAttack;
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
    *this = *(const gcDoEntityPartialBodyAttack *)copy;
}

gcDoEntityPartialBodyAttack::gcDoEntityPartialBodyAttack(cBase *parent) {
    gcAction_gcAction(this, parent);

    int *obj_i = (int *)this;
    obj_i[1] = (int)D_00002978;

    void *desired0 = (char *)this + 0x0C;
    gcDesiredObject_gcDesiredObject(desired0, (cBase *)this);

    obj_i[4] = (int)D_00000338;

    gcDesiredEntityHelper_ctor((char *)this + 0x18, 1, 0, 0);

    obj_i[4] = 0x388A48;
    obj_i[8] = (int)desired0;
    int *helperVTable = (int *)0x388568;
    obj_i[9] = (int)helperVTable;
    ((char *)this)[0x28] = 1;
    ((char *)this)[0x29] = 0;
    obj_i[11] = 0;
    int desired0Encoded = (int)desired0 | 1;
    obj_i[12] = 0;
    obj_i[13] = desired0Encoded;
    obj_i[14] = 0;
    obj_i[15] = (int)this;
    obj_i[16] = (int)helperVTable;
    ((char *)this)[0x44] = 1;
    ((char *)this)[0x45] = 0;
    obj_i[18] = 0;
    int encoded = (int)this | 1;
    obj_i[19] = 0;
    obj_i[20] = encoded;

    void *desired1 = (char *)this + 0x54;
    gcDesiredObject_gcDesiredObject(desired1, (cBase *)this);

    obj_i[24] = 7;
    obj_i[25] = 0;
    obj_i[22] = 0x389270;
    obj_i[26] = encoded;
    obj_i[27] = 0;
    obj_i[28] = (int)this;
    obj_i[29] = (int)helperVTable;
    ((char *)this)[0x78] = 1;
    ((char *)this)[0x79] = 0;
    obj_i[31] = 0;
    obj_i[32] = 0;
    obj_i[33] = encoded;
}

const cType *gcDoEntityPartialBodyAttack::GetType(void) const {
    if (!type_gcDoEntityPartialBodyAttack) {
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
        type_gcDoEntityPartialBodyAttack = cType::InitializeType(
            0, 0, 0x149, type_action, gcDoEntityPartialBodyAttack::New, 0, 0, 0x80);
    }
    return type_gcDoEntityPartialBodyAttack;
}

void gcDoEntityPartialBodyAttack::Write(cFile &file) const {
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

    char *typeInfo2 = *(char **)((const char *)this + 0x74);
    WriteRec *rec2 = (WriteRec *)(typeInfo2 + 0x28);
    short off2 = rec2->offset;
    void *base2 = (char *)this + 0x70;
    rec2->fn((char *)base2 + off2, wb._file);

    wb.Write(*(int *)((const char *)this + 0x38));

    char *typeInfo3 = *(char **)((const char *)this + 0x58);
    WriteRec *rec3 = (WriteRec *)(typeInfo3 + 0x28);
    short off3 = rec3->offset;
    void *base3 = (char *)this + 0x54;
    rec3->fn((char *)base3 + off3, wb._file);

    wb.End();
}
