// ── dcast<gcStateHandler *>(const cBase *) @ 0x00222c6c ──
// RTTI downcast: register gcStateHandler's cType lazily via a three-level
// InitializeType cascade (outer derived D_0009A3D4 ordinal 0xD1 with
// &gcStateHandler::New @ 0x258244 -> middle D_0009A3D0 ordinal 0xD9 with
// &gcStateHandlerBase::New @ 0x25708C -> inner base D_000385DC ordinal 1,
// name strings 0x36D894/0x36D89C), then walk the object's dynamic type chain
// looking for the registered type. Mirrors the matched 348B
// dcast<eSurfaceProperty*> shape (gcAll-specific slots/New ptrs from this fn's
// own disassembly).

class cBase;
class cMemPool;
class cType;
class gcStateHandler;

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
    cBase *gcStateHandlerBase__New_cMemPoolptr_cBaseptrstatic__0025708C(cMemPool *, cBase *);
    cBase *gcStateHandler__New_cMemPoolptr_cBaseptrstatic__00258244(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_0009A3D0;
extern cType *D_0009A3D4;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009A3D4 == 0) {
            if (D_0009A3D0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009A3D0 = cType::InitializeType(
                    0, 0, 0xD9, D_000385DC,
                    (cBase * (*)(cMemPool *, cBase *))
                        &gcStateHandlerBase__New_cMemPoolptr_cBaseptrstatic__0025708C,
                    0, 0, 0);
            }
            D_0009A3D4 = cType::InitializeType(
                0, 0, 0xD1, D_0009A3D0,
                (cBase * (*)(cMemPool *, cBase *))
                    &gcStateHandler__New_cMemPoolptr_cBaseptrstatic__00258244,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009A3D4;
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

template gcStateHandler *dcast<gcStateHandler *>(const cBase *base);
