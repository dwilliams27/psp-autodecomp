class cBase;

template <class T> T *dcast(const cBase *);

class eSurfaceSetGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class eStaticLightGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class eLensFlareGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class eCameraEffectGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class ePointGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

void eSurfaceSetGroup::AssignCopy(const cBase *base) {
    eSurfaceSetGroup *src = dcast<eSurfaceSetGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void eStaticLightGroup::AssignCopy(const cBase *base) {
    eStaticLightGroup *src = dcast<eStaticLightGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void eLensFlareGroup::AssignCopy(const cBase *base) {
    eLensFlareGroup *src = dcast<eLensFlareGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void eCameraEffectGroup::AssignCopy(const cBase *base) {
    eCameraEffectGroup *src = dcast<eCameraEffectGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool ePointGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}
