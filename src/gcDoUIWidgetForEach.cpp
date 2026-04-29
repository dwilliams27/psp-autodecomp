#include "gcDoUIFade.h"
#include "cBase.h"

class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

struct gcDesiredUIWidgetHelper {
    int _a;
    int _b;
    int _c;
    void Write(cWriteBlock &) const;
    void GetText(char *) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class gcExpressionList {
public:
    void Write(cWriteBlock &) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    gcExpressionList &operator=(const gcExpressionList &);
    ~gcExpressionList(void);
};

void gcAction_gcAction(gcDoUIWidgetForEach *, cBase *);
void gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper(void *, int);
void gcExpressionList_gcExpressionList(void *, cBase *);
void cStrAppend(char *, const char *, ...);
void gcAction_Write(const gcDoUIWidgetForEach *, cFile &);
gcDoUIWidgetForEach *dcast(const cBase *);
void *cMemPool_GetPoolFromPtr(const void *);

extern "C" {
    void gcAction___dtor_gcAction_void(void *, int);
}

extern char gcDoUIWidgetForEachvirtualtable[];
extern const char gcDoUIWidgetForEach_fmt[];

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// 0x00316388, 160B
cBase *gcDoUIWidgetForEach::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoUIWidgetForEach *result = 0;
    gcDoUIWidgetForEach *obj =
        (gcDoUIWidgetForEach *)rec->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        *(void **)((char *)obj + 4) = gcDoUIWidgetForEachvirtualtable;
        gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper((char *)obj + 0xC, 1);
        gcExpressionList_gcExpressionList((char *)obj + 0x18, (cBase *)obj);
        result = obj;
    }
    return (cBase *)result;
}

// 0x00316b6c, 8B — already matched
int gcDoUIWidgetForEach::GetMaxBranches(void) const {
    return 1;
}

// 0x00316b74, 8B — already matched
gcExpression *gcDoUIWidgetForEach::GetBranch(int) const {
    return branch;
}

// 0x00316b7c, 8B — already matched
void gcDoUIWidgetForEach::SetBranch(int, gcExpression *expr) {
    branch = expr;
}

// 0x00316b24, 72B
void gcDoUIWidgetForEach::GetText(char *buf) const {
    char local[256];
    local[0] = *local = '\0';
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->GetText(local);
    cStrAppend(buf, gcDoUIWidgetForEach_fmt, local);
}

// 0x00316540, 100B
void gcDoUIWidgetForEach::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->Write(wb);
    ((gcExpressionList *)((char *)this + 0x18))->Write(wb);
    wb.End();
}

// 0x0031630c, 124B
void gcDoUIWidgetForEach::AssignCopy(const cBase *other) {
    gcDoUIWidgetForEach *src = dcast(other);
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
    ((gcExpressionList *)((char *)this + 0x18))
        ->operator=(*(const gcExpressionList *)((char *)src + 0x18));
}

// 0x00316b84, 156B
void gcDoUIWidgetForEach::VisitReferences(unsigned int flags, cBase *ctx, void (*cb)(cBase *, unsigned int, void *), void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->VisitReferences(flags, (cBase *)this, cb, user, mask);
    ((gcExpressionList *)((char *)this + 0x18))->VisitReferences(flags, (cBase *)this, cb, user, mask);
}

// 0x00316c20, 136B
extern "C" {

void gcDoUIWidgetForEach___dtor_gcDoUIWidgetForEach_void(gcDoUIWidgetForEach *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = gcDoUIWidgetForEachvirtualtable;
        ((gcExpressionList *)((char *)self + 0x18))->~gcExpressionList();
        gcAction___dtor_gcAction_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

}

int gcDoWhile::GetMaxChildren(void) const {
    return 1;
}

int gcDoWhile::GetMaxBranches(void) const {
    return 1;
}

gcExpression *gcDoWhile::GetBranch(int) const {
    return branch;
}
