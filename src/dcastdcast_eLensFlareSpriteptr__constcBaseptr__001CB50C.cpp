// ── dcast<eLensFlareSprite *>(const cBase *) @ 0x001cb50c ──
// RTTI downcast: register eLensFlareSprite's cType lazily, then walk the
// object's dynamic type chain looking for the registered type.
// Mirrors the matched dcast<eSound *> block (shared eAll base type
// D_000385DC @ 0x385DC, name strings 0x36CD74/0x36CD7C). eLensFlareSprite
// type ordinal 0x19A, derived-type global @ 0x468C8, allocator
// &eLensFlareSprite::New @ 0x1E83D8.

class cBase;
class cMemPool;
class cType;
class eLensFlareSprite;

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
    cBase *eLensFlareSprite__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_000468C8;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_000468C8 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000468C8 = cType::InitializeType(
                0, 0, 0x19A, D_000385DC,
                (cBase * (*)(cMemPool *, cBase *))&eLensFlareSprite__New_cMemPoolptr_cBaseptr,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_000468C8;
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

template eLensFlareSprite *dcast<eLensFlareSprite *>(const cBase *base);
