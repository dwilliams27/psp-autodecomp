// gcValEntityHasVariables — value type wrapping an entity reference plus a
// handle. Layout: gcValue base (~offset 8 .. 0x34), int field at 0x34.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cHandle {
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

class gcValEntityHasVariables {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
};

template <class T> T *dcast(const cBase *);
void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

extern char cBaseclassdesc[];
extern char gcValEntityHasVariablesvirtualtable[];
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

struct gcValVTEntry {
    short this_adjust;
    short pad;
    void (*fn)(void *, cFile *);
};

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValEntityHasVariables;

// ── gcValEntityHasVariables::AssignCopy(const cBase *) @ 0x003316d8 ──
void gcValEntityHasVariables::AssignCopy(const cBase *base) {
    gcValEntityHasVariables *other = dcast<gcValEntityHasVariables>(base);
    const gcDesiredEntity &src = *(const gcDesiredEntity *)(other + 8);
    *(gcDesiredEntity *)((char *)this + 8) = src;
    int &dst = *(int *)((char *)this + 0x34);
    const int &val = *(const int *)((char *)other + 0x34);
    dst = val;
}

// ── gcValEntityHasVariables::New(cMemPool *, cBase *) @ 0x00331728 ──
cBase *gcValEntityHasVariables::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValEntityHasVariables *result = 0;
    gcValEntityHasVariables *obj =
        (gcValEntityHasVariables *)entry->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = cBaseclassdesc;
        ((int *)obj)[0] = (int)parent;
        ((char **)obj)[1] = gcValEntityHasVariablesvirtualtable;
        char *sub = (char *)obj + 8;
        gcDesiredObject_ctor(sub, obj);
        ((char **)obj)[3] = D_00000338;
        gcDesiredEntityHelper_ctor((char *)obj + 0x14, 1, 0, 0);
        ((void **)obj)[3] = (void *)0x388A48;
        ((void **)obj)[8] = cBaseclassdesc;
        ((void **)obj)[7] = sub;
        ((void **)obj)[8] = (void *)0x388568;
        ((char *)obj)[0x24] = 1;
        ((char *)obj)[0x25] = 0;
        ((int *)obj)[10] = 0;
        ((int *)obj)[11] = 0;
        ((int *)obj)[12] = (int)sub | 1;
        ((int *)obj)[13] = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValEntityHasVariables::GetType(void) const @ 0x0033182c ──
const cType *gcValEntityHasVariables::GetType(void) const {
    if (!type_gcValEntityHasVariables) {
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
        type_gcValEntityHasVariables = cType::InitializeType(
            0, 0, 0xD3, type_value, gcValEntityHasVariables::New, 0, 0, 0);
    }
    return type_gcValEntityHasVariables;
}

// ── gcValEntityHasVariables::Write(cFile &) const @ 0x00331944 ──
void gcValEntityHasVariables::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcValue *)this)->Write(file);
    char *vt = *(char **)((char *)this + 0x0C);
    gcValVTEntry *entry = (gcValVTEntry *)(vt + 0x28);
    char *base = (char *)this + 8;
    ((void (*)(void *, cFile *))entry->fn)(base + entry->this_adjust,
                                           (cFile *)wb._data[0]);
    ((const cHandle *)((char *)this + 0x34))->Write(wb);
    wb.End();
}
