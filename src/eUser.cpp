#include "eUser.h"
#include "eBoxShape.h"

extern unsigned short gEUserSignedIn;    // 0x37D2F8
extern unsigned short gEUserActiveMask;  // 0x37D2FC
extern const char gEUserNames[][20];     // 0x46B48

void cStrCopy(char *, const char *);

void eUser::PlatformUpdate(void) {
}

void eUser::Update(void) {
    PlatformUpdate();
}

#pragma control sched=1
void eUser::SignIn(unsigned short user) {
    gEUserSignedIn = user;
    PlatformSignIn();
}

bool eUser::GetUserName(unsigned short user, char *dst) {
    unsigned short m = gEUserActiveMask;
    int pos = user & 0xFFFF;
    __asm__ volatile("" ::: "memory");
    int val = (m & (1 << pos)) != 0;
    int found = val & 0xFF;
    if (found) {
        cStrCopy(dst, gEUserNames[user]);
        return true;
    }
    return false;
}
#pragma control sched=2

#pragma control sched=1
void eUser::ChangeMask(unsigned short pos, bool set, unsigned short &mask) {
    unsigned short m = mask;
    __asm__ volatile("" ::: "memory");
    if (set) {
        mask = m | (1 << pos);
    } else {
        mask = m & ~(1 << pos);
    }
}
#pragma control sched=2

float eBoxShape::GetVolume(void) const {
    int bits;
    float tmp[4];
    __asm__ volatile(
        "lui $a1, 0x4000\n"
        "mtc1 $a1, $f12\n"
        "mfc1 $a1, $f12\n"
        "mtv $a1, S100\n"
        : "=r"(bits) :: "$a1", "$f12"
    );
    __asm__ volatile(
        "lv.q C120, 0x80(%0)\n"
        "vscl.t C120, C120, S100\n"
        "sv.q C120, 0(%1)\n"
        :: "r"(this), "r"(tmp)
        : "memory"
    );
    return tmp[0] * tmp[1] * tmp[2];
}
