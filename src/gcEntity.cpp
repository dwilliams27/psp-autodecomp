#include "gcEntity.h"

extern const mVec3 mVec3_Zero;

void gcEntity::SetVelocity(const mVec3 &vel) {
    mVec3 *body = *(mVec3 **)((char *)this + 0x1F8);
    if (body != 0) {
        *body = vel;
    }
}

void *gcEntity::GetSubGeomController(int index) {
    if (index < 0) {
        return 0;
    }
    void **arr = *(void ***)((char *)this + 0x1F0);
    int count = 0;
    if (arr != 0) {
        count = *((int *)arr - 1);
    }
    if (index < count) {
        return arr[index];
    }
    return 0;
}

float gcEntity::GetSoundFrequencyOffset(int index) const {
    int flags = *(int *)((char *)this + 0x5C);
    int inactive = 1;
    if (flags & 1) inactive = 0;
    if (inactive) {
        return 0.0f;
    }
    if (index < 0) goto fail;
    {
        int count = 0;
        void **arr = *(void ***)((char *)this + 0x1D8);
        if (arr != 0) count = *((int *)arr - 1);
        if (index < count) goto success;
    fail:
        return 0.0f;
    success:
        return *(float *)((char *)arr[index] + 0x34);
    }
}

void gcEntity::GetVelocity(mVec3 *out) const {
    mVec3 *body = *(mVec3 **)((char *)this + 0x1F8);
    if (body == 0) {
        mVec3 zero = mVec3_Zero;
        body = &zero;
    }
    *out = *body;
}
