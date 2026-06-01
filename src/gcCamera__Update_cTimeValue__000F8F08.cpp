// gcCamera::Update(cTimeValue) @ 0x000f8f08
// Symbol: gcCamera__Update_cTimeValue__000F8F08  (__0fIgcCameraGUpdate6KcTimeValue)
// Split-TU: gcCamera declared locally to avoid touching shared headers.

class cTimeValue {
public:
    int mTime;
    cTimeValue() {}
    cTimeValue(int t) : mTime(t) {}
};

// Global "paused" context object, read at 0x37D7FC; only field 0x140 is touched.
struct gcCameraGlobal {
    char pad[0x140];
    unsigned int mFlags;  // 0x140
};

class eCameraEffectMgr {
public:
    void Update(cTimeValue);
};

class eCamera {
public:
    void Update(cTimeValue);
};

class gcUI {
public:
    void Update(cTimeValue);
};

class gcCamera {
public:
    void Update(cTimeValue);
    bool HandlePausedState(void);
    void UpdateMotion(cTimeValue);
    void UpdateEnvironment(cTimeValue, bool);
    void UpdateZoomAndFoV(cTimeValue);
};

// Sibling / cross-TU callees. Relocations are masked by compare_func; these
// asm() names only need to resolve at link time.
extern "C" bool gcCamera_HandlePausedState(void *)
    asm("__0fIgcCameraRHandlePausedStatev");
extern "C" void gcCamera_UpdateMotion(void *, cTimeValue)
    asm("__0fIgcCameraLUpdateMotion6KcTimeValue");
extern "C" void gcCamera_UpdateEnvironment(void *, cTimeValue, bool)
    asm("__0fIgcCameraRUpdateEnvironment6KcTimeValueb");
extern "C" void gcCamera_UpdateZoomAndFoV(void *, cTimeValue)
    asm("__0fIgcCameraOUpdateZoomAndFoV6KcTimeValue");
extern "C" void eCameraEffectMgr_Update(void *, cTimeValue)
    asm("__0fQeCameraEffectMgrGUpdate6KcTimeValue");
extern "C" void eCamera_Update(void *, cTimeValue)
    asm("__0fHeCameraGUpdate6KcTimeValue");
extern "C" void gcUI_Update(void *, cTimeValue)
    asm("__0fEgcUIGUpdate6KcTimeValue");

#define GC_CAMERA_GLOBAL (*(gcCameraGlobal **)0x37D7FC)

void gcCamera::Update(cTimeValue dt)
{
    if (dt.mTime <= 0) {
        return;
    }
    if (gcCamera_HandlePausedState(this)) {
        return;
    }

    gcCameraGlobal *g = GC_CAMERA_GLOBAL;
    if (g == 0 || ((unsigned char)((0u != (g->mFlags & 0x40)) & 0xFF)) == 0) {
        int before = *(int *)((char *)this + 0x1128);
        gcCamera_UpdateMotion(this, dt);
        gcCamera_UpdateEnvironment(this, dt,
            (unsigned char)((0u != (unsigned int)(before ^ *(int *)((char *)this + 0x1128))) & 0xFF));
    }

    eCameraEffectMgr_Update((char *)this + 0x112C, dt);
    gcCamera_UpdateZoomAndFoV(this, dt);
    eCamera_Update((char *)this + 0xED0, dt);
    gcUI_Update((char *)this + 0x11E4, dt);
    *(unsigned char *)((char *)this + 0xEC3) = 0;
}
