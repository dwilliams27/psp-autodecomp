// ── dcast<gcRigidBodyControllerTemplate *>(const cBase *) @ 0x0022c40c ──
// RTTI downcast (340B = 3-level InitializeType cascade). Lazily registers
// gcRigidBodyControllerTemplate's cType via:
//   inner base  D_000385DC ordinal 1     name strings 0x36D894 / 0x36D89C
//   level 1     D_0009A400 ordinal 0x9a  parent D_000385DC
//   outer       D_0009F7B0 ordinal 0x142 parent D_0009A400, 5th arg &New@0x31F280
// then walks the object's dynamic type chain looking for the registered type.
// Mirrors the 340B dcast siblings.

class cBase;
class cMemPool;
class cType;
class gcRigidBodyControllerTemplate;

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
    cBase *gcRigidBodyControllerTemplate__New_cMemPoolptr_cBaseptrstatic__0031F280(cMemPool *, cBase *);
}

extern cType *D_0009F7B0;
extern cType *D_0009A400;
extern cType *D_000385DC;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009F7B0 == 0) {
            if (D_0009A400 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009A400 = cType::InitializeType(0, 0, 0x9a, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F7B0 = cType::InitializeType(
                0, 0, 0x142, D_0009A400,
                (cBase * (*)(cMemPool *, cBase *))
                    &gcRigidBodyControllerTemplate__New_cMemPoolptr_cBaseptrstatic__0031F280,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F7B0;
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

template gcRigidBodyControllerTemplate *dcast<gcRigidBodyControllerTemplate *>(const cBase *base);
