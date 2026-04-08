#include "eMovie.h"
#include "eNavMesh.h"
#include "eDynamicFluid.h"
#include "eUser.h"

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
