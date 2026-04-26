class cBase;

template <class T> T *dcast(const cBase *);

class eSoundDataGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class gcStringTableGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class gcTriggerGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcCinematicGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcDesiredPoint;
class gcDesiredPointHelper;
class ePoint;

template <class T, class H, class O>
class gcDesiredObjectT {
public:
    H *Get(bool) const;
    cBase *GetObject(bool) const;
};

template <class T, class H, class O>
cBase *gcDesiredObjectT<T, H, O>::GetObject(bool b) const {
    return (cBase *)Get(b);
}

template class gcDesiredObjectT<gcDesiredPoint, gcDesiredPointHelper, ePoint>;

void eSoundDataGroup::AssignCopy(const cBase *base) {
    eSoundDataGroup *src = dcast<eSoundDataGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void gcStringTableGroup::AssignCopy(const cBase *base) {
    gcStringTableGroup *src = dcast<gcStringTableGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool gcTriggerGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool gcCinematicGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

