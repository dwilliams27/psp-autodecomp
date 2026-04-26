class cBase;

template <class T> T *dcast(const cBase *);

class eSurfacePropertyTableGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class eSkyGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eRoomGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class ePortalGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eRoomSetGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

void eSurfacePropertyTableGroup::AssignCopy(const cBase *base) {
    eSurfacePropertyTableGroup *src = dcast<eSurfacePropertyTableGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool eSkyGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eRoomGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool ePortalGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool eRoomSetGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}
