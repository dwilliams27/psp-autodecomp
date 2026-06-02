// ============================================================
// dcast<gcFunctionGroup *>(const cBase *)
// @ 0x0021e834, 340B  (gcAll_psp.obj)
// Symbol: __0FFdcast7P6PgcFunctionGroup_PC6FcBase_9BA
//
// TIER-1 mechanical RTTI downcast template (free function template).
// THREE-level cType::InitializeType registration cascade (gcAll has an
// extra middle node vs the eAll 2-level form) feeding a cBase
// type-chain walk: fetch the run-time type via the vtable dispatch
// record (vtable+8), then follow the base type chain (unk1C) looking
// for the registered template type.
//
// Cascade (from this fn's own disassembly):
//   inner  D_000385DC : InitializeType(0x36D894,0x36D89C,1, 0,0,0,0,0)
//   middle D_00040C94 : InitializeType(0,0,4,    D_000385DC, 0,0,0,0)
//   outer  D_000998A4 : InitializeType(0,0,0x75, D_00040C94,
//                                      &gcFunctionGroup::New, 0,0,8)
//
// Mirrors the matched goto/split-if form of dcast<eVolume *>
// (0x001cdde8) to reproduce the beqzl/bnel branch-likely chain.
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

class gcFunctionGroup {
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
extern cType *D_00040C94;   // 0x00040C94
extern cType *D_000998A4;   // 0x000998A4

template <class T>
T dcast(const cBase *obj)
{
    T result = 0;

    if (obj != 0) {
        if (D_000998A4 == 0) {
            if (D_00040C94 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_000998A4 = cType::InitializeType(0, 0, 0x75, D_00040C94,
                                               &gcFunctionGroup::New,
                                               0, 0, 8);
        }

        cBaseDispatchRecord *rec =
            (cBaseDispatchRecord *)(*(char **)((char *)obj + 4) + 8);
        cType *want = D_000998A4;
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

// Force instantiation of the gcFunctionGroup * specialization.
template gcFunctionGroup *dcast<gcFunctionGroup *>(const cBase *);
