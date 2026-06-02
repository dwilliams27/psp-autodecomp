// ── dcast<gcPartialBodyControllerTemplate *>(const cBase *) @ 0x002292ec ──
// RTTI downcast (gcAll_psp.obj). Lazily register the cType via a 3-level
// InitializeType cascade, then walk the object's dynamic type chain for the
// registered type. Mirrors the matched gcViewport dcast (00221AE0) split-if /
// goto parent-walk structure, extended to a 3-level registration cascade.
// Constants read from this function's OWN 0x002292ec disasm:
//   inner  D_000385DC : InitializeType(0x36D894, 0x36D89C, 1, 0, 0, 0,0,0)
//   middle D_0009F5DC : InitializeType(0, 0, 0x104, D_000385DC, 0, 0,0,0)
//   outer  D_0009F5F0 : InitializeType(0, 0, 0x106, D_0009F5DC,
//                                       &gcPartialBodyControllerTemplate::New,
//                                       0,0,0)
// gcPartialBodyControllerTemplate::New @ 0x2A5580.

class cBase;
class cMemPool;
class cType;
class gcPartialBodyControllerTemplate;

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
    cBase *__0ffgcPartialBodyControllerTemplateDNewP6IcMemPoolP6FcBaseT(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_0009F5DC;
extern cType *D_0009F5F0;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009F5F0 == 0) {
            if (D_0009F5DC == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F5DC = cType::InitializeType(0, 0, 0x104, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F5F0 = cType::InitializeType(
                0, 0, 0x106, D_0009F5DC,
                (cBase * (*)(cMemPool *, cBase *))&__0ffgcPartialBodyControllerTemplateDNewP6IcMemPoolP6FcBaseT,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F5F0;
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

template gcPartialBodyControllerTemplate *dcast<gcPartialBodyControllerTemplate *>(const cBase *base);
