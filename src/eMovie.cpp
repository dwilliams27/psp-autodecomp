#pragma control sched=1

#include "eMovie.h"
#include "eNavMesh.h"
#include "eDynamicFluid.h"
#include "eUser.h"

extern eMovie *D_0037D2E0;
extern int D_0037D2F0;

extern "C" {
    void eMoviePlatform___ct_eMoviePlatform_void(void *self);
    void eMoviePlatform__Initialize_voidstatic(void);
    void eMoviePlatform__Close_bool(void *self, bool force);
    void eMoviePlatform__Update_cTimeValueptr(void *self, cTimeValue *tv);
}

eMovie::eMovie(void) {
    __asm__ volatile("" ::: "memory");
    ((int *)this)[0] = 0;
    ((int *)this)[1] = 0;
    eMoviePlatform___ct_eMoviePlatform_void((char *)this + 8);
    D_0037D2E0 = this;
}

void eMovie::Initialize(void) {
    eMoviePlatform__Initialize_voidstatic();
}

void eMovie::Close(void) {
    __asm__ volatile("" ::: "memory");
    eMoviePlatform__Close_bool((char *)this + 8, true);
}

void eMovie::Update(cTimeValue *tv) {
    eMoviePlatform__Update_cTimeValueptr((char *)this + 8, tv);
}

void eMovie::Pause(void) {
    eMovie *inst = D_0037D2E0;
    if (inst != 0) {
        int c = D_0037D2F0 + 1;
        D_0037D2F0 = c;
        __asm__ volatile("" ::: "memory");
        if (c == 1) {
            inst->PlatformPause();
        }
    }
}

void eMovie::PlatformResume(void) {
}

void eNavMesh::Reset(cMemPool *, bool) {
}

void eDynamicFluid::Cull(unsigned int, const eCamera &, const mFrustum &, eCameraBins *, unsigned int, int, const cHandleT<eMaterial> *, float) const {
}

void eDynamicFluid::Draw(const eDrawInfo &) const {
}

bool eUser::PlatformSignIn(void) {
    return true;
}
