#include "gcUIWidgetGroup.h"
#include "gcUIWidget.h"

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct cTypeMethod {
    short offset;
    short pad;
    const cType *(*fn)(const void *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_0009990C;
extern cType *D_00099920;

extern "C" {
    void gcUIWidgetGroup__gcUIWidgetGroup_cBaseptr(void *self, cBase *parent);
}

void gcUIWidgetGroup__operator_eq_constgcUIWidgetGroupref(
    gcUIWidgetGroup *, const gcUIWidgetGroup &)
    asm("__0oPgcUIWidgetGroupasRC6PgcUIWidgetGroup");

cBase *gcUIWidgetGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcUIWidgetGroup *result = 0;
    gcUIWidgetGroup *obj = (gcUIWidgetGroup *)rec->fn(base, 0xD8, 4, 0, 0);
    if (obj != 0) {
        gcUIWidgetGroup__gcUIWidgetGroup_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcUIWidgetGroup::GetType(void) const {
    if (D_00099920 == 0) {
        if (D_0009990C == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC,
                    (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
            }
            D_0009990C =
                cType::InitializeType(0, 0, 0x84, D_000385E0, 0, 0, 0, 0);
        }
        D_00099920 = cType::InitializeType(0, 0, 0x85, D_0009990C,
                                           gcUIWidgetGroup::New, 0, 0, 0);
    }
    return D_00099920;
}

void gcUIWidgetGroup::AssignCopy(const cBase *base) {
    gcUIWidgetGroup *other = 0;

    if (base != 0) {
        if (D_00099920 == 0) {
            if (D_0009990C == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC =
                            cType::InitializeType((const char *)0x36D894,
                                                  (const char *)0x36D89C,
                                                  1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(
                        0, 0, 2, D_000385DC,
                        (cBase *(*)(cMemPool *, cBase *))0x1C3C58, 0, 0, 0);
                }
                D_0009990C =
                    cType::InitializeType(0, 0, 0x84, D_000385E0, 0, 0, 0, 0);
            }
            D_00099920 = cType::InitializeType(0, 0, 0x85, D_0009990C,
                                               gcUIWidgetGroup::New, 0, 0, 0);
        }

        const cType *type = D_00099920;
        const cTypeMethod *method =
            (const cTypeMethod *)(*(char **)((const char *)base + 4) + 8);
        const cType *current = method->fn((const char *)base + method->offset);
        int match;

        if (type == 0) {
            match = 0;
        } else if (current != 0) {
loop:
            if (current == type) {
                match = 1;
            } else {
                current = current->mParent;
                if (current != 0) {
                    goto loop;
                }
                match = 0;
            }
        } else {
            match = 0;
        }
        if (match != 0) {
            other = (gcUIWidgetGroup *)base;
        }
    }
    gcUIWidgetGroup__operator_eq_constgcUIWidgetGroupref(this, *other);
}

int gcUIWidgetGroup::NeedsEffectUpdate(void) const {
    int result = 0;
    int groupEffect = ((*(unsigned int *)((char *)this + 0xB0) & 0x20000000) != 0) & 0xFF;
    if (groupEffect ||
        (((*(unsigned int *)((char *)this + 0x24) & 0x08000000) != 0) & 0xFF)) {
        result = 1;
    }
    return result & 0xFF;
}

void *gcUIWidgetGroup::GetResizableSprites(void) {
    int localSprites = ((*(unsigned int *)((char *)this + 0xB0) & 8) != 0) & 0xFF;
    if (localSprites) {
        return (char *)this + 0xC8;
    }
    return ((gcUIWidget *)this)->GetResizableSprites();
}
