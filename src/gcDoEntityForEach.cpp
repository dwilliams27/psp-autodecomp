#include "gcDoEntityForEach.h"
#include "cBase.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    cFile *_file;
    int _pos;

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredEntityTemplate {
public:
    gcDesiredEntityTemplate &operator=(const gcDesiredEntityTemplate &);
};

class gcExpressionList {
public:
    gcExpressionList &operator=(const gcExpressionList &);
    void Write(cWriteBlock &) const;
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

struct WriteRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *);
};

void gcAction_gcAction(void *, cBase *);
void gcAction_Write(const gcDoEntityForEach *, cFile &);
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

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoEntityForEach asm("D_0009F618");

gcExpression *gcDoEntityForEach::GetBranch(int) const {
    return branch;
}

void gcDoEntityForEach::SetBranch(int, gcExpression *expr) {
    branch = expr;
}

const cType *gcDoEntityForEach::GetType(void) const {
    if (!type_gcDoEntityForEach) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A,
                                                        type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoEntityForEach = cType::InitializeType(
            0, 0, 0x136, type_action, gcDoEntityForEach::New, 0, 0, 0);
    }
    return type_gcDoEntityForEach;
}

void gcDoEntityForEach::Write(cFile &file) const {
    const gcDoEntityForEach *self = this;
    cFile &out = file;

    cWriteBlock wb(out, 4);
    gcAction_Write(self, out);
    wb.Write(*(const int *)((const char *)self + 0x0C));
    wb.Write(*(const int *)((const char *)self + 0x10));

    {
        char *typeInfo = *(char **)((const char *)self + 0x18);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)self + 0x14;
        rec->fn(base + rec->offset, wb._file);
    }

    ((const gcExpressionList *)((const char *)self + 0x2C))->Write(wb);

    void *data = *(void **)((const char *)self + 0x34);
    int size = 0;
    if (data != 0) {
        size = ((int *)data)[-1] & 0x3FFFFFFF;
    }
    wb.Write(size);

    data = *(void **)((const char *)self + 0x34);
    size = 0;
    if (data != 0) {
        size = ((int *)data)[-1] & 0x3FFFFFFF;
    }

    int i = 0;
    if (i < size) {
        do {
            char *entry = (char *)data + i * 8;
            ((cHandle *)entry)->Write(wb);
            wb.Write(((unsigned int *)entry)[1]);
            i++;
        } while (i < size);
    }

    wb.End();
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
