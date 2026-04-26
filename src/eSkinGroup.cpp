// IsManagedTypeExternal / VisitReferences / AssignCopy trampolines.

class cBase;

template <class T> T *dcast(const cBase *);

class eSkinGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

class eSurfacePropertyTableGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class cObject {
public:
    void VisitReferences(unsigned int, cBase *,
        void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class eDynamicFluidTemplate : public cObject {
public:
    void VisitReferences(unsigned int, cBase *,
        void (*)(cBase *, unsigned int, void *), void *, unsigned int);
};

class gcVariableGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcStringTableGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

void eSkinGroup::AssignCopy(const cBase *base) {
    eSkinGroup *src = dcast<eSkinGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool eSurfacePropertyTableGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

void eDynamicFluidTemplate::VisitReferences(unsigned int a, cBase *b,
    void (*c)(cBase *, unsigned int, void *), void *d, unsigned int e) {
    cObject::VisitReferences(a, b, c, d, e);
}

bool gcVariableGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

bool gcStringTableGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}
