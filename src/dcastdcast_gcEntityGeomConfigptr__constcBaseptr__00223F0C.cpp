// ── dcast<gcEntityGeomConfig *>(const cBase *) @ 0x00223f0c ──
// RTTI downcast: register gcEntityGeomConfig's cType lazily, then walk the
// object's dynamic type chain looking for the registered type.
// gcAll base type D_000385DC @ 0x385DC, name strings 0x36D894/0x36D89C.
// gcEntityGeomConfig type ordinal 0xA0, derived-type global @ 0x9F43C,
// allocator &gcEntityGeomConfig::New @ 0x267384.
// Mirrors the matched 2-level gcViewport dcast (00221AE0); constants from this
// fn's own disassembly + extracted symbols.

class cBase;
class cMemPool;
class cType;
class gcEntityGeomConfig;

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
    cBase *__0fSgcEntityGeomConfigDNewP6IcMemPoolP6FcBaseT(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_0009F43C;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009F43C == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F43C = cType::InitializeType(
                0, 0, 0xA0, D_000385DC,
                (cBase * (*)(cMemPool *, cBase *))&__0fSgcEntityGeomConfigDNewP6IcMemPoolP6FcBaseT,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F43C;
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

template gcEntityGeomConfig *dcast<gcEntityGeomConfig *>(const cBase *base);
