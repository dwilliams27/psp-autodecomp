// ── dcast<gcEnumerationGroup *>(const cBase *) @ 0x0021f2d4 ──
// RTTI downcast: register gcEnumerationGroup's cType lazily via a three-level
// InitializeType cascade (outer derived 0x998C4 ordinal 0xAC -> middle 0x40C94
// ordinal 4 -> inner base D_000385DC ordinal 1, name strings 0x36D894/0x36D89C),
// then walk the object's dynamic type chain looking for the registered type.
// Mirrors the matched three-level dcast<eAnimationGroup*> shape @ 0x001c8d6c
// with this function's own constants. The outer (0xAC) InitializeType call
// passes &gcEnumerationGroup::New @ 0x2372c4 as the 5th arg and 8 as the 8th
// arg; the middle/inner calls pass zeros for those slots.

class cBase;
class cMemPool;
class cType;
class gcEnumerationGroup;

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
    cBase *gcEnumerationGroup__New_cMemPoolptr_cBaseptrstatic__002372C4(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_000998C4;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_000998C4 == 0) {
            if (D_00040C94 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000998C4 = cType::InitializeType(
                0, 0, 0xAC, D_00040C94,
                (cBase * (*)(cMemPool *, cBase *))
                    &gcEnumerationGroup__New_cMemPoolptr_cBaseptrstatic__002372C4,
                0, 0, 8);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_000998C4;
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

template gcEnumerationGroup *dcast<gcEnumerationGroup *>(const cBase *base);
