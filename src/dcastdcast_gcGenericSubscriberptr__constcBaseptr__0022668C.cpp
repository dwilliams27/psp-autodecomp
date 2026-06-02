// ── dcast<gcGenericSubscriber *>(const cBase *) @ 0x0022668c ──
// RTTI downcast (gcAll_psp.obj): register gcGenericSubscriber's cType lazily
// via a three-level InitializeType cascade, then walk the object's dynamic type
// chain for the registered type. Mirrors the matched dcast<gcStaticInstanceGroup*>.
//   inner  D_000385DC : InitializeType(0x36D894, 0x36D89C, 1, 0, 0, 0,0,0)
//   middle D_00038880 : InitializeType(0, 0, 0x187, D_000385DC, 0, 0,0,0)
//   outer  D_0009F4E0 : InitializeType(0, 0, 0x203, D_00038880,
//                                      &gcGenericSubscriber::New, 0,0,0)
// gcGenericSubscriber::New @ 0x27E75C.

class cBase;
class cMemPool;
class cType;
class gcGenericSubscriber;

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
    cBase *gcGenericSubscriber__New_cMemPoolptr_cBaseptrstatic__0027E75C(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_00038880;
extern cType *D_0009F4E0;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009F4E0 == 0) {
            if (D_00038880 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00038880 = cType::InitializeType(0, 0, 0x187, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F4E0 = cType::InitializeType(
                0, 0, 0x203, D_00038880,
                (cBase * (*)(cMemPool *, cBase *))
                    &gcGenericSubscriber__New_cMemPoolptr_cBaseptrstatic__0027E75C,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F4E0;
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

template gcGenericSubscriber *dcast<gcGenericSubscriber *>(const cBase *base);
