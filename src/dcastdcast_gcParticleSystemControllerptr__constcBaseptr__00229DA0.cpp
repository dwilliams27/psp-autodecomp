// ── dcast<gcParticleSystemController *>(const cBase *) @ 0x00229da0 ──
// RTTI downcast (gcAll_psp.obj) with a 3-level lazy cType::InitializeType
// cascade (verified from this fn's own expected/build/func/00229da0.o):
//   inner base  D_000385DC (name strings 0x36D894/0x36D89C, ordinal 1)
//   middle      D_0009F64C (ordinal 0x1D5, parent = inner base, New = 0)
//   outer       D_0009F66C (ordinal 0x1F8, parent = middle,
//                           allocator &gcParticleSystemController::New @ 0x2D5C9C)
// Then walk the object's dynamic type chain (t = t->mParent) with ||-form
// null guards reproducing beqzl/bnel. 340B, 3-level shape matching
// dcast<gcTableColumnShort *>; same shape, different cascade slots/ordinals/New.

class cBase;
class cMemPool;
class cType;
class gcParticleSystemController;

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
    cBase *__0fagcParticleSystemControllerDNewP6IcMemPoolP6FcBaseT(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_0009F64C;
extern cType *D_0009F66C;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009F66C == 0) {
            if (D_0009F64C == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F64C = cType::InitializeType(0, 0, 0x1D5, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F66C = cType::InitializeType(
                0, 0, 0x1F8, D_0009F64C,
                (cBase * (*)(cMemPool *, cBase *))&__0fagcParticleSystemControllerDNewP6IcMemPoolP6FcBaseT,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F66C;
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

template gcParticleSystemController *dcast<gcParticleSystemController *>(const cBase *base);
