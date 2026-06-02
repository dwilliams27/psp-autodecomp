// ── dcast<gcBoolSet *>(const cBase *) @ 0x00220318 ──
// RTTI downcast (gcAll_psp.obj). Lazily register gcBoolSet's cType via a
// 3-level InitializeType cascade, then walk the object's dynamic type chain
// looking for the registered type. Mirrors the matched 3-level dcast shape;
// constants read from THIS function's expected .o (00220318) disassembly:
//   inner D_000385DC : InitializeType(0x36D894, 0x36D89C, 1, 0, 0, 0,0,0)
//   mid   D_000998F4 : InitializeType(0,0,0xAD, D_000385DC, 0, 0,0,0)
//   outer D_000998F8 : InitializeType(0,0,0xAF, D_000998F4, &gcBoolSet::New, 0,0,0)
// gcBoolSet::New @ 0x239DCC; shared base strings 0x36D894/0x36D89C.

class cBase;
class cMemPool;
class cType;
class gcBoolSet;

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
    cBase *gcBoolSet__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
}

extern cType *D_000998F8;
extern cType *D_000998F4;
extern cType *D_000385DC;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_000998F8 == 0) {
            if (D_000998F4 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000998F4 = cType::InitializeType(
                    0, 0, 0xAD, D_000385DC,
                    0, 0, 0, 0);
            }
            D_000998F8 = cType::InitializeType(
                0, 0, 0xAF, D_000998F4,
                (cBase * (*)(cMemPool *, cBase *))&gcBoolSet__New_cMemPoolptr_cBaseptr,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_000998F8;
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

template gcBoolSet *dcast<gcBoolSet *>(const cBase *base);
