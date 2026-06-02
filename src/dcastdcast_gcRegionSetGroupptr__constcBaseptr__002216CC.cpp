// ── dcast<gcRegionSetGroup *>(const cBase *) @ 0x002216cc ──
// RTTI downcast (gcAll_psp.obj). 3-level lazy cType registration cascade, then
// walk the object's dynamic type chain looking for the registered type.
//   inner  D_000385DC : InitializeType(0x36D894, 0x36D89C, 1, 0, 0, ...)
//   middle D_000385E0 : InitializeType(0, 0, 2,     D_000385DC, &alloc243c58, ...)
//   outer  D_00099AEC : InitializeType(0, 0, 0x113, D_000385E0, &gcRegionSetGroup::New, ...)
// gcRegionSetGroup::New @ 0x2460fc. Middle allocator @ 0x243c58 (anonymous in
// the .map; the call reloc is masked by compare_func so its symbol identity is
// irrelevant to the byte comparison).
// Mirrors the matched gcViewport 00221AE0 split-if/goto parent-walk form,
// extended to the 3-level cascade this fn's own disassembly requires.

class cBase;
class cMemPool;
class cType;
class gcRegionSetGroup;

class cType {
public:
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
    cBase *__0fPgcRegionSetGroupDNewP6IcMemPoolP6FcBaseT(cMemPool *, cBase *);
    cBase *__middle_alloc_243c58(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_00099AEC;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_00099AEC == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(
                    0, 0, 2, D_000385DC,
                    (cBase * (*)(cMemPool *, cBase *))&__middle_alloc_243c58,
                    0, 0, 0);
            }
            D_00099AEC = cType::InitializeType(
                0, 0, 0x113, D_000385E0,
                (cBase * (*)(cMemPool *, cBase *))&__0fPgcRegionSetGroupDNewP6IcMemPoolP6FcBaseT,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_00099AEC;
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

template gcRegionSetGroup *dcast<gcRegionSetGroup *>(const cBase *base);
