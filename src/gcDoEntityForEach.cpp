#include "gcDoEntityForEach.h"

class gcDesiredEntityTemplate {
public:
    gcDesiredEntityTemplate &operator=(const gcDesiredEntityTemplate &);
};

class gcExpressionList {
public:
    gcExpressionList &operator=(const gcExpressionList &);
};

class gcEnumeration;
class gcEnumerationEntry;

template <class T> class cSubHandleT;
template <class A, class B> class cHandlePairT;
template <class T> class cArrayBase {
public:
    cArrayBase<T> &operator=(const cArrayBase<T> &);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcDoUISetTextSprite {
public:
    static cBase *New(cMemPool *, cBase *);
};

void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, void *);
extern "C" void gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper(void *, int);
extern "C" void gcExpressionList_gcExpressionList(void *, void *);
void *dcastdcast_gcDoEntityForEachptr__constcBaseptr(const cBase *)
    __asm__("__0FFdcast7P6RgcDoEntityForEach_PC6FcBase_9BA");

extern char gcDoEntityForEachvirtualtable[];
extern char gcDoUISetTextSpritevirtualtable[];
extern char D_000006F8[];
extern char D_00389508[];
extern char D_003898A0[];

gcExpression *gcDoEntityForEach::GetBranch(int) const {
    return branch;
}

void gcDoEntityForEach::SetBranch(int, gcExpression *expr) {
    branch = expr;
}

int gcDoEntityForEachAttached::GetMaxBranches(void) const {
    return 1;
}

gcExpression *gcDoEntityForEachAttached::GetBranch(int) const {
    return branch;
}

void gcDoEntityForEachAttached::SetBranch(int, gcExpression *expr) {
    branch = expr;
}

cBase *gcDoEntityForEach::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntityForEach *result = 0;
    gcDoEntityForEach *obj =
        (gcDoEntityForEach *)entry->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoEntityForEachvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = 0;
        gcDesiredObject_gcDesiredObject((char *)obj + 0x14, obj);
        ((void **)obj)[6] = D_00389508;
        ((int *)obj)[8] = 7;
        ((int *)obj)[9] = 0;
        ((int *)obj)[10] = (int)((unsigned int)obj | 1);
        gcExpressionList_gcExpressionList((char *)obj + 0x2C, obj);
        ((int *)obj)[13] = 0;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoEntityForEach::AssignCopy(const cBase *src) {
    gcDoEntityForEach *other =
        (gcDoEntityForEach *)dcastdcast_gcDoEntityForEachptr__constcBaseptr(src);

    int *self = (int *)this;
    int *src_i = (int *)other;

    int flags = self[2] & ~3;
    self[2] = flags;
    self[2] = flags | (src_i[2] & 3);
    self[3] = src_i[3];
    int value = src_i[4];
    self[4] = value;

    ((gcDesiredEntityTemplate *)((char *)this + 0x14))
        ->operator=(*(const gcDesiredEntityTemplate *)((char *)other + 0x14));

    ((gcExpressionList *)((char *)this + 0x2C))
        ->operator=(*(const gcExpressionList *)((char *)other + 0x2C));

    ((cArrayBase<cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > > *)((char *)this + 0x34))
        ->operator=(*(const cArrayBase<cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > > *)((char *)other + 0x34));
}

cBase *gcDoUISetTextSprite::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoUISetTextSprite *result = 0;
    gcDoUISetTextSprite *obj =
        (gcDoUISetTextSprite *)entry->fn(base, 0x30, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoUISetTextSpritevirtualtable;
        gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper((char *)obj + 0x0C, 1);
        ((int *)obj)[6] = (int)((unsigned int)obj | 1);
        gcDesiredObject_gcDesiredObject((char *)obj + 0x1C, obj);
        ((void **)obj)[8] = D_000006F8;
        ((int *)obj)[10] = 7;
        ((int *)obj)[11] = 0;
        ((void **)obj)[8] = D_003898A0;
        result = obj;
    }
    return (cBase *)result;
}
