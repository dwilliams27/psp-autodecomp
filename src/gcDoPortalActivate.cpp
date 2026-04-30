// gcDoPortalActivate.cpp — gcAll_psp.obj
//
// Functions in this file:
//   0x00319534 gcDoPortalActivate::New(cMemPool *, cBase *)   192B
//   0x0031981c gcDoPortalActivate::Write(cFile &) const       120B

class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cType;

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    cFile *_file;       // +0
    int    _pos;        // +4
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

// gcAction layout (matches gcAction.cpp).
class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    cBase        *mOwner;       // 0x00
    void         *mVTable;      // 0x04
    unsigned int  mNext;        // 0x08

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

// Dispatch slot in gcDesiredObject's secondary vtable (at this+0x10).
typedef void (*EntityWriteFn)(cBase *, cFile *);
typedef void (*EntityReadFn)(void *, cFile *, cMemPool *);

struct EntityWriteSlot {
    short          mOffset;     // +0
    short          _pad;        // +2
    EntityWriteFn  mFn;         // +4
};

struct EntityReadSlot {
    short        mOffset;       // +0
    short        _pad;          // +2
    EntityReadFn mFn;           // +4
};

struct EntityTypeInfoWrite {
    char            _pad[0x28];
    EntityWriteSlot mSlot;       // +0x28
};

struct EntityTypeInfoRead {
    char           _pad[0x30];
    EntityReadSlot mSlot;        // +0x30
};

class ePortal {
public:
    void Activate(bool);
};

class gcDesiredPortal;
class gcDesiredPortalHelper : public ePortal {
};

template <class T, class H, class O>
class gcDesiredObjectT {
public:
    H *Get(bool) const;
};

class gcDoPortalActivate : public gcAction {
public:
    // 0x0C: gcDesiredObject sub-object (vtable)
    // 0x10: gcDesiredObject secondary vtable (EntityTypeInfoWrite *)
    // 0x18: int (set to 7 in ctor)
    // 0x1C: int (set to 0 in ctor)
    // 0x20: bool
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    float Evaluate(void) const;
};

// External constructors / vtables.
extern "C" {
    void gcAction_gcAction(gcDoPortalActivate *, cBase *);
    void gcDesiredObject_gcDesiredObject(void *, void *);
}
extern char gcDoPortalActivatevirtualtable[];
extern char gcDoPortalActivate_desobj_vtable[];
extern char gcDoPortalActivate_vtable1[];

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoPortalActivate asm("D_0009F764");

struct PoolBlock {
    char  _pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short _pad;
    int (*fn)(void *, int, int, int, int);
};

// ── gcDoPortalActivate::New @ 0x00319534 ──
cBase *gcDoPortalActivate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoPortalActivate *result = 0;
    gcDoPortalActivate *obj = (gcDoPortalActivate *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoPortalActivatevirtualtable;
        void *desobj = (char *)obj + 0x0C;
        gcDesiredObject_gcDesiredObject(desobj, obj);
        ((void **)obj)[4] = gcDoPortalActivate_desobj_vtable;
        ((int *)obj)[6] = 7;
        ((int *)obj)[7] = 0;
        ((void **)obj)[4] = gcDoPortalActivate_vtable1;
        ((bool *)obj)[0x20] = true;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoPortalActivate::GetType(void) const {
    if (!type_gcDoPortalActivate) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        (const char *)0x36D894, (const char *)0x36D89C,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoPortalActivate = cType::InitializeType(
            0, 0, 0x226, type_action, gcDoPortalActivate::New, 0, 0, 0);
    }
    return type_gcDoPortalActivate;
}

// ── gcDoPortalActivate::Write @ 0x0031981c ──
void gcDoPortalActivate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);
    wb.Write(*(const bool *)((char *)this + 0x20));

    EntityTypeInfoWrite *ti = *(EntityTypeInfoWrite **)((char *)this + 0x10);
    EntityWriteSlot *slot = &ti->mSlot;
    cBase *embedded = (cBase *)((char *)this + 0x0C);
    slot->mFn((cBase *)((char *)embedded + slot->mOffset), wb._file);

    wb.End();
}

// ── gcDoPortalActivate::Read @ 0x00319894 ──
int gcDoPortalActivate::Read(cFile &file, cMemPool *pool) {
    register int result asm("s3");
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] != 1)
        goto fail;
    if (!gcAction::Read(file, pool))
        goto fail;
    goto success;
fail:
    ((cFile *)rb._data[0])->SetCurrentPos((unsigned int)rb._data[1]);
    return 0;

success:
    char active;
    cFileSystem::Read(*(cFileHandle **)rb._data[0], &active, 1);
    *(bool *)((char *)this + 0x20) = active != 0;

    EntityTypeInfoRead *ti = *(EntityTypeInfoRead **)((char *)this + 0x10);
    EntityReadSlot *slot = &ti->mSlot;
    char *embedded = (char *)this + 0x0C;
    slot->mFn(embedded + slot->mOffset,
              (cFile *)rb._data[0],
              cMemPool::GetPoolFromPtr(embedded));
    return result;
}

// ── gcDoPortalActivate::Evaluate @ 0x003199a0 ──
float gcDoPortalActivate::Evaluate(void) const {
    gcDesiredPortalHelper *portal =
        ((const gcDesiredObjectT<gcDesiredPortal, gcDesiredPortalHelper, ePortal> *)((const char *)this + 0x0C))->Get(true);
    if (portal != 0) {
        portal->Activate(*(const bool *)((const char *)this + 0x20));
    }
    return 1.0f;
}
