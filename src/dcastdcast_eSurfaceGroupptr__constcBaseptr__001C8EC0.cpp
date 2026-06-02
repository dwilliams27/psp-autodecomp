// ── dcast<eSurfaceGroup *>(const cBase *) @ 0x001c8ec0 ──
// RTTI downcast: lazily register eSurfaceGroup's cType (a 3-level cascade —
// outer derived eSurfaceGroup type @ 0x40E38, intermediate base type @ 0x40C94,
// shared eAll base type D_000385DC @ 0x385DC), then walk the object's dynamic
// type chain (mParent @ +0x1C) looking for the registered target type.
// Mirrors the matched dcast<eSound *> shape. eSurfaceGroup type ordinal 0x3A,
// allocator &eSurfaceGroup::New @ 0x1DCB3C; intermediate base ordinal 4; the
// shared eAll base uses name strings 0x36CD74/0x36CD7C with ordinal 1.

class cBase;
class cMemPool;
class cType;
class eSurfaceGroup;

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
    cBase *eSurfaceGroup__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_00040E38;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_00040E38 == 0) {
            if (D_00040C94 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_00040E38 = cType::InitializeType(
                0, 0, 0x3A, D_00040C94,
                (cBase * (*)(cMemPool *, cBase *))&eSurfaceGroup__New_cMemPoolptr_cBaseptr,
                0, 0, 8);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00040E38;
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

template eSurfaceGroup *dcast<eSurfaceGroup *>(const cBase *base);
