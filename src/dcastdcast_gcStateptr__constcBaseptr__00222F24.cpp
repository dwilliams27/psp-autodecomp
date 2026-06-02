// ── dcast<gcState *>(const cBase *) @ 0x00222f24 ──
// RTTI downcast (gcAll_psp.obj): lazily register gcState's cType via a 3-level
// InitializeType cascade, then walk the object's dynamic type chain looking for
// the registered type. Mirrors the matched immediately-preceding sibling
// dcast<gcStateDefaultHandler *> @ 0x00222dc8 (byte-identical structure);
// constants read from THIS function's 0x00222f24 disassembly:
//   inner  D_000385DC : InitializeType(0x36D894,0x36D89C,1, 0, 0, 0,0,0)
//   l2     D_000385E0 : InitializeType(0,0,0x2,  D_000385DC, &cNamed::New,  0,0,0)
//   outer  D_0009A3DC : InitializeType(0,0,0xB5, D_000385E0, &gcState::New, 0,0,0)
// cNamed::New @ 0x1C3C58.  gcState::New @ 0x25908C.
// Signature is (const cBase *).

class cBase;
class cMemPool;
class cType;
class gcState;

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
    cBase *cNamed__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
    cBase *gcState__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
}

extern cType *D_0009A3DC;
extern cType *D_000385E0;
extern cType *D_000385DC;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    void *result = 0;

    if (base != 0) {
        if (D_0009A3DC == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 0x2, D_000385DC,
                    (cBase * (*)(cMemPool *, cBase *))&cNamed__New_cMemPoolptr_cBaseptr,
                    0, 0, 0);
            }
            D_0009A3DC = cType::InitializeType(
                0, 0, 0xB5, D_000385E0,
                (cBase * (*)(cMemPool *, cBase *))&gcState__New_cMemPoolptr_cBaseptr,
                0, 0, 0);
        }

        const void *classDesc = *(const void **)((const char *)base + 4);
        cType *target = D_0009A3DC;
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

template gcState *dcast<gcState *>(const cBase *base);
