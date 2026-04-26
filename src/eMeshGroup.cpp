// IsManagedTypeExternal trampolines and AssignCopy.
// Each IsManagedTypeExternal is a non-leaf wrapper around the class's
// static IsManagedTypeExternalStatic() implementation.

class eMeshGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const class cBase *);
};

class eSkinGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eStaticLightGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eLensFlareGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eCameraEffectGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class cBase;

template <class T> T *dcast(const cBase *);

void eMeshGroup::AssignCopy(const cBase *base) {
    eMeshGroup *src = dcast<eMeshGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool eSkinGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eStaticLightGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eLensFlareGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eCameraEffectGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}
