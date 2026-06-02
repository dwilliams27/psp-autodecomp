// ── dcast<gcStreamedCinematicConfig *>(const cBase *) @ 0x00221128 ──
// RTTI downcast: lazily register gcStreamedCinematicConfig's cType (a 2-level
// InitializeType cascade), then walk the object's dynamic type chain looking
// for the registered type. Per-function values read from the 0x00221128
// disassembly (gcAll_psp.obj — its OWN base strings, NOT eAll's):
//   inner   D_000385DC : InitializeType(0x36D894,0x36D89C,1, 0, 0, ...)
//   outer   D_00099AD8 : InitializeType(0,0, 0x1BE, D_000385DC,
//                                        &gcStreamedCinematicConfig::New, 0,0, 0)
// gcStreamedCinematicConfig::New @ 0x2445F0.

class cBase;
class cMemPool;
class cType;
class gcStreamedCinematicConfig;

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
    cBase *gcStreamedCinematicConfig__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_00099AD8;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_00099AD8 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00099AD8 = cType::InitializeType(
                0, 0, 0x1BE, D_000385DC,
                (cBase * (*)(cMemPool *, cBase *))&gcStreamedCinematicConfig__New_cMemPoolptr_cBaseptr,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00099AD8;
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

template gcStreamedCinematicConfig *dcast<gcStreamedCinematicConfig *>(const cBase *base);
