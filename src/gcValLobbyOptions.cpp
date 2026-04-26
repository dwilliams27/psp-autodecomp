// Batch of small methods assigned to this session.

class cBase;

template <class T> T *dcast(const cBase *);

void cStrAppend(char *, const char *, ...);

extern const char gcValLobbyOptions_fmt[];  // @ 0x36F50C
extern const char gcValGetText_text[];      // @ 0x36DAF0

// gcValLobbyOptions::GetText(char *) const @ 0x0034ae98
class gcValLobbyOptions {
public:
    void GetText(char *) const;
};

void gcValLobbyOptions::GetText(char *buf) const {
    cStrAppend(buf, gcValLobbyOptions_fmt, gcValGetText_text);
}

// gcUIMarqueeControl::OnTextChanged(void) @ 0x0013c314
class gcUITextControl {
public:
    void OnTextChanged(void);
};

class gcUIMarqueeControl : public gcUITextControl {
public:
    char _pad0[0x118];
    struct MarqueeState {
        int f118;
        int f11C;
        float f120;
    } mState;
    void OnTextChanged(void);
};

void gcUIMarqueeControl::OnTextChanged(void) {
    gcUITextControl::OnTextChanged();
    mState.f120 = 0.0f;
    MarqueeState *state = &mState;
    state->f118 = 0;
}

// eAnimationGroup::AssignCopy(const cBase *) @ 0x001dc89c
class eAnimationGroup {
public:
    char _pad0[8];
    unsigned char f8;
    char _pad9[3];
    int fC;
    void AssignCopy(const cBase *);
};

void eAnimationGroup::AssignCopy(const cBase *base) {
    eAnimationGroup *other = dcast<eAnimationGroup>(base);
    f8 = other->f8;
    fC = other->fC;
}

// gcEntityCustomAttackGroup::AssignCopy(const cBase *) @ 0x002380fc
class gcEntityCustomAttackGroup {
public:
    char _pad0[8];
    unsigned char f8;
    char _pad9[3];
    int fC;
    void AssignCopy(const cBase *);
};

void gcEntityCustomAttackGroup::AssignCopy(const cBase *base) {
    gcEntityCustomAttackGroup *other = dcast<gcEntityCustomAttackGroup>(base);
    f8 = other->f8;
    fC = other->fC;
}

// gcValCameraIsValid::AssignCopy(const cBase *) @ 0x00322fb4
class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValCameraIsValid {
public:
    char _pad0[0x10];
    int f10;
    gcDesiredEntity f14;
    void AssignCopy(const cBase *);
};

void gcValCameraIsValid::AssignCopy(const cBase *base) {
    gcValCameraIsValid *other = dcast<gcValCameraIsValid>(base);
    f10 = other->f10;
    f14 = other->f14;
}
