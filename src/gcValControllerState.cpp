class cBase;
class cFile;
class cMemPool;

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcValControllerState : public gcValue {
public:
    gcValControllerState(cBase *);

    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

extern const char gcValControllerState_base_name[];
extern const char gcValControllerState_base_desc[];

cBase *gcValControllerState::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcValControllerState *result = 0;
    gcValControllerState *obj =
        (gcValControllerState *)rec->fn(base, 0x44, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValControllerState(parent);
        result = obj;
    }
    return (cBase *)result;
}

void gcValControllerState::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcValue::Write(file);
    wb.Write(*(const int *)((const char *)this + 8));
    wb.Write(*(const int *)((const char *)this + 0xC));
    wb.Write(*(const int *)((const char *)this + 0x10));

    cTypeMethod *writeMethod =
        (cTypeMethod *)(*(char **)((const char *)this + 0x18) + 0x28);
    char *desiredBase = (char *)this + 0x14;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)writeMethod->fn)(desiredBase + writeMethod->offset,
                                (cFile *)wb._data[0]);

    ((const gcDesiredValue *)((const char *)this + 0x40))->Write(wb);
    wb.End();
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValControllerState;

const cType *gcValControllerState::GetType(void) const {
    if (!type_gcValControllerState) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcValControllerState_base_name,
                                                       gcValControllerState_base_desc,
                                                       1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValControllerState = cType::InitializeType(
            0, 0, 0x89, type_value, gcValControllerState::New, 0, 0, 0);
    }
    return type_gcValControllerState;
}
