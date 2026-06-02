// ── dcast<gcFloatSet *>(const cBase *) @ 0x00221bf8 ──
// RTTI downcast (gcAll_psp.obj) with a 3-level lazy cType::InitializeType
// cascade:
//   inner base  D_000385DC (name strings 0x36D894/0x36D89C, ordinal 1)
//   middle      D_000998F4 (ordinal 0xAD, parent = inner base)
//   outer       D_0009A2EC (ordinal 0xAE, parent = middle,
//                           allocator &gcFloatSet::New @ 0x249370,
//                           8th InitializeType arg = 0)
// Then walk the object's dynamic type chain (t = t->mParent) with ||-form
// null guards reproducing beqzl/bnel. Mirrors matched dcast<eMaterialSetGroup*>
// (340B, 3-level); same shape, different cascade slots/ordinals/New.

class cBase;
class cMemPool;
class cType;
class gcFloatSet;

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
    cBase *gcFloatSet__New_cMemPoolptr_cBaseptrstatic__00249370(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_000998F4;
extern cType *D_0009A2EC;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009A2EC == 0) {
            if (D_000998F4 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000998F4 = cType::InitializeType(0, 0, 0xAD, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009A2EC = cType::InitializeType(
                0, 0, 0xAE, D_000998F4,
                (cBase * (*)(cMemPool *, cBase *))&gcFloatSet__New_cMemPoolptr_cBaseptrstatic__00249370,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009A2EC;
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

template gcFloatSet *dcast<gcFloatSet *>(const cBase *base);
