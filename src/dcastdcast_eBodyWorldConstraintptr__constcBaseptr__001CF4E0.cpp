// ── dcast<eBodyWorldConstraint *>(const cBase *) @ 0x001cf4e0 ──
// RTTI downcast: lazily register eBodyWorldConstraint's cType (a 3-level
// InitializeType cascade), then walk the object's dynamic type chain looking
// for the registered type. Mirrors the matched 340B dcast<eTextureGroup *>
// shape (3-level registration cascade):
//   inner   D_000385DC : InitializeType(0x36CD74,0x36CD7C,1, 0, 0, ...)
//   middle  D_00046BC4 : InitializeType(0,0, 0x25E, D_000385DC, 0, ...)
//   outer   D_00046BC8 : InitializeType(0,0,0x25F, D_00046BC4,
//                                        &eBodyWorldConstraint::New, 0,0, 0)
// eBodyWorldConstraint::New @ 0x20998C. Shared eAll base strings 0x36CD74/0x36CD7C.

class cBase;
class cMemPool;
class cType;
class eBodyWorldConstraint;

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
    cBase *eBodyWorldConstraint__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_00046BC4;
extern cType *D_00046BC8;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_00046BC8 == 0) {
            if (D_00046BC4 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00046BC4 = cType::InitializeType(0, 0, 0x25E, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_00046BC8 = cType::InitializeType(
                0, 0, 0x25F, D_00046BC4,
                (cBase * (*)(cMemPool *, cBase *))&eBodyWorldConstraint__New_cMemPoolptr_cBaseptr,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00046BC8;
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

template eBodyWorldConstraint *dcast<eBodyWorldConstraint *>(const cBase *base);
