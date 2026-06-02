// ── dcast<gcStringTableValue *>(const cBase *) @ 0x00227248 ──
// RTTI downcast (gcAll_psp.obj): lazily register gcStringTableValue's cType
// (a 3-level InitializeType cascade), then walk the object's dynamic type
// chain looking for the registered type. Mirrors the matched 340B 3-level
// dcast<eMaterialGroup *> shape (split-if/goto parent-walk).
//   inner   D_000385DC : InitializeType(0x36D894,0x36D89C,1, 0, 0, ...)
//   middle  D_0009F454 : InitializeType(0,0, 0x170, D_000385DC, 0, ...)
//   outer   D_0009F56C : InitializeType(0,0,0x17b, D_0009F454,
//                                        &gcStringTableValue::New, 0,0, 0)
// gcStringTableValue::New @ 0x28995C. gcAll base strings 0x36D894/0x36D89C.
// All constants/ordinals read from this fn's own disassembly + symbols.

class cBase;
class cMemPool;
class cType;
class gcStringTableValue;

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
    cBase *__0fSgcStringTableValueDNewP6IcMemPoolP6FcBaseT(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F56C;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009F56C == 0) {
            if (D_0009F454 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F56C = cType::InitializeType(
                0, 0, 0x17b, D_0009F454,
                (cBase * (*)(cMemPool *, cBase *))&__0fSgcStringTableValueDNewP6IcMemPoolP6FcBaseT,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F56C;
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

template gcStringTableValue *dcast<gcStringTableValue *>(const cBase *base);
