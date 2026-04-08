#include "gcGameCore.h"

struct gcCamera {
    struct gcState {};
};

struct eCamera;
struct mOCS;
struct mVec2;

struct gcCameraModes {
    static void UpdateLookFromPoint(gcCamera &, gcCamera::gcState &, cTimeValue);
};

struct gcVoiceOver {
    int IsVoiceOver(void) const;
};

struct gcUIControl {
    void OnDraw(const eCamera &, const mOCS &, const mVec2 &, const mVec2 &, float, float, float, unsigned int);
};

struct gcEntityGeomConfig {
    int GetControllerType(void) const;
};

struct gcTableColumn {
    int IsValid(void) const;
};

void gcCameraModes::UpdateLookFromPoint(gcCamera &, gcCamera::gcState &, cTimeValue) {
}

int gcVoiceOver::IsVoiceOver(void) const {
    return 1;
}

void gcUIControl::OnDraw(const eCamera &, const mOCS &, const mVec2 &, const mVec2 &, float, float, float, unsigned int) {
}

int gcEntityGeomConfig::GetControllerType(void) const {
    return 0;
}

int gcTableColumn::IsValid(void) const {
    return 1;
}
