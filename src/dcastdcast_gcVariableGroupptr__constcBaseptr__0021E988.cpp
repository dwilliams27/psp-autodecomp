// ── dcast<gcVariableGroup *>(const cBase *) @ 0x0021e988 ──
// RTTI downcast (gcAll_psp.obj). Lazily register gcVariableGroup's cType via a
// 3-level InitializeType cascade, then walk the object's dynamic type chain for
// the registered type. Body mirrors the matched gcViewport dcast (00221AE0)
// goto/split-if form, with one extra middle InitializeType tier.
// Constants read from this function's OWN 0x0021e988 disasm:
//   inner  D_000385DC : InitializeType(0x36D894, 0x36D89C, 1, 0, 0, 0,0,0)
//   middle D_00040C94 : InitializeType(0, 0, 0x4,  D_000385DC, 0, 0,0,0)
//   outer  D_000998A8 : InitializeType(0, 0, 0x7A, D_00040C94,
//                                       &gcVariableGroup::New, 0,0, 0x8)
// gcVariableGroup::New @ 0x2361EC.

class cBase;
class cMemPool;
class cType;

class cType {
public:
    char _pad[0x1C];
    const cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcVariableGroup {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct DispatchEntry {
    short offset;
    short pad;
    cType *(*fn)(void *, short, void *);
};

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_000998A8;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_000998A8 == 0) {
            if (D_00040C94 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040C94 = cType::InitializeType(0, 0, 0x4, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000998A8 = cType::InitializeType(
                0, 0, 0x7A, D_00040C94,
                (cBase * (*)(cMemPool *, cBase *))&gcVariableGroup::New,
                0, 0, 0x8);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_000998A8;
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

template gcVariableGroup *dcast<gcVariableGroup *>(const cBase *base);
