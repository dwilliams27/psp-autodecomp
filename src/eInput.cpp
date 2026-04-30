// Local declarations to avoid header conflicts; matches the project's
// pattern (e.g. src/eRigidBodyState.cpp) for tightly-scoped TUs.

class eInputJoystick {
public:
    static bool Initialize();
    static void Reset();
    static void ResetIdleTime(int);
};

class eInputMouse {
public:
    static bool Initialize();
    static void Reset();
};

class eInputKeyboard {
public:
    static bool Initialize();
    static void Reset();
};

namespace eInput {
    bool Initialize();
    void Reset();
    void ResetIdleTime();
}

// ── eInput::Initialize(void) static @ 0x00030d14 ──
bool eInput::Initialize() {
    if (eInputJoystick::Initialize() &&
        eInputMouse::Initialize() &&
        eInputKeyboard::Initialize()) {
        Reset();
        return true;
    }
    return false;
}

// ── eInput::Reset(void) static @ 0x00030d74 ──
void eInput::Reset() {
    eInputJoystick::Reset();
    eInputMouse::Reset();
    eInputKeyboard::Reset();
    *(char *)0x37D14C = 0;
}

// ── eInput::ResetIdleTime(void) static @ 0x00030e24 ──
struct JoyIdle {
    char pad[0x88];
    int  timeAccum;   // 0x88
};
struct KbdIdle {
    char pad[0xA0C];
    int  timeAccum;   // 0xA0C
};
extern JoyIdle D_00045338;
extern KbdIdle D_000454A8;

void eInput::ResetIdleTime() {
    eInputJoystick::ResetIdleTime(-1);
    D_00045338.timeAccum = 0;
    D_000454A8.timeAccum = 0;
}

// ── eFont::HasTags(const wchar_t *) static @ 0x00039b14 ──
class eFont {
public:
    struct eTag {
        int   type;        // 0
        int   value;       // 4
        short flag;        // 8
        char  pad[510];    // 10..519
        int   field520;    // 520
    };
    static bool HasTags(const wchar_t *str);
    static void ParseNextTag(const wchar_t *str, eTag *tag);
};

bool eFont::HasTags(const wchar_t *str) {
    eTag tag;
    tag.type     = 0;
    tag.value    = 0;
    tag.flag     = 0;
    tag.field520 = 0;
    ParseNextTag(str, &tag);
    return tag.type != 0;
}

// ── eRigidBodyController::eRigidBodyController(cBase *) @ 0x000768c8 ──
class cBase;
class eSimulatedController {
public:
    eSimulatedController(cBase *);
};
class eRigidBodyController : public eSimulatedController {
public:
    eRigidBodyController(cBase *);
};

eRigidBodyController::eRigidBodyController(cBase *cb) : eSimulatedController(cb) {
    *(void **)((char *)this + 4) = (void *)0x384078;
}

// ── eRigidBodyControllerConfig::eRigidBodyControllerConfig(cBase *) @ 0x00077048 ──
class eSimulatedControllerConfig {
public:
    eSimulatedControllerConfig(cBase *);
};
class eRigidBodyControllerConfig : public eSimulatedControllerConfig {
public:
    eRigidBodyControllerConfig(cBase *);
};

eRigidBodyControllerConfig::eRigidBodyControllerConfig(cBase *cb)
        : eSimulatedControllerConfig(cb) {
    *(void **)((char *)this + 4) = (void *)0x384188;
}
