// gcDoEntitySetGeomCurveTarget.cpp - gcAll_psp.obj

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

class gcDoEntitySetGeomCurveTarget : public gcAction {
public:
    gcDoEntitySetGeomCurveTarget(cBase *);

    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void GetText(char *) const;
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

struct GetTextRec {
    short offset;
    short _pad;
    void (*fn)(void *, char *);
};

extern "C" void cStrAppend(char *, const char *, ...);
extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern const char gcDoEntitySetGeomCurveTarget_base_name[];
extern const char gcDoEntitySetGeomCurveTarget_base_desc[];
extern char D_00000338[];
extern char gcDoEntitySetGeomCurveTargetvirtualtable[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntitySetGeomCurveTarget;

gcDoEntitySetGeomCurveTarget::gcDoEntitySetGeomCurveTarget(cBase *parent) {
    gcAction_gcAction(this, parent);
    mVTable = gcDoEntitySetGeomCurveTargetvirtualtable;

    char *desired0 = (char *)this + 0x0C;
    gcDesiredObject_gcDesiredObject(desired0, (cBase *)this);

    void *entityType = D_00000338;
    *(void **)((char *)this + 0x10) = entityType;

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
    *(int *)((char *)this + 0x38) = 0;
    *(int *)((char *)this + 0x3C) = 0;

    gcDesiredObject_gcDesiredObject((char *)this + 0x40, (cBase *)this);
    *(int *)((char *)this + 0x4C) = 7;
    *(int *)((char *)this + 0x50) = 0;
    *(void **)((char *)this + 0x44) = (void *)0x389760;

    char *desired1 = (char *)this + 0x54;
    gcDesiredObject_gcDesiredObject(desired1, (cBase *)this);
    *(void **)((char *)this + 0x58) = entityType;

    gcDesiredEntityHelper_ctor((char *)this + 0x60, 1, 0, 0);

    *(void **)((char *)this + 0x58) = helperDesc;
    *(void **)((char *)this + 0x68) = desired1;
    *(void **)((char *)this + 0x6C) = desiredVTable;
    *(unsigned char *)((char *)this + 0x70) = one;
    *(unsigned char *)((char *)this + 0x71) = 0;
    *(int *)((char *)this + 0x74) = 0;
    *(int *)((char *)this + 0x78) = 0;
    *(int *)((char *)this + 0x7C) = (int)desired1 | 1;
    *(short *)((char *)this + 0x94) = 0;
    *(short *)((char *)this + 0x96) = 0;
    *(unsigned char *)((char *)this + 0x80) = 0;
}

cBase *gcDoEntitySetGeomCurveTarget::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntitySetGeomCurveTarget *result = 0;
    gcDoEntitySetGeomCurveTarget *obj =
        (gcDoEntitySetGeomCurveTarget *)rec->fn(base, 0x98, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntitySetGeomCurveTarget(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoEntitySetGeomCurveTarget::GetType(void) const {
    if (!type_gcDoEntitySetGeomCurveTarget) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntitySetGeomCurveTarget_base_name,
                        gcDoEntitySetGeomCurveTarget_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntitySetGeomCurveTarget = cType::InitializeType(
            0, 0, 0x1D8, type_action, gcDoEntitySetGeomCurveTarget::New, 0, 0,
            0);
    }
    return type_gcDoEntitySetGeomCurveTarget;
}

void gcDoEntitySetGeomCurveTarget::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction::Write(file);

    char *typeInfo0 = *(char **)((const char *)this + 0x10);
    WriteRec *rec0 = (WriteRec *)(typeInfo0 + 0x28);
    short off0 = rec0->offset;
    void *base0 = (char *)this + 0x0C;
    rec0->fn((char *)base0 + off0, wb._file);

    wb.Write(*(int *)((const char *)this + 0x38));
    wb.Write(*(int *)((const char *)this + 0x3C));

    char *typeInfo1 = *(char **)((const char *)this + 0x44);
    WriteRec *rec1 = (WriteRec *)(typeInfo1 + 0x28);
    short off1 = rec1->offset;
    void *base1 = (char *)this + 0x40;
    rec1->fn((char *)base1 + off1, wb._file);

    char *typeInfo2 = *(char **)((const char *)this + 0x58);
    WriteRec *rec2 = (WriteRec *)(typeInfo2 + 0x28);
    short off2 = rec2->offset;
    void *base2 = (char *)this + 0x54;
    rec2->fn((char *)base2 + off2, wb._file);

    ((const cName *)((const char *)this + 0x80))->Write(wb);
    wb.End();
}

void gcDoEntitySetGeomCurveTarget::GetText(char *buf) const {
    char text[256];

    char *typeInfo0 = *(char **)((const char *)this + 0x10);
    GetTextRec *rec0 = (GetTextRec *)(typeInfo0 + 0x78);
    char *base0 = (char *)this + 0x0C;
    rec0->fn(base0 + rec0->offset, buf);

    int which = *(int *)((const char *)this + 0x3C);
    text[0] = '\0';

    if (which <= 0) {
        if (which >= 0) {
            char *typeInfo2 = *(char **)((const char *)this + 0x58);
            GetTextRec *rec2 = (GetTextRec *)(typeInfo2 + 0x78);
            char *base2 = (char *)this + 0x54;
            rec2->fn(base2 + rec2->offset, text);
            cStrAppend(buf, (const char *)0x36E9E4,
                       *(int *)((const char *)this + 0x38), text,
                       (char *)this + 0x80);
        }
    } else if (which < 2) {
        char *typeInfo1 = *(char **)((const char *)this + 0x44);
        GetTextRec *rec1 = (GetTextRec *)(typeInfo1 + 0x78);
        char *base1 = (char *)this + 0x40;
        rec1->fn(base1 + rec1->offset, text);
        cStrAppend(buf, (const char *)0x36EA10,
                   *(int *)((const char *)this + 0x38), text);
    }
}
