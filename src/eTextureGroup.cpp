// IsManagedTypeExternal trampolines.
// Each one is a non-leaf wrapper around the class's static
// IsManagedTypeExternalStatic() implementation.

class eTextureGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
    void AssignCopy(const class cBase *);
};

class eMaterialGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
    void AssignCopy(const class cBase *);
};

class eMaterialSetGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eSurfaceGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eSpriteGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class ePathGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eMeshGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eGeomTemplateGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class cBase;

template <class T> T *dcast(const cBase *);

void eTextureGroup::AssignCopy(const cBase *base) {
    eTextureGroup *src = dcast<eTextureGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool eTextureGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

void eMaterialGroup::AssignCopy(const cBase *base) {
    eMaterialGroup *src = dcast<eMaterialGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool eMaterialGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eMaterialSetGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eSurfaceGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eSpriteGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool ePathGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eMeshGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eGeomTemplateGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}
