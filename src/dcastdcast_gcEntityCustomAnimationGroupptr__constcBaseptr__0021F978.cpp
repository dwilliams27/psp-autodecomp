// dcast<gcEntityCustomAnimationGroup *>(const cBase *) @ 0x0021f978
// RTTI downcast: lazy cType registration cascade (3-tier hierarchy) + dynamic-type
// chain walk. Mirrors the matched gcEntityTemplateGroup/gcViewport dcast pattern; the
// gcAll-specific cache slots, type ordinals and &New read from this fn's own disasm:
//   leaf slot D_000998D8 (lui 0xa, -26408), ordinal 0x197, 8th arg 0x8, &New 0x237ECC
//   mid slot  D_00040C94 (lui 0x4,  3220),  ordinal 0x4
//   base slot D_000385DC (lui 0x4, -31268), ordinal 0x1, strings 0x36D894/0x36D89C

class cBase;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcEntityCustomAnimationGroup {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
};

extern cType *D_000385DC;   // base type cache  (lui 0x4, -31268)
extern cType *D_00040C94;   // mid type cache   (lui 0x4,  3220)
extern cType *D_000998D8;   // leaf type cache  (lui 0xa, -26408)

template <class T> T dcast(const cBase *);

template <class T> T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_000998D8 == 0) {
            if (D_00040C94 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040C94 = cType::InitializeType(0, 0, 0x4, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000998D8 = cType::InitializeType(0, 0, 0x197, D_00040C94,
                                               &gcEntityCustomAnimationGroup::New,
                                               0, 0, 0x8);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_000998D8;
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
                type = (cType *)*(void **)((char *)type + 0x1C);
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

template gcEntityCustomAnimationGroup *dcast<gcEntityCustomAnimationGroup *>(const cBase *);
