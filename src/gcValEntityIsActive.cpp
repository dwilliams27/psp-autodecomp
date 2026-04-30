// gcValEntityIsActive — gcValue subclass holding an entity reference whose
// "is active" flag is read at evaluation time.
//
// Functions matched here:
//   gcValEntityIsActive::Write(cFile &) const   @ 0x00332610  (gcAll_psp.obj)
//   gcValEntityIsActive::GetText(char *) const  @ 0x003327d0  (gcAll_psp.obj)
//
// The class layout (relevant subset):
//   0x00..0x07  gcValue base (mParent, mClassdesc)
//   0x08..0x0f  gcDesiredEntity { _parent, mType }  ← reflective member
//
// Both methods do reflective dispatch through gcDesiredEntity::mType: the
// cType holds entries at 0x28 (write) and 0x78 (text), each a {short offset,
// short pad, void *fn} record. The offset re-bases the call inside the
// gcDesiredEntity payload.

class cFile;
class cBase;
class cMemPool;

void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    char _p0[0x28];
    cTypeMethod write_m;    // 0x28
    cTypeMethod read_m;     // 0x30
    char _p1[0x40];
    cTypeMethod text_m;     // 0x78

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
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcValEntityIsActive : public gcValue {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void GetText(char *) const;
};

extern "C" void cStrCat(char *, const char *);

extern const char gcValEntityIsActive_text[];   // @ 0x36F2DC

extern char cBaseclassdesc[];
extern char D_00007488[];
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

// 0x003323f8 (256B) — New (static factory)
cBase *gcValEntityIsActive::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValEntityIsActive *result = 0;
    gcValEntityIsActive *obj =
        (gcValEntityIsActive *)entry->fn(base, 0x34, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = cBaseclassdesc;
        ((int *)obj)[0] = (int)parent;
        ((char **)obj)[1] = D_00007488;
        char *sub = (char *)obj + 8;
        gcDesiredObject_ctor(sub, obj);
        ((char **)obj)[3] = D_00000338;
        gcDesiredEntityHelper_ctor((char *)obj + 0x14, 1, 0, 0);
        ((void **)obj)[3] = (void *)0x388A48;
        ((char **)obj)[8] = cBaseclassdesc;
        ((char **)obj)[7] = sub;
        ((void **)obj)[8] = (void *)0x388568;
        ((char *)obj)[0x24] = 1;
        ((char *)obj)[0x25] = 0;
        ((int *)obj)[10] = 0;
        ((int *)obj)[11] = 0;
        ((int *)obj)[12] = (int)sub | 1;
        result = obj;
    }
    return (cBase *)result;
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_gcValEntityIsActive;

// 0x003324f8 (280B) — GetType
const cType *gcValEntityIsActive::GetType(void) const {
    if (!type_gcValEntityIsActive) {
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
        type_gcValEntityIsActive =
            cType::InitializeType(0, 0, 0x158, type_value,
                                  gcValEntityIsActive::New, 0, 0, 0);
    }
    return type_gcValEntityIsActive;
}

// 0x00332610 (108B) — Write
void gcValEntityIsActive::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 40);
    char *base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    wb.End();
}

// 0x003327d0 (80B) — GetText
void gcValEntityIsActive::GetText(char *buf) const {
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 120);
    char *base = (char *)this + 8;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)e->fn)(base + e->offset, buf);
    cStrCat(buf, gcValEntityIsActive_text);
}
