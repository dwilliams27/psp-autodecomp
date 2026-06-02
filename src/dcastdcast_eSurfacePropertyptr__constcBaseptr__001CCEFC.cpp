// ── dcast<eSurfaceProperty *>(const cBase *) @ 0x001ccefc ──
// RTTI downcast: register eSurfaceProperty's cType lazily via a three-level
// InitializeType cascade (outer derived D_00046A14 ordinal 0x2B7 with
// &eSurfaceProperty::New @ 0x1f5ff4 -> middle D_000385E0 ordinal 2 with
// &cNamed::New @ 0x1c3c58 -> inner base D_000385DC ordinal 1, name strings
// 0x36CD74/0x36CD7C), then walk the object's dynamic type chain looking for the
// registered type. Mirrors the matched dcast<eSurfacePropertyTableGroup*> shape.

class cBase;
class cMemPool;
class cType;
class eSurfaceProperty;

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
    cBase *cNamed__New_cMemPoolptr_cBaseptrstatic__001C3C58(cMemPool *, cBase *);
    cBase *eSurfaceProperty__New_cMemPoolptr_cBaseptrstatic__001F5FF4(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_00046A14;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_00046A14 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC,
                    (cBase * (*)(cMemPool *, cBase *))
                        &cNamed__New_cMemPoolptr_cBaseptrstatic__001C3C58,
                    0, 0, 0);
            }
            D_00046A14 = cType::InitializeType(
                0, 0, 0x2B7, D_000385E0,
                (cBase * (*)(cMemPool *, cBase *))
                    &eSurfaceProperty__New_cMemPoolptr_cBaseptrstatic__001F5FF4,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00046A14;
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

template eSurfaceProperty *dcast<eSurfaceProperty *>(const cBase *base);
