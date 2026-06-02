// ── dcast<eLightGrid *>(const cBase *) @ 0x001cd398 ──
// RTTI downcast: lazily register eLightGrid's cType (a 2-level InitializeType
// cascade), then walk the object's dynamic type chain looking for the
// registered type. Mirrors the matched dcast<eShadow *> shape but with a
// shallower (2-level) registration cascade. Per-function values read from the
// 0x001cd398 disassembly:
//   inner   D_000385DC : InitializeType(0x36CD74,0x36CD7C,1, 0, 0, ...)
//   outer   D_00046A24 : InitializeType(0,0, 0x1BA, D_000385DC,
//                                        &eLightGrid::New, 0,0, 0)
// eLightGrid::New @ 0x2020A4. Shared eAll base strings 0x36CD74/0x36CD7C.

class cBase;
class cMemPool;
class cType;

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eLightGrid {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
};

extern cType *D_000385DC;
extern cType *D_00046A24;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_00046A24 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00046A24 = cType::InitializeType(0, 0, 0x1BA, D_000385DC,
                                               &eLightGrid::New, 0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00046A24;
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

template eLightGrid *dcast<eLightGrid *>(const cBase *base);
