// ── dcast<gcStringSet *>(const cBase *) @ 0x00221d4c (340B, gcAll_psp.obj) ──
// RTTI downcast. Lazily register gcStringSet's cType via a 3-level
// InitializeType cascade, then walk the object's dynamic type chain for the
// registered type. 3-level *Set shape (vs the 280B 2-level dcast).
//   inner  D_000385DC : InitializeType(0x36D894, 0x36D89C, 1, 0, 0, 0,0,0)
//   middle D_000998F4 : InitializeType(0,0, 0xAD,  D_000385DC, 0, 0,0,0)
//   outer  D_0009A2F0 : InitializeType(0,0, 0x1ED, D_000998F4,
//                                      &gcStringSet::New, 0,0,0)
// gcStringSet::New @ 0x249A1C.

class cBase;
class cMemPool;
class cType;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcStringSet {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
};

extern cType *D_000385DC;
extern cType *D_000998F4;
extern cType *D_0009A2F0;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base)
{
    const void *result = 0;

    if (base != 0) {
        if (D_0009A2F0 == 0) {
            if (D_000998F4 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000998F4 = cType::InitializeType(0, 0, 0xAD, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009A2F0 = cType::InitializeType(
                0, 0, 0x1ED, D_000998F4,
                (cBase * (*)(cMemPool *, cBase *))&gcStringSet::New, 0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009A2F0;
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
                type = (cType *)*(void **)((char *)type + 0x1C);
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

template gcStringSet *dcast<gcStringSet *>(const cBase *base);
