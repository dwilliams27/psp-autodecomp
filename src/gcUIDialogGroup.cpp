class cBase;

template <class T> T *dcast(const cBase *);

class gcUIDialogGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class gcEntityTemplateGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcEnumerationGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcStateMachineGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcEntityCustomAttackGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

void gcUIDialogGroup::AssignCopy(const cBase *base) {
    gcUIDialogGroup *src = dcast<gcUIDialogGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool gcEntityTemplateGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool gcEnumerationGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool gcStateMachineGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool gcEntityCustomAttackGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}
