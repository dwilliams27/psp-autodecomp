// ── dcast<gcConstantGroup *>(const cBase *) @ 0x0021eadc ──
// RTTI downcast: register gcConstantGroup's cType lazily through a 3-level
// type chain (gcAll base 0x385DC -> intermediate 0x40C94 -> gcConstantGroup
// 0x998AC), then walk the object's dynamic type chain looking for the type.
// gcAll base type D_000385DC, name strings 0x36D894/0x36D89C, ordinal 1.
// Intermediate type D_00040C94 ordinal 4 (parent base).
// gcConstantGroup type D_000998AC ordinal 0x7C (parent intermediate),
// allocator @ 0x236454, trailing arg 8.
// Mirrors the matched gcViewport dcast (00221AE0), 3-level cascade.

class cBase;
class cMemPool;
class cType;
class gcConstantGroup;

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
    cBase *__0fOgcConstantGroupDNewP6IcMemPoolP6FcBaseT(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_000998AC;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_000998AC == 0) {
            if (D_00040C94 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000998AC = cType::InitializeType(
                0, 0, 0x7C, D_00040C94,
                (cBase * (*)(cMemPool *, cBase *))&__0fOgcConstantGroupDNewP6IcMemPoolP6FcBaseT,
                0, 0, 8);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_000998AC;
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

template gcConstantGroup *dcast<gcConstantGroup *>(const cBase *base);
