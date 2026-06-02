// ── dcast<eSpriteFilter *>(const cBase *) @ 0x001d4564 ──
// RTTI downcast: lazily register eSpriteFilter's cType (a 3-level
// InitializeType cascade), then walk the object's dynamic type chain looking
// for the registered type. Mirrors the matched 340B 3-level dcast shape
// (e.g. dcast<eCapsuleShape *>):
//   inner   D_000385DC : InitializeType(0x36CD74,0x36CD7C,1, 0, 0, ...)
//   middle  D_00046C60 : InitializeType(0,0, 0x13B, D_000385DC, 0, ...)
//   outer   D_00046CBC : InitializeType(0,0, 0x63, D_00046C60,
//                                        &eSpriteFilter::New, 0,0, 0)
// eSpriteFilter::New @ 0x21CD84. Shared eAll base strings 0x36CD74/0x36CD7C.
// Outer final (unsigned int) arg is 0 here.

class cBase;
class cMemPool;
class cType;
class eSpriteFilter;

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
    cBase *eSpriteFilter__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_00046C60;
extern cType *D_00046CBC;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_00046CBC == 0) {
            if (D_00046C60 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00046C60 = cType::InitializeType(0, 0, 0x13B, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_00046CBC = cType::InitializeType(
                0, 0, 0x63, D_00046C60,
                (cBase * (*)(cMemPool *, cBase *))&eSpriteFilter__New_cMemPoolptr_cBaseptr,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00046CBC;
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

template eSpriteFilter *dcast<eSpriteFilter *>(const cBase *base);
