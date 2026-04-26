// -----------------------------------------------------------------------------
// cObjectKey::Read(cInStream &)  @ 0x00009b04 (cAll_psp.obj, 44B)
// -----------------------------------------------------------------------------

class cInStream;

class cInStreamRef {
public:
    void Read(unsigned int &, int, bool);
};

class cObjectKey {
public:
    unsigned int mKey;
    void Read(cInStream &);
};

void cObjectKey::Read(cInStream &s) {
    ((cInStreamRef *)&s)->Read((unsigned int &)mKey, 0x20, true);
}

// -----------------------------------------------------------------------------
// eSound::Stop(void)  @ 0x000215e0 (eAll_psp.obj, 44B)
// -----------------------------------------------------------------------------

class cTimeValue {
public:
    int mTime;
    cTimeValue() : mTime(0) {}
};

extern "C" void eAudio__StopSound_int_cTimeValuestatic(int idx, cTimeValue dt);

class eSound {
public:
    char _pad[0x30];
    short mChannelIdx;   // at offset 0x30
    void Stop(void);
};

void eSound::Stop(void) {
    if (mChannelIdx >= 0) {
        eAudio__StopSound_int_cTimeValuestatic(mChannelIdx, cTimeValue());
    }
}

// -----------------------------------------------------------------------------
// gcUIMarqueeControl::GetAdditionalDrawSize(mVec2 *, unsigned int) const
//   @ 0x0013c2e8 (gcAll_psp.obj, 44B)
// -----------------------------------------------------------------------------

struct mVec2 {
    float x;
    float y;
};

class gcUITextControl {
public:
    void GetAdditionalDrawSize(mVec2 *, unsigned int) const;
};

class gcUIMarqueeControl : public gcUITextControl {
public:
    void GetAdditionalDrawSize(mVec2 *, unsigned int) const;
};

void gcUIMarqueeControl::GetAdditionalDrawSize(mVec2 *out, unsigned int flags) const {
    gcUITextControl::GetAdditionalDrawSize(out, flags);
    out->x = 0.0f;
}

// -----------------------------------------------------------------------------
// __abort_execution  @ 0x001b9730 (gMain_psp.obj, 44B)
// -----------------------------------------------------------------------------

extern "C" {
typedef int an_error_code;
extern void display_abort_message(an_error_code);
extern void abort(void);

void __abort_execution(an_error_code code) {
    display_abort_message(code);
    abort();
    for (;;) {}
}
}

// -----------------------------------------------------------------------------
// cGetParentalLevel(void)  @ 0x00001b80 (cAll_psp.obj, 48B)
// -----------------------------------------------------------------------------

extern "C" int cGetConfigTerritory(void);

extern "C" int cGetParentalLevel(void) {
    int t = cGetConfigTerritory();
    int *p = *(int **)0x37C074 + t;
    __asm__ volatile("" ::: "memory");
    return p[165];
}
