// ── dcast<gcEnumerationEntry *>(const cBase *) @ 0x0022001c ──
// RTTI downcast: register gcEnumerationEntry's cType lazily via a three-level
// InitializeType cascade (outer derived 0x998EC ordinal 0xAA -> middle 0x385E0
// ordinal 2 -> inner base D_000385DC ordinal 1, name strings 0x36D894/0x36D89C),
// then walk the object's dynamic type chain looking for the registered type.
// Mirrors the matched three-level dcast<gcEnumerationGroup*> shape @ 0x0021f2d4
// with this function's own constants. The outer (0xAA) InitializeType call
// passes &gcEnumerationEntry::New @ 0x238B14 as the 5th arg; the middle/cNamed
// call passes &cNamed::New @ 0x1C3C58 as its 5th arg; all three calls pass 0
// for the 8th arg, the inner call also zeros the 5th.

class cBase;
class cMemPool;
class cType;
class gcEnumerationEntry;

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
    cBase *gcEnumerationEntry__New_cMemPoolptr_cBaseptrstatic__00238B14(cMemPool *, cBase *);
    cBase *cNamed__New_cMemPoolptr_cBaseptrstatic__001C3C58(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000998EC;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_000998EC == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC,
                    (cBase * (*)(cMemPool *, cBase *))
                        &cNamed__New_cMemPoolptr_cBaseptrstatic__001C3C58,
                    0, 0, 0);
            }
            D_000998EC = cType::InitializeType(
                0, 0, 0xAA, D_000385E0,
                (cBase * (*)(cMemPool *, cBase *))
                    &gcEnumerationEntry__New_cMemPoolptr_cBaseptrstatic__00238B14,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_000998EC;
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

template gcEnumerationEntry *dcast<gcEnumerationEntry *>(const cBase *base);
