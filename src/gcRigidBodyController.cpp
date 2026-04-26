// Multi-class wrapper file for small leaf-of-leaf methods.
// Functions: gcRigidBodyController::OnDeactivated,
//            cLanguageSubscriber::Attach,
//            gcState::GetName(char *) const,
//            gcValDriveStatus::GetText(char *) const,
//            gcValHeadset::AssignCopy(const cBase *)

class cBase;
class cType;

// ─────────────────────────────────────────────────────────────────────────
// Function 1: gcRigidBodyController::OnDeactivated(void) @ 0x001563d0
// ─────────────────────────────────────────────────────────────────────────

class gcEntityController {
public:
    void OnDeactivated(void);
    void SetPhysicsController(const cType *);
};

class gcRigidBodyController : public gcEntityController {
public:
    void OnDeactivated(void);
};

void gcRigidBodyController::OnDeactivated(void) {
    gcEntityController::OnDeactivated();
    gcEntityController::SetPhysicsController(0);
}

// ─────────────────────────────────────────────────────────────────────────
// Function 2: cLanguageSubscriber::Attach(void) @ 0x001c756c
// ─────────────────────────────────────────────────────────────────────────

class cListSubscriber {
public:
    void Attach(void);
};

class cLanguage {
public:
    enum gcSubscription {};
    static void Subscribe(gcSubscription, cListSubscriber *);
};

class cLanguageSubscriber : public cListSubscriber {
public:
    void Attach(void);
};

void cLanguageSubscriber::Attach(void) {
    cListSubscriber::Attach();
    cLanguage::Subscribe((cLanguage::gcSubscription)0, this);
}

// ─────────────────────────────────────────────────────────────────────────
// Function 3: gcState::GetName(char *) const @ 0x002591ec
// ─────────────────────────────────────────────────────────────────────────

void cStrCopy(char *, const char *);

class gcState {
public:
    const char *GetName(void) const;
    void GetName(char *) const;
};

void gcState::GetName(char *buf) const {
    cStrCopy(buf, GetName());
}

// ─────────────────────────────────────────────────────────────────────────
// Function 4: gcValDriveStatus::GetText(char *) const @ 0x00325fac
// ─────────────────────────────────────────────────────────────────────────

void cStrAppend(char *, const char *, ...);

extern const char gcValDriveStatus_fmt[];   // @ 0x36F1D0
extern const char gcValDriveStatus_text[];  // @ 0x36DAF0

class gcValDriveStatus {
public:
    void GetText(char *) const;
};

void gcValDriveStatus::GetText(char *buf) const {
    cStrAppend(buf, gcValDriveStatus_fmt, gcValDriveStatus_text);
}

// ─────────────────────────────────────────────────────────────────────────
// Function 5: gcValHeadset::AssignCopy(const cBase *) @ 0x00346e98
// ─────────────────────────────────────────────────────────────────────────

class gcValHeadset {
public:
    void AssignCopy(const cBase *);
    int pad0;
    int pad4;
    int f8;
};

gcValHeadset *dcast(const cBase *);

void gcValHeadset::AssignCopy(const cBase *base) {
    gcValHeadset *other = dcast(base);
    this->f8 = other->f8;
}
