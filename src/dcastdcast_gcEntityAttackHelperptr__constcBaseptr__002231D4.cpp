// ── dcast<gcEntityAttackHelper *>(const cBase *) @ 0x002231d4 ──
// RTTI downcast (gcAll_psp.obj). TWO-level InitializeType cascade (not three):
// inner D_000385DC ordinal 1 (names 0x36D894/0x36D89C), outer D_009A3F4 ordinal
// 0x145 with parent = D_000385DC. Only the OUTER call passes
// &gcEntityAttackHelper::New (0x25B888) as the 5th arg (8th arg 0); the inner
// call passes 0 for &New. SNC hoists the &New hi-half (lui t0,0x26) early but
// the inner call overwrites t0 with move t0,zero.
// Then walk the object's dynamic type chain looking for the registered type.
// Mirrors the matched dcast<eAnimationGroup*> shape, reduced to two levels.

class cBase;
class cMemPool;
class cType;
class gcEntityAttackHelper;

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
    cBase *gcEntityAttackHelper__New_cMemPoolptr_cBaseptrstatic__0025B888(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_009A3F4;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_009A3F4 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType(
                    (const char *)0x36D894, (const char *)0x36D89C,
                    1, 0, 0, 0, 0, 0);
            }
            D_009A3F4 = cType::InitializeType(
                0, 0, 0x145, D_000385DC,
                (cBase * (*)(cMemPool *, cBase *))
                    &gcEntityAttackHelper__New_cMemPoolptr_cBaseptrstatic__0025B888,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_009A3F4;
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

template gcEntityAttackHelper *dcast<gcEntityAttackHelper *>(const cBase *base);
