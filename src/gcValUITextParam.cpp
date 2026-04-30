// Days of Thunder decompilation: gcValUITextParam methods
// Obj: gcAll_psp.obj.
//
// Class layout (alloc size 0x18, 24 bytes):
//   [0x00] mParent (cBase *)             ─┐ gcLValue base
//   [0x04] mVtable                        ┘
//   [0x08] mHelper (gcDesiredUIWidgetHelper, 12 bytes)
//   [0x14] mValue (gcDesiredValue, 4 bytes — tagged self-pointer (this | 1))
//
// Functions:
//   0x00367454  gcValUITextParam::New(cMemPool *, cBase *) static    160B
//   0x00367648  gcValUITextParam::Write(cFile &) const               100B

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

struct gcDesiredUIWidgetHelper {
    int _a;
    int _b;
    int _c;
    void Write(cWriteBlock &) const;
};

struct gcDesiredValue {
    int _a;
    void Write(cWriteBlock &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

void gcDesiredUIWidgetHelper_ctor(void *, int);

extern char gcLValuevirtualtable[];
extern char gcValUITextParamvirtualtable[];

class gcLValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
};

class gcValUITextParam : public gcLValue {
public:
    gcDesiredUIWidgetHelper mHelper;
    gcDesiredValue mValue;

    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_variable;
static cType *type_gcValUITextParam;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcValUITextParam::New(cMemPool *, cBase *) static @ 0x00367454 ──
cBase *gcValUITextParam::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcValUITextParam *result = 0;
    gcValUITextParam *obj = (gcValUITextParam *)e->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcLValuevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValUITextParamvirtualtable;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 8, 1);
        *(int *)((char *)obj + 0x14) = (int)((unsigned int)obj | 1);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValUITextParam::Write(cFile &) const @ 0x00367648 ──
void gcValUITextParam::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcLValue::Write(file);
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 8))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x14))->Write(wb);
    wb.End();
}

const cType *gcValUITextParam::GetType(void) const {
    if (!type_gcValUITextParam) {
        if (!type_variable) {
            if (!type_value) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(0, 0, 0x6A,
                                                            type_base, 0, 0,
                                                            0, 0);
                }
                type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                                   0, 0, 0, 0x80);
            }
            type_variable = cType::InitializeType(0, 0, 0x6D, type_value,
                                                  0, 0, 0, 0);
        }
        type_gcValUITextParam = cType::InitializeType(0, 0, 0x97,
                                                      type_variable,
                                                      gcValUITextParam::New,
                                                      0, 0, 0);
    }
    return type_gcValUITextParam;
}
