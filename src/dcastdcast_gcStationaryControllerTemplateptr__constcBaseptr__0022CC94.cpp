// ── dcast<gcStationaryControllerTemplate *>(const cBase *) @ 0x0022cc94 ──
// RTTI downcast (gcAll_psp.obj) with a 3-level lazy cType::InitializeType
// cascade (constants read from this fn's OWN 0x0022cc94 disasm):
//   inner base  D_000385DC (name strings 0x36D894/0x36D89C, ordinal 1)
//   middle      D_0009A400 (ordinal 0x9a, parent = inner base)
//   outer       D_0009F7C8 (ordinal 0x102, parent = middle,
//                           allocator &gcStationaryControllerTemplate::New @ 0x320444,
//                           8th InitializeType arg = 0)
// Then walk the object's dynamic type chain (t = t->mParent) with the goto-form
// null guards reproducing beqzl/bnel. Mirrors matched dcast<gcLookAtControllerTemplate*>
// (340B, 3-level); same shape, different cascade slots/ordinals/New.

class cBase;
class cMemPool;
class cType;
class gcStationaryControllerTemplate;

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
    cBase *gcStationaryControllerTemplate__New_cMemPoolptr_cBaseptrstatic__00320444(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_0009A400;
extern cType *D_0009F7C8;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009F7C8 == 0) {
            if (D_0009A400 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009A400 = cType::InitializeType(0, 0, 0x9a, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F7C8 = cType::InitializeType(
                0, 0, 0x102, D_0009A400,
                (cBase * (*)(cMemPool *, cBase *))&gcStationaryControllerTemplate__New_cMemPoolptr_cBaseptrstatic__00320444,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F7C8;
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

template gcStationaryControllerTemplate *dcast<gcStationaryControllerTemplate *>(const cBase *base);
