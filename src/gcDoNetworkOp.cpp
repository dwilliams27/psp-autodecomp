class cBase;
class cFile;
class cMemPool;
class cType;
class gcDoNetworkOp;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

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

void gcAction_gcAction(void *, cBase *);
void gcAction_Write(const gcDoNetworkOp *, cFile &);
extern const char gcDoNetworkOp_base_name[];
extern const char gcDoNetworkOp_base_desc[];
extern char gcDoNetworkOpvirtualtable[];

class gcDoNetworkOp {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoNetworkOp;

cBase *gcDoNetworkOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoNetworkOp *result = 0;
    gcDoNetworkOp *obj = (gcDoNetworkOp *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        *(void **)((char *)obj + 4) = gcDoNetworkOpvirtualtable;
        *(int *)((char *)obj + 0xC) = 0;
        *(int *)((char *)obj + 0x10) = (int)obj | 1;
        *(int *)((char *)obj + 0x14) = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoNetworkOp::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction_Write(this, file);
    wb.Write(((int *)this)[3]);
    ((gcDesiredValue *)((char *)this + 0x10))->Write(wb);

    int val = ((int *)this)[5];
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)val;
    }
    wb.WriteBase(ptr);
    wb.End();
}

const cType *gcDoNetworkOp::GetType(void) const {
    if (!type_gcDoNetworkOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoNetworkOp_base_name,
                                                      gcDoNetworkOp_base_desc,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoNetworkOp = cType::InitializeType(0, 0, 0x1A8, type_action,
                                                   gcDoNetworkOp::New, 0, 0, 0);
    }
    return type_gcDoNetworkOp;
}
