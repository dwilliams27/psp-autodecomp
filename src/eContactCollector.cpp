#pragma control sched=1
#include "eContactCollector.h"
#include "eContactForceSolver.h"
#include "mVec3.h"

void eContactCollector::ClearAllContacts(void) {
    int i = 0;
    do {
        ((int *)((char *)this + 0x4860))[i] = 0;
        i++;
    } while (i < 4);
}

void eContactCollector::FindSlideVelocity(mVec3 *v) const {
    eContactForceSolver solver;
    const int *cp = (const int *)((const char *)this + 0x4860);
    int n = *cp;
    if (n != 0) {
        solver.ComputeForces(v, (const eContact *)((const char *)this + 0x60), n);
    }
}

int eContactCollector::ValidateTri(eContact *contacts, int n) {
    char *ref = (char *)contacts + n * 0x90;
    int t2 = 0;
    if (t2 < n) {
        int eps_tmp;
        __asm__ volatile(
            "lui %0, 0x3727\n"
            "ori %0, %0, 0xc5ac\n"
            "mtc1 %0, $f12\n"
            "mfc1 %0, $f12\n"
            "mtv %0, S100\n"
            : "=r"(eps_tmp) :: "$f12"
        );
    do {
        int flags = *(int *)((char *)contacts + 0x34);
        if (flags & 1) {
            int t1 = 1;
            int t0 = 0;
            char *refV;
            __asm__ ("addiu %0, %1, 64" : "=r"(refV) : "r"(ref));
            char *conV = (char *)contacts + 0x40;
            do {
                int v0;
                __asm__ volatile(
                    "lv.q C120, 0(%1)\n"
                    "lv.q C130, 0(%2)\n"
                    "vsub.t C120, C120, C130\n"
                    "vabs.t C120, C120\n"
                    "vpfxt X, X, X, W\n"
                    "vcmp.t le, C120, C100\n"
                    "vnop\n"
                    "mfvc %0, $131\n"
                    "andi %0, %0, 0x20\n"
                    "sltu %0, $zero, %0\n"
                    "andi %0, %0, 0xff\n"
                    : "=r"(v0)
                    : "r"(refV), "r"(conV)
                );
                t0++;
                if (v0 == 0) {
                    t1 = 0;
                    break;
                }
                refV += 16;
                conV += 16;
            } while (t0 < 3);
            if (t1) return 0;
        }
        t2++;
        contacts = (eContact *)((char *)contacts + 0x90);
    } while (t2 < n);
    }
    return 1;
}

struct SoftItem {
    char  pad00[0x20]; // 0x00-0x1F
    float f20;         // 0x20
    int   i24;         // 0x24
    int   i28;         // 0x28
    int   i2c;         // 0x2C
    int   i30;         // 0x30
    int   i34;         // 0x34
    char  pad38[0x44]; // 0x38-0x7B
    float f7c;         // 0x7C
    int   i80;         // 0x80
};

struct CollectorThis {
    char  pad[0x2480];
    float check_val;
};

int eContactCollector::ValidatePoint(eContact *contacts, int index, int n) {
    char stack[64];  // 4x v4sf_t
    char *p0 = stack;
    char *p1 = stack + 0x10;
    char *p2 = stack + 0x20;
    char *p3 = stack + 0x30;

    eContact *base = (eContact *)((char *)contacts + index * 0x90);
    int flags0 = *(int *)((char *)base + 0x34);
    int t3;
    if (flags0 == 0x40) {
        __asm__ volatile(
            "lv.q C120, 0x60(%0)\n"
            "sv.q C120, 0(%1)\n"
            :: "r"(base), "r"(p0) : "memory"
        );
        t3 = 0;
    } else if (flags0 == 0x20) {
        __asm__ volatile(
            "lv.q C120, 0x50(%0)\n"
            "sv.q C120, 0(%1)\n"
            :: "r"(base), "r"(p0) : "memory"
        );
        t3 = 0;
    } else if (flags0 == 0x10) {
        t3 = 0;
        __asm__ volatile(
            "lv.q C120, 0x40(%0)\n"
            "sv.q C120, 0(%1)\n"
            :: "r"(base), "r"(p0) : "memory"
        );
    } else {
        t3 = 0;
    }

    if (t3 < n) {
        int eps_tmp;
        __asm__ volatile(
            "lv.q C210, 0(%1)\n"
            "lui %0, 0x3727\n"
            "ori %0, %0, 0xc5ac\n"
            "mtc1 %0, $f12\n"
            "mfc1 %0, $f12\n"
            "mtv %0, S100\n"
            : "=r"(eps_tmp) : "r"(p0) : "$f12", "memory"
        );
        __asm__ volatile(
            "lv.q C200, 0(%0)\n"
            "lv.q C130, 0(%1)\n"
            "lv.q C120, 0(%2)\n"
            :: "r"(p1), "r"(p2), "r"(p3) : "memory"
        );

        do {
            if (t3 == index) {
                t3++;
                contacts = (eContact *)((char *)contacts + 0x90);
                continue;
            }
            int flags = *(int *)((char *)contacts + 0x34);
            if (flags & 0x70) {
                if (t3 < index) {
                    int flags2 = *(int *)((char *)contacts + 0x34);
                    if (flags2 == 0x40) {
                        __asm__ volatile("lv.q C200, 0x60(%0)\n" :: "r"(contacts));
                    } else if (flags2 == 0x20) {
                        __asm__ volatile("lv.q C200, 0x50(%0)\n" :: "r"(contacts));
                    } else if (flags2 == 0x10) {
                        __asm__ volatile("lv.q C200, 0x40(%0)\n" :: "r"(contacts));
                    }
                    int res;
                    __asm__ volatile(
                        "vsub.t C220, C200, C210\n"
                        "vabs.t C220, C220\n"
                        "vpfxt X, X, X, W\n"
                        "vcmp.t le, C220, C100\n"
                        "vnop\n"
                        "mfvc %0, $131\n"
                        "andi %0, %0, 0x20\n"
                        "sltu %0, $zero, %0\n"
                        "andi %0, %0, 0xff\n"
                        : "=r"(res)
                    );
                    if (res) {
                        __asm__ volatile(
                            "sv.q C200, 0(%0)\n"
                            "sv.q C130, 0(%1)\n"
                            "sv.q C120, 0(%2)\n"
                            :: "r"(p1), "r"(p2), "r"(p3) : "memory"
                        );
                        return 0;
                    }
                    goto advance;
                }
            }
            if (flags & 0xe) {
                int flags3 = *(int *)((char *)contacts + 0x34);
                if (flags3 == 8) {
                    __asm__ volatile(
                        "lv.q C130, 0x60(%0)\n"
                        "lv.q C120, 0x40(%0)\n"
                        :: "r"(contacts)
                    );
                } else if (flags3 == 4) {
                    __asm__ volatile(
                        "lv.q C130, 0x50(%0)\n"
                        "lv.q C120, 0x60(%0)\n"
                        :: "r"(contacts)
                    );
                } else if (flags3 == 2) {
                    __asm__ volatile(
                        "lv.q C130, 0x40(%0)\n"
                        "lv.q C120, 0x50(%0)\n"
                        :: "r"(contacts)
                    );
                }
                int res1;
                __asm__ volatile(
                    "vsub.t C220, C130, C210\n"
                    "vabs.t C220, C220\n"
                    "vpfxt X, X, X, W\n"
                    "vcmp.t le, C220, C100\n"
                    "vnop\n"
                    "mfvc %0, $131\n"
                    "andi %0, %0, 0x20\n"
                    "sltu %0, $zero, %0\n"
                    "andi %0, %0, 0xff\n"
                    : "=r"(res1)
                );
                if (res1) {
                    __asm__ volatile(
                        "sv.q C200, 0(%0)\n"
                        "sv.q C130, 0(%1)\n"
                        "sv.q C120, 0(%2)\n"
                        :: "r"(p1), "r"(p2), "r"(p3) : "memory"
                    );
                    return 0;
                }
                int res2;
                __asm__ volatile(
                    "vsub.t C220, C120, C210\n"
                    "vabs.t C220, C220\n"
                    "vpfxt X, X, X, W\n"
                    "vcmp.t le, C220, C100\n"
                    "vnop\n"
                    "mfvc %0, $131\n"
                    "andi %0, %0, 0x20\n"
                    "sltu %0, $zero, %0\n"
                    "andi %0, %0, 0xff\n"
                    : "=r"(res2)
                );
                if (res2) {
                    __asm__ volatile(
                        "sv.q C200, 0(%0)\n"
                        "sv.q C130, 0(%1)\n"
                        "sv.q C120, 0(%2)\n"
                        :: "r"(p1), "r"(p2), "r"(p3) : "memory"
                    );
                    return 0;
                }
            } else if (flags & 1) {
                int j = 0;
                char *vp = (char *)contacts + 0x40;
                do {
                    int res3;
                    __asm__ volatile(
                        "lv.q C220, 0(%1)\n"
                        "vsub.t C220, C220, C210\n"
                        "vabs.t C220, C220\n"
                        "vpfxt X, X, X, W\n"
                        "vcmp.t le, C220, C100\n"
                        "vnop\n"
                        "mfvc %0, $131\n"
                        "andi %0, %0, 0x20\n"
                        "sltu %0, $zero, %0\n"
                        "andi %0, %0, 0xff\n"
                        : "=r"(res3) : "r"(vp)
                    );
                    j++;
                    if (res3) {
                        __asm__ volatile(
                            "sv.q C200, 0(%0)\n"
                            "sv.q C130, 0(%1)\n"
                            "sv.q C120, 0(%2)\n"
                            :: "r"(p1), "r"(p2), "r"(p3) : "memory"
                        );
                        return 0;
                    }
                    vp += 16;
                } while (j < 3);
            }
        advance:
            t3++;
            contacts = (eContact *)((char *)contacts + 0x90);
        } while (t3 < n);

        __asm__ volatile(
            "sv.q C200, 0(%0)\n"
            "sv.q C130, 0(%1)\n"
            "sv.q C120, 0(%2)\n"
            :: "r"(p1), "r"(p2), "r"(p3) : "memory"
        );
    }
    return 1;
}

void eContactCollector::VerifySoftContacts(void) {
    int cnt = *(int *)((char *)this + 0x4860);
    if (cnt == 0) return;

    float thresh = *(float *)((char *)this + 0x80);
    int k = 0;
    int total = *(int *)((char *)this + 0x4868);
    int i = 0;
    if (i < total) {
        char *src = (char *)this;
        SoftItem *srcItem = (SoftItem *)(src + 0x2460);
        do {
            float cv;
            __asm__ ("lwc1 %0, 0x2480(%1)" : "=f"(cv) : "r"(src));
            if (cv <= thresh) {
                int k128 = k << 7;
                int k16 = k << 4;
                int off = k128 + k16;
                k++;
                SoftItem *dst = (SoftItem *)((char *)this + off + 0x2460);
                SoftItem *srcIt = srcItem;
                __asm__ (
                    "lv.q C120, 0(%0)\n"
                    "sv.q C120, 0(%1)\n"
                    "lv.q C120, 0x10(%0)\n"
                    "sv.q C120, 0x10(%1)\n"
                    :
                    : "r"(srcIt), "r"(dst)
                );
                dst->f20 = srcIt->f20;
                dst->i24 = srcIt->i24;
                dst->i28 = srcIt->i28;
                dst->i2c = srcIt->i2c;
                dst->i30 = srcIt->i30;
                dst->i34 = srcIt->i34;
                unsigned int j = 0;
                char *d2 = (char *)dst + 0x40;
                char *s2 = (char *)srcIt + 0x40;
                do {
                    __asm__ (
                        "lv.q C120, 0(%0)\n"
                        "sv.q C120, 0(%1)\n"
                        :
                        : "r"(s2), "r"(d2)
                    );
                    j++;
                    d2 += 16;
                    s2 += 16;
                } while (j < 3U);
                float f7cTemp = srcIt->f7c;
                __asm__ (
                    "lv.q C120, 0x70(%0)\n"
                    "sv.q C120, 0x70(%1)\n"
                    :
                    : "r"(srcIt), "r"(dst)
                );
                dst->f7c = f7cTemp;
                dst->i80 = srcIt->i80;
                total = *(int *)((char *)this + 0x4868);
            }
            i++;
            src += 0x90;
            srcItem = (SoftItem *)((char *)srcItem + 0x90);
        } while (i < total);
    }
    *(int *)((char *)this + 0x4868) = k;
}
