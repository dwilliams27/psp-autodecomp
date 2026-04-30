// gcValCameraIsValid - decompiled from gcAll_psp.obj
// Methods in this file:
//   0x003232f8  Evaluate(void) const
//   0x003231a0  Write(cFile &) const
//   0x00323560  ~gcValCameraIsValid(void)
//   0x00322ff0  New(cMemPool *, cBase *) static
//   0x0032320c  Read(cFile &, cMemPool *)
//
// Already matched (in src/gcValLobbyOptions.cpp):
//   AssignCopy(const cBase *), GetText(char *) const

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    cFile *file;
    unsigned int _pos;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    cFile *file;
    unsigned int _pos;
    int _pad[3];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class gcValue {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cType {
public:
    char _pad[0x1C];
    cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcValCameraIsValid : public gcValue {
public:
    float Evaluate(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
    void VisitReferences(unsigned int, cBase *,
                         void (*)(cBase *, unsigned int, void *), void *,
                         unsigned int);
    static cBase *New(cMemPool *, cBase *);
};

class gcDesiredCamera {
public:
    void VisitReferences(unsigned int, cBase *,
                         void (*)(cBase *, unsigned int, void *), void *,
                         unsigned int);
};

extern "C" {
    void *cMemPool_GetPoolFromPtr(const void *);
    void cFile_SetCurrentPos(void *, unsigned int);
    void *gcDesiredCamera_Get(void *, bool);
    void gcDesiredCamera_gcDesiredCamera(void *, cBase *);
    void gcDesiredCamera___dtor_gcDesiredCamera_void(void *, int);
}

extern char gcValCameraIsValidvirtualtable[];
extern char gcValCameraIsValid_cBase_vtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

// =============================================================================
// 0x003232f8 (60B) - Evaluate
// =============================================================================
float gcValCameraIsValid::Evaluate(void) const {
    if (gcDesiredCamera_Get((char *)this + 8, false)) {
        return 1.0f;
    }
    return 0.0f;
}

// =============================================================================
// 0x003231a0 (108B) - Write
// =============================================================================
void gcValCameraIsValid::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcValue *)this)->Write(file);
    const cTypeMethod *e =
        (const cTypeMethod *)((*(char **)((const char *)this + 12)) + 0x28);
    char *base = (char *)this + 8;
    typedef void (*WriteFn)(void *, cFile *);
    ((WriteFn)e->fn)(base + e->offset, wb.file);
    wb.End();
}

// =============================================================================
// 0x00322ff0 (152B) - New (static factory)
// =============================================================================
cBase *gcValCameraIsValid::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValCameraIsValid *result = 0;
    gcValCameraIsValid *obj =
        (gcValCameraIsValid *)entry->fn(base, 0x40, 4, 0, 0);
    if (obj != 0) {
        ((char **)obj)[1] = gcValCameraIsValid_cBase_vtable;
        ((cBase **)obj)[0] = parent;
        ((char **)obj)[1] = gcValCameraIsValidvirtualtable;
        gcDesiredCamera_gcDesiredCamera((char *)obj + 8, (cBase *)obj);
        result = obj;
    }
    return (cBase *)result;
}

// =============================================================================
// 0x0032320c (236B) - Read
// =============================================================================
int gcValCameraIsValid::Read(cFile &file, cMemPool *pool) {
    cReadBlock rb(file, 1, true);
    if (rb._pad[1] != 1 || ((gcValue *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(rb.file, rb._pos);
        return 0;
    }
    char *sub = (char *)this + 8;
    char *mType = *(char **)((char *)this + 12);
    const cTypeMethod *e = (const cTypeMethod *)(mType + 0x30);
    cFile *f = rb.file;
    typedef void (*ReadFn)(void *, cFile *, void *);
    ((ReadFn)e->fn)(sub + e->offset, f, cMemPool_GetPoolFromPtr(sub));
    return 1;
}

static cType *type_base;
static cType *type_expression;
static cType *type_value;
static cType *type_named;
static cType *type_object;
static cType *type_gcValCameraIsValid;

const cType *gcValCameraIsValid::GetType(void) const {
    if (!type_gcValCameraIsValid) {
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
        type_gcValCameraIsValid = cType::InitializeType(
            0, 0, 0x1C7, type_value, gcValCameraIsValid::New, 0, 0, 0);
    }
    return type_gcValCameraIsValid;
}

void gcValCameraIsValid::VisitReferences(unsigned int flags, cBase *ctx,
                                         void (*cb)(cBase *, unsigned int,
                                                    void *),
                                         void *user, unsigned int mask) {
    gcValCameraIsValid *self = this;
    gcDesiredCamera *camera = (gcDesiredCamera *)((char *)self + 8);
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)self, user);
    }
    if (camera != 0) {
        gcValCameraIsValid *owner = self;
        gcDesiredCamera *matched = 0;
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

        void *classDesc = *(void **)((char *)self + 12);
        cType *myType = type_object;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        cType *type = entry->fn((char *)camera + entry->offset);
        int ok;

        if (myType == 0) {
            goto fail;
        }
        if (type != 0) {
        loop:
            if (type != myType) {
                type = type->mParent;
                if (type == 0) {
                    goto fail;
                }
                goto loop;
            }
            ok = 1;
        } else {
fail:
            ok = 0;
        }
        if (ok != 0) {
            matched = camera;
        }
        if (matched != 0) {
            unsigned int direct =
                (((flags & 0xFE00) & *(unsigned short *)((char *)matched + 0x28)) != 0);
            if (direct != 0) {
                if (cb != 0) {
                    cb((cBase *)owner, (unsigned int)(void *)camera, user);
                }
            } else {
                camera->VisitReferences(flags, (cBase *)owner, cb, user, mask);
            }
        } else {
            camera->VisitReferences(flags, (cBase *)owner, cb, user, mask);
        }
    }
}

// =============================================================================
// 0x00323560 (136B) - destructor
// =============================================================================
extern "C" void gcValCameraIsValid___dtor_gcValCameraIsValid_void(void *self, int flags) {
    if (self != 0) {
        ((char **)self)[1] = gcValCameraIsValidvirtualtable;
        gcDesiredCamera___dtor_gcDesiredCamera_void((char *)self + 8, 2);
        ((char **)self)[1] = gcValCameraIsValid_cBase_vtable;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec =
                (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}
