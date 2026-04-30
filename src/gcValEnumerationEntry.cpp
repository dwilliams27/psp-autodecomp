// gcValEnumerationEntry — gcAll_psp.obj
//   0x00342118  gcValEnumerationEntry::Write(cFile &) const          (108B)
//   0x00341f4c  gcValEnumerationEntry::New(cMemPool *, cBase *)      (180B)
//
// Layout (size 0x20):
//   +0x00  cBase *parent
//   +0x04  primary classdesc pointer (cBase / cValue chain)
//   +0x08  self-pointer for secondary subobject (initialised to (cBase*)this)
//   +0x0C  secondary classdesc pointer (cType-like metadata table)
//   +0x10  unsigned char flag        (= 1)
//   +0x11  unsigned char pad         (= 0)
//   +0x14  int field                 (= 0)
//   +0x18  int field                 (= 0)
//   +0x1C  tagged self-handle        (= ((int)this) | 1)
//
// The Write entry is dispatched through (*(this+12)) + 0x28, which holds a
// {short offset, short pad, void (*fn)(void*, void*)} record.  The body
// passes (this+8 + offset, wb._data[0]) — i.e. the secondary subobject's
// adjusted address and the cFile pointer the cWriteBlock holds at +0.

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcValue : public gcExpression {
public:
    void Write(cFile &) const;
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

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct WriteEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *);
};

class gcDesiredEnumerationEntry {
public:
    void VisitReferences(unsigned int, cBase *,
                         void (*)(cBase *, unsigned int, void *),
                         void *, unsigned int);
};

class gcValEnumerationEntry : public gcValue {
public:
    void Write(cFile &) const;
    const cType *GetType(void) const;
    void VisitReferences(unsigned int, cBase *,
                         void (*)(cBase *, unsigned int, void *),
                         void *, unsigned int);
    static cBase *New(cMemPool *, cBase *);
};

extern char gcValEnumerationEntry_inner_classdesc[];   // 0x000081D0

static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_named asm("D_000385E0");
static cType *type_object asm("D_000385E4");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValEnumerationEntry asm("D_0009F854");

// ── 0x00342118 — gcValEnumerationEntry::Write(cFile &) const ──
void gcValEnumerationEntry::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcValue::Write(file);
    char *desc = *(char **)((char *)this + 12);
    WriteEntry *e = (WriteEntry *)(desc + 0x28);
    short off = e->offset;
    char *sub = (char *)this + 8;
    e->fn(sub + off, (void *)wb._data[0]);
    wb.End();
}

// ── 0x00341f4c — gcValEnumerationEntry::New(cMemPool *, cBase *) static ──
cBase *gcValEnumerationEntry::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcValEnumerationEntry *result = 0;
    int *p = (int *)entry->fn(base, 0x20, 4, 0, 0);
    if (p != 0) {
        *(int *)((char *)p + 4)  = 0x37E6A8;
        *(cBase **)((char *)p + 0) = parent;
        *(char **)((char *)p + 4)  = gcValEnumerationEntry_inner_classdesc;
        *(int *)((char *)p + 12) = 0x37E6A8;
        *(int **)((char *)p + 8) = p;
        *(int *)((char *)p + 12) = 0x388568;
        *((unsigned char *)p + 16) = 1;
        *((unsigned char *)p + 17) = 0;
        *(int *)((char *)p + 20) = 0;
        *(int *)((char *)p + 24) = 0;
        *(int *)((char *)p + 28) = (int)p | 1;
        result = (gcValEnumerationEntry *)p;
    }
    return (cBase *)result;
}

const cType *gcValEnumerationEntry::GetType(void) const {
    if (!type_gcValEnumerationEntry) {
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
        type_gcValEnumerationEntry = cType::InitializeType(
            0, 0, 0x155, type_value, gcValEnumerationEntry::New, 0, 0, 0);
    }
    return type_gcValEnumerationEntry;
}

void gcValEnumerationEntry::VisitReferences(
    unsigned int flags,
    cBase *ctx,
    void (*cb)(cBase *, unsigned int, void *),
    void *user,
    unsigned int mask)
{
    gcValEnumerationEntry *self = this;
    gcDesiredEnumerationEntry *entry =
        (gcDesiredEnumerationEntry *)((char *)self + 8);

    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)self, user);
    }
    if (entry != 0) {
        gcValEnumerationEntry *owner = self;
        gcDesiredEnumerationEntry *matched = 0;

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
        DispatchEntry *dispatch = (DispatchEntry *)((char *)classDesc + 8);
        cType *type = dispatch->fn((char *)entry + dispatch->offset);
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
            matched = entry;
        }
        if (matched != 0) {
            unsigned char direct =
                (((flags & 0xFE00) & *(unsigned short *)((char *)matched + 0x28)) != 0);
            if (direct != 0) {
                if (cb != 0) {
                    cb((cBase *)owner, (unsigned int)(void *)entry, user);
                }
            } else {
                entry->VisitReferences(flags, (cBase *)owner, cb, user, mask);
            }
        } else {
            entry->VisitReferences(flags, (cBase *)owner, cb, user, mask);
        }
    }
}
