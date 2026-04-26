class cBase;

template <class T> T *dcast(const cBase *);

class eRoomGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class eRoomSetGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class gcStaticInstanceGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcFunctionGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcUIDialogGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

void eRoomGroup::AssignCopy(const cBase *base) {
    eRoomGroup *src = dcast<eRoomGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void eRoomSetGroup::AssignCopy(const cBase *base) {
    eRoomSetGroup *src = dcast<eRoomSetGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool gcStaticInstanceGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool gcFunctionGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool gcUIDialogGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}
