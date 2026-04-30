class cBase;
class cFile;
class cMemPool;

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _p0[0x28];
    cTypeMethod write_m;
    cTypeMethod read_m;
    char _p1[0x40];
    cTypeMethod text_m;

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

class gcLValue {
public:
    void Write(cFile &) const;
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

typedef void (*DesiredWriteFn)(void *, cFile *);

struct DesiredWriteSlot {
    short mOffset;
    short _pad;
    DesiredWriteFn mFn;
};

struct DesiredTypeInfoWrite {
    char _pad[0x28];
    DesiredWriteSlot mSlot;
};

class gcValRigidBodyControllerVariable {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

class gcValTableEntry {
public:
    void Write(cFile &) const;
};

struct SixInts {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
};

template <class T>
T dcast(const cBase *);

void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

__asm__(".globl gcValRigidBodyControllerVariablevirtualtable_abs\n"
        "gcValRigidBodyControllerVariablevirtualtable_abs = 0x9c40\n");
__asm__(".globl cBaseclassdesc_abs\n"
        "cBaseclassdesc_abs = 0x37e6a8\n");
__asm__(".globl D_00000338_abs\n"
        "D_00000338_abs = 0x338\n");

extern char cBaseclassdesc[];
extern char D_00000338[];
extern char cBaseclassdesc_abs[];
extern char D_00000338_abs[];
extern char gcValRigidBodyControllerVariablevirtualtable_abs[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_variable asm("D_0009F3EC");
static cType *type_gcValRigidBodyControllerVariable asm("D_0009F8D0");

cBase *gcValRigidBodyControllerVariable::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValRigidBodyControllerVariable *result = 0;
    gcValRigidBodyControllerVariable *obj =
        (gcValRigidBodyControllerVariable *)entry->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = cBaseclassdesc_abs;
        ((int *)obj)[0] = (int)parent;
        ((int *)obj)[1] = (int)gcValRigidBodyControllerVariablevirtualtable_abs;
        char *sub = (char *)obj + 8;
        gcDesiredObject_ctor(sub, obj);
        ((int *)obj)[3] = (int)D_00000338_abs;
        gcDesiredEntityHelper_ctor((char *)obj + 0x14, 1, 0, 0);
        ((void **)obj)[3] = (void *)0x388A48;
        ((void **)obj)[8] = cBaseclassdesc_abs;
        ((void **)obj)[7] = sub;
        ((void **)obj)[8] = (void *)0x388568;
        ((char *)obj)[0x24] = 1;
        ((char *)obj)[0x25] = 0;
        ((int *)obj)[10] = 0;
        ((int *)obj)[11] = 0;
        ((int *)obj)[12] = (int)sub | 1;
        ((int *)obj)[13] = 3;
        *(short *)((char *)obj + 0x4C) = 0;
        *(short *)((char *)obj + 0x4E) = 0;
        *(char *)((char *)obj + 0x38) = 0;
        result = obj;
    }
    return (cBase *)result;
}

void gcValRigidBodyControllerVariable::AssignCopy(const cBase *base) {
    gcValRigidBodyControllerVariable *other = dcast<gcValRigidBodyControllerVariable *>(base);
    gcDesiredEntity *srcptr = (gcDesiredEntity *)((char *)other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*srcptr);
    *(int *)((char *)this + 0x34) = *(const int *)((const char *)other + 0x34);
    *(SixInts *)((char *)this + 0x38) = *(const SixInts *)((const char *)other + 0x38);
}

void gcValRigidBodyControllerVariable::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcLValue *)this)->Write(file);

    DesiredTypeInfoWrite *type = *(DesiredTypeInfoWrite **)((const char *)this + 0x0C);
    DesiredWriteSlot *slot = &type->mSlot;
    const char *desired = (const char *)this + 0x08;
    slot->mFn((void *)(desired + slot->mOffset), wb._file);

    wb.Write(*(const int *)((const char *)this + 0x34));
    ((const cName *)((const char *)this + 0x38))->Write(wb);
    wb.End();
}

const cType *gcValRigidBodyControllerVariable::GetType(void) const {
    if (!type_gcValRigidBodyControllerVariable) {
        if (!type_variable) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_value = cType::InitializeType(
                    0, 0, 0x6C, type_expression, 0, 0, 0, 0x80);
            }
            type_variable = cType::InitializeType(
                0, 0, 0x6D, type_value, 0, 0, 0, 0);
        }
        type_gcValRigidBodyControllerVariable = cType::InitializeType(
            0, 0, 0x1A1, type_variable, gcValRigidBodyControllerVariable::New,
            0, 0, 0);
    }
    return type_gcValRigidBodyControllerVariable;
}

void gcValTableEntry::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const gcLValue *)this)->Write(file);

    DesiredTypeInfoWrite *type = *(DesiredTypeInfoWrite **)((const char *)this + 0x0C);
    DesiredWriteSlot *slot = &type->mSlot;
    const char *desired = (const char *)this + 0x08;
    slot->mFn((void *)(desired + slot->mOffset), wb._file);

    ((const gcDesiredValue *)((const char *)this + 0x1C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x20))->Write(wb);
    wb.End();
}
