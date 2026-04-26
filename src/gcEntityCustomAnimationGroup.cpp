class cBase;

template <class T> T *dcast(const cBase *);

class gcEntityCustomAnimationGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
    void AssignCopy(const cBase *);
};

class eInputJoystick {
public:
    static void RecheckPresent();
};

class eInputMouse {
public:
    static void RecheckPresent();
};

class eInputKeyboard {
public:
    static void RecheckPresent();
};

namespace eInput {
    void RecheckPresent();
}

class gcUI {
public:
    class gcFader {
    public:
        char _pad0[0x24];
        void Reset();
    };

    char _pad0[8];
    gcFader mFader0;
    gcFader mFader1;

    void Reset();
};

class cType;

class gcEntityController {
public:
    void OnDeselected();
    void SetPhysicsController(const cType *);
};

class gcRigidBodyController : public gcEntityController {
public:
    void OnDeselected();
};

class gcValLobbyInfo {
public:
    int _pad0;
    int _pad4;
    int mValue;
    void AssignCopy(const cBase *);
};

void gcEntityCustomAnimationGroup::AssignCopy(const cBase *base) {
    gcEntityCustomAnimationGroup *src = dcast<gcEntityCustomAnimationGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

bool gcEntityCustomAnimationGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

void eInput::RecheckPresent() {
    eInputJoystick::RecheckPresent();
    eInputMouse::RecheckPresent();
    eInputKeyboard::RecheckPresent();
}

void gcUI::Reset() {
    mFader0.Reset();
    mFader1.Reset();
}

void gcRigidBodyController::OnDeselected() {
    gcEntityController::OnDeselected();
    gcEntityController::SetPhysicsController(0);
}

void gcValLobbyInfo::AssignCopy(const cBase *base) {
    gcValLobbyInfo *src = dcast<gcValLobbyInfo>(base);
    mValue = src->mValue;
}
