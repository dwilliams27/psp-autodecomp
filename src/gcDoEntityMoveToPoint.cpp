class cBase;
class cFile;
class cMemPool;

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
    void Write(unsigned int);
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

struct cTypeNode {
    char pad[0x1C];
    const cType *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

class gcDoEntityMoveToPoint : public gcAction {
public:
    gcDoEntityMoveToPoint(cBase *);

    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    gcDoEntityMoveToPoint &operator=(const gcDoEntityMoveToPoint &);
};

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, cBase *);
extern "C" void gcDesiredCamera_gcDesiredCamera(void *, cBase *);
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");

extern char D_00000338[];
extern char gcDoEntityMoveToPointvirtualtable[];

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoEntityMoveToPoint asm("D_0009F620");

void gcDoEntityMoveToPoint::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntityMoveToPoint) {
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
            type_gcDoEntityMoveToPoint = cType::InitializeType(
                0, 0, 0xDC, type_action,
                gcDoEntityMoveToPoint::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntityMoveToPoint;
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
    *this = *(const gcDoEntityMoveToPoint *)copy;
}

gcDoEntityMoveToPoint::gcDoEntityMoveToPoint(cBase *parent) {
    gcAction_gcAction(this, parent);
    mVTable = gcDoEntityMoveToPointvirtualtable;

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

    gcDesiredObject_gcDesiredObject((char *)this + 0x38, (cBase *)this);
    *(int *)((char *)this + 0x44) = 7;
    *(int *)((char *)this + 0x48) = 0;
    *(void **)((char *)this + 0x3C) = (void *)0x389760;

    char *desired1 = (char *)this + 0x4C;
    gcDesiredObject_gcDesiredObject(desired1, (cBase *)this);
    *(void **)((char *)this + 0x50) = entityType;

    gcDesiredEntityHelper_ctor((char *)this + 0x58, 1, 0, 0);

    *(void **)((char *)this + 0x50) = helperDesc;
    *(void **)((char *)this + 0x60) = desired1;
    *(void **)((char *)this + 0x64) = desiredVTable;
    *(unsigned char *)((char *)this + 0x68) = one;
    *(unsigned char *)((char *)this + 0x69) = 0;
    *(int *)((char *)this + 0x6C) = 0;
    *(int *)((char *)this + 0x70) = 0;
    *(int *)((char *)this + 0x74) = (int)desired1 | 1;

    gcDesiredCamera_gcDesiredCamera((char *)this + 0x78, (cBase *)this);

    int encoded = (int)this | 1;
    *(int *)((char *)this + 0xB0) = encoded;
    *(int *)((char *)this + 0xB4) = encoded;
    *(int *)((char *)this + 0xB8) = encoded;
    *(int *)((char *)this + 0xBC) = encoded;
    *(int *)((char *)this + 0xC0) = encoded;
    *(int *)((char *)this + 0xC4) = encoded;
    *(int *)((char *)this + 0xC8) = encoded;
    *(int *)((char *)this + 0xCC) = encoded;
    *(int *)((char *)this + 0xD0) = encoded;
    *(int *)((char *)this + 0xD4) = encoded;
    *(int *)((char *)this + 0xD8) = 0;
    *(int *)((char *)this + 0xDC) = one;
    *(int *)((char *)this + 0xE0) = 2;
    *(int *)((char *)this + 0xE4) = 0;
    *(short *)((char *)this + 0xFC) = 0;
    *(short *)((char *)this + 0xFE) = 0;
    *(unsigned char *)((char *)this + 0xE8) = 0;
    *(int *)((char *)this + 0x100) = encoded;
    *(unsigned char *)((char *)this + 0x104) = 0;
}

const cType *gcDoEntityMoveToPoint::GetType(void) const {
    if (!type_gcDoEntityMoveToPoint) {
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
        type_gcDoEntityMoveToPoint = cType::InitializeType(
            0, 0, 0xDC, type_action, gcDoEntityMoveToPoint::New, 0, 0, 0);
    }
    return type_gcDoEntityMoveToPoint;
}

void gcDoEntityMoveToPoint::Write(cFile &file) const {
    cWriteBlock wb(file, 7);
    gcAction::Write(file);

    {
        char *typeInfo = *(char **)((const char *)this + 0x10);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x0C;
        rec->fn(base + rec->offset, wb._file);
    }

    {
        char *typeInfo = *(char **)((const char *)this + 0x3C);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x38;
        rec->fn(base + rec->offset, wb._file);
    }

    {
        char *typeInfo = *(char **)((const char *)this + 0x50);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x4C;
        rec->fn(base + rec->offset, wb._file);
    }

    ((const gcDesiredValue *)((const char *)this + 0xB0))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xB4))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xB8))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xC0))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xC4))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xC8))->Write(wb);
    wb.Write(*(const unsigned int *)((const char *)this + 0xD8));
    wb.Write(*(const unsigned int *)((const char *)this + 0xE4));
    ((const cName *)((const char *)this + 0xE8))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x100))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0x104));
    wb.Write(*(const unsigned int *)((const char *)this + 0xDC));
    wb.Write(*(const unsigned int *)((const char *)this + 0xE0));
    ((const gcDesiredValue *)((const char *)this + 0xBC))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xCC))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xD0))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xD4))->Write(wb);

    {
        char *typeInfo = *(char **)((const char *)this + 0x7C);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x78;
        rec->fn(base + rec->offset, wb._file);
    }

    wb.End();
}
