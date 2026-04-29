#include "cBase.h"

class cFile;
class cMemPool;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void End(void);
};

struct gcDesiredUIWidgetHelper {
    int _a;
    int _b;
    int _c;
    void Write(cWriteBlock &) const;
    void GetText(char *) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDoUISetFocus : public gcAction {
public:
    ~gcDoUISetFocus();
    void GetText(char *) const;
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

void cStrAppend(char *, const char *, ...);
void gcAction_Write(const gcDoUISetFocus *, cFile &);
gcDoUISetFocus *dcast(const cBase *);
void gcAction__gcAction_cBaseptr__0012F4C8(void *, cBase *);
void gcDesiredUIWidgetHelper_ctor(void *, int);

extern char gcDoUISetFocusvirtualtable[];
extern const char gcDoUISetFocus_fmt[];
extern const char gcDoUISetFocus_base_name[];
extern const char gcDoUISetFocus_base_desc[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

// 0x0030c580, 152B
cBase *gcDoUISetFocus::New(cMemPool *pool, cBase *parent) {
    gcDoUISetFocus *result = 0;
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUISetFocus *obj = (gcDoUISetFocus *)entry->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        gcAction__gcAction_cBaseptr__0012F4C8(obj, parent);
        ((void **)obj)[1] = gcDoUISetFocusvirtualtable;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 0xC, 1);
        *(unsigned char *)((char *)obj + 0x18) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// 0x0030ca34, 72B
void gcDoUISetFocus::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->GetText(local);
    cStrAppend(buf, gcDoUISetFocus_fmt, local);
}

// 0x0030c730, 100B
void gcDoUISetFocus::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction_Write(this, file);
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->Write(wb);
    wb.Write(*(bool *)((char *)this + 0x18));
    wb.End();
}

// 0x0030c508, 120B
void gcDoUISetFocus::AssignCopy(const cBase *other) {
    gcDoUISetFocus *src = dcast(other);
    int *self_i = (int *)this;
    int *src_i = (int *)src;
    int v = self_i[2] & ~3;
    self_i[2] = v;
    self_i[2] = v | (src_i[2] & 3);
    self_i[3] = src_i[3];
    int *s1 = (int *)((char *)src + 0x10);
    int *d1 = (int *)((char *)this + 0x10);
    int *s2 = (int *)((char *)src + 0x14);
    int *d2 = (int *)((char *)this + 0x14);
    *d1 = *s1;
    *d2 = *s2;
    *(unsigned char *)((char *)this + 0x18) = *(const unsigned char *)((char *)src + 0x18);
}

// 0x0030ca7c, 128B
void gcDoUISetFocus::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->VisitReferences(flags, (cBase *)this, cb, user, mask);
}

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoUISetFocus;

// 0x0030c618, 280B
const cType *gcDoUISetFocus::GetType(void) const {
    if (!type_gcDoUISetFocus) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoUISetFocus_base_name,
                                                       gcDoUISetFocus_base_desc,
                                                       1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoUISetFocus = cType::InitializeType(0, 0, 0x151, type_action,
                                                    gcDoUISetFocus::New,
                                                    0, 0, 0);
    }
    return type_gcDoUISetFocus;
}

// 0x0030cafc, 124B
gcDoUISetFocus::~gcDoUISetFocus() {
    *(void **)((char *)this + 4) = gcDoUISetFocusvirtualtable;
}
