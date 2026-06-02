// ============================================================
// dcast<eFluidVolume *>(const cBase *)
// @ 0x001cb7c4, 280B
// Symbol: __0FFdcast7P6MeFluidVolume_PC6FcBase_9BA
//
// TIER-1 mechanical RTTI downcast template (free function template).
// Two-level cType::InitializeType registration cascade feeding a
// cBase type-chain walk: fetch the run-time type via the vtable
// dispatch record (vtable+8), then follow the base type chain
// (unk1C) looking for the registered template type.
// ============================================================

class cMemPool;
class cBase;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eFluidVolume;

class eFluidVolumeCreator {
public:
    static cBase *New(cMemPool *, cBase *);
};

struct cBaseDispatchRecord {
    short offset;
    short pad;
    void *(*fn)(void *);
};

struct cTypeChain {
    char pad[0x1c];
    cTypeChain *next;   // unk1C
};

extern cType *D_000385DC;   // 0x000385DC
extern cType *D_000468D0;   // 0x000468D0

template <class T>
T dcast(const cBase *obj)
{
    T result = 0;

    if (obj != 0) {
        if (D_000468D0 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                   (const char *)0x36CD7C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_000468D0 = cType::InitializeType(0, 0, 0x25B, D_000385DC,
                                               &eFluidVolumeCreator::New,
                                               0, 0, 0);
        }

        cBaseDispatchRecord *rec =
            (cBaseDispatchRecord *)(*(char **)((char *)obj + 4) + 8);
        cType *want = D_000468D0;
        short off = rec->offset;
        cTypeChain *t = (cTypeChain *)rec->fn((char *)obj + off);

        int found;
        if (want == 0) {
            found = 0;
        } else if (t != 0) {
            for (;;) {
                if (t == (cTypeChain *)want) {
                    found = 1;
                    break;
                }
                t = t->next;
                if (t == 0)
                    goto not_found;
            }
        } else {
not_found:
            found = 0;
        }

        if (found)
            result = (T)obj;
    }

    return result;
}

// Force instantiation of the eFluidVolume * specialization.
template eFluidVolume *dcast<eFluidVolume *>(const cBase *);
