// ── dcast<gcStreamedCinematicConfigGroup *>(const cBase *) @ 0x00221240 ──
// RTTI downcast: lazily register gcStreamedCinematicConfigGroup's cType (a
// 3-level InitializeType cascade), then walk the object's dynamic type chain
// looking for the registered type. Mirrors the matched dcast<gcUIWidget *>
// shape; constants read from THIS function's 0x00221240 disassembly
// (gcAll_psp.obj — its OWN base strings/slots):
//   inner  D_000385DC : InitializeType(0x36D894,0x36D89C,1, 0, 0, 0,0,0)
//   l2     D_000385E0 : InitializeType(0,0,2, D_000385DC, &cNamed::New, 0,0,0)
//   outer  D_00099ADC : InitializeType(0,0,0x114, D_000385E0,
//                                      &gcStreamedCinematicConfigGroup::New, 0,0,0)
// cNamed::New @ 0x1C3C58.  gcStreamedCinematicConfigGroup::New @ 0x244A90.

class cBase;
class cMemPool;
class cType;
class gcStreamedCinematicConfigGroup;

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
    cBase *cNamed__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
    cBase *gcStreamedCinematicConfigGroup__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
}

extern cType *D_00099ADC;
extern cType *D_000385E0;
extern cType *D_000385DC;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_00099ADC == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC,
                    (cBase * (*)(cMemPool *, cBase *))&cNamed__New_cMemPoolptr_cBaseptr,
                    0, 0, 0);
            }
            D_00099ADC = cType::InitializeType(
                0, 0, 0x114, D_000385E0,
                (cBase * (*)(cMemPool *, cBase *))&gcStreamedCinematicConfigGroup__New_cMemPoolptr_cBaseptr,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00099ADC;
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

template gcStreamedCinematicConfigGroup *dcast<gcStreamedCinematicConfigGroup *>(const cBase *base);
