// ============================================================
// gcGeomCurveConfig::GetControllerType(void) const
// @ 0x0031be68, 220B
// Symbol: __0fRgcGeomCurveConfigRGetControllerTypevK
//
// Nested static type-registration accessor (mirrors cObject::GetType).
// Three-level cType::InitializeType chain; the creator function pointer
// (gcGeomCurveController::New) is passed only on the outermost call.
// ============================================================

class cMemPool;
class cBase;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcGeomCurveConfig {
public:
    const cType *GetControllerType(void) const;
};

class gcGeomCurveController {
public:
    static cBase *New(cMemPool *, cBase *);
};

// String literals in .rodata.
extern const char gcGeomCurveController_name0[];   // 0x0036D894
extern const char gcGeomCurveController_name1[];   // 0x0036D89C

extern cType *D_000385DC;   // 0x000385DC
extern cType *D_0009F64C;   // 0x0009F64C
extern cType *D_0009F650;   // 0x0009F650

const cType *gcGeomCurveConfig::GetControllerType(void) const {
    if (D_0009F650 == 0) {
        if (D_0009F64C == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType(gcGeomCurveController_name0,
                                                   gcGeomCurveController_name1,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F64C = cType::InitializeType(0, 0, 0x1D5, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F650 = cType::InitializeType(0, 0, 0x1D7, D_0009F64C,
                                           &gcGeomCurveController::New,
                                           0, 0, 0);
    }
    return D_0009F650;
}
