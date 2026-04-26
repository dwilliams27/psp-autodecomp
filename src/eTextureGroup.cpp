// IsManagedTypeExternal trampolines.
// Each one is a non-leaf wrapper around the class's static
// IsManagedTypeExternalStatic() implementation.

class eTextureGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class eMaterialGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
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

bool eTextureGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
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
