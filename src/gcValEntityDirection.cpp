// gcValEntityDirection -- entity direction value helpers.

class cBase;
class cFile;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cWriteBlock {
public:
    cFile *file;
    unsigned int pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void End(void);
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char pad[0x28];
    cTypeMethod write_m;

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

class gcValEntityDirection : public gcValue {
public:
    gcValEntityDirection(cBase *);

    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

// -- gcValEntityDirection::New(cMemPool *, cBase *) static @ 0x00328904 --
cBase *gcValEntityDirection::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcValEntityDirection *result = 0;
    gcValEntityDirection *obj =
        (gcValEntityDirection *)rec->fn(base, 0x108, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValEntityDirection(parent);
        result = obj;
    }
    return (cBase *)result;
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValEntityDirection;

const cType *gcValEntityDirection::GetType(void) const {
    if (!type_gcValEntityDirection) {
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
        type_gcValEntityDirection =
            cType::InitializeType(0, 0, 0xF0, type_value,
                                  gcValEntityDirection::New, 0, 0, 0);
    }
    return type_gcValEntityDirection;
}

void gcValEntityDirection::Write(cFile &file) const {
    cWriteBlock wb(file, 0xB);
    gcValue::Write(file);
    wb.Write(*(const int *)((const char *)this + 8));

    typedef void (*WriteFn)(void *, cFile *);

    const cTypeMethod *leftWrite =
        &((const cType *)*(void *const *)((const char *)this + 0x10))->write_m;
    char *leftBase = (char *)this + 0xC;
    ((WriteFn)leftWrite->fn)(leftBase + leftWrite->offset, wb.file);

    const cTypeMethod *rightWrite =
        &((const cType *)*(void *const *)((const char *)this + 0x3C))->write_m;
    char *rightBase = (char *)this + 0x38;
    ((WriteFn)rightWrite->fn)(rightBase + rightWrite->offset, wb.file);

    ((const cName *)((const char *)this + 0xB0))->Write(wb);
    ((const cName *)((const char *)this + 0xCC))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xE4))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 0xEC));
    ((const gcDesiredValue *)((const char *)this + 0xF4))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xF8))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 0xE8));

    const cTypeMethod *cameraTargetWrite =
        &((const cType *)*(void *const *)((const char *)this + 0x68))->write_m;
    char *cameraTargetBase = (char *)this + 0x64;
    ((WriteFn)cameraTargetWrite->fn)(cameraTargetBase + cameraTargetWrite->offset,
                                     wb.file);

    wb.Write(*(const bool *)((const char *)this + 0x104));
    ((const gcDesiredValue *)((const char *)this + 0xFC))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0x105));
    wb.Write(*(const bool *)((const char *)this + 0x106));
    wb.Write(*(const int *)((const char *)this + 0xF0));
    ((const gcDesiredValue *)((const char *)this + 0x100))->Write(wb);

    const cTypeMethod *cameraWrite =
        &((const cType *)*(void *const *)((const char *)this + 0x7C))->write_m;
    char *cameraBase = (char *)this + 0x78;
    ((WriteFn)cameraWrite->fn)(cameraBase + cameraWrite->offset, wb.file);

    ((const gcDesiredValue *)((const char *)this + 0xC8))->Write(wb);
    wb.End();
}
