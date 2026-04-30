// gcDoEntityPlayPartialBodyAnimation.cpp - gcAll_psp.obj

inline void *operator new(unsigned int, void *p) { return p; }

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
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    void Write(cFile &) const;
};

class gcDoEntityPlayPartialBodyAnimation : public gcAction {
public:
    gcDoEntityPlayPartialBodyAnimation(cBase *);

    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    gcDoEntityPlayPartialBodyAnimation &
    operator=(const gcDoEntityPlayPartialBodyAnimation &);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
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

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char D_00002B38[];
extern const char gcDoEntityPlayPartialBodyAnimation_base_name[];
extern const char gcDoEntityPlayPartialBodyAnimation_base_desc[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityPlayPartialBodyAnimation;

cBase *gcDoEntityPlayPartialBodyAnimation::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntityPlayPartialBodyAnimation *result = 0;
    gcDoEntityPlayPartialBodyAnimation *obj =
        (gcDoEntityPlayPartialBodyAnimation *)rec->fn(base, 0x80, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntityPlayPartialBodyAnimation(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoEntityPlayPartialBodyAnimation::GetType(void) const {
    if (!type_gcDoEntityPlayPartialBodyAnimation) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntityPlayPartialBodyAnimation_base_name,
                        gcDoEntityPlayPartialBodyAnimation_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntityPlayPartialBodyAnimation = cType::InitializeType(
            0, 0, 0x10C, type_action,
            gcDoEntityPlayPartialBodyAnimation::New, 0, 0, 0x80);
    }
    return type_gcDoEntityPlayPartialBodyAnimation;
}

void gcDoEntityPlayPartialBodyAnimation::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntityPlayPartialBodyAnimation) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoEntityPlayPartialBodyAnimation_base_name,
                            gcDoEntityPlayPartialBodyAnimation_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoEntityPlayPartialBodyAnimation = cType::InitializeType(
                0, 0, 0x10C, type_action,
                gcDoEntityPlayPartialBodyAnimation::New, 0, 0, 0x80);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntityPlayPartialBodyAnimation;
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
    *this = *(const gcDoEntityPlayPartialBodyAnimation *)copy;
}

gcDoEntityPlayPartialBodyAnimation::gcDoEntityPlayPartialBodyAnimation(
    cBase *parent) {
    gcAction_gcAction(this, parent);

    int *obj_i = (int *)this;
    obj_i[1] = (int)D_00002B38;

    void *desired0 = (char *)this + 0x0C;
    gcDesiredObject_gcDesiredObject(desired0, (cBase *)this);

    obj_i[4] = (int)D_00000338;

    gcDesiredEntityHelper_ctor((char *)this + 0x18, 1, 0, 0);

    obj_i[4] = 0x388A48;
    obj_i[8] = (int)desired0;
    obj_i[9] = 0x388568;
    ((char *)this)[0x28] = 1;
    ((char *)this)[0x29] = 0;
    obj_i[11] = 0;
    int desired0Encoded = (int)desired0 | 1;
    obj_i[12] = 0;
    obj_i[13] = desired0Encoded;
    obj_i[14] = (int)this;
    obj_i[15] = 0x388568;
    ((char *)this)[0x40] = 1;
    ((char *)this)[0x41] = 0;
    obj_i[17] = 0;
    int encoded = (int)this | 1;
    obj_i[18] = 0;
    obj_i[19] = encoded;
    obj_i[20] = 0;
    obj_i[21] = (int)this;
    obj_i[22] = 0x388568;
    ((char *)this)[0x5C] = 1;
    ((char *)this)[0x5D] = 0;
    obj_i[24] = 0;
    obj_i[25] = 0;
    obj_i[26] = encoded;

    gcDesiredObject_gcDesiredObject((char *)this + 0x6C, (cBase *)this);

    obj_i[30] = 7;
    obj_i[31] = 0;
    obj_i[28] = 0x389130;
}

void gcDoEntityPlayPartialBodyAnimation::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    gcAction::Write(file);

    char *typeInfo0 = *(char **)((const char *)this + 0x10);
    WriteRec *rec0 = (WriteRec *)(typeInfo0 + 0x28);
    short off0 = rec0->offset;
    void *base0 = (char *)this + 0x0C;
    rec0->fn((char *)base0 + off0, wb._file);

    char *typeInfo1 = *(char **)((const char *)this + 0x3C);
    WriteRec *rec1 = (WriteRec *)(typeInfo1 + 0x28);
    short off1 = rec1->offset;
    void *base1 = (char *)this + 0x38;
    rec1->fn((char *)base1 + off1, wb._file);

    char *typeInfo2 = *(char **)((const char *)this + 0x58);
    WriteRec *rec2 = (WriteRec *)(typeInfo2 + 0x28);
    short off2 = rec2->offset;
    void *base2 = (char *)this + 0x54;
    rec2->fn((char *)base2 + off2, wb._file);

    wb.Write(*(int *)((const char *)this + 0x50));

    char *typeInfo3 = *(char **)((const char *)this + 0x70);
    WriteRec *rec3 = (WriteRec *)(typeInfo3 + 0x28);
    short off3 = rec3->offset;
    void *base3 = (char *)this + 0x6C;
    rec3->fn((char *)base3 + off3, wb._file);

    wb.End();
}
