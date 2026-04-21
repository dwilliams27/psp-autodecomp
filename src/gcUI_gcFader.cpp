// gcUI::gcFader methods

class cMemPool;
class cBase;

class gcEvent {
public:
    char pad_0[8];
    int unk8;
};

template <class T> class cHandleT {
public:
    unsigned int handle;
};

class cHandle {
public:
    unsigned int handle;
};

class eMaterial;
class eCamera;
class mOCS;
class eRect;
class eViewport;

struct cTimeValue {
    int t;
};

namespace gcUI {

class gcFader {
public:
    bool mEnabled;        // 0x00
    bool mFadeIn;         // 0x01
    bool mFadeOut;        // 0x02
    bool mLoopable;       // 0x03
    int mStartTime;       // 0x04
    int mDuration;        // 0x08
    float mAlpha;         // 0x0C
    unsigned int mMat0;   // 0x10
    float mDelay;         // 0x14
    unsigned int mEvent;  // 0x18
    unsigned int mMat1;   // 0x1C
    float mEndAlpha;      // 0x20

    gcFader(void);
    void Reset(void);
    void OnMemPoolReset(const cMemPool *pool, unsigned int flags);
    void Fade(bool fadeIn, bool fadeOut, bool loopable,
              cHandleT<eMaterial> mat0, float delay,
              cTimeValue duration, const gcEvent *evt,
              cHandle subscriber, float endAlpha);
    static void FadeImmediate(bool fadeIn, cTimeValue duration,
                              cHandleT<eMaterial> mat);
    bool Update(cTimeValue);
};

}  // namespace gcUI

namespace cObject_ns {
// cObject::WillBeDeleted is called with (cBase*, cMemPool*, unsigned int)
extern "C" int cObject_WillBeDeleted(void *obj, const cMemPool *pool, unsigned int flags);
}

using namespace gcUI;

gcFader::gcFader(void) {
    mEnabled = false;
    mFadeIn = false;
    mFadeOut = false;
    mLoopable = false;
    mStartTime = *(int *)0x36C7FC;
    mDuration = 0;
    mAlpha = 0.0f;
    mMat0 = 0;
    mDelay = 0.0f;
    mEvent = 0;
    mMat1 = 0;
}

static inline void SetInt(int *p, int v) { *p = v; }

void gcFader::Reset(void) {
    mEnabled = false;
    mFadeIn = false;
    mFadeOut = false;
    int *p = (int *)((char *)this + 8);
    *p = 0;
    int g = *(int *)0x36C7FC;
    p = (int *)((char *)this + 4);
    *p = g;
    mAlpha = 0.0f;
    mEvent = 0;
}

void gcFader::OnMemPoolReset(const cMemPool *pool, unsigned int flags) {
    if (cObject_ns::cObject_WillBeDeleted((void *)mEvent, pool, flags)) {
        mEvent = 0;
    }
}

void gcFader::Fade(bool fadeIn, bool fadeOut, bool loopable,
                   cHandleT<eMaterial> mat0, float delay,
                   cTimeValue duration, const gcEvent *evt,
                   cHandle subscriber, float endAlpha) {
    if (fadeIn) {
        mFadeIn = true;
        mFadeOut = false;
        if (fadeOut) {
            mAlpha = 0.0f;
        }
    } else {
        mFadeIn = false;
        mFadeOut = true;
        if (fadeOut) {
            mAlpha = 1.0f;
        }
    }
    mEnabled = true;
    mLoopable = loopable;

    int *p;
    p = (int *)((char *)this + 16);
    *p = (int)mat0.handle;
    mDelay = delay;
    p = (int *)((char *)this + 8);
    *p = duration.t;

    cTimeValue globalTime;
    globalTime.t = *(int *)0x40E7C;
    cTimeValue adj;
    adj.t = duration.t - (globalTime.t + globalTime.t);
    if (adj.t < 0) adj.t = 0;
    p = (int *)((char *)this + 4);
    *p = adj.t;

    unsigned int ev = 0;
    if (evt != 0) {
        if (evt->unk8 != 0) {
            ev = (unsigned int)evt;
        }
    }
    mEvent = ev;

    p = (int *)((char *)this + 28);
    *p = (int)subscriber.handle;
    mEndAlpha = endAlpha;
}

extern "C" {
    int cUpdateOS(void);
    void eVideo_BeginFrame(void);
    void eVideo_EndFrame(void);
    void eVideo_PreFlip(void);
    void eVideo_Flip(void);
    void eVideo_Clear(bool, bool, unsigned int, float, const eViewport *);
    void eVideo_SetViewport(const eViewport *);
    void gcMap_Draw(int);
    void gcViewport_Draw(void);
}

void gcFader::FadeImmediate(bool fadeIn, cTimeValue duration,
                            cHandleT<eMaterial> mat) {
    if (duration.t == 0) return;

    // Handle table lookup
    void *matPtr;
    if (mat.handle == 0) {
        matPtr = 0;
    } else {
        unsigned int idx = mat.handle & 0xffff;
        void **table = (void **)0x38890;
        void *entry = table[idx];
        matPtr = 0;
        if (entry != 0) {
            if (*(unsigned int *)((char *)entry + 0x30) == mat.handle) {
                matPtr = entry;
            }
        }
    }
    if (matPtr == 0) return;

    // Static gcFader at 0x99954
    gcFader *sFader = (gcFader *)0x99954;
    sFader->Fade(fadeIn, false, true, mat, 0.0f, duration, 0, cHandle(), 0.0f);

    cTimeValue gt;
    gt.t = *(int *)0x40E7C;
    while (cUpdateOS()) {
        if (!sFader->Update(gt)) break;
        eVideo_PreFlip();
        int map = *(int *)0x37D7FC;
        if (map != 0) {
            gcMap_Draw(map);
        } else {
            eVideo_BeginFrame();
            eVideo_SetViewport(0);
            eVideo_Clear(true, true, 0, 1.0f, 0);
            gcViewport_Draw();
            eVideo_EndFrame();
        }
        eVideo_Flip();
    }
}
