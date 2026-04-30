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
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(unsigned int);
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

class cName {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcValEntityDistance : public gcValue {
public:
    gcValEntityDistance(cBase *);
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    const cType *GetType(void) const;
};

cBase *gcValEntityDistance::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcValEntityDistance *result = 0;
    gcValEntityDistance *obj =
        (gcValEntityDistance *)rec->fn(base, 0xB8, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValEntityDistance(parent);
        result = obj;
    }
    return (cBase *)result;
}

// 0x0032c31c
static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValEntityDistance;

const cType *gcValEntityDistance::GetType(void) const {
    if (!type_gcValEntityDistance) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValEntityDistance = cType::InitializeType(
            0, 0, 0xE5, type_value, gcValEntityDistance::New, 0, 0, 0);
    }
    return type_gcValEntityDistance;
}

// 0x0032c434
void gcValEntityDistance::Write(cFile &file) const {
    cWriteBlock wb(file, 7);
    gcValue::Write(file);

    const cTypeMethod *write0 =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 0x28);
    char *base0 = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)write0->fn)(base0 + write0->offset, wb.file);

    const cTypeMethod *write1 =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 0x34))->mType + 0x28);
    char *base1 = (char *)this + 0x34;
    ((WriteFn)write1->fn)(base1 + write1->offset, wb.file);

    const cTypeMethod *write2 =
        (const cTypeMethod *)((const char *)((const gcDesiredObject *)((const char *)this + 0x60))->mType + 0x28);
    char *base2 = (char *)this + 0x60;
    ((WriteFn)write2->fn)(base2 + write2->offset, wb.file);

    wb.Write(*(const unsigned int *)((const char *)this + 0xA4));
    ((const cName *)((const char *)this + 0x74))->Write(wb);
    ((const cName *)((const char *)this + 0x8C))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0xAC));
    wb.Write(*(const unsigned int *)((const char *)this + 0xA8));
    wb.Write(*(const bool *)((const char *)this + 0xAD));
    ((const gcDesiredValue *)((const char *)this + 0xB0))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xB4))->Write(wb);
    wb.End();
}
