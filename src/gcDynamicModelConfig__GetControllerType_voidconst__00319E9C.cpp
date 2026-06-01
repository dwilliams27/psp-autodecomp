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

class gcDynamicModelController {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcDynamicModelConfig {
public:
    const cType *GetControllerType(void) const;
};

extern cType *D_000385DC;
extern cType *D_0009F64C;
extern cType *D_0009F670;

// gcDynamicModelConfig::GetControllerType(void) const @ 0x00319e9c
const cType *gcDynamicModelConfig::GetControllerType(void) const {
    if (D_0009F670 == 0) {
        if (D_0009F64C == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F64C = cType::InitializeType(0, 0, 0x1D5, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F670 = cType::InitializeType(0, 0, 0xE2, D_0009F64C,
                                           &gcDynamicModelController::New,
                                           0, 0, 0);
    }
    return D_0009F670;
}
