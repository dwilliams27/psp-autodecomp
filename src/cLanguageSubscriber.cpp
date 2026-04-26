// Batch of unrelated functions grouped by the orchestrator into one TU.
// Sections are independent.

// ─────────────────────────────────────────────────────────────────────────
// Function 1: cLanguageSubscriber::GetIndex(void *) const @ 0x001c75d0
// Function 2: cLanguageSubscriber::Detach(void)         @ 0x001c759c
// ─────────────────────────────────────────────────────────────────────────

class cListSubscriber {
public:
    void Detach(void);
};

class cLanguage {
public:
    enum gcSubscription {};
    static int GetLanguageIndex(void *);
    static void Unsubscribe(gcSubscription, cListSubscriber *);
};

class cLanguageSubscriber : public cListSubscriber {
public:
    int GetIndex(void *) const;
    void Detach(void);
};

int cLanguageSubscriber::GetIndex(void *p) const {
    return cLanguage::GetLanguageIndex(p);
}

void cLanguageSubscriber::Detach(void) {
    cLanguage::Unsubscribe((cLanguage::gcSubscription)0, this);
    cListSubscriber::Detach();
}

// ─────────────────────────────────────────────────────────────────────────
// Function 3: eFontGroup::IsManagedTypeExternal(void) const @ 0x001dd40c
// ─────────────────────────────────────────────────────────────────────────

class eFontGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

bool eFontGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ─────────────────────────────────────────────────────────────────────────
// Function 4: gcConstantGroup::IsManagedTypeExternal(void) const @ 0x002365ec
// ─────────────────────────────────────────────────────────────────────────

class gcConstantGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

bool gcConstantGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ─────────────────────────────────────────────────────────────────────────
// Function 5: gcExternalCinematicGroup::IsManagedTypeExternal(void) const @ 0x00238a04
// ─────────────────────────────────────────────────────────────────────────

class gcExternalCinematicGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

bool gcExternalCinematicGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}
