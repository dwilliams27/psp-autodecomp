// gcDoEntitySetSkin.cpp - gcAll_psp.obj

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;

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

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDoEntitySetSkin : public gcAction {
public:
    gcDoEntitySetSkin(cBase *);

    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
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

extern const char gcDoEntitySetSkin_base_name[];
extern const char gcDoEntitySetSkin_base_desc[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntitySetSkin;

cBase *gcDoEntitySetSkin::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntitySetSkin *result = 0;
    gcDoEntitySetSkin *obj = (gcDoEntitySetSkin *)rec->fn(base, 0x8C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntitySetSkin(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoEntitySetSkin::GetType(void) const {
    if (!type_gcDoEntitySetSkin) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntitySetSkin_base_name,
                        gcDoEntitySetSkin_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntitySetSkin = cType::InitializeType(
            0, 0, 0x134, type_action, gcDoEntitySetSkin::New, 0, 0, 0);
    }
    return type_gcDoEntitySetSkin;
}

void gcDoEntitySetSkin::Write(cFile &file) const {
    cWriteBlock wb(file, 7);
    gcAction::Write(file);

    char *typeInfo0 = *(char **)((const char *)this + 0x10);
    WriteRec *rec0 = (WriteRec *)(typeInfo0 + 0x28);
    short off0 = rec0->offset;
    void *base0 = (char *)this + 0x0C;
    rec0->fn((char *)base0 + off0, wb._file);

    ((const gcDesiredValue *)((const char *)this + 0x3C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x80))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x84))->Write(wb);

    char *typeInfo1 = *(char **)((const char *)this + 0x44);
    WriteRec *rec1 = (WriteRec *)(typeInfo1 + 0x28);
    short off1 = rec1->offset;
    void *base1 = (char *)this + 0x40;
    rec1->fn((char *)base1 + off1, wb._file);

    char *typeInfo2 = *(char **)((const char *)this + 0x70);
    WriteRec *rec2 = (WriteRec *)(typeInfo2 + 0x28);
    short off2 = rec2->offset;
    void *base2 = (char *)this + 0x6C;
    rec2->fn((char *)base2 + off2, wb._file);

    wb.Write(*(const int *)((const char *)this + 0x38));
    ((const gcDesiredValue *)((const char *)this + 0x88))->Write(wb);
    wb.End();
}
