// ── dcast<gcParticleSystemConfig *>(const cBase *) @ 0x0022bf88 ──
// RTTI downcast: lazily register gcParticleSystemConfig's cType (3-level
// cascade: base D_000385DC -> gcEntityGeomConfig parent D_0009F43C ->
// gcParticleSystemConfig D_0009F7A0), then walk the object's dynamic type
// chain looking for the registered type.
// Ordinals: base 1, gcEntityGeomConfig 0xA0 (New @ 0x267384),
// gcParticleSystemConfig 0x1F9 (New @ 0x31DD14). Name strings 0x36D894/0x36D89C.
// Mirrors the matched goto-form gcViewport dcast (00221AE0), extended one level
// so the branch-likely cascade tail (beqzl/bnel) reproduces exactly.

class cBase;
class cMemPool;
class cType;
class gcParticleSystemConfig;

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
    cBase *__0fSgcEntityGeomConfigDNewP6IcMemPoolP6FcBaseT(cMemPool *, cBase *);
    cBase *__0fWgcParticleSystemConfigDNewP6IcMemPoolP6FcBaseT(cMemPool *, cBase *);
}

extern cType *D_000385DC;
extern cType *D_0009F43C;
extern cType *D_0009F7A0;

template <class T>
T dcast(const cBase *base);

template <class T>
T dcast(const cBase *base) {
    const void *result = 0;

    if (base != 0) {
        if (D_0009F7A0 == 0) {
            if (D_0009F43C == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F43C = cType::InitializeType(
                    0, 0, 0xA0, D_000385DC,
                    (cBase * (*)(cMemPool *, cBase *))&__0fSgcEntityGeomConfigDNewP6IcMemPoolP6FcBaseT,
                    0, 0, 0);
            }
            D_0009F7A0 = cType::InitializeType(
                0, 0, 0x1F9, D_0009F43C,
                (cBase * (*)(cMemPool *, cBase *))&__0fWgcParticleSystemConfigDNewP6IcMemPoolP6FcBaseT,
                0, 0, 0);
        }

        void *classDesc = *(void **)((char *)base + 4);
        cType *target = D_0009F7A0;
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

template gcParticleSystemConfig *dcast<gcParticleSystemConfig *>(const cBase *base);
