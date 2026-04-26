class cBase;

template <class T> T *dcast(const cBase *);

class ePathGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class eGeomTemplateGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class eSoundDataGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eAnimationGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eSurfaceSetGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

void ePathGroup::AssignCopy(const cBase *base) {
    ePathGroup *src = dcast<ePathGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void eGeomTemplateGroup::AssignCopy(const cBase *base) {
    eGeomTemplateGroup *src = dcast<eGeomTemplateGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool eSoundDataGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eAnimationGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eSurfaceSetGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}
