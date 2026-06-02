// ── dcast<eKeyframedController *>(const cBase *) @ 0x001cfbf8 ──
// RTTI downcast: register eKeyframedController's cType lazily via a three-level
// InitializeType cascade (outer derived D_00046BEC ordinal 0x23A -> middle
// D_000469D8 ordinal 0x232 -> inner base D_000385DC ordinal 1, name strings
// 0x37CD74/0x37CD7C), then walk the object's dynamic type chain looking for the
// registered type. Strings live at 0x36CD74/0x36CD7C. Mirrors the matched
// dcast<eAnimationGroup*>/dcast<eSound*> shape. The outer (0x23A) InitializeType
// call passes &eKeyframedController::New as the 5th arg; the middle/inner calls
// pass zeros for those slots.

class cBase;
class cMemPool;
class cType;
class eKeyframedController;

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
    cBase *eKeyframedController__New_cMemPoolptr_cBaseptrstatic__0020DE5C(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_000469D8;
extern cType *D_00046BEC;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_00046BEC == 0) {
            if (D_000469D8 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000469D8 = cType::InitializeType(0, 0, 0x232, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_00046BEC = cType::InitializeType(
                0, 0, 0x23A, D_000469D8,
                (cBase * (*)(cMemPool *, cBase *))
                    &eKeyframedController__New_cMemPoolptr_cBaseptrstatic__0020DE5C,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00046BEC;
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

template eKeyframedController *dcast<eKeyframedController *>(const cBase *base);
