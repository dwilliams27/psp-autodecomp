// ── dcast<cNamed *>(const cBase *) @ 0x001c5c7c ──
// RTTI downcast (cAll_psp.obj): lazily register cNamed's cType via a 2-level
// InitializeType cascade (inner shared type D_000385DC + outer D_000385E0),
// then walk the object's dynamic type chain for the registered type.
// Mirrors the matched 280B dcast<gcViewport *> split-if/goto parent-walk shape
// (src/dcastdcast_gcViewportptr__constcBaseptr__00221AE0.cpp), which reproduces
// the target's beqzl/bnel branch-likely form.
//   inner D_000385DC : InitializeType(0x36C830, 0x36C838, 1, 0, 0, ...)
//   outer D_000385E0 : InitializeType(0, 0, 2, D_000385DC,
//                                     &cNamed::New, 0, 0, 0)
//   cNamed::New @ 0x1c3c58 (__0fGcNamedDNewP6IcMemPoolP6FcBaseT)

class cBase;
class cMemPool;
class cType;
class cNamed;

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
    cBase *__0fGcNamedDNewP6IcMemPoolP6FcBaseT(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_000385E0;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_000385E0 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36C830,
                                                   (const char *)0x36C838,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000385E0 = cType::InitializeType(
                0, 0, 2, D_000385DC,
                (cBase * (*)(cMemPool *, cBase *))&__0fGcNamedDNewP6IcMemPoolP6FcBaseT,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_000385E0;
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

template cNamed *dcast<cNamed *>(const cBase *base);
