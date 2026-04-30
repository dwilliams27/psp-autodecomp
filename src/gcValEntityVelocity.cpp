// gcValEntityVelocity — gcValue subclass holding an entity reference whose
// velocity component is read at evaluation time.
//
// Functions matched here:
//   gcValEntityVelocity::Write(cFile &) const            @ 0x00341274  (gcAll_psp.obj)
//   gcValEntityVelocity::GetText(char *) const           @ 0x00341720  (gcAll_psp.obj)

class cFile;
class cBase;
class cMemPool;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cReadBlock {
public:
    cFile *file;
    unsigned int _pos;
    int _pad[3];
    cReadBlock(cFile &, int, bool);
    ~cReadBlock(void);
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
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValue {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcValEntityVelocity : public gcValue {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void GetText(char *) const;
};

void cStrAppend(char *, const char *, ...);
void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

extern "C" {
    void *cMemPool_GetPoolFromPtr(const void *);
    void cFile_SetCurrentPos(void *, unsigned int);
    void cFileSystem_Read(void *, void *, unsigned int);
}

extern const char gcValEntityVelocity_text_fmt[];   // @ 0x36DCB8
extern const char gcValEntityVelocity_text_arg[];   // @ 0x36DAF0
extern char cBaseclassdesc[];
extern char D_00000338[];
extern char D_000080F0[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValEntityVelocity asm("D_0009F850");

// 0x00341054 (264B) — New
cBase *gcValEntityVelocity::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValEntityVelocity *result = 0;
    gcValEntityVelocity *obj =
        (gcValEntityVelocity *)entry->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = cBaseclassdesc;
        ((int *)obj)[0] = (int)parent;
        ((char **)obj)[1] = D_000080F0;
        int size = 8;
        char *sub = (char *)obj + 12;
        ((int *)obj)[2] = size;
        gcDesiredObject_ctor(sub, obj);
        ((char **)obj)[4] = D_00000338;
        gcDesiredEntityHelper_ctor((char *)obj + 0x18, 1, 0, 0);
        ((void **)obj)[4] = (void *)0x388A48;
        ((void **)obj)[9] = cBaseclassdesc;
        ((void **)obj)[8] = sub;
        ((void **)obj)[9] = (void *)0x388568;
        ((char *)obj)[0x28] = 1;
        ((char *)obj)[0x29] = 0;
        ((int *)obj)[11] = 0;
        ((int *)obj)[12] = 0;
        ((int *)obj)[13] = (int)sub | 1;
        result = obj;
    }
    return (cBase *)result;
}

// 0x00341274 (120B) — Write
void gcValEntityVelocity::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 12))->mType + 40);
    char *base = (char *)this + 12;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    wb.Write(*(const int *)((const char *)this + 8));
    wb.End();
}

// SNC emits this file-scope asm after the following function, padding Read.
__asm__(".word 0x00000000\n");
__asm__(".size __0fTgcValEntityVelocityEReadR6FcFileP6IcMemPool, 0x10c\n");

// 0x003412ec (268B) — Read
int gcValEntityVelocity::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    int tag = rb._pad[1];
    int version;
    __asm__ volatile("ori %0, $0, 1" : "=r"(version));
    if (tag != version || gcValue::Read(file, pool) == 0) {
        cFile_SetCurrentPos(rb.file, rb._pos);
        return 0;
    }
    char *sub = (char *)this + 12;
    char *mType = *(char **)((char *)this + 16);
    const cTypeMethod *e = (const cTypeMethod *)(mType + 48);
    register char *target __asm__("$20") = sub + e->offset;
    cFile *f = rb.file;
    typedef void (*ReadFn)(void *, cFile *, void *);
    ((ReadFn)e->fn)(target, f, cMemPool_GetPoolFromPtr(sub));
    cFileSystem_Read(*(void **)rb.file, (char *)this + 8, 4);
    return 1;
}

// 0x00341720 (88B) — GetText
void gcValEntityVelocity::GetText(char *buf) const {
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 12))->mType + 120);
    char *base = (char *)this + 12;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)e->fn)(base + e->offset, buf);
    cStrAppend(buf,
               gcValEntityVelocity_text_fmt,
               gcValEntityVelocity_text_arg);
}

// 0x0034115c (280B) — GetType
const cType *gcValEntityVelocity::GetType(void) const {
    if (!type_gcValEntityVelocity) {
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
        type_gcValEntityVelocity =
            cType::InitializeType(0, 0, 0x137, type_value,
                                  gcValEntityVelocity::New, 0, 0, 0);
    }
    return type_gcValEntityVelocity;
}
