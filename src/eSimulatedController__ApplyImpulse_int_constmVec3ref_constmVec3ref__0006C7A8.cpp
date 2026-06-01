// eSimulatedController::ApplyImpulse(int, const mVec3 &, const mVec3 &)
// 0x0006C7A8, 216B, eAll_psp.obj
// Isolated split-TU: local class declaration (ApplyImpulse is absent from
// include/eSimulatedController.h, which must not be edited).

#pragma control sched=2

struct mVec3;

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
    void ApplyImpulse(int index, const mVec3 &linImpulse, const mVec3 &angImpulse);

    char _pad[0x24];
    int collisionMask;      // 0x34
    void *bodyEntries;      // 0x38
};

void eSimulatedController::ApplyImpulse(int index, const mVec3 &linImpulse,
                                        const mVec3 &angImpulse) {
    float linMag;
    __asm__ volatile(
        "lv.q C120, 0(%1)\n"
        "vdot.t S100, C120, C120\n"
        "vsqrt.s S100, S100\n"
        "mfv %0, S100\n"
        : "=r"(linMag)
        : "r"(&linImpulse)
        : "memory");
    if (linMag != 0.0f) goto apply;
    {
        float angMag;
        __asm__ volatile(
            "lv.q C120, 0(%1)\n"
            "vdot.t S100, C120, C120\n"
            "vsqrt.s S100, S100\n"
            "mfv %0, S100\n"
            : "=r"(angMag)
            : "r"(&angImpulse)
            : "memory");
        if (angMag == 0.0f) return;
    }
apply:
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
                "lv.q C120, 0x40(%0)\n"
                "lv.q C130, 0(%1)\n"
                "vadd.t C120, C120, C130\n"
                "sv.q C120, 0x40(%0)\n"
                "lv.q C120, 0x50(%0)\n"
                "lv.q C130, 0(%2)\n"
                "vadd.t C120, C120, C130\n"
                "sv.q C120, 0x50(%0)\n"
                :: "r"(body), "r"(&linImpulse), "r"(&angImpulse)
                : "memory");
            if ((*(unsigned short *)((char *)body + 0x98) & 0x40) != 0) {
                ((eSimulatedState *)entry)->WakeUp();
            }
        }
    }
}
