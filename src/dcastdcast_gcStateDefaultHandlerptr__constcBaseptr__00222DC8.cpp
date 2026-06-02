// ── dcast<gcStateDefaultHandler *>(const cBase *) @ 0x00222dc8 ──
// RTTI downcast (gcAll_psp.obj): lazily register gcStateDefaultHandler's cType
// via a 3-level InitializeType cascade, then walk the object's dynamic type
// chain looking for the registered type. Mirrors the matched sibling
// dcast<gcStateFunction *> 3-level shape; constants read from THIS function's
// 0x00222dc8 disassembly (and confirmed by gcStateDefaultHandler::GetType):
//   inner  D_000385DC : InitializeType(0x36D894,0x36D89C,1, 0, 0, 0,0,0)
//   l2     D_0009A3D0 : InitializeType(0,0,0xD9, D_000385DC, &gcStateHandlerBase::New,   0,0,0)
//   outer  D_0009A3D8 : InitializeType(0,0,0xDA, D_0009A3D0, &gcStateDefaultHandler::New,0,0,0)
// gcStateHandlerBase::New @ 0x25708C.  gcStateDefaultHandler::New @ 0x25869C.
// Signature is (const cBase *).

class cBase;
class cMemPool;
class cType;
class gcStateDefaultHandler;

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
    cType *(*fn)(const void *, short, void *);
};

extern "C" {
    cBase *gcStateHandlerBase__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
    cBase *gcStateDefaultHandler__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
}

extern cType *D_0009A3D8;
extern cType *D_0009A3D0;
extern cType *D_000385DC;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    void *result = 0;

    if (base != 0) {
        if (D_0009A3D8 == 0) {
            if (D_0009A3D0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009A3D0 = cType::InitializeType(
                    0, 0, 0xD9, D_000385DC,
                    (cBase * (*)(cMemPool *, cBase *))&gcStateHandlerBase__New_cMemPoolptr_cBaseptr,
                    0, 0, 0);
            }
            D_0009A3D8 = cType::InitializeType(
                0, 0, 0xDA, D_0009A3D0,
                (cBase * (*)(cMemPool *, cBase *))&gcStateDefaultHandler__New_cMemPoolptr_cBaseptr,
                0, 0, 0);
        }

        const void *classDesc = *(const void **)((const char *)base + 4);
        cType *target = D_0009A3D8;
        DispatchEntry *entry = (DispatchEntry *)((const char *)classDesc + 8);
        short offset = entry->offset;
        cType *(*fn)(const void *, short, void *) = entry->fn;
        cType *type = fn((const char *)base + offset, offset, (void *)fn);
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
            result = (void *)base;
        }
    }

    return (T)result;
}

template gcStateDefaultHandler *dcast<gcStateDefaultHandler *>(const cBase *base);
