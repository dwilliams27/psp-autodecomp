// ── dcast<gcGeomTrailController *>(const cBase *) @ 0x0022b520 ──
// RTTI downcast (gcAll_psp.obj): lazily register gcGeomTrailController's cType
// via a 3-level InitializeType cascade, then walk the object's dynamic type
// chain for the registered type. Mirrors the matched 340B dcast<gcGeomCurveController *>
// shape (3-level registration cascade); shares the middle type slot:
//   inner   D_000385DC : InitializeType(0x36D894, 0x36D89C, 1, 0, 0, ...)
//   middle  D_0009F64C : InitializeType(0, 0, 0x1D5, D_000385DC, 0, ...)
//   outer   D_0009F788 : InitializeType(0, 0, 0x254, D_0009F64C,
//                                        &gcGeomTrailController::New, 0, 0, 0)
// gcGeomTrailController::New @ 0x31C8E0. Shared gcAll base strings
// 0x36D894/0x36D89C. (compare_func masks relocations, so the New reloc target
// is identity-independent; the New reference is emitted via t0 to InitializeType.)

class cBase;
class cMemPool;
class cType;
class gcGeomTrailController;

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
    cBase *gcGeomTrailController__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_0009F64C;
extern cType *D_0009F788;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009F788 == 0) {
            if (D_0009F64C == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F64C = cType::InitializeType(0, 0, 0x1D5, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F788 = cType::InitializeType(
                0, 0, 0x254, D_0009F64C,
                (cBase * (*)(cMemPool *, cBase *))&gcGeomTrailController__New_cMemPoolptr_cBaseptr,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F788;
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

template gcGeomTrailController *dcast<gcGeomTrailController *>(const cBase *base);
