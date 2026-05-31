// gcCinematicInstance::Play(cHandle, float)
// Address: 0x000eb3e8, Size: 264B
// Obj: gcAll_psp.obj
// Split-TU local class redeclaration — Play/GetUI/OnCinematicEvent are not in
// include/gcCinematic.h and must NOT be added there (banned).

#define NULL 0

class cHandle {
public:
    int mIndex;
};

class cTimeValue {
public:
    int mTime;
    cTimeValue() { mTime = 0; }
};

class gcCinematicInstance {
public:
    char pad_00[0x08];
    cHandle mHandle;                 // 0x08
    float mTime;                     // 0x0C
    cTimeValue mField10;             // 0x10
    cTimeValue mField14;             // 0x14
    int mField18;                    // 0x18
    char pad_1C[0x1C - 0x1C];
    int mUI;                         // 0x1C
    char pad_20[0x3C - 0x20];
    int mField3C;                    // 0x3C
    int mField40;                    // 0x40
    char pad_44[0x48 - 0x44];
    gcCinematicInstance *mPrev;      // 0x48
    gcCinematicInstance *mNext;      // 0x4C
    gcCinematicInstance *mPrev2;     // 0x50
    gcCinematicInstance *mNext2;     // 0x54

    void Play(cHandle handle, float time);
    int GetUI(void) const;

    enum gcCinematicEventType { };
    void OnCinematicEvent(gcCinematicEventType);
};

// The object referenced by *(this+0) — its mFirstInstance lives at +0x7C.
struct gcCinematicOwner {
    char pad[0x7C];
    gcCinematicInstance *mFirstInstance; // 0x7C
};

struct gcCinematicInstanceLink {
    gcCinematicOwner *mpOwner;       // 0x00
};

extern gcCinematicInstance *D_0037D7C4; // global head, lui 0x38 / -10300
extern int D_0037D7B0;                  // flag, lui 0x38 / -10320

void gcCinematicInstance::Play(cHandle handle, float time) {
    gcCinematicOwner *owner = ((gcCinematicInstanceLink *)this)->mpOwner;
    if (owner->mFirstInstance != NULL) {
        this->mPrev = owner->mFirstInstance->mPrev;
        this->mNext = owner->mFirstInstance;
        this->mPrev->mNext = this;
        this->mNext->mPrev = this;
    } else {
        owner->mFirstInstance = this;
        this->mNext = this;
        this->mPrev = this;
    }

    if (D_0037D7C4 != NULL) {
        this->mPrev2 = D_0037D7C4->mPrev2;
        this->mNext2 = D_0037D7C4;
        this->mPrev2->mNext2 = this;
        this->mNext2->mPrev2 = this;
    } else {
        D_0037D7C4 = this;
        this->mNext2 = this;
        this->mPrev2 = this;
    }

    this->mHandle = handle;
    this->mTime = time;
    this->mField10 = cTimeValue();
    this->mField14 = cTimeValue();
    this->mField18 = 0;
    this->mField3C = 0;
    this->mField40 = 0;
    this->mUI = this->GetUI();

    if (((unsigned char)(D_0037D7B0 == 1)) != 0) {
        this->OnCinematicEvent((gcCinematicEventType)0);
    }
}
