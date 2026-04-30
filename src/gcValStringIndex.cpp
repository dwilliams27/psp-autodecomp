// gcValStringIndex — gcValue subclass holding a string-table reference.
//
// Functions matched here:
//   gcValStringIndex::Write(cFile &) const   @ 0x0035ea8c  (gcAll_psp.obj)
//   gcValStringIndex::GetText(char *) const  @ 0x0035ec18  (gcAll_psp.obj)
//
// The class layout (relevant subset):
//   0x00..0x07  gcValue base (mParent, mClassdesc)
//   0x08..0x0f  gcDesiredObject { _parent, mType }  ← reflective member
//
// Both methods do reflective dispatch through gcDesiredObject::mType: the
// cType holds entries at 0x28 (write) and 0x78 (text), each a {short offset,
// short pad, void *fn} record. The offset re-bases the call inside the
// gcDesiredObject payload.

class cFile;
class cBase;
class cMemPool;

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
    char _p0[0x1C];
    cType *mParent;        // 0x1C
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

class gcDesiredString : public gcDesiredObject {
public:
    void VisitReferences(unsigned int, cBase *,
                         void (*)(cBase *, unsigned int, void *),
                         void *, unsigned int);
};

class gcValue {
public:
    cBase *mParent;
    void *mVtable;
    void Write(cFile &) const;
};

class gcValStringIndex : public gcValue {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
    void GetText(char *) const;
    const cType *GetType(void) const;
    void VisitReferences(unsigned int, cBase *,
                         void (*)(cBase *, unsigned int, void *),
                         void *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

extern "C" void cStrCat(char *, const char *);
void gcDesiredObject_gcDesiredObject(void *, cBase *);

extern const char gcValStringIndex_text[];   // @ 0x36F79C
extern char cBaseclassdesc[];
extern char gcValuevirtualtable[];
extern char gcDesiredString_cBase_vtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_named asm("D_000385E0");
static cType *type_object asm("D_000385E4");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValStringIndex asm("D_0009F8E4");

// 0x0035e8b4 (192B) — New
cBase *gcValStringIndex::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValStringIndex *result = 0;
    gcValStringIndex *obj = (gcValStringIndex *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = cBaseclassdesc;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = gcValuevirtualtable;
        void *desired = (char *)obj + 8;
        gcDesiredObject_gcDesiredObject(desired, (cBase *)obj);
        ((void **)obj)[3] = gcDesiredString_cBase_vtable;
        ((unsigned char *)obj)[0x14] = 0;
        ((unsigned char *)obj)[0x15] = 0;
        ((int *)obj)[6] = 0;
        ((int *)obj)[7] = 0;
        ((int *)obj)[8] = (int)((unsigned int)desired | 1);
        result = obj;
    }
    return (cBase *)result;
}

// 0x0035ea8c (108B) — Write
void gcValStringIndex::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue::Write(file);
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 40);
    char *base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    wb.End();
}

// 0x0035ec18 (80B) — GetText
void gcValStringIndex::GetText(char *buf) const {
    const cTypeMethod *e =
        (const cTypeMethod *)((char *)((const gcDesiredObject *)((const char *)this + 8))->mType + 120);
    char *base = (char *)this + 8;
    typedef void (*TextFn)(void *, char *);
    ((TextFn)e->fn)(base + e->offset, buf);
    cStrCat(buf, gcValStringIndex_text);
}

const cType *gcValStringIndex::GetType(void) const {
    if (!type_gcValStringIndex) {
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
        type_gcValStringIndex = cType::InitializeType(0, 0, 0x20F, type_value,
                                                      gcValStringIndex::New,
                                                      0, 0, 0);
    }
    return type_gcValStringIndex;
}

void gcValStringIndex::VisitReferences(
    unsigned int flags,
    cBase *ctx,
    void (*cb)(cBase *, unsigned int, void *),
    void *user,
    unsigned int mask)
{
    gcDesiredString *temp_s4 = (gcDesiredString *)((char *)this + 8);
    gcValStringIndex *self = this;

    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)self, user);
    }
    if (temp_s4 != 0) {
        gcValStringIndex *owner = self;
        gcDesiredString *var_s6 = 0;

        if (!type_object) {
            if (!type_named) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_named = cType::InitializeType(0, 0, 2, type_base,
                                                   &cNamed::New, 0, 0, 0);
            }
            type_object = cType::InitializeType(0, 0, 3, type_named,
                                                0, 0, 0, 0);
        }

        void *temp_a0 = *(void **)((char *)self + 12);
        cType *temp_s0 = type_object;
        DispatchEntry *dispatch = (DispatchEntry *)((char *)temp_a0 + 8);
        short temp_a1 = dispatch->offset;
        cType *(*temp_a2)(void *) = dispatch->fn;
        cType *var_s3 = temp_a2((char *)temp_s4 + temp_a1);
        int var_a0 = 0;

        if (temp_s0 != 0 && var_s3 != 0) {
            cType *node = var_s3;
            do {
                if (node == temp_s0) {
                    var_a0 = 1;
                    break;
                }
                node = node->mParent;
            } while (node != 0);
        }
        if (var_a0 != 0) {
            var_s6 = temp_s4;
        }

        if (var_s6 != 0) {
            if ((unsigned char)((((flags & 0xFE00) &
                                  *(unsigned short *)((char *)var_s6 + 0x28)) != 0))) {
                if (cb != 0) {
                    cb((cBase *)owner, (unsigned int)(void *)temp_s4, user);
                }
            } else {
                goto block_23;
            }
        } else {
block_23:
            temp_s4->VisitReferences(flags, (cBase *)owner, cb, user, mask);
        }
    }
}
