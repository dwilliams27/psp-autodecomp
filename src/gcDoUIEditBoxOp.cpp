// gcDoUIEditBoxOp — gcAll_psp.obj
//   0x00309070  gcDoUIEditBoxOp::New(cMemPool *, cBase *) static
//   0x00309118  gcDoUIEditBoxOp::GetType(void) const
//   0x00309230  gcDoUIEditBoxOp::Write(cFile &) const

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class gcDesiredValue {
public:
    unsigned int mValue;
    void Write(cWriteBlock &) const;
};

class gcDesiredUIWidgetHelper {
public:
    int mField0;
    int mField4;
    unsigned int mField8;
    void Write(cWriteBlock &) const;
};

class gcAction {
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

class gcDoUIEditBoxOp : public gcAction {
public:
    gcDesiredUIWidgetHelper mWidget;
    int mField18;
    unsigned int mField1C;
    gcDesiredValue mField20;

    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

void gcAction_gcAction(gcDoUIEditBoxOp *, cBase *);
void gcDesiredUIWidgetHelper_ctor(gcDesiredUIWidgetHelper *, int);

extern char gcDoUIEditBoxOpvirtualtable[];
extern const char gcDoUIEditBoxOp_base_name[];
extern const char gcDoUIEditBoxOp_base_desc[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

cBase *gcDoUIEditBoxOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUIEditBoxOp *result = 0;
    gcDoUIEditBoxOp *obj = (gcDoUIEditBoxOp *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoUIEditBoxOpvirtualtable;
        gcDesiredUIWidgetHelper_ctor((gcDesiredUIWidgetHelper *)((char *)obj + 0x0C), 1);
        ((int *)obj)[6] = 3;
        int tagged = (int)obj | 1;
        ((int *)obj)[7] = tagged;
        ((int *)obj)[8] = tagged;
        result = obj;
    }
    return (cBase *)result;
}

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoUIEditBoxOp;

const cType *gcDoUIEditBoxOp::GetType(void) const {
    if (!type_gcDoUIEditBoxOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoUIEditBoxOp_base_name,
                                                      gcDoUIEditBoxOp_base_desc,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoUIEditBoxOp = cType::InitializeType(0, 0, 0x174, type_action,
                                                      gcDoUIEditBoxOp::New,
                                                      0, 0, 0);
    }
    return type_gcDoUIEditBoxOp;
}

void gcDoUIEditBoxOp::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction::Write(file);
    mWidget.Write(wb);
    wb.Write(mField18);

    int value = mField1C;
    int flag = 0;
    if (value & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)value;
    }
    wb.WriteBase(ptr);
    mField20.Write(wb);
    wb.End();
}
