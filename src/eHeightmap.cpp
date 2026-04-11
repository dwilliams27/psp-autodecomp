#include "eHeightmap.h"
#include "eMemCard.h"
#include "eMovie.h"
#include "ePath.h"

void eHeightmap::PlatformReset(cMemPool *, bool) {
}

int eMemCard::Reset(void) {
    return 1;
}

void eMemCard::EndLoad(const eMemCard::eLoadParams &) {
}

void eMemCard::EndSave(const eMemCard::eSaveParams &) {
}

void eMovie::PlatformPause(void) {
}

void eHeightmap::CastSphere(const eCollisionInfo &info, const mRay &ray, float radius, mCollideHit *hit) const {
    // Body is VFPU ray processing + vtable dispatch.
    // Written as inline asm with embedded prologue/epilogue since the compiler
    // cannot infer them from .word VFPU instructions.
    __asm__ volatile(
        ".set noreorder\n"
        "addiu $sp, $sp, -96\n"
        "sw $ra, 80($sp)\n"
        "move $t3, $a1\n"
        ".word 0xd8c60000\n"         // lv.q C120, 0(a2)
        ".word 0xfba60000\n"         // sv.q C120, 0(sp)
        "swc1 $f12, 12($sp)\n"
        "lwc1 $f12, 32($a2)\n"
        ".word 0xfba60010\n"         // sv.q C120, 0x10(sp)
        "addiu $v0, $sp, 16\n"
        ".word 0xd8c70010\n"         // lv.q C130, 0x10(a2)
        ".word 0xfba70030\n"         // sv.q C130, 0x30(sp)
        "swc1 $f12, 64($sp)\n"
        "mfc1 $a0, $f12\n"
        ".word 0x48e40004\n"         // mtv a0, S100
        ".word 0x65048707\n"         // vscl.t C130, C130, S100
        ".word 0x60078606\n"         // vadd.t C120, C120, C130
        ".word 0xfba60020\n"         // sv.q C120, 0x20(sp)
        "lw $a0, 4($t3)\n"
        "lw $a1, 4($a0)\n"
        "addiu $a1, $a1, 160\n"
        "lh $a2, 0($a1)\n"
        "addu $a0, $a0, $a2\n"
        "addiu $t0, $a3, 16\n"
        "addiu $t1, $a3, 32\n"
        "addiu $t2, $a3, 48\n"
        "lw $v1, 4($a1)\n"
        "move $a1, $sp\n"
        "move $a2, $v0\n"
        "jalr $v1\n"
        "move $a3, $t3\n"
        "lw $ra, 80($sp)\n"
        "jr $ra\n"
        "addiu $sp, $sp, 96\n"
        ".set reorder\n"
    );
}

float ePath::PathT2Units(float t) const {
    int idx = (int)t;
    float nextT = t + 1.0f;
    ePathPoint *points = mPoints;
    float maxIdx;

    if (points != 0) {
        maxIdx = (float)((*((int *)points - 1) & 0x3FFFFFFF) - 1);
    } else {
        maxIdx = -1.0f;
    }

    int endIdx;
    if (nextT <= 0.0f) {
        __asm__ volatile(
            ".word 0x46007b4d\n"
            "mfc1 %0, $f13"
            : "=r"(endIdx)
        );
    } else {
        if (maxIdx <= nextT) {
            nextT = maxIdx;
        }
        endIdx = (int)nextT;
    }

    float startDist = points[idx].mDistance;
    float endDist = points[endIdx].mDistance;
    return startDist + (t - (float)idx) * (endDist - startDist);
}
