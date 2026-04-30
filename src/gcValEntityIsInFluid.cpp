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

void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

extern char cBaseclassdesc[];
extern char D_00000338[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// 0x00333f38 (120B) — AssignCopy
void gcValEntityIsInFluid::AssignCopy(const cBase *base) {
    gcValEntityIsInFluid *other = dcast<gcValEntityIsInFluid>(base);
    const gcDesiredEntity *src = (const gcDesiredEntity *)((char *)other + 8);
    ((gcDesiredEntity *)((char *)this + 8))->operator=(*src);
    *(cNameData *)((char *)this + 0x34) = *(const cNameData *)((char *)other + 0x34);
}

// 0x00333fb0 (268B) — New
cBase *gcValEntityIsInFluid::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValEntityIsInFluid *result = 0;
    gcValEntityIsInFluid *obj =
        (gcValEntityIsInFluid *)entry->fn(base, 0x4C, 4, 0, 0);
    if (obj != 0) {
        register void *type0 asm("a0");
        register void *type1 asm("a0");
        register void *helper_desc asm("a0");

        ((void **)obj)[1] = (void *)0x37E6A8;
        __asm__ volatile("lui %0,0x0" : "=r"(type0));
        ((cBase **)obj)[0] = parent;
        __asm__ volatile("addiu %0,%0,0x7648" : "+r"(type0));
        ((void **)obj)[1] = type0;
        char *sub = (char *)obj + 8;
        gcDesiredObject_ctor(sub, obj);
        __asm__ volatile("lui %0,0x0\n\taddiu %0,%0,0x338"
                         : "=r"(type1));
        ((void **)obj)[3] = type1;
        gcDesiredEntityHelper_ctor((char *)obj + 0x14, 1, 0, 0);
        __asm__ volatile("lui %0,0x39\n\taddiu %0,%0,-0x75b8"
                         : "=r"(helper_desc));
        ((void **)obj)[8] = (void *)0x37E6A8;
        ((void **)obj)[3] = helper_desc;
        ((char **)obj)[7] = sub;
        ((void **)obj)[8] = (void *)0x388568;
        ((char *)obj)[0x24] = 1;
        ((char *)obj)[0x25] = 0;
        ((int *)obj)[10] = 0;
        int sub_or_1 = (int)sub | 1;
        ((int *)obj)[11] = 0;
        ((int *)obj)[12] = sub_or_1;
        ((short *)obj)[0x24] = 0;
        ((short *)obj)[0x25] = 0;
        ((char *)obj)[0x34] = 0;
        result = obj;
    }
    return (cBase *)result;
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
