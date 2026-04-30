// gcDoEntitySetAttractor.cpp - gcAll_psp.obj

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

class gcExpression {
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

class gcDoEntitySetAttractor : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

struct WriteRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

extern char D_00002EB8[];
extern char D_00000338[];

extern "C" {
void gcAction_ctor_cBase(void *, cBase *);
void gcDesiredObject_ctor_cBase(void *, cBase *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);
}

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoEntitySetAttractor asm("D_0009F63C");

cBase *gcDoEntitySetAttractor::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntitySetAttractor *result = 0;
    gcDoEntitySetAttractor *obj =
        (gcDoEntitySetAttractor *)entry->fn(base, 0x84, 4, 0, 0);
    if (obj != 0) {
        gcAction_ctor_cBase(obj, parent);

        int *obj_i = (int *)obj;
        obj_i[1] = (int)D_00002EB8;

        void *desired = (char *)obj + 0x10;
        obj_i[3] = 0;
        gcDesiredObject_ctor_cBase(desired, (cBase *)obj);

        obj_i[5] = (int)D_00000338;

        gcDesiredEntityHelper_ctor((char *)obj + 0x1C, 0, 0, 0);

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
        ((char *)obj)[0x68] = 0;
        obj_i[32] = (int)obj | 1;

        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoEntitySetAttractor::GetType(void) const {
    if (!type_gcDoEntitySetAttractor) {
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
        type_gcDoEntitySetAttractor = cType::InitializeType(
            0, 0, 0x1FB, type_action, gcDoEntitySetAttractor::New, 0, 0, 0);
    }
    return type_gcDoEntitySetAttractor;
}

void gcDoEntitySetAttractor::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);
    wb.Write(*(const int *)((const char *)this + 0xC));

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
    wb.End();
}
