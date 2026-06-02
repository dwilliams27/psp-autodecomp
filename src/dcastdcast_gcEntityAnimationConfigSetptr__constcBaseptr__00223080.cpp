// ── dcast<gcEntityAnimationConfigSet *>(const cBase *) @ 0x00223080 ──
// RTTI downcast (gcAll_psp.obj). Lazily register gcEntityAnimationConfigSet's
// cType via a 3-level InitializeType cascade, then walk the object's dynamic
// type chain for the registered type. Mirrors the MATCHED gcViewport dcast
// (00221AE0) split-if/goto form (R7 breakthrough idiom), extended to 3 cascade
// levels per this function's own asm entry-guard.
//   base D_000385DC : InitializeType(0x36D894, 0x36D89C, 1, 0, 0, ...)   ord 1
//   mid  D_000998F4 : InitializeType(0,0, 0xAD, D_000385DC, 0, ...)      ord 0xAD
//   tgt  D_0009A3EC : InitializeType(0,0, 0xB1, D_000998F4,
//                                    &gcEntityAnimationConfigSet::New, ...) ord 0xB1
// gcEntityAnimationConfigSet::New @ 0x25A908.

class cBase;
class cMemPool;
class cType;
class gcEntityAnimationConfigSet;

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
    cBase *__0fZgcEntityAnimationConfigSetDNewP6IcMemPoolP6FcBaseT(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_000998F4;
extern cType *D_0009A3EC;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009A3EC == 0) {
            if (D_000998F4 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000998F4 = cType::InitializeType(0, 0, 0xAD, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009A3EC = cType::InitializeType(
                0, 0, 0xB1, D_000998F4,
                (cBase * (*)(cMemPool *, cBase *))&__0fZgcEntityAnimationConfigSetDNewP6IcMemPoolP6FcBaseT,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009A3EC;
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

template gcEntityAnimationConfigSet *dcast<gcEntityAnimationConfigSet *>(const cBase *base);
