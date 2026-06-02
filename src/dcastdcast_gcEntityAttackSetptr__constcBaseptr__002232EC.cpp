// ── dcast<gcEntityAttackSet *>(const cBase *) @ 0x002232ec ──
// RTTI downcast (gcAll_psp.obj) with a 3-level lazy cType::InitializeType cascade:
//   inner base  D_000385DC (name strings 0x36D894/0x36D89C, ordinal 1)
//   middle      D_000998F4 (ordinal 0xad, parent = inner base)
//   outer       D_0009A3FC (ordinal 0x147, parent = middle,
//                           allocator &gcEntityAttackSet::New @ 0x25C6EC,
//                           8th InitializeType arg = 0)
// Then walk the object's dynamic type chain (t = t->mParent) with the
// beqzl/bnel ||-form null guards. Mirrors matched 3-level dcast<eMaterialSetGroup*>.

class cBase;
class cMemPool;
class cType;
class gcEntityAttackSet;

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
    cBase *gcEntityAttackSet__New_cMemPoolptr_cBaseptrstatic__0025C6EC(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_000998F4;
extern cType *D_0009A3FC;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009A3FC == 0) {
            if (D_000998F4 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000998F4 = cType::InitializeType(0, 0, 0xad, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009A3FC = cType::InitializeType(
                0, 0, 0x147, D_000998F4,
                (cBase * (*)(cMemPool *, cBase *))&gcEntityAttackSet__New_cMemPoolptr_cBaseptrstatic__0025C6EC,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009A3FC;
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

template gcEntityAttackSet *dcast<gcEntityAttackSet *>(const cBase *base);
