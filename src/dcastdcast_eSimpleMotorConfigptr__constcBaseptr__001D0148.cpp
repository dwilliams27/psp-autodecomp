// ── dcast<eSimpleMotorConfig *>(const cBase *) @ 0x001d0148 ──
// RTTI downcast: register eSimpleMotorConfig's cType lazily via a
// three-level InitializeType cascade (outer derived 0x46C00 ordinal 0x263 ->
// middle 0x469F0 ordinal 0x260 -> inner base D_000385DC ordinal 1, name
// strings 0x36CD74/0x36CD7C), then walk the object's dynamic type chain
// looking for the registered type. Mirrors the matched dcast<eBipedControllerConfig*>
// shape. The outer (0x263) InitializeType call passes &eSimpleMotorConfig::New
// as the 5th arg; all calls pass zero for the 8th arg.

class cBase;
class cMemPool;
class cType;
class eSimpleMotorConfig;

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
    cBase *eSimpleMotorConfig__New_cMemPoolptr_cBaseptrstatic__0020E758(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_000469F0;
extern cType *D_00046C00;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_00046C00 == 0) {
            if (D_000469F0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000469F0 = cType::InitializeType(0, 0, 0x260, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_00046C00 = cType::InitializeType(
                0, 0, 0x263, D_000469F0,
                (cBase * (*)(cMemPool *, cBase *))
                    &eSimpleMotorConfig__New_cMemPoolptr_cBaseptrstatic__0020E758,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00046C00;
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

template eSimpleMotorConfig *dcast<eSimpleMotorConfig *>(const cBase *base);
