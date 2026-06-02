// ── dcast<eBipedControllerTemplate *>(const cBase *) @ 0x001cf0e4 ──
// RTTI downcast: register eBipedControllerTemplate's cType lazily via a
// three-level InitializeType cascade (outer derived D_00046BB0 ordinal 0x237 ->
// middle D_000469F4 ordinal 0x22F -> inner base D_000385DC ordinal 1, name
// strings 0x36CD74/0x36CD7C), then walk the object's dynamic type chain looking
// for the registered type. Mirrors the matched dcast<eRoomGroup*> shape.
// The outer (0x237) InitializeType call passes &eBipedControllerTemplate::New as
// the 5th arg; all other slots (incl. the 8th) are zero on every level.

class cBase;
class cMemPool;
class cType;
class eBipedControllerTemplate;

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
    cBase *eBipedControllerTemplate__New_cMemPoolptr_cBaseptrstatic__00208638(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_000469F4;
extern cType *D_00046BB0;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_00046BB0 == 0) {
            if (D_000469F4 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000469F4 = cType::InitializeType(0, 0, 0x22f, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_00046BB0 = cType::InitializeType(
                0, 0, 0x237, D_000469F4,
                (cBase * (*)(cMemPool *, cBase *))
                    &eBipedControllerTemplate__New_cMemPoolptr_cBaseptrstatic__00208638,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00046BB0;
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

template eBipedControllerTemplate *dcast<eBipedControllerTemplate *>(const cBase *base);
