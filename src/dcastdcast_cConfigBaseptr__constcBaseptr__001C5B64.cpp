// ── dcast<cConfigBase *>(const cBase *) @ 0x001c5b64 ──
// RTTI downcast (cAll_psp.obj): lazily register cConfigBase's cType via a
// 2-level InitializeType cascade (inner shared type D_000385DC + outer
// D_00038888), then walk the object's dynamic type chain for the registered
// type. Structurally identical to the matched cNamed dcast (001C5C7C); only the
// registered type differs: outer slot D_00038888, ordinal 0x28A, allocator
// &cConfigBase::New (@ 0x1c6848, __0fLcConfigBaseDNewP6IcMemPoolP6FcBaseT).
// Inner base type strings 0x36C830/0x36C838. Verified vs expected/bin/001c5b64.bin.

class cBase;
class cMemPool;
class cType;
class cConfigBase;

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
    cBase *__0fLcConfigBaseDNewP6IcMemPoolP6FcBaseT(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_00038888;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_00038888 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36C830,
                                                   (const char *)0x36C838,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00038888 = cType::InitializeType(
                0, 0, 0x28A, D_000385DC,
                (cBase * (*)(cMemPool *, cBase *))&__0fLcConfigBaseDNewP6IcMemPoolP6FcBaseT,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00038888;
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

template cConfigBase *dcast<cConfigBase *>(const cBase *base);
