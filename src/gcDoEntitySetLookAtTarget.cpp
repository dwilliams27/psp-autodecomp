// gcDoEntitySetLookAtTarget.cpp - gcAll_psp.obj

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

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class cName {
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

class gcDoEntitySetLookAtTarget : public gcAction {
public:
    gcDoEntitySetLookAtTarget(cBase *);

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

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char gcDoEntitySetLookAtTargetvirtualtable[];

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoEntitySetLookAtTarget asm("D_0009F658");

gcDoEntitySetLookAtTarget::gcDoEntitySetLookAtTarget(cBase *parent) {
    gcAction_gcAction(this, parent);
    mVTable = gcDoEntitySetLookAtTargetvirtualtable;

    char *desired0 = (char *)this + 0x0C;
    gcDesiredObject_gcDesiredObject(desired0, (cBase *)this);

    void *desiredType = D_00000338;
    *(void **)((char *)this + 0x10) = desiredType;

    void *helper0 = (char *)this + 0x18;
    int one = 1;
    gcDesiredEntityHelper_ctor(helper0, 1, 0, 0);

    void *helperDesc = (void *)0x388A48;
    *(void **)((char *)this + 0x10) = helperDesc;
    *(void **)((char *)this + 0x20) = desired0;

    void *desiredVTable = (void *)0x388568;
    *(void **)((char *)this + 0x24) = desiredVTable;
    *(unsigned char *)((char *)this + 0x28) = one;
    *(unsigned char *)((char *)this + 0x29) = 0;
    *(int *)((char *)this + 0x2C) = 0;
    *(int *)((char *)this + 0x30) = 0;
    *(int *)((char *)this + 0x34) = (int)desired0 | 1;
    *(void **)((char *)this + 0x38) = this;
    *(void **)((char *)this + 0x3C) = desiredVTable;
    *(unsigned char *)((char *)this + 0x40) = one;
    *(unsigned char *)((char *)this + 0x41) = 0;
    *(int *)((char *)this + 0x44) = 0;
    *(int *)((char *)this + 0x48) = 0;
    *(int *)((char *)this + 0x4C) = (int)this | 1;
    *(int *)((char *)this + 0x54) = 0;

    char *desired1 = (char *)this + 0x58;
    gcDesiredObject_gcDesiredObject(desired1, (cBase *)this);
    *(void **)((char *)this + 0x5C) = desiredType;
    void *helper1 = (char *)this + 0x64;
    gcDesiredEntityHelper_ctor(helper1, 1, 0, 0);
    *(void **)((char *)this + 0x5C) = helperDesc;
    *(void **)((char *)this + 0x6C) = desired1;
    *(void **)((char *)this + 0x70) = desiredVTable;
    *(unsigned char *)((char *)this + 0x74) = one;
    *(unsigned char *)((char *)this + 0x75) = 0;
    *(int *)((char *)this + 0x78) = 0;
    *(int *)((char *)this + 0x7C) = 0;
    *(int *)((char *)this + 0x80) = (int)desired1 | 1;
    *(short *)((char *)this + 0x98) = 0;
    *(short *)((char *)this + 0x9A) = 0;
    *(unsigned char *)((char *)this + 0x84) = 0;
}

const cType *gcDoEntitySetLookAtTarget::GetType(void) const {
    if (!type_gcDoEntitySetLookAtTarget) {
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
        type_gcDoEntitySetLookAtTarget = cType::InitializeType(
            0, 0, 0x11B, type_action, gcDoEntitySetLookAtTarget::New, 0, 0, 0);
    }
    return type_gcDoEntitySetLookAtTarget;
}

cBase *gcDoEntitySetLookAtTarget::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntitySetLookAtTarget *result = 0;
    gcDoEntitySetLookAtTarget *obj =
        (gcDoEntitySetLookAtTarget *)rec->fn(base, 0x9C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntitySetLookAtTarget(parent);
        result = obj;
    }
    return (cBase *)result;
}

void gcDoEntitySetLookAtTarget::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
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

    wb.Write(*(int *)((const char *)this + 0x50));
    ((const cHandle *)((const char *)this + 0x54))->Write(wb);

    char *typeInfo2 = *(char **)((const char *)this + 0x5C);
    WriteRec *rec2 = (WriteRec *)(typeInfo2 + 0x28);
    short off2 = rec2->offset;
    void *base2 = (char *)this + 0x58;
    rec2->fn((char *)base2 + off2, wb._file);

    ((const cName *)((const char *)this + 0x84))->Write(wb);
    wb.End();
}
