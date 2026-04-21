#include "eCollision.h"
#include "eVideo.h"
#include "mVec3.h"

extern "C" void *memcpy(void *, const void *, unsigned int);

int eCollision::GetTriTriContacts(const eCollisionSupport *, const eCollisionSupport *, eContactResult *) {
    return 0;
}

// ── eCollision::GetTriFaceContacts @ 0x0002b584 ──
int eCollision::GetTriFaceContacts(const eCollisionSupport *a, const eCollisionSupport *b, eContactResult *r) {
    return Clip(a, b, r, *(const unsigned int *)((const char *)a + 0x554));
}

// ── eCollision::GetTriCircleContacts @ 0x0002b5a0 ──
int eCollision::GetTriCircleContacts(const eCollisionSupport *a, const eCollisionSupport *b, eContactResult *r) {
    return ClipCircle(a, b, r, *(const unsigned int *)((const char *)a + 0x554));
}

// ── eCollision::GetFaceFaceContacts @ 0x0002b5bc ──
int eCollision::GetFaceFaceContacts(const eCollisionSupport *a, const eCollisionSupport *b, eContactResult *r) {
    return Clip(a, b, r, (unsigned int)-1);
}

// ── eCollision::GetFaceCircleContacts @ 0x0002b5d8 ──
int eCollision::GetFaceCircleContacts(const eCollisionSupport *a, const eCollisionSupport *b, eContactResult *r) {
    return ClipCircle(a, b, r, (unsigned int)-1);
}

// ── eCollision::GJKCopySimplex @ 0x0002aa28 ──
void eCollision::GJKCopySimplex(int count, const mVec3 *src, int *outCount, mVec3 *dst) {
    if (dst != 0) {
        memcpy(dst, src, count * 16);
        *outCount = count;
    }
}

void eMaterial::PlatformFree(void) {
}

void eMaterial::CopyMaterialData(eMaterialData *) {
}

void eCameraEffectGeom::Cull(unsigned int, const eCamera &, const mFrustum &, eCameraBins *, unsigned int, int, const cHandleT<eMaterial> *, float) const {
}

void eVideo::PreFlip(void) {
}
