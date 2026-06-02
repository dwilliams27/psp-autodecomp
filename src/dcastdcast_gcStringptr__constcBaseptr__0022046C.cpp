// ── dcast<gcString *>(const cBase *) @ 0x0022046c ──
// RTTI downcast: register gcString's cType lazily via a two-level
// InitializeType cascade (outer derived D_00099900 ordinal 0x87 -> inner base
// D_000385DC ordinal 1, name strings 0x36D894/0x36D89C), then walk the object's
// dynamic type chain looking for the registered type. Mirrors the matched
// dcast<eFontGroup*> shape but with one fewer cascade level. The outer (0x87)
// InitializeType call passes &gcString::New as the 5th arg and 0 as the 8th.

class cBase;
class cMemPool;
class cType;
class gcString;

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
    cBase *__0fIgcStringDNewP6IcMemPoolP6FcBaseT(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_00099900;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_00099900 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00099900 = cType::InitializeType(
                0, 0, 0x87, D_000385DC,
                (cBase * (*)(cMemPool *, cBase *))
                    &__0fIgcStringDNewP6IcMemPoolP6FcBaseT,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00099900;
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

template gcString *dcast<gcString *>(const cBase *base);
