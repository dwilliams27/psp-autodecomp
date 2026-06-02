// ── dcast<gcViewport *>(const cBase *) @ 0x00221ae0 ──
// RTTI downcast: register gcViewport's cType lazily, then walk the object's
// dynamic type chain looking for the registered type.
// gcAll base type D_000385DC @ 0x385DC, name strings 0x36D894/0x36D89C.
// gcViewport type ordinal 0xEE, derived-type global @ 0x9A2E8,
// allocator &gcViewport::New @ 0x2491CC.
// Mirrors the matched 2-level gcLightingConfig dcast (002219C8); constants from
// this fn's own disassembly + extracted symbols.

class cBase;
class cMemPool;
class cType;
class gcViewport;

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
    cBase *__0fKgcViewportDNewP6IcMemPoolP6FcBaseT(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_0009A2E8;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009A2E8 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009A2E8 = cType::InitializeType(
                0, 0, 0xEE, D_000385DC,
                (cBase * (*)(cMemPool *, cBase *))&__0fKgcViewportDNewP6IcMemPoolP6FcBaseT,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009A2E8;
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

template gcViewport *dcast<gcViewport *>(const cBase *base);
