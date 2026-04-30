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

class gcEntityGeomConfig {
public:
    char _base[0x50];
    bool mBool50;
    char _pad51[0xF];

    gcEntityGeomConfig(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

class cHandle {
public:
    int mIndex;
};

void gcEvent_gcEvent(void *, cBase *, const char *);

class gcGeomCurveConfig : public gcEntityGeomConfig {
public:
    bool mBool60;
    char _pad61[3];
    int mInt64;
    char mEvent68[0x1C];
    int mTaggedBase84;
    unsigned int mValue88;
    bool mBool8C;
    char _pad8D[3];
    cHandle mHandle90;
    unsigned int mValue94;
    void *mHelpers98;

    gcGeomCurveConfig(cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

class gcGeomTrailConfig {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

class gcPartialEntityController {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcLookAtController {
public:
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

extern cType *D_000385DC;
extern cType *D_0009F43C;
extern cType *D_0009F5E0;
extern cType *D_0009F648;
extern cType *D_0009F784;
extern cType *D_0009F78C;

// gcGeomCurveConfig::gcGeomCurveConfig(cBase *) @ 0x001546f8
gcGeomCurveConfig::gcGeomCurveConfig(cBase *parent)
    : gcEntityGeomConfig(parent) {
    *(void **)((char *)this + 4) = (void *)0x38C9E8;
    mBool60 = 0;
    mInt64 = 1;
    gcEvent_gcEvent((char *)this + 0x68, (cBase *)this, (const char *)0x36E5D4);
    mTaggedBase84 = (int)((unsigned int)this | 1);
    mValue88 = (unsigned int)-1;
    mBool8C = 0;
    mHandle90.mIndex = 0;
    mValue94 = 0;
    mHelpers98 = 0;
    mBool50 = 1;
}

// gcGeomCurveConfig::GetType(void) const @ 0x0031bd84
const cType *gcGeomCurveConfig::GetType(void) const {
    if (D_0009F784 == 0) {
        if (D_0009F43C == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F43C = cType::InitializeType(0, 0, 0xA0, D_000385DC,
                                               &gcEntityGeomConfig::New,
                                               0, 0, 0);
        }
        D_0009F784 = cType::InitializeType(0, 0, 0x1D6, D_0009F43C,
                                           &gcGeomCurveConfig::New,
                                           0, 0, 0);
    }
    return D_0009F784;
}

// gcGeomTrailConfig::GetType(void) const @ 0x0031cb4c
const cType *gcGeomTrailConfig::GetType(void) const {
    if (D_0009F78C == 0) {
        if (D_0009F43C == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F43C = cType::InitializeType(0, 0, 0xA0, D_000385DC,
                                               &gcEntityGeomConfig::New,
                                               0, 0, 0);
        }
        D_0009F78C = cType::InitializeType(0, 0, 0x255, D_0009F43C,
                                           &gcGeomTrailConfig::New,
                                           0, 0, 0);
    }
    return D_0009F78C;
}

// gcLookAtController::GetType(void) const @ 0x002c6d78
const cType *gcLookAtController::GetType(void) const {
    if (D_0009F648 == 0) {
        if (D_0009F5E0 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F5E0 = cType::InitializeType(0, 0, 0x105, D_000385DC,
                                               &gcPartialEntityController::New,
                                               0, 0, 0);
        }
        D_0009F648 = cType::InitializeType(0, 0, 0x10E, D_0009F5E0,
                                           &gcLookAtController::New,
                                           0, 0, 0);
    }
    return D_0009F648;
}
