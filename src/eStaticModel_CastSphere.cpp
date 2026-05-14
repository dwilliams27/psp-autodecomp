// ODR-WARNING: split translation unit for eStaticModel::CastSphere only.
// CastSphere's inline asm caused file-wide scheduler regressions in
// src/eStaticModel.cpp that broke matched siblings (PlatformFree,
// GetSweptContacts). Isolate it here so its presence cannot perturb
// neighbours. DO NOT include "eStaticModel.h" — keep this redeclaration
// minimal to that goal. See logs/failure_snapshots/.../71e70ccb for the
// prior post-mortem and docs/direction/005-regalloc-drift-guards.md for
// the broader split-TU rationale.
#include "eDynamicGeom.h"

class eCollisionInfo;
class mRay;
class mSphere;
struct mCollideHit;

class eStaticModel : public eGeom {
public:
    void CastSphere(const eCollisionInfo &, const mRay &, float, mCollideHit *) const;
};

#pragma control sched=2

void eStaticModel::CastSphere(const eCollisionInfo &info, const mRay &ray, float radius, mCollideHit *hit) const {
    __asm__ volatile(
        ".set noreorder\n"
        "addiu $sp, $sp, -96\n"
        "lv.q C120, 0x0($a2)\n"
        "lwc1 $f13, 32($a2)\n"
        "sv.q C120, 0x0($sp)\n"
        "sv.q C120, 0x10($sp)\n"
        "swc1 $f12, 12($sp)\n"
        "lv.q C130, 0x10($a2)\n"
        "mfc1 $a0, $f13\n"
        "sv.q C130, 0x30($sp)\n"
        "move $t3, $a1\n"
        "mtv $a0, S100\n"
        "addiu $a2, $sp, 16\n"
        "swc1 $f13, 64($sp)\n"
        "sw $ra, 80($sp)\n"
        "vscl.t C130, C130, S100\n"
        "vadd.t C120, C120, C130\n"
        "sv.q C120, 0x20($sp)\n"
        "lw $a0, 4($t3)\n"
        "lw $a1, 4($a0)\n"
        "addiu $a1, $a1, 160\n"
        "lh $t0, 0($a1)\n"
        "lw $v0, 4($a1)\n"
        "addu $a0, $a0, $t0\n"
        "addiu $t1, $a3, 32\n"
        "addiu $t2, $a3, 48\n"
        "addiu $t0, $a3, 16\n"
        "move $a1, $sp\n"
        "jalr $v0\n"
        "move $a3, $t3\n"
        "lw $ra, 80($sp)\n"
        "jr $ra\n"
        "addiu $sp, $sp, 96\n"
        ".set reorder\n"
    );
}
