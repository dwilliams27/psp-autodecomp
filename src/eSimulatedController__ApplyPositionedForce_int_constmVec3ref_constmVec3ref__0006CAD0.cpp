// eSimulatedController::ApplyPositionedForce(int, const mVec3 &, const mVec3 &)
// 0x0006CAD0, 232B, eAll_psp.obj
// Isolated split-TU: local class declaration (ApplyPositionedForce is absent
// from include/eSimulatedController.h, which must not be edited).
// Sibling template: src/eSimulatedController__ApplyImpulse_..._0006C7A8.cpp

#pragma control sched=2

struct mVec3 { float x, y, z, w; };

class cBase;

class ePhysicsController {
public:
    ePhysicsController(cBase *);
    char _basePad[0x10];
};

struct eSimulatedBodyEntry {
    char _pad[0x20];
    void *body;     // 0x20
    void *cache;    // 0x24
};

class eSimulatedState {
public:
    void WakeUp(void);
};

class eSimulatedController : public ePhysicsController {
public:
    void ApplyPositionedForce(int index, const mVec3 &force, const mVec3 &pos);

    char _pad[0x24];
    int collisionMask;      // 0x34
    void *bodyEntries;      // 0x38
};

void eSimulatedController::ApplyPositionedForce(int index, const mVec3 &force,
                                               const mVec3 &pos) {
    struct { mVec3 lo, hi, top; } pad;
    float mag;
    __asm__ volatile(
        "lv.q C120, 0(%1)\n"
        "vdot.t S100, C120, C120\n"
        "vsqrt.s S100, S100\n"
        "mfv %0, S100\n"
        : "=r"(mag)
        : "r"(&pos)
        : "memory");
    if (mag < 0.00001f) return;
    {
        char *entries = (char *)bodyEntries;
        __asm__ volatile("" : "+r"(entries));
        eSimulatedBodyEntry *entry =
            (eSimulatedBodyEntry *)(entries + (index * 0x30));
        int ok = 0;
        if (entry->body != 0) {
            if (entry->cache != 0) {
                ok = 1;
            }
        }
        ok = ok & 0xFF;
        if (ok != 0) {
            void *body = entry->body;
            __asm__ volatile(
                "lv.q C120, 0(%1)\n"
                "lv.q C130, 0(%4)\n"
                "vadd.t C120, C120, C130\n"
                "sv.q C120, 0(%1)\n"
                "lv.q C120, 0(%2)\n"
                "lv.q C130, 0x30(%0)\n"
                "vsub.t C120, C120, C130\n"
                "lv.q C130, 0x10(%3)\n"
                "lv.q C200, 0(%4)\n"
                "vcrsp.t C130, C120, C200\n"
                "sv.q C130, 0x10(%3)\n"
                "lv.q C000, 0(%0)\n"
                "lv.q C010, 0x10(%0)\n"
                "lv.q C020, 0x20(%0)\n"
                "lv.q C030, 0x30(%0)\n"
                "vtfm3.t C120, M000, C130\n"
                "lv.q C130, 0x10(%1)\n"
                "vadd.t C120, C130, C120\n"
                "sv.q C120, 0x10(%1)\n"
                :: "r"(body), "r"(entry), "r"(&force), "r"(&pad), "r"(&pos)
                : "memory");
            if ((*(unsigned short *)((char *)body + 0x98) & 0x40) != 0) {
                ((eSimulatedState *)entry)->WakeUp();
            }
        }
    }
}
