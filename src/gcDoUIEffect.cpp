// gcDoUIEffect — gcAll_psp.obj
//   0x0030a118  New(cMemPool *, cBase *) static               (156B)
//   0x0030a2cc  Write(cFile &) const                       (112B)
//   0x0030a73c  ~gcDoUIEffect(void)                        (124B)

class cBase;
class cFile;
class cMemPool;

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
    void Write(class cWriteBlock &) const;
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

void gcAction_gcAction(gcDoUIEffect *, cBase *);
void gcDesiredUIWidgetHelper_ctor(gcDesiredUIWidgetHelper *, int);
gcDoUIEffect *dcast(const cBase *);
extern char gcDoUIEffectvirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

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
