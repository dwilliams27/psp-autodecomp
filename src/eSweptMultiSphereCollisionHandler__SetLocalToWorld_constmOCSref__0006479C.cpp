// 0x0006479c, 224B: per-sphere world->local transform for the swept multi-sphere
// handler. Calls the base eSweptVolumeCollisionHandler::SetLocalToWorld first,
// then for each of the mCount spheres transforms the world-space center through
// the OCS matrix (vtfm3.t) and rebuilds the per-sample sphere record.
// Split-TU local redeclaration of the targeted method only.

typedef unsigned int SceULong128 __attribute__((mode(TI)));

struct mOCS;

struct mSphere {
    float center[3];      // +0x0 (loaded as quad)
    float radius;         // +0xc
};

struct mSphereRec {
    SceULong128 q0;       // +0x0
    SceULong128 q1;       // +0x10
    SceULong128 q2;       // +0x20
    float w;              // +0x30
    char _pad[0xc];       // -> 0x40
};

class eSweptVolumeCollisionHandler {
public:
    void SetLocalToWorld(const mOCS &ocs);
};

class eSweptMultiSphereCollisionHandler : public eSweptVolumeCollisionHandler {
public:
    char _pad0[0x4950];
    int mCount;                  // +0x4950
    void SetLocalToWorld(const mOCS &ocs);
};

#pragma control sched=1
void eSweptMultiSphereCollisionHandler::SetLocalToWorld(const mOCS &ocs) {
    eSweptVolumeCollisionHandler::SetLocalToWorld(ocs);

    int i = 0;
    __asm__ volatile("" : "+r"(i));
    if (i < mCount) {
    mSphere *dst = (mSphere *)((char *)this + 0x49D0);   // t0
    mSphere *src = (mSphere *)((char *)this + 0x4990);   // a3
    mSphereRec *out = (mSphereRec *)((char *)this + 0x4A10); // a2
    SceULong128 *out2 = (SceULong128 *)((char *)this + 0x4A20); // a1
    do {
        float radius = src->radius;
        SceULong128 center = *(SceULong128 *)src;
        __asm__ volatile(
            "lv.q C000, 0x0(%1)\n"
            "lv.q C010, 0x10(%1)\n"
            "lv.q C020, 0x20(%1)\n"
            "lv.q C030, 0x30(%1)\n"
            "vsub.t C120, %2, C030\n"
            "vtfm3.t C130, M000, C120\n"
            "sv.q C130, 0x0(%0)\n"
            : "+r"(dst)
            : "r"(&ocs), "X"(center)
            : "memory"
        );
        dst->radius = radius;

        out->q0 = *(SceULong128 *)((char *)this + 0x80);
        out->q1 = *(SceULong128 *)((char *)this + 0x90);
        out->q2 = *(SceULong128 *)((char *)this + 0xA0);
        out->w = *(float *)((char *)this + 0xB0);

        out->q0 = *(SceULong128 *)dst;

        SceULong128 res = *(volatile SceULong128 *)dst;
        float t = *(float *)((char *)this + 0xB0);
        __asm__ volatile(
            "mfc1 $t1, %2\n"
            "mtv $t1, S100\n"
            "lv.q C130, 0xa0(%1)\n"
            "vscl.t C130, C130, S100\n"
            "vadd.t %0, %0, C130\n"
            : "+X"(res)
            : "r"(this), "f"(t)
            : "$t1", "memory"
        );
        *out2 = res;

        i++;
        dst = (mSphere *)((char *)dst + 0x10);
        src = (mSphere *)((char *)src + 0x10);
        out++;
        out2 += 4;
    } while (i < mCount);
    }
}
#pragma control sched=2
