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
    const cBase *src = other;
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
        short off = *(short *)((char *)vt + 8);
        const cType *(*getType)(void *) =
            *(const cType *(**)(void *))((char *)vt + 12);
        const cType *type = getType((char *)other + off);
        int ok;

        if (myType == 0) {
            goto fail;
        }
        if (type != 0) {
        loop:
            if (type != myType) {
                type = *(const cType **)((char *)type + 0x1C);
                if (type == 0) {
                    goto fail;
                }
                if (type != myType) {
                    goto loop;
                }
            }
            ok = 1;
        } else {
fail:
            ok = 0;
        }
        if (ok != 0) {
            copy = other;
        }
    }
    *this = *(const gcDoEntityPlayPartialBodyAnimation *)copy;
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
