// gcDoTriggerActivate.cpp — gcAll_psp.obj
//
// Functions in this file:
//   0x00308758 gcDoTriggerActivate::New(cMemPool *, cBase *)   192B
//   0x00308930 gcDoTriggerActivate::Write(cFile &) const       120B

class cBase;
class cFile;
class cType;

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
};

// Dispatch slot in gcDesiredObject's secondary vtable (at this+0x10).
typedef void (*EntityWriteFn)(cBase *, cFile *);

struct EntityWriteSlot {
    short          mOffset;     // +0
    short          _pad;        // +2
    EntityWriteFn  mFn;         // +4
};

struct EntityTypeInfoWrite {
    char            _pad[0x28];
    EntityWriteSlot mSlot;       // +0x28
};

struct DtorSlot {
    short offset;
    short _pad;
    void (*fn)(void *, int);
};

class gcDoTriggerActivate : public gcAction {
public:
    // 0x0C: gcDesiredObject sub-object (vtable)
    // 0x10: gcDesiredObject secondary vtable (EntityTypeInfoWrite *)
    // 0x14: owned object pointer
    // 0x18: int (set to 1 in ctor)
    // 0x1C: int (set to 0 in ctor)
    // 0x20: bool
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    ~gcDoTriggerActivate(void);
    static void operator delete(void *);
};

// External constructors / vtables.
extern "C" {
    void gcAction_gcAction(gcDoTriggerActivate *, cBase *);
    void gcAction___dtor_gcAction_void(void *, int);
    void gcDesiredObject_gcDesiredObject(void *, void *);
}
extern char gcDoTriggerActivatevirtualtable[];
extern char gcDoTriggerActivate_desobj_vtable[];
extern char gcDoTriggerActivate_desobj_mid_vtable[] asm("D_00000978");
extern char gcDoTriggerActivate_vtable1[];
extern const char gcDoTriggerActivate_base_name[];
extern const char gcDoTriggerActivate_base_desc[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoTriggerActivate;

struct PoolBlock {
    char  _pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short _pad;
    int (*fn)(void *, int, int, int, int);
};

inline void gcDoTriggerActivate::operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    char *block = ((char **)pool)[9];
    char *entries = ((char **)block)[7];
    AllocEntry *slot = (AllocEntry *)(entries + 0x30);
    short off = slot->offset;
    int (*fn)(void *, int, int, int, int) = slot->fn;
    ((void (*)(void *, void *))fn)(block + off, p);
}

// ── gcDoTriggerActivate::GetType @ 0x00308818 ──
const cType *gcDoTriggerActivate::GetType(void) const {
    if (!type_gcDoTriggerActivate) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoTriggerActivate_base_name, gcDoTriggerActivate_base_desc, 1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoTriggerActivate = cType::InitializeType(0, 0, 0xFB, type_action, gcDoTriggerActivate::New, 0, 0, 0);
    }
    return type_gcDoTriggerActivate;
}

// ── gcDoTriggerActivate::New @ 0x00308758 ──
cBase *gcDoTriggerActivate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoTriggerActivate *result = 0;
    gcDoTriggerActivate *obj = (gcDoTriggerActivate *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoTriggerActivatevirtualtable;
        void *desobj = (char *)obj + 0x0C;
        gcDesiredObject_gcDesiredObject(desobj, obj);
        ((void **)obj)[4] = gcDoTriggerActivate_desobj_vtable;
        ((int *)obj)[6] = 1;
        ((int *)obj)[7] = 0;
        ((void **)obj)[4] = gcDoTriggerActivate_vtable1;
        ((bool *)obj)[0x20] = true;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcDoTriggerActivate::Write @ 0x00308930 ──
void gcDoTriggerActivate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);

    EntityTypeInfoWrite *ti = *(EntityTypeInfoWrite **)((char *)this + 0x10);
    EntityWriteSlot *slot = &ti->mSlot;
    cBase *embedded = (cBase *)((char *)this + 0x0C);
    slot->mFn((cBase *)((char *)embedded + slot->mOffset), wb._file);

    wb.Write(*(const bool *)((char *)this + 0x20));
    wb.End();
}

// Original object keeps this dead branch tail inside the destructor symbol.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

// 0x00308dbc - gcDoTriggerActivate::~gcDoTriggerActivate(void)
gcDoTriggerActivate::~gcDoTriggerActivate(void) {
    *(void **)((char *)this + 4) = gcDoTriggerActivatevirtualtable;

    if ((void *)((char *)this + 0x0C) != 0) {
        *(void * volatile *)((char *)this + 0x10) = gcDoTriggerActivate_desobj_vtable;
        *(void * volatile *)((char *)this + 0x10) = gcDoTriggerActivate_desobj_mid_vtable;
        *(void * volatile *)((char *)this + 0x10) = gcDoTriggerActivate_vtable1;
        if ((void *)((char *)this + 0x14) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x14);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0 && val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorSlot *slot = (DtorSlot *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, 3);
                *(int *)((char *)this + 0x14) = 0;
            }
        }
        *(void **)((char *)this + 0x10) = (void *)0x37E6A8;
    }

    gcAction___dtor_gcAction_void(this, 0);
}
