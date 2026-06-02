// ── dcast<gcGeomTrailConfig *>(const cBase *) @ 0x0022b674 ──
// RTTI downcast (gcAll_psp.obj): lazily register gcGeomTrailConfig's cType via
// a 3-level InitializeType cascade, then walk the object's dynamic type chain
// for the registered type. Mirrors the matched dcast<gcGeomTrailController *>
// shape (3-level registration cascade), but gcGeomTrailConfig derives
// gcEntityGeomConfig, so the MIDDLE slot also carries the parent New pointer:
//   inner   D_000385DC : InitializeType(0x36D894, 0x36D89C, 1, 0, 0, ...)
//   middle  D_0009F43C : InitializeType(0, 0, 0xA0, D_000385DC,
//                                        &gcEntityGeomConfig::New, 0, 0, 0)
//   outer   D_0009F78C : InitializeType(0, 0, 0x255, D_0009F43C,
//                                        &gcGeomTrailConfig::New, 0, 0, 0)
// gcEntityGeomConfig::New @ 0x267384, gcGeomTrailConfig::New @ 0x31CAD0.
// Shared gcAll base strings 0x36D894/0x36D89C. compare_func masks relocations,
// so the New/string reloc targets are identity-independent; New refs are
// emitted via t0 to InitializeType.

class cBase;
class cMemPool;
class cType;
class gcGeomTrailConfig;

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
    cBase *gcEntityGeomConfig__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
    cBase *gcGeomTrailConfig__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_0009F43C;
extern cType *D_0009F78C;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009F78C == 0) {
            if (D_0009F43C == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F43C = cType::InitializeType(
                    0, 0, 0xA0, D_000385DC,
                    (cBase * (*)(cMemPool *, cBase *))&gcEntityGeomConfig__New_cMemPoolptr_cBaseptr,
                    0, 0, 0);
            }
            D_0009F78C = cType::InitializeType(
                0, 0, 0x255, D_0009F43C,
                (cBase * (*)(cMemPool *, cBase *))&gcGeomTrailConfig__New_cMemPoolptr_cBaseptr,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F78C;
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

template gcGeomTrailConfig *dcast<gcGeomTrailConfig *>(const cBase *base);
