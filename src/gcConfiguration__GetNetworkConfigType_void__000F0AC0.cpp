// gcConfiguration::GetNetworkConfigType(void) @ 0x000f0ac0
// Obj: gcAll_psp.obj
// Emits: __0fPgcConfigurationUGetNetworkConfigTypev
//
// cType registration getter — three-level InitializeType cascade with
// per-level static guards. Mirrors the matched sibling pattern in
// gcConfiguration::GetType (gcConfiguration.cpp). Isolated split-TU.

class cBase;
class cMemPool;

class cType {
public:
    char _pad[0x10];
    cBase *(*mCreate)(cMemPool *, cBase *);

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcConfiguration {
public:
    cType *GetNetworkConfigType(void);
};

// Per-level static type registry slots (masked relocations).
extern cType *D_0009F924;  // network config type (result)
extern cType *D_0009F928;  // 0x298 intermediate
extern cType *D_000385DC;  // base "gcConfig" type (shared with GetType)

// mCreate function pointers (masked relocations).
extern "C" cBase *nwConfigPSP__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);
extern "C" cBase *nwConfig__New_cMemPoolptr_cBaseptr(cMemPool *, cBase *);

cType *gcConfiguration::GetNetworkConfigType(void) {
    if (D_0009F924 == 0) {
        if (D_0009F928 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F928 = cType::InitializeType(
                0, 0, 0x28C, D_000385DC,
                (cBase * (*)(cMemPool *, cBase *)) & nwConfig__New_cMemPoolptr_cBaseptr,
                0, 0, 0);
        }
        D_0009F924 = cType::InitializeType(
            0, 0, 0x298, D_0009F928,
            (cBase * (*)(cMemPool *, cBase *)) & nwConfigPSP__New_cMemPoolptr_cBaseptr,
            0, 0, 0);
    }
    return D_0009F924;
}
