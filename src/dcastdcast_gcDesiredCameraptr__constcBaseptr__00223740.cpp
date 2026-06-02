// ── dcast<gcDesiredCamera *>(const cBase *) @ 0x00223740 ──
// RTTI downcast: register gcDesiredCamera's cType lazily, then walk the object's
// dynamic type chain looking for the registered type.
// Mirrors the matched eSound dcast goto structure (which produces the SNC
// beqzl/bnel branch-likely idiom byte-exactly). gcAll-specific slots read from
// this fn's own disassembly: derived-type global D_0009F408, base type
// D_000385DC, name strings 0x36D894/0x36D89C, type ordinal 0xA6,
// allocator &gcDesiredCamera::New @ 0x00262AC4.

class cBase;
class cMemPool;
class cType;
class gcDesiredCamera;

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
    cBase *gcDesiredCamera__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_0009F408;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009F408 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F408 = cType::InitializeType(
                0, 0, 0xA6, D_000385DC,
                (cBase * (*)(cMemPool *, cBase *))&gcDesiredCamera__New_cMemPoolptr_cBaseptr,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F408;
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

template gcDesiredCamera *dcast<gcDesiredCamera *>(const cBase *base);
