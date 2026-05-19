// gcDoUIEffect — gcAll_psp.obj
//   0x0030a118  New(cMemPool *, cBase *) static               (156B)
//   0x0030a2cc  Write(cFile &) const                       (112B)
//   0x0030a73c  ~gcDoUIEffect(void)                        (124B)

class cBase;
class cFile;
class cMemPool;
class cType;
class gcUIWidget;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" {
    void *cMemPool_GetPoolFromPtr(const void *);
    void gcAction_Write(const void *, cFile &);
}

class gcAction {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    ~gcAction();
    void Write(cFile &) const;
};

class gcDesiredUIWidgetHelper {
public:
    void GetText(char *) const;
    void Write(class cWriteBlock &) const;
    gcUIWidget *GetWidget(const cType *, bool) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class gcDoUIEffect : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    float Evaluate(void) const;
    void GetText(char *) const;
    void Write(cFile &) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    ~gcDoUIEffect();
    static void operator delete(void *p) {
        void *pool = cMemPool_GetPoolFromPtr(p);
        void *block = *(void **)((char *)pool + 0x24);
        DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
        short off = rec->offset;
        rec->fn((char *)block + off, p);
    }
};

class gcUIWidget {
public:
    int PushPull(bool);
};

struct gcUIEffectSlot {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

void gcAction_gcAction(gcDoUIEffect *, cBase *);
void gcDesiredUIWidgetHelper_ctor(gcDesiredUIWidgetHelper *, int);
gcDoUIEffect *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);
extern char gcDoUIEffectvirtualtable[];
extern const char gcDoUIEffect_base_name[];
extern const char gcDoUIEffect_base_desc[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoUIEffect;

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009990C;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

// 0x0030a1b4 — GetType(void) const
const cType *gcDoUIEffect::GetType(void) const {
    if (!type_gcDoUIEffect) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoUIEffect_base_name, gcDoUIEffect_base_desc, 1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoUIEffect = cType::InitializeType(0, 0, 0x204, type_action, gcDoUIEffect::New, 0, 0, 0);
    }
    return type_gcDoUIEffect;
}

// 0x0030a098 — AssignCopy(const cBase *)
void gcDoUIEffect::AssignCopy(const cBase *other) {
    gcDoUIEffect *src = dcast(other);
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
    self_i[7] = src_i[7];
}

// 0x0030a118 — New(cMemPool *, cBase *) static
cBase *gcDoUIEffect::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUIEffect *result = 0;
    gcDoUIEffect *obj = (gcDoUIEffect *)entry->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoUIEffectvirtualtable;
        gcDesiredUIWidgetHelper_ctor((gcDesiredUIWidgetHelper *)((char *)obj + 12), 1);
        *(int *)((char *)obj + 24) = 0;
        *(int *)((char *)obj + 28) = 0;
        result = obj;
    }
    return (cBase *)result;
}

// 0x0030a2cc — Write(cFile &) const, 112B
void gcDoUIEffect::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction_Write(this, file);
    ((gcDesiredUIWidgetHelper *)((char *)this + 12))->Write(*(cWriteBlock *)&wb);
    wb.Write(((int *)this)[7]);
    wb.Write(((int *)this)[6]);
    wb.End();
}

// 0x0030a448 — Evaluate(void) const
float gcDoUIEffect::Evaluate(void) const {
    gcDesiredUIWidgetHelper *helper =
        (gcDesiredUIWidgetHelper *)((char *)this + 0x0C);

    if (D_0009990C == 0) {
        if (D_000385E0 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType(
                    gcDoUIEffect_base_name, gcDoUIEffect_base_desc,
                    1, 0, 0, 0, 0, 0);
            }
            D_000385E0 = cType::InitializeType(
                0, 0, 2, D_000385DC,
                (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
        }
        D_0009990C = cType::InitializeType(
            0, 0, 0x84, D_000385E0, 0, 0, 0, 0);
    }

    gcUIWidget *widget = helper->GetWidget(D_0009990C, true);
    if (widget != 0) {
        int mode = *(int *)((char *)this + 0x18);
        if ((unsigned int)mode >= 5U) goto one;
        switch (mode) {
        case 0: {
            gcUIEffectSlot *slot =
                (gcUIEffectSlot *)(*(char **)((char *)widget + 4) + 0x90);
            slot->fn((char *)widget + slot->offset,
                     *(int *)((char *)this + 0x1C));
            goto one;
        }
        case 1:
            widget->PushPull(false);
            goto one;
        case 2:
            widget->PushPull(true);
            goto one;
        case 3:
            do {
            } while (widget->PushPull(false) != 0);
            goto one;
        case 4:
            do {
            } while (widget->PushPull(true) != 0);
            goto one;
        default:
            return 0.0f;
        }
    }
    return 0.0f;

one:
    return 1.0f;
}

// 0x0030a634 — GetText(char *) const, 136B
void gcDoUIEffect::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((const gcDesiredUIWidgetHelper *)((const char *)this + 0x0C))->GetText(local);

    if (*(const int *)((const char *)this + 0x18) == 0) {
        cStrAppend(buf, (const char *)0x36F004, local, (const char *)0x36DAF0);
    } else {
        cStrAppend(buf, (const char *)0x36F014, local, (const char *)0x36DAF0);
    }
}

// 0x0030a6bc — VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int)
void gcDoUIEffect::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->VisitReferences(flags, (cBase *)this, cb, user, mask);
}

// 0x0030a73c — ~gcDoUIEffect(void), 124B
gcDoUIEffect::~gcDoUIEffect() {
    ((void **)this)[1] = gcDoUIEffectvirtualtable;
}
