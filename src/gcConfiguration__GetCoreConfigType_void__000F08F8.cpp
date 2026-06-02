// gcConfiguration::GetCoreConfigType(void) @ 0x000f08f8 (gcAll_psp.obj)
// Lazily registers the "core config" cType via a 3-level InitializeType
// cascade (inner shared type D_000385DC, mid D_00038888 ordinal 0x28A,
// outer D_0003888C ordinal 0x291) and returns the cached outer type.

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

class gcConfiguration {
public:
    cType *GetCoreConfigType();
};

extern cType *D_000385DC;
extern cType *D_00038888;
extern cType *D_0003888C;

extern "C" {
    cBase *__0fLcConfigBaseDNewP6IcMemPoolP6FcBaseT(cMemPool *, cBase *);
    cBase *__0fKcConfigPSPDNewP6IcMemPoolP6FcBaseT(cMemPool *, cBase *);
}

cType *gcConfiguration::GetCoreConfigType() {
    if (D_0003888C == 0) {
        if (D_00038888 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00038888 = cType::InitializeType(
                0, 0, 0x28A, D_000385DC,
                (cBase * (*)(cMemPool *, cBase *))&__0fLcConfigBaseDNewP6IcMemPoolP6FcBaseT,
                0, 0, 0);
        }
        D_0003888C = cType::InitializeType(
            0, 0, 0x291, D_00038888,
            (cBase * (*)(cMemPool *, cBase *))&__0fKcConfigPSPDNewP6IcMemPoolP6FcBaseT,
            0, 0, 0);
    }
    return D_0003888C;
}
