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

class gcShadowController {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcShadowConfig {
public:
    const cType *GetControllerType(void) const;
};

extern cType *D_000385DC;
extern cType *D_0009F64C;
extern cType *D_0009F7B8;

// gcShadowConfig::GetControllerType(void) const @ 0x0031fc10
const cType *gcShadowConfig::GetControllerType(void) const {
    if (D_0009F7B8 == 0) {
        if (D_0009F64C == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F64C = cType::InitializeType(0, 0, 0x1D5, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F7B8 = cType::InitializeType(0, 0, 0x1EB, D_0009F64C,
                                           &gcShadowController::New,
                                           0, 0, 0);
    }
    return D_0009F7B8;
}
