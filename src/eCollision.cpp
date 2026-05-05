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

// ── eCollision::UpdateSimplexLine @ 0x00029dc8 ──
void __attribute__((naked)) eCollision::UpdateSimplexLine(mVec3 *, int *, mVec3 *) {
    __asm__ volatile(
        ".set push\n"
        ".set noreorder\n"
        "addiu $sp, $sp, -48\n"
        "addiu $a3, $a0, 16\n"
        "lv.q C130, 0($a3)\n"
        "vneg.t C130, C130\n"
        "sv.q C130, 0($sp)\n"
        "lv.q C120, 0($a0)\n"
        "lv.q C200, 0($a3)\n"
        "vsub.t C120, C120, C200\n"
        "sv.q C120, 16($sp)\n"
        "vdot.t S100, C120, C130\n"
        "mfv $t0, S100\n"
        "mtc1 $zero, $f13\n"
        "mtc1 $t0, $f12\n"
        "c.le.s $f12, $f13\n"
        "nop\n"
        "bc1t 1f\n"
        "nop\n"
        "lv.q C200, 32($sp)\n"
        "vcrsp.t C200, C120, C130\n"
        "sv.q C200, 32($sp)\n"
        "lv.q C130, 0($a2)\n"
        "vcrsp.t C130, C200, C120\n"
        "sv.q C130, 0($a2)\n"
        "jr $ra\n"
        "addiu $sp, $sp, 48\n"
        "1:\n"
        "lv.q C120, 0($a3)\n"
        "sv.q C120, 0($a0)\n"
        "lv.q C120, 0($sp)\n"
        "ori $a0, $zero, 1\n"
        "sv.q C120, 0($a2)\n"
        "sw $a0, 0($a1)\n"
        "jr $ra\n"
        "addiu $sp, $sp, 48\n"
        ".set pop\n"
    );
}

// ── eCollision::UpdateSimplex @ 0x0002a6f0 ──
int eCollision::UpdateSimplex(mVec3 *simplex, int *count, mVec3 *direction) {
    int n = *count;
    if (n < 3) {
        if (n >= 2) {
            UpdateSimplexLine(simplex, count, direction);
        }
        return 0;
    }
    if (n < 4) {
        UpdateSimplexTri(simplex, count, direction);
        return 0;
    }
    if (n >= 5) return 0;
    return UpdateSimplexTetra(simplex, count, direction);
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
