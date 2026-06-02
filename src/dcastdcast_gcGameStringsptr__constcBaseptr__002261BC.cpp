// ── dcast<gcGameStrings *>(const cBase *) @ 0x002261bc ──
// RTTI downcast (gcAll_psp.obj). Lazily register gcGameStrings's cType via a
// 3-level InitializeType cascade, then walk the object's dynamic type chain
// looking for the registered type. Mirrors the matched 3-level dcast shape
// (gcBoolSet @ 0x00220318); constants read from THIS function's expected .o
// (002261bc) disassembly:
//   inner D_000385DC : InitializeType(0x36D894, 0x36D89C, 1, 0, 0, 0,0,0)
//   mid   D_0009F454 : InitializeType(0,0,0x170, D_000385DC, 0, 0,0,0)
//   outer D_0009F4D0 : InitializeType(0,0,0x1CC, D_0009F454, &gcGameStrings::New, 0,0,0)
// gcGameStrings::New @ 0x27DAA8; shared base strings 0x36D894/0x36D89C.

class cBase;
class cMemPool;
class cType;
class gcGameStrings;

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
    cBase *gcGameStrings__New_cMemPoolptr_cBaseptrstatic__0027DAA8(cMemPool *, cBase *);
}

extern cType *D_0009F4D0;
extern cType *D_0009F454;
extern cType *D_000385DC;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009F4D0 == 0) {
            if (D_0009F454 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F454 = cType::InitializeType(
                    0, 0, 0x170, D_000385DC,
                    0, 0, 0, 0);
            }
            D_0009F4D0 = cType::InitializeType(
                0, 0, 0x1CC, D_0009F454,
                (cBase * (*)(cMemPool *, cBase *))&gcGameStrings__New_cMemPoolptr_cBaseptrstatic__0027DAA8,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F4D0;
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

template gcGameStrings *dcast<gcGameStrings *>(const cBase *base);
