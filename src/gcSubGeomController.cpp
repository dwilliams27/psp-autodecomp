class cBase;
class cMemPool;
class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcSubGeomController {
public:
    const cType *GetType(void) const;
};

extern cType *D_000385DC;
extern cType *D_0009F64C;

const cType *gcSubGeomController::GetType(void) const {
    if (D_0009F64C == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_0009F64C = cType::InitializeType(0, 0, 0x1D5, D_000385DC, 0, 0, 0, 0);
    }
    return D_0009F64C;
}
