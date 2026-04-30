// gcDoTimerActivate.cpp - gcAll_psp.obj
//
// Functions in this file:
//   0x00307cc0 gcDoTimerActivate::New(cMemPool *, cBase *) static  196B
//   0x00307e9c gcDoTimerActivate::Write(cFile &) const             132B

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

typedef void (*TimerWriteFn)(cBase *, cFile *);

struct TimerWriteSlot {
    short        mOffset;     // +0
    short        _pad;        // +2
    TimerWriteFn mFn;         // +4
};

struct TimerTypeInfoWrite {
    char           _pad[0x28];
    TimerWriteSlot mSlot;     // +0x28
};

struct DtorSlot {
    short offset;
    short _pad;
    void (*fn)(void *, int);
};

class gcDoTimerActivate : public gcAction {
public:
    // 0x0C: gcDesiredObject sub-object
    // 0x10: gcDesiredObject secondary vtable (TimerTypeInfoWrite *)
    // 0x14: owned object pointer
    // 0x18: int (set to 1 in ctor)
    // 0x1C: int (set to 0 in ctor)
    // 0x20: bool
    // 0x21: bool
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    ~gcDoTimerActivate(void);
    static void operator delete(void *);
};

extern "C" {
    void gcAction_gcAction(gcDoTimerActivate *, cBase *);
    void gcAction___dtor_gcAction_void(void *, int);
    void gcDesiredObject_gcDesiredObject(void *, void *);
}

extern char gcDoTimerActivatevirtualtable[];
extern char gcDoTimerActivate_desobj_vtable[];
extern char gcDoTimerActivate_desobj_mid_vtable[] asm("D_000008D8");
extern char gcDoTimerActivate_vtable1[];
extern const char gcDoTimerActivate_base_name[] asm("D_0036D894");
extern const char gcDoTimerActivate_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoTimerActivate asm("D_0009F718");

struct PoolBlock {
    char  _pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short _pad;
    int (*fn)(void *, int, int, int, int);
};

inline void gcDoTimerActivate::operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    char *block = ((char **)pool)[9];
    char *entries = ((char **)block)[7];
    AllocEntry *slot = (AllocEntry *)(entries + 0x30);
    short off = slot->offset;
    int (*fn)(void *, int, int, int, int) = slot->fn;
    ((void (*)(void *, void *))fn)(block + off, p);
}

// 0x00307cc0 - gcDoTimerActivate::New(cMemPool *, cBase *) static
cBase *gcDoTimerActivate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoTimerActivate *result = 0;
    gcDoTimerActivate *obj =
        (gcDoTimerActivate *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoTimerActivatevirtualtable;
        void *desobj = (char *)obj + 0x0C;
        gcDesiredObject_gcDesiredObject(desobj, obj);
        ((void **)obj)[4] = gcDoTimerActivate_desobj_vtable;
        ((int *)obj)[6] = 1;
        ((int *)obj)[7] = 0;
        ((void **)obj)[4] = gcDoTimerActivate_vtable1;
        ((bool *)obj)[0x20] = true;
        ((bool *)obj)[0x21] = false;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoTimerActivate::GetType(void) const {
    if (!type_gcDoTimerActivate) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoTimerActivate_base_name,
                        gcDoTimerActivate_base_desc, 1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoTimerActivate = cType::InitializeType(
            0, 0, 0xDF, type_action, gcDoTimerActivate::New, 0, 0, 0);
    }
    return type_gcDoTimerActivate;
}

// 0x00307e9c - gcDoTimerActivate::Write(cFile &) const
void gcDoTimerActivate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction::Write(file);

    TimerTypeInfoWrite *ti = *(TimerTypeInfoWrite **)((char *)this + 0x10);
    TimerWriteSlot *slot = &ti->mSlot;
    cBase *embedded = (cBase *)((char *)this + 0x0C);
    slot->mFn((cBase *)((char *)embedded + slot->mOffset), wb._file);

    wb.Write(*(const bool *)((char *)this + 0x20));
    wb.Write(*(const bool *)((char *)this + 0x21));
    wb.End();
}

// Original object keeps this dead branch tail inside the destructor symbol.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

// 0x0030836c - gcDoTimerActivate::~gcDoTimerActivate(void)
gcDoTimerActivate::~gcDoTimerActivate(void) {
    *(void **)((char *)this + 4) = gcDoTimerActivatevirtualtable;

    if ((void *)((char *)this + 0x0C) != 0) {
        *(void * volatile *)((char *)this + 0x10) = gcDoTimerActivate_desobj_vtable;
        *(void * volatile *)((char *)this + 0x10) = gcDoTimerActivate_desobj_mid_vtable;
        *(void * volatile *)((char *)this + 0x10) = gcDoTimerActivate_vtable1;
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
