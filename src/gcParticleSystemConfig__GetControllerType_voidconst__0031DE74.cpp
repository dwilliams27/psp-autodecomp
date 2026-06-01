// gcParticleSystemConfig::GetControllerType(void) const  — 0x0031DE74, 220B
// gcAll_psp.obj. Meyers-singleton cType::InitializeType registration accessor.
// Split-TU: class declared locally; symbol __0fWgcParticleSystemConfigRGetControllerTypevK.

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

class gcParticleSystemController {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcParticleSystemConfig {
public:
    const cType *GetControllerType(void) const;
};

extern cType *D_000385DC;
extern cType *D_0009F64C;
extern cType *D_0009F66C;

const cType *gcParticleSystemConfig::GetControllerType(void) const {
    if (D_0009F66C == 0) {
        if (D_0009F64C == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F64C = cType::InitializeType(0, 0, 0x1D5, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F66C = cType::InitializeType(0, 0, 0x1F8, D_0009F64C,
                                           &gcParticleSystemController::New,
                                           0, 0, 0);
    }
    return D_0009F66C;
}
