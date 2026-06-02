// ── dcast<ePathGroup *>(const cBase *) @ 0x001c86c8 ──
// RTTI downcast: lazily register ePathGroup's cType (3-level nested-if
// cascade — derived D_00040E20, middle D_00040C94, base D_000385DC),
// then walk the object's dynamic type chain looking for the registered type.
// Mirrors the matched dcast<eSound*>/dcast<eBspTree*> shape, extended to the
// deeper cascade. Base type D_000385DC @ 0x385DC, name strings
// 0x36CD74/0x36CD7C; middle type ord 4 @ 0x40C94, derived ord 0x19 @ 0x40E20,
// arg8=8 on the derived registration; allocator &ePathGroup::New @ 0x1DBCCC
// passed ONLY to the derived (outer) registration (middle/inner arg5=0).

class cBase;
class cMemPool;
class cType;
class ePathGroup;

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

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_00040E20;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_00040E20 == 0) {
            if (D_00040C94 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040C94 = cType::InitializeType(
                    0, 0, 4, D_000385DC,
                    0, 0, 0, 0);
            }
            D_00040E20 = cType::InitializeType(
                0, 0, 0x19, D_00040C94,
                (cBase * (*)(cMemPool *, cBase *))0x1DBCCC,
                0, 0, 8);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00040E20;
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

template ePathGroup *dcast<ePathGroup *>(const cBase *base);
