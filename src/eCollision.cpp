#include "eCollision.h"
#include "eVideo.h"

int eCollision::GetTriTriContacts(const eCollisionSupport *, const eCollisionSupport *, eContactResult *) {
    return 0;
}

void eMaterial::PlatformFree(void) {
}

void eMaterial::CopyMaterialData(eMaterialData *) {
}

void eCameraEffectGeom::Cull(unsigned int, const eCamera &, const mFrustum &, eCameraBins *, unsigned int, int, const cHandleT<eMaterial> *, float) const {
}

void eVideo::PreFlip(void) {
}
