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

extern char gcDoUISetTextColorvirtualtable[];
void gcAction__gcAction_cBaseptr__0012F4C8(void *, cBase *);
void gcDesiredUIWidgetHelper_ctor(void *, int);
gcDoUISetTextColor *dcast(const cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

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

// ── gcDoUISetTextColor::Write @ 0x00314fdc ──
void gcDoUISetTextColor::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);
    mWidget.Write(wb);
    wb.Write(mTextColorInt);
    wb.Write(mTextColorUint);
    wb.End();
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
