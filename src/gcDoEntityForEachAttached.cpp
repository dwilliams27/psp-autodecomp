#include "gcDoEntityForEach.h"
#include "cBase.h"

// ---------------------------------------------------------------------------
// Local external classes / helpers used across these methods
// ---------------------------------------------------------------------------

class cReadBlock {
public:
    int _data[5];  // [0]=cFile*, [1]=savedPos, [2]=?, [3]=ok, [4]=?
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class gcExpressionList {
public:
    gcExpressionList(cBase *);
    gcExpressionList &operator=(const gcExpressionList &);
    int Read(cReadBlock &);
};

class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcDesiredEntityTemplate {
public:
    gcDesiredEntityTemplate &operator=(const gcDesiredEntityTemplate &);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcEnumeration;
class gcEnumerationEntry;
template <class T> class cHandlePairT;
template <class T> class cSubHandleT;
template <class T> class cArrayBase {
public:
    cArrayBase<T> &operator=(const cArrayBase<T> &);
};

extern "C" {
void *dcast_gcDoEntityForEachAttached_cBase_ptr(const cBase *);
void gcAction_ctor_cBase(void *, cBase *);
void gcDesiredObject_ctor_cBase(void *, cBase *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);
void gcExpressionList_ctor_cBase(void *, cBase *);
}

extern const char gcDoEntityForEachAttached_base_name[];  // "cBase"
extern const char gcDoEntityForEachAttached_base_desc[];  // "Base"
extern const char gcDoEntityForEachAttached_text_label[]; // "ForEachAttachedEntity"

extern char gcDoEntityForEachAttached_vtable1[];
extern char gcDoEntityForEachAttached_vtable2[];
extern char gcDoEntityForEachAttached_vtable3[];
extern char gcDoEntityForEachAttached_vtable4[];
extern char gcDoEntityForEachAttached_vtable5[];
extern char gcDoEntityForEachAttached_final_vtable[];
extern char gcDoEntityForEachAttached_const_338[];

// ---------------------------------------------------------------------------
// Function 1: GetText (0x2B43A0, 36B)
// ---------------------------------------------------------------------------

void gcDoEntityForEachAttached::GetText(char *buf) const {
    gcDoEntityFindAttachedBase::GetText(buf,
                                        (char *)gcDoEntityForEachAttached_text_label,
                                        true);
}

// ---------------------------------------------------------------------------
// Function 2: AssignCopy (0x2B3E50, 164B)
// ---------------------------------------------------------------------------

void gcDoEntityForEachAttached::AssignCopy(const cBase *src) {
    gcDoEntityForEachAttached *other =
        (gcDoEntityForEachAttached *)dcast_gcDoEntityForEachAttached_cBase_ptr(src);

    int *self_i = (int *)this;
    int *other_i = (int *)other;

    int cleared = self_i[2] & ~3;
    self_i[2] = cleared;
    self_i[2] = cleared | (other_i[2] & 3);

    self_i[3] = other_i[3];

    ((gcDesiredEntity *)((char *)this + 0x10))
        ->operator=(*(const gcDesiredEntity *)((char *)other + 0x10));

    self_i[15] = other_i[15];
    self_i[16] = other_i[16];
    self_i[17] = other_i[17];

    ((gcDesiredEntityTemplate *)((char *)this + 0x48))
        ->operator=(*(const gcDesiredEntityTemplate *)((char *)other + 0x48));

    ((cArrayBase<cHandlePairT<cSubHandleT<gcEnumerationEntry> > > *)((char *)this + 0x60))
        ->operator=(*(const cArrayBase<cHandlePairT<cSubHandleT<gcEnumerationEntry> > > *)((char *)other + 0x60));

    ((gcExpressionList *)((char *)this + 0x64))
        ->operator=(*(const gcExpressionList *)((char *)other + 0x64));
}

// ---------------------------------------------------------------------------
// Function 3: Read — split out to gcDoEntityForEachAttached_Read.cpp
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Function 4: GetType (0x2B4050, 340B)
// ---------------------------------------------------------------------------

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityFindAttachedBase;
static cType *type_gcDoEntityForEachAttached;

const cType *gcDoEntityForEachAttached::GetType(void) const {
    if (!type_gcDoEntityForEachAttached) {
        if (!type_gcDoEntityFindAttachedBase) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoEntityForEachAttached_base_name,
                            gcDoEntityForEachAttached_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoEntityFindAttachedBase = cType::InitializeType(
                0, 0, 0x157, type_action, 0, 0, 0, 0);
        }
        type_gcDoEntityForEachAttached = cType::InitializeType(
            0, 0, 0x15A, type_gcDoEntityFindAttachedBase,
            gcDoEntityForEachAttached::New, 0, 0, 0);
    }
    return type_gcDoEntityForEachAttached;
}

// ---------------------------------------------------------------------------
// Function 5: New (0x2B3EF4, 348B)
// ---------------------------------------------------------------------------

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

cBase *gcDoEntityForEachAttached::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntityForEachAttached *result = 0;
    gcDoEntityForEachAttached *obj =
        (gcDoEntityForEachAttached *)entry->fn(base, 0x6C, 4, 0, 0);
    if (obj != 0) {
        gcAction_ctor_cBase(obj, parent);

        int *obj_i = (int *)obj;
        obj_i[1] = (int)gcDoEntityForEachAttached_vtable1;

        void *desobj = (char *)obj + 0x10;
        obj_i[3] = 0;
        gcDesiredObject_ctor_cBase(desobj, (cBase *)obj);

        obj_i[5] = (int)gcDoEntityForEachAttached_const_338;

        gcDesiredEntityHelper_ctor((char *)obj + 0x1C, 1, 0, 0);

        obj_i[5] = (int)gcDoEntityForEachAttached_vtable2;
        obj_i[10] = (int)gcDoEntityForEachAttached_vtable3;
        obj_i[9] = (int)desobj;
        obj_i[10] = (int)gcDoEntityForEachAttached_vtable4;
        ((char *)obj)[0x2C] = 1;
        ((char *)obj)[0x2D] = 0;
        obj_i[12] = 0;
        obj_i[13] = 0;
        obj_i[14] = (int)desobj | 1;
        obj_i[15] = 3;
        obj_i[16] = 2;
        obj_i[17] = 0;

        gcDesiredObject_ctor_cBase((char *)obj + 0x48, (cBase *)obj);

        obj_i[19] = (int)gcDoEntityForEachAttached_vtable5;
        obj_i[21] = 7;
        obj_i[22] = 0;
        obj_i[23] = (int)obj | 1;
        obj_i[24] = 0;
        obj_i[1] = (int)gcDoEntityForEachAttached_final_vtable;

        gcExpressionList_ctor_cBase((char *)obj + 0x64, (cBase *)obj);

        result = obj;
    }
    return (cBase *)result;
}
