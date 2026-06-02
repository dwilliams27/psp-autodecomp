// gcConfiguration::GetEngineConfigType(void)
//   0x000f09dc  gcAll_psp.obj  228B
//
// Lazy cType registration getter. Mirrors gcFloatSet::GetType (0x00249408)
// but the inner two InitializeType calls each pass a concrete New() function
// pointer as the 5th argument (eConfigBase::New, eConfigPSP::New), which makes
// this 8 bytes larger than the FloatSet sibling.

class cBase;
class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class eConfigBase {
public:
    static cBase *New(cMemPool *, cBase *);
};

class eConfigPSP {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcConfiguration {
public:
    const cType *GetEngineConfigType(void);
};

extern cType *D_000385DC;   // shared base "cConfigBase" type slot
extern cType *D_00040E78;   // engine intermediate type slot
extern cType *D_00040E80;   // engine config type slot

const cType *gcConfiguration::GetEngineConfigType(void)
{
    if (D_00040E80 == 0) {
        if (D_00040E78 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894, (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040E78 = cType::InitializeType(0, 0, 0x28B, D_000385DC,
                                               eConfigBase::New, 0, 0, 0);
        }
        D_00040E80 = cType::InitializeType(0, 0, 0x295, D_00040E78,
                                           eConfigPSP::New, 0, 0, 0);
    }
    return D_00040E80;
}
