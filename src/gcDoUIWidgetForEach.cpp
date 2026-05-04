#include "gcDoUIFade.h"
#include "cBase.h"

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class gcUIWidget;

struct gcDesiredUIWidgetHelper {
    int _a;
    int _b;
    int _c;
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
    void GetText(char *) const;
    gcUIWidget *GetWidget(const cType *, bool) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class gcExpressionList {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
    float Evaluate(void) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    gcExpressionList &operator=(const gcExpressionList &);
    ~gcExpressionList(void);
};

extern "C" float gcExpressionList_Evaluate(const void *);

class gcUIWidgetGroup {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

extern cType *D_000385E0;
extern cType *D_0009990C;
extern cType *D_00099920;
extern void *g_expressionEvalStack asm("D_0037D7B4");


void gcAction_gcAction(gcDoUIWidgetForEach *, cBase *);
void gcDesiredUIWidgetHelper_gcDesiredUIWidgetHelper(void *, int);
void gcExpressionList_gcExpressionList(void *, cBase *);
void cStrAppend(char *, const char *, ...);
void gcAction_Write(const gcDoUIWidgetForEach *, cFile &);
gcDoUIWidgetForEach *dcast(const cBase *);
void *cMemPool_GetPoolFromPtr(const void *);

extern "C" {
    void gcAction___dtor_gcAction_void(void *, int);
    void cFile_SetCurrentPos(void *file, unsigned int pos);
    void __0oKcReadBlockctR6FcFileUib(void *rb, cFile &file,
                                       unsigned int id, bool validate);
    void __0oKcReadBlockdtv(void *rb, int flags);
}

extern char gcDoUIWidgetForEachvirtualtable[];
extern const char gcDoUIWidgetForEach_fmt[];

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoUIWidgetForEach asm("D_0009F754");

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

inline void operator delete(void *p) {
    void *pool = cMemPool_GetPoolFromPtr(p);
    void *block = *(void **)((char *)pool + 0x24);
    DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
    short off = rec->offset;
    rec->fn((char *)block + off, p);
}

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

// 0x00316428, 280B
const cType *gcDoUIWidgetForEach::GetType(void) const {
    if (!type_gcDoUIWidgetForEach) {
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
        type_gcDoUIWidgetForEach = cType::InitializeType(
            0, 0, 0x1D2, type_action, gcDoUIWidgetForEach::New, 0, 0, 0);
    }
    return type_gcDoUIWidgetForEach;
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

// 0x003165a4, 212B
int gcDoUIWidgetForEach::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1 || ((gcAction *)this)->gcAction::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->Read(*(cReadBlock *)rb);
    ((gcExpressionList *)((char *)this + 0x18))->Read(*(cReadBlock *)rb);
    __0oKcReadBlockdtv(rb, 2);
    return result;
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

// 0x00316678, 1196B
float gcDoUIWidgetForEach::Evaluate(void) const {
    float result = 0.0f;

    if (D_0009990C == 0) {
        if (D_000385E0 == 0) {
            if (type_base == 0) {
                type_base = cType::InitializeType((const char *)0x36D894,
                                                  (const char *)0x36D89C,
                                                  1, 0, 0, 0, 0, 0);
            }
            D_000385E0 = cType::InitializeType(
                0, 0, 2, type_base,
                (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
        }
        D_0009990C = cType::InitializeType(0, 0, 0x84, D_000385E0,
                                            0, 0, 0, 0);
    }

    gcUIWidget *widget = ((gcDesiredUIWidgetHelper *)((char *)this + 12))
                            ->GetWidget(D_0009990C, true);
    gcUIWidget *target = 0;

    if (widget != 0) {
        if (D_00099920 == 0) {
            if (D_0009990C == 0) {
                if (D_000385E0 == 0) {
                    if (type_base == 0) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894,
                            (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(
                        0, 0, 2, type_base,
                        (cBase *(*)(cMemPool *, cBase *))0x1C3C58,
                        0, 0, 0);
                }
                D_0009990C = cType::InitializeType(0, 0, 0x84, D_000385E0,
                                                    0, 0, 0, 0);
            }
            D_00099920 = cType::InitializeType(0, 0, 0x85, D_0009990C,
                                                gcUIWidgetGroup::New,
                                                0, 0, 0);
        }

        VTableSlot *slot = (VTableSlot *)((char *)((void **)widget)[1] + 8);
        const cType *target_type = D_00099920;
        short voff = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        const cType *t = getType((char *)widget + voff);
        int matches;

        if (target_type == 0) {
            matches = 0;
            goto type_done;
        }
        if (t != 0) {
        type_loop:
            if (t == target_type) {
                matches = 1;
                goto type_done;
            }
            t = (const cType *)((const cTypeNode *)t)->parent;
            if (t != 0) {
                goto type_loop;
            }
        }
        matches = 0;
    type_done:
        if (matches != 0) {
            target = widget;
        }
    }

    if (target == 0) {
        return 0.0f;
    }

    void *outer = *(void **)((char *)target + 0xC0);
    void *inner = *(void **)((char *)target + 0xC4);
    int cap_outer = 0;
    void *stack = g_expressionEvalStack;
    if (outer != 0) {
        cap_outer = (int)(*(unsigned int *)((char *)outer - 4) & 0x3FFFFFFFU);
    }
    int cap_inner = 0;
    if (inner != 0) {
        cap_inner = (int)(*(unsigned int *)((char *)inner - 4) & 0x3FFFFFFFU);
    }

    int *exprData = 0;
    if (stack != 0) {
        exprData = ((int **)stack)[4];
    }
    int saved = exprData[15];

    short coord[2];
    short i;
    short j = 0;
    for (i = 0; i < cap_outer; i++) {
        for (j = 0; j < cap_inner; j++) {
            coord[0] = i;
            coord[1] = j;
            void **bucketArray = *(void ***)((char *)target + 0xBC);
            void *innerCheck = *(void **)((char *)target + 0xC4);
            int width = 0;
            if (innerCheck != 0) {
                width = (int)(*(unsigned int *)((char *)innerCheck - 4) & 0x3FFFFFFFU);
            }
            int idx = (int)coord[0] * width + (int)coord[1];
            void *head = bucketArray[idx];
            if (head == 0) continue;
            void *cur = head;
            do {
                exprData[15] = (int)cur;
                result = ((const gcExpressionList *)((const char *)this + 0x18))->Evaluate();
                int flags = exprData[0];
                int b1 = 0;
                if (flags & 1) b1 = 1;
                int has_any = 0;
                if (b1 != 0 || (((flags & 2) != 0) & 0xFF) != 0) {
                    has_any = 1;
                }
                int neg = ((has_any == 0) & 0xFF);
                if (neg == 0) break;
                cur = ((void **)cur)[26];
            } while (cur != head);

            int flags2 = exprData[0];
            int c1 = 0;
            if (flags2 & 1) c1 = 1;
            int has_any2 = 0;
            if (c1 != 0 || (((flags2 & 2) != 0) & 0xFF) != 0) {
                has_any2 = 1;
            }
            if (has_any2 != 0) goto done_loops;
        }
    }
done_loops:
    exprData[15] = saved;
    exprData[0] = exprData[0] & ~2;
    return result;
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
gcDoUIWidgetForEach::~gcDoUIWidgetForEach() {
    ((void **)this)[1] = gcDoUIWidgetForEachvirtualtable;
    ((gcExpressionList *)((char *)this + 0x18))->~gcExpressionList();
    gcAction___dtor_gcAction_void(this, 0);
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
