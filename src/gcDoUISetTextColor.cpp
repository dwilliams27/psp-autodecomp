// gcDoUISetTextColor.cpp — decompiled from gcAll_psp.obj.
// Parent: gcAction.
//
// Functions:
//   0x00314d9c gcDoUISetTextColor::AssignCopy(const cBase *)  136B
//   0x00314e24 gcDoUISetTextColor::New(cMemPool *, cBase *) static  160B
//   0x00314fdc gcDoUISetTextColor::Write(cFile &) const   112B
//   0x00315370 gcDoUISetTextColor::VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int)  128B
//   0x003153f0 gcDoUISetTextColor::~gcDoUISetTextColor(void)  124B

class cBase;
class cFile;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    char _pad[12];   // mOwner, mVTable, mNext at 0/4/8

    gcAction(cBase *);
    ~gcAction();
    void Write(cFile &) const;
};

class gcDesiredUIWidgetHelper {
public:
    char _pad[12];   // 3 ints
    void Write(cWriteBlock &) const;
    void GetText(char *) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class gcDoUISetTextColor : public gcAction {
public:
    gcDesiredUIWidgetHelper mWidget;  // 0x0C
    int  mTextColorInt;                // 0x18
    unsigned int mTextColorUint;       // 0x1C

    gcDoUISetTextColor(cBase *);
    ~gcDoUISetTextColor();
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    void Write(cFile &) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

class gcDoUISetTextSprite {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

extern char gcDoUISetTextColorvirtualtable[];
extern const char gcDoUISetTextColor_base_name[] asm("D_0036D894");
extern const char gcDoUISetTextColor_base_desc[] asm("D_0036D89C");
extern const char gcDoUISetTextColor_fmt[] asm("D_0036F0E8");
extern const char gcDoUISetTextColor_arg[] asm("D_0036DAF0");
void gcAction__gcAction_cBaseptr__0012F4C8(void *, cBase *);
void gcDesiredUIWidgetHelper_ctor(void *, int);
gcDoUISetTextColor *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoUISetTextColor asm("D_0009F74C");
static cType *type_gcDoUISetTextSprite asm("D_0009F750");

// ── gcDoUISetTextColor::AssignCopy @ 0x00314d9c ──
void gcDoUISetTextColor::AssignCopy(const cBase *other) {
    gcDoUISetTextColor *src = dcast(other);
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
    self_i[6] = src_i[6];
    int *s3 = (int *)((char *)src + 0x1C);
    int *d3 = (int *)((char *)this + 0x1C);
    *d3 = *s3;
}

// ── gcDoUISetTextColor::New @ 0x00314e24 ──
cBase *gcDoUISetTextColor::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUISetTextColor *result = 0;
    gcDoUISetTextColor *obj = (gcDoUISetTextColor *)entry->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        gcAction__gcAction_cBaseptr__0012F4C8(obj, parent);
        ((void **)obj)[1] = gcDoUISetTextColorvirtualtable;
        gcDesiredUIWidgetHelper_ctor((char *)obj + 0xC, 1);
        *(int *)((char *)obj + 0x18) = 0;
        *(int *)((char *)obj + 0x1C) = -1;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcDoUISetTextColor::GetType @ 0x00314ec4 ──
const cType *gcDoUISetTextColor::GetType(void) const {
    if (!type_gcDoUISetTextColor) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoUISetTextColor_base_name, gcDoUISetTextColor_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoUISetTextColor = cType::InitializeType(0, 0, 0x1FA, type_action, gcDoUISetTextColor::New, 0, 0, 0);
    }
    return type_gcDoUISetTextColor;
}

// ── gcDoUISetTextSprite::GetType @ 0x003156dc ──
const cType *gcDoUISetTextSprite::GetType(void) const {
    if (!type_gcDoUISetTextSprite) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoUISetTextColor_base_name, gcDoUISetTextColor_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoUISetTextSprite = cType::InitializeType(0, 0, 0x150, type_action, gcDoUISetTextSprite::New, 0, 0, 0);
    }
    return type_gcDoUISetTextSprite;
}

// ── gcDoUISetTextColor::Write @ 0x00314fdc ──
void gcDoUISetTextColor::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);
    mWidget.Write(wb);
    wb.Write(mTextColorInt);
    wb.Write(mTextColorUint);
    wb.End();
}

// ── gcDoUISetTextColor::GetText @ 0x00315310 ──
void gcDoUISetTextColor::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->GetText(local);
    cStrAppend(buf, gcDoUISetTextColor_fmt, local, gcDoUISetTextColor_arg, mTextColorUint);
}

// ── gcDoUISetTextColor::VisitReferences @ 0x00315370 ──
void gcDoUISetTextColor::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->VisitReferences(flags, (cBase *)this, cb, user, mask);
}

// ── gcDoUISetTextColor::~gcDoUISetTextColor @ 0x003153f0 ──
gcDoUISetTextColor::~gcDoUISetTextColor() {
    *(void **)((char *)this + 4) = gcDoUISetTextColorvirtualtable;
}
