#include "cBase.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern "C" {
    void gcAction_gcAction(void *, cBase *);
    void gcDesiredCamera_gcDesiredCamera(void *, cBase *);
    void gcDesiredObject_gcDesiredObject(void *, cBase *);
    void *gcDesiredCamera_Get(void *, bool);
}

extern char gcDoUIShowDialogvirtualtable[];
extern char gcDoUIShowDialog_objectvtable[];
extern const char gcDoUIShowDialog_base_name[];
extern const char gcDoUIShowDialog_base_desc[];

class gcDoUIShowDialog {
public:
    gcDoUIShowDialog(cBase *);
    gcDoUIShowDialog &operator=(const gcDoUIShowDialog &);
    void AssignCopy(const cBase *);
    void GetName(char *) const;
    void *GetUI(void) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
};

// ============================================================================
// gcDoUIShowDialog::GetName(char *) const  @ 0x001395a0  52B
// ============================================================================
struct _VtEntry {
    short adj;
    short pad;
    void (*fn)(void *, char *);
};

void gcDoUIShowDialog::GetName(char *out) const {
    *out = 0;
    _VtEntry *e = (_VtEntry *)((*(char **)((char *)this + 0x48)) + 0x78);
    short adj = e->adj;
    void (*fn)(void *, char *) = e->fn;
    char *t = (char *)this + 0x44;
    fn(t + adj, out);
}

// ============================================================================
// gcDoUIShowDialog::gcDoUIShowDialog(cBase *)  @ 0x001394ec  180B
// ============================================================================
gcDoUIShowDialog::gcDoUIShowDialog(cBase *b) {
    gcAction_gcAction(this, b);
    ((void **)this)[1] = gcDoUIShowDialogvirtualtable;
    gcDesiredCamera_gcDesiredCamera((char *)this + 0x0C, (cBase *)this);
    gcDesiredObject_gcDesiredObject((char *)this + 0x44, (cBase *)this);

    int *self = (int *)this;
    self[0x50 / 4] = 1;
    self[0x54 / 4] = 0;
    *(char **)((char *)this + 0x48) = gcDoUIShowDialog_objectvtable;
    self[0x58 / 4] = 0x51;
    int h = (int)this | 1;
    self[0x5C / 4] = 0;
    self[0x60 / 4] = h;
    self[0x64 / 4] = 0;
    *(float *)((char *)this + 0x68) = 0.0f;
    *(float *)((char *)this + 0x6C) = 0.0f;
    *(float *)((char *)this + 0x70) = 0.0f;
    *(float *)((char *)this + 0x74) = 0.0f;
    self[0x78 / 4] = h;
    self[0x7C / 4] = h;
    self[0x80 / 4] = h;
    self[0x84 / 4] = h;
    self[0x88 / 4] = h;
    *((char *)this + 0x8C) = 0;
    self[0x90 / 4] = 0;
    self[0x94 / 4] = h;
    self[0x98 / 4] = h;
}

// ============================================================================
// gcDoUIShowDialog::GetUI(void) const  @ 0x001395d4  128B
// ============================================================================
static inline bool _hasFlag(unsigned int flags, unsigned int bit) {
    return (flags & bit) != 0;
}

void *gcDoUIShowDialog::GetUI(void) const {
    unsigned int flags = *(unsigned int *)((char *)this + 0x58);
    void *result = 0;
    if (_hasFlag(flags, 0x40)) {
        result = *(void **)0x37D7BC;
    } else if (!_hasFlag(flags, 0x20)) {
        void *cam = gcDesiredCamera_Get((char *)this + 0x0C, true);
        if (cam != 0) {
            result = (char *)cam + 0x11E4;
        }
    }
    if (result == 0) {
        result = (void *)0x99928;
    }
    return result;
}

// ============================================================================
// gcDoUIShowDialog::New(cMemPool *, cBase *) static  @ 0x0028c3c0  124B
// ============================================================================
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

cBase *gcDoUIShowDialog::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUIShowDialog *result = 0;
    gcDoUIShowDialog *obj = (gcDoUIShowDialog *)entry->fn(base, 0x9C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoUIShowDialog(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================================
// gcDoUIShowDialog::GetType(void) const  @ 0x0028c43c  280B
// ============================================================================
static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoUIShowDialog;

const cType *gcDoUIShowDialog::GetType(void) const {
    if (!type_gcDoUIShowDialog) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoUIShowDialog_base_name, gcDoUIShowDialog_base_desc, 1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoUIShowDialog = cType::InitializeType(0, 0, 0x8A, type_action, gcDoUIShowDialog::New, 0, 0, 0);
    }
    return type_gcDoUIShowDialog;
}

// ============================================================================
// gcDoUIShowDialog::AssignCopy(const cBase *)  @ 0x0028c21c  420B
// ============================================================================
void gcDoUIShowDialog::AssignCopy(const cBase *other) {
    const gcDoUIShowDialog *copy = 0;

    if (other != 0) {
        if (!type_gcDoUIShowDialog) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoUIShowDialog_base_name, gcDoUIShowDialog_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoUIShowDialog = cType::InitializeType(
                0, 0, 0x8A, type_action, gcDoUIShowDialog::New, 0, 0, 0);
        }

        void *classDesc = *(void **)((char *)other + 4);
        cType *target = type_gcDoUIShowDialog;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)other + offset, offset, fn);
        int isValid;

        if (target != 0) {
            goto have_target;
        }
        isValid = 0;
        goto cast_done;

have_target:
        if (type != 0) {
loop_cast:
            if (type == target) {
                isValid = 1;
            } else {
                type = (cType *)type->mParent;
                if (type != 0) {
                    goto loop_cast;
                }
                goto invalid_cast;
            }
        } else {
invalid_cast:
            isValid = 0;
        }

cast_done:
        if (isValid != 0) {
            copy = (const gcDoUIShowDialog *)other;
        }
    }
    operator=(*copy);
}
