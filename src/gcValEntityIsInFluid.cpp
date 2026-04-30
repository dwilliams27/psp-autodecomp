// gcValEntityIsInFluid — gcValue subclass referencing an entity, with a
// cName at offset 0x34 used as the "fluid" identifier.
//
// Functions matched here:
//   gcValEntityIsInFluid::AssignCopy(const cBase *)  @ 0x00333f38  (gcAll_psp.obj)
//   gcValEntityIsInFluid::Write(cFile &) const       @ 0x003341d4  (gcAll_psp.obj)
//
// Class layout (relevant subset):
//   0x00..0x07  cBase (parent, classdesc) via gcValue
//   0x08..0x0f  gcDesiredEntity { _parent, mType }
//   0x34..0x4b  cName (24-byte name buffer = 6 ints)

class cBase;
class cFile;
class cMemPool;

template <class T> T *dcast(const cBase *);

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock;

class cName {
public:
    void Read(cReadBlock &);
    void Set(const char *, ...);
    void Write(cWriteBlock &) const;
};

struct cNameData { int _w[6]; };

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _p0[0x28];
    cTypeMethod write_m;    // 0x28

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredObject {
public:
    int _parent;
    cType *mType;
};

class gcDesiredEntity : public gcDesiredObject {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcValEntityIsInFluid : public gcValue {
public:
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

// 0x00333f38 (120B) — AssignCopy
void gcValEntityIsInFluid::AssignCopy(const cBase *base) {
    gcValEntityIsInFluid *other = dcast<gcValEntityIsInFluid>(base);
    const gcDesiredEntity *src = (const gcDesiredEntity *)((char *)other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*src);
    *(cNameData *)((char *)this + 0x34) = *(const cNameData *)((char *)other + 0x34);
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValEntityIsInFluid;

// 0x003340bc (280B) — GetType
const cType *gcValEntityIsInFluid::GetType(void) const {
    if (!type_gcValEntityIsInFluid) {
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
        type_gcValEntityIsInFluid =
            cType::InitializeType(0, 0, 0x24E, type_value,
                                  gcValEntityIsInFluid::New, 0, 0, 0);
    }
    return type_gcValEntityIsInFluid;
}

// 0x003341d4 (120B) — Write
void gcValEntityIsInFluid::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 40);
    char *base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    ((const cName *)((const char *)this + 0x34))->Write(wb);
    wb.End();
}
