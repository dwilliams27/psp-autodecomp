// ── dcast<eMaterialSetGroup *>(const cBase *) @ 0x001c8ac4 ──
// RTTI downcast with a 3-level lazy cType::InitializeType cascade:
//   inner base  D_000385DC (name strings 0x36CD74/0x36CD7C, ordinal 1)
//   middle      D_00040C94 (ordinal 4, parent = inner base)
//   outer       D_00040E2C (ordinal 0x28, parent = middle,
//                           allocator &eMaterialSetGroup::New @ 0x1DC404,
//                           8th InitializeType arg = 8)
// Then walk the object's dynamic type chain (t = t->mParent) with ||-form
// null guards reproducing beqzl/bnel. Mirrors matched dcast<eSound*> (280B,
// 2-level); this target adds the middle level for +60B.

class cBase;
class cMemPool;
class cType;
class eMaterialSetGroup;

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
    cBase *eMaterialSetGroup__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_00040E2C;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_00040E2C == 0) {
            if (D_00040C94 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_00040E2C = cType::InitializeType(
                0, 0, 0x28, D_00040C94,
                (cBase * (*)(cMemPool *, cBase *))&eMaterialSetGroup__New_cMemPoolptr_cBaseptr,
                0, 0, 8);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00040E2C;
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

template eMaterialSetGroup *dcast<eMaterialSetGroup *>(const cBase *base);
