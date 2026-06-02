// ── dcast<gcRegionSet *>(const cBase *) @ 0x002215b4 ──
// RTTI downcast: register gcRegionSet's cType lazily, then walk the object's
// dynamic type chain looking for the registered type.
// gcAll base type D_000385DC @ 0x385DC, name strings 0x36D894/0x36D89C.
// gcRegionSet type ordinal 0xFE, derived-type global @ 0x99AE8,
// allocator &gcRegionSet::New @ 0x245C5C.
// Mirrors the matched 2-level gcViewport dcast (00221AE0); constants from
// this fn's own disassembly + extracted symbols.

class cBase;
class cMemPool;
class cType;
class gcRegionSet;

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
    cBase *__0fLgcRegionSetDNewP6IcMemPoolP6FcBaseT(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_00099AE8;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_00099AE8 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00099AE8 = cType::InitializeType(
                0, 0, 0xFE, D_000385DC,
                (cBase * (*)(cMemPool *, cBase *))&__0fLgcRegionSetDNewP6IcMemPoolP6FcBaseT,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00099AE8;
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

template gcRegionSet *dcast<gcRegionSet *>(const cBase *base);
