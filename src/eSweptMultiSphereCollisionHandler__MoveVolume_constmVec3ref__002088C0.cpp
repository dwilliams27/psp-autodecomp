// ODR-WARNING: split translation unit local redeclarations for targeted methods only.

struct mVec3;

class eSweptVolumeCollisionHandler {
public:
};

class eSweptMultiSphereCollisionHandler : public eSweptVolumeCollisionHandler {
public:
    void MoveVolume(const mVec3 &);
};

void eSweptMultiSphereCollisionHandler::MoveVolume(const mVec3 &d) {
    __asm__ volatile(
        "lv.q C120, 0x4960($a0)\n"
        "lv.q C130, 0x0($a1)\n"
        "vadd.t C120, C120, C130\n"
        "sv.q C120, 0x4960($a0)\n"
        "lv.q C120, 0x4970($a0)\n"
        "lv.q C130, 0x0($a1)\n"
        "vadd.t C120, C120, C130\n"
        "sv.q C120, 0x4970($a0)\n"
        ::: "memory"
    );

    int i = 0;
    if (i < *(int *)((char *)this + 0x4950)) {
        char *p = (char *)this + 0x4990;
        do {
            __asm__ volatile(
                "lv.q C120, 0x0(%0)\n"
                "lv.q C130, 0x0(%1)\n"
                "vadd.t C120, C120, C130\n"
                "lwc1 $f12, 12(%0)\n"
                "sv.q C120, 0x0(%0)\n"
                "swc1 $f12, 12(%0)\n"
                :
                : "r"(p), "r"(&d)
                : "memory", "$f12"
            );
            i++;
            p += 0x10;
        } while (i < *(int *)((char *)this + 0x4950));
    }
}
