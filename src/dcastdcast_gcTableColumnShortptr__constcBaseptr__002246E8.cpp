// ── dcast<gcTableColumnShort *>(const cBase *) @ 0x002246e8 ──
// RTTI downcast (gcAll_psp.obj) with a 3-level lazy cType::InitializeType
// cascade (verified from this fn's own expected/build/func/002246e8.o):
//   inner base  D_000385DC (name strings 0x36D894/0x36D89C, ordinal 1)
//   middle      D_0009F478 (ordinal 0x241, parent = inner base, New = 0)
//   outer       D_0009F480 (ordinal 0x243, parent = middle,
//                           allocator &gcTableColumnShort::New @ 0x271538)
// Then walk the object's dynamic type chain (t = t->mParent) with ||-form
// null guards reproducing beqzl/bnel. 340B, 3-level shape matching
// dcast<gcFloatSet *>; same shape, different cascade slots/ordinals/New.

class cBase;
class cMemPool;
class cType;
class gcTableColumnShort;

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
};

extern "C" {
    cBase *gcTableColumnShort__New_cMemPoolptr_cBaseptrstatic__00271538(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_0009F478;
extern cType *D_0009F480;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009F480 == 0) {
            if (D_0009F478 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F478 = cType::InitializeType(0, 0, 0x241, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F480 = cType::InitializeType(
                0, 0, 0x243, D_0009F478,
                (cBase * (*)(cMemPool *, cBase *))&gcTableColumnShort__New_cMemPoolptr_cBaseptrstatic__00271538,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F480;
        DispatchEntry *entry = (DispatchEntry *)((char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(void *, short, void *) = entry->fn;
        cType *type = fn((char *)base + offset, offset, (void *)fn);
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
            result = base;
        }
    }

    return (T)result;
}

template gcTableColumnShort *dcast<gcTableColumnShort *>(const cBase *base);
