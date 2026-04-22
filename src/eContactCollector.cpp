#include "eContactCollector.h"
#include "mVec3.h"

class eContact;

class eContactForceSolver {
public:
    char _pad[4804];
    void ComputeForces(mVec3 *, const eContact *, int);
};

void eContactCollector::FindSlideVelocity(mVec3 *v) const {
    int count;
    __asm__(
        "addiu $a2, %1, 0x4860\n\t"
        "lw %0, 0($a2)"
        : "=r"(count)
        : "r"(this)
        : "$a2");
    if (count != 0) {
        eContactForceSolver solver;
        solver.ComputeForces(v, (const eContact *)((char *)this + 0x60), count);
    }
}

int eContactCollector::ValidateTri(eContact *contacts, int count) {
    char *newBase = (char *)contacts + count * 0x90;
    int i = 0;
    if (i < count) {
        __asm__ volatile(
            "lui $a0, 0x3727\n\t"
            "ori $a0, $a0, 0xc5ac\n\t"
            "mtc1 $a0, $f12\n\t"
            "mfc1 $a0, $f12\n\t"
            "mtv $a0, S100\n\t"
            ::: "$a0", "$f12");
        do {
            int matched = 1;
            int flag = ((int *)contacts)[13];
            if (flag & 1) {
                int j = 0;
                char *newW, *curW;
                __asm__("addiu %0, %1, 64" : "=r"(newW) : "r"(newBase));
                __asm__("addiu %0, %1, 64" : "=r"(curW) : "r"(contacts));
                do {
                    int mask;
                    __asm__ volatile(
                        "lv.q C120, 0(%1)\n\t"
                        "lv.q C130, 0(%2)\n\t"
                        "vsub.t C120, C120, C130\n\t"
                        "vabs.t C120, C120\n\t"
                        "vpfxt X, X, X, W\n\t"
                        "vcmp.t le, C120, C100\n\t"
                        "vnop\n\t"
                        "mfvc %0, $131"
                        : "=r"(mask) : "r"(newW), "r"(curW));
                    if (!(((mask & 0x20) != 0) & 0xff)) {
                        matched = 0;
                        break;
                    }
                    j++;
                    newW += 16;
                    curW += 16;
                } while (j < 3);
                if (matched) return 0;
            }
            i++;
            contacts = (eContact *)((char *)contacts + 0x90);
        } while (i < count);
    }
    return 1;
}

void eContactCollector::VerifySoftContacts(void) {
    int flag = *(int *)((char *)this + 0x4860);
    if (flag != 0) {
        float threshold = *(float *)((char *)this + 0x80);
        int i = 0;
        int k = 0;
        int count = *(int *)((char *)this + 0x4868);
        if (i < count) {
            char *srcBase = (char *)this;
            char *srcItem = srcBase + 0x2460;
            do {
                if (*(float *)(srcBase + 0x2480) <= threshold) {
                    char *dst = (char *)this + 0x2460 + k * 0x90;
                    k++;
                    __asm__ volatile(
                        "lv.q C120, 0(%0)\n\t"
                        "sv.q C120, 0(%1)\n\t"
                        "lv.q C120, 0x10(%0)\n\t"
                        "sv.q C120, 0x10(%1)\n\t"
                        :: "r"(srcItem), "r"(dst) : "memory");
                    *(float *)(dst + 0x20) = *(float *)(srcItem + 0x20);
                    *(int *)(dst + 0x24) = *(int *)(srcItem + 0x24);
                    *(int *)(dst + 0x28) = *(int *)(srcItem + 0x28);
                    *(int *)(dst + 0x2C) = *(int *)(srcItem + 0x2C);
                    *(int *)(dst + 0x30) = *(int *)(srcItem + 0x30);
                    *(int *)(dst + 0x34) = *(int *)(srcItem + 0x34);
                    int j = 0;
                    char *sp = srcItem + 0x40;
                    char *dp = dst + 0x40;
                    do {
                        __asm__ volatile(
                            "lv.q C120, 0(%0)\n\t"
                            "sv.q C120, 0(%1)\n\t"
                            :: "r"(sp), "r"(dp) : "memory");
                        j++;
                        dp += 16;
                        sp += 16;
                    } while ((unsigned)j < 3U);
                    float f7c = *(float *)(srcItem + 0x7C);
                    __asm__ volatile(
                        "lv.q C120, 0x70(%0)\n\t"
                        "sv.q C120, 0x70(%1)\n\t"
                        :: "r"(srcItem), "r"(dst) : "memory");
                    *(float *)(dst + 0x7C) = f7c;
                    *(int *)(dst + 0x80) = *(int *)(srcItem + 0x80);
                }
                i++;
                srcBase += 0x90;
                srcItem += 0x90;
            } while (i < *(int *)((char *)this + 0x4868));
        }
        *(int *)((char *)this + 0x4868) = k;
    }
}

int eContactCollector::ValidatePoint(eContact *contacts, int arg2, int arg3) {
    eContact *newContact = (eContact *)((char *)contacts + arg2 * 0x90);
    int refFlag = *(int *)((char *)newContact + 0x34);
    int i = 0;
    float refStore[4];
    if (refFlag == 0x40) {
        __asm__ volatile("lv.q C120, 0x60(%0)\n\tsv.q C120, (%1)" :: "r"(newContact), "r"(refStore) : "memory");
    } else if (refFlag == 0x20) {
        __asm__ volatile("lv.q C120, 0x50(%0)\n\tsv.q C120, (%1)" :: "r"(newContact), "r"(refStore) : "memory");
    } else if (refFlag == 0x10) {
        __asm__ volatile("lv.q C120, 0x40(%0)\n\tsv.q C120, (%1)" :: "r"(newContact), "r"(refStore) : "memory");
    } else {
        goto skip_body;
    }
    if (0 < arg3) {
        __asm__ volatile("lv.q C210, (%0)" :: "r"(refStore));
        float eps;
        int epsBits = 0x3727c5ac;
        __asm__ volatile("mtc1 %1, %0" : "=f"(eps) : "r"(epsBits));
        __asm__ volatile("mtv %0, S100" :: "r"(eps));
        float preload200[4], preload130[4], preload120[4];
        // preload C200, C130, C120 from uninitialized sp+0x10/0x20/0x30
        __asm__ volatile(
            "lv.q C200, (%0)\n\t"
            "lv.q C130, (%1)\n\t"
            "lv.q C120, (%2)\n\t"
            :: "r"(preload200), "r"(preload130), "r"(preload120));
        do {
            if (i == arg2) goto advance;
            int flag = *(int *)((char *)contacts + 0x34);
            if (flag & 0x70) {
                if (i < arg2) {
                    int f2 = *(int *)((char *)contacts + 0x34);
                    if (f2 == 0x40) {
                        __asm__ volatile("lv.q C200, 0x60(%0)" :: "r"(contacts));
                    } else if (f2 == 0x20) {
                        __asm__ volatile("lv.q C200, 0x50(%0)" :: "r"(contacts));
                    } else if (f2 == 0x10) {
                        __asm__ volatile("lv.q C200, 0x40(%0)" :: "r"(contacts));
                    }
                    int mask;
                    __asm__ volatile(
                        "vsub.t C220, C200, C210\n\t"
                        "vabs.t C220, C220\n\t"
                        "vpfxt X, X, X, W\n\t"
                        "vcmp.t le, C220, C100\n\t"
                        "vnop\n\t"
                        "mfvc %0, $131" : "=r"(mask));
                    if (((mask & 0x20) != 0) & 0xff) {
                        __asm__ volatile("sv.q C200, (%0)" :: "r"(preload200) : "memory");
                        __asm__ volatile("sv.q C130, (%0)" :: "r"(preload130) : "memory");
                        __asm__ volatile("sv.q C120, (%0)" :: "r"(preload120) : "memory");
                        return 0;
                    }
                    goto advance;
                }
            }
            if (flag & 0xE) {
                int f2 = *(int *)((char *)contacts + 0x34);
                if (f2 == 8) {
                    __asm__ volatile(
                        "lv.q C130, 0x60(%0)\n\t"
                        "lv.q C120, 0x40(%0)" :: "r"(contacts));
                } else if (f2 == 4) {
                    __asm__ volatile(
                        "lv.q C130, 0x50(%0)\n\t"
                        "lv.q C120, 0x60(%0)" :: "r"(contacts));
                } else if (f2 == 2) {
                    __asm__ volatile(
                        "lv.q C130, 0x40(%0)\n\t"
                        "lv.q C120, 0x50(%0)" :: "r"(contacts));
                }
                int mask;
                __asm__ volatile(
                    "vsub.t C220, C130, C210\n\t"
                    "vabs.t C220, C220\n\t"
                    "vpfxt X, X, X, W\n\t"
                    "vcmp.t le, C220, C100\n\t"
                    "vnop\n\t"
                    "mfvc %0, $131" : "=r"(mask));
                if (((mask & 0x20) != 0) & 0xff) {
                    __asm__ volatile("sv.q C200, (%0)" :: "r"(preload200) : "memory");
                    __asm__ volatile("sv.q C130, (%0)" :: "r"(preload130) : "memory");
                    __asm__ volatile("sv.q C120, (%0)" :: "r"(preload120) : "memory");
                    return 0;
                }
                __asm__ volatile(
                    "vsub.t C220, C120, C210\n\t"
                    "vabs.t C220, C220\n\t"
                    "vpfxt X, X, X, W\n\t"
                    "vcmp.t le, C220, C100\n\t"
                    "vnop\n\t"
                    "mfvc %0, $131" : "=r"(mask));
                if (((mask & 0x20) != 0) & 0xff) {
                    __asm__ volatile("sv.q C200, (%0)" :: "r"(preload200) : "memory");
                    __asm__ volatile("sv.q C130, (%0)" :: "r"(preload130) : "memory");
                    __asm__ volatile("sv.q C120, (%0)" :: "r"(preload120) : "memory");
                    return 0;
                }
                goto advance;
            }
            if (flag & 1) {
                int j = 0;
                char *p = (char *)contacts + 0x40;
                do {
                    int mask;
                    __asm__ volatile(
                        "lv.q C220, 0(%1)\n\t"
                        "vsub.t C220, C220, C210\n\t"
                        "vabs.t C220, C220\n\t"
                        "vpfxt X, X, X, W\n\t"
                        "vcmp.t le, C220, C100\n\t"
                        "vnop\n\t"
                        "mfvc %0, $131" : "=r"(mask) : "r"(p));
                    if (((mask & 0x20) != 0) & 0xff) {
                        __asm__ volatile("sv.q C200, (%0)" :: "r"(preload200) : "memory");
                        __asm__ volatile("sv.q C130, (%0)" :: "r"(preload130) : "memory");
                        __asm__ volatile("sv.q C120, (%0)" :: "r"(preload120) : "memory");
                        return 0;
                    }
                    j++;
                    p += 16;
                } while (j < 3);
            }
        advance:
            i++;
            contacts = (eContact *)((char *)contacts + 0x90);
        } while (i < arg3);
        __asm__ volatile("sv.q C200, (%0)" :: "r"(preload200) : "memory");
        __asm__ volatile("sv.q C130, (%0)" :: "r"(preload130) : "memory");
        __asm__ volatile("sv.q C120, (%0)" :: "r"(preload120) : "memory");
    }
skip_body:
    return 1;
}

int eContactCollector::ValidateEdge(eContact *contacts, int arg2, int arg3) {
    eContact *newContact = (eContact *)((char *)contacts + arg2 * 0x90);
    int refFlag = *(int *)((char *)newContact + 0x34);
    int i = 0;
    float refA[4], refB[4];
    if (refFlag == 8) {
        __asm__ volatile(
            "lv.q C120, 0x60(%0)\n\t"
            "sv.q C120, (%1)\n\t"
            "lv.q C120, 0x40(%0)\n\t"
            "sv.q C120, (%2)\n\t"
            :: "r"(newContact), "r"(refA), "r"(refB) : "memory");
    } else if (refFlag == 4) {
        __asm__ volatile(
            "lv.q C120, 0x50(%0)\n\t"
            "sv.q C120, (%1)\n\t"
            "lv.q C120, 0x60(%0)\n\t"
            "sv.q C120, (%2)\n\t"
            :: "r"(newContact), "r"(refA), "r"(refB) : "memory");
    } else if (refFlag == 2) {
        __asm__ volatile(
            "lv.q C120, 0x40(%0)\n\t"
            "sv.q C120, (%1)\n\t"
            "lv.q C120, 0x50(%0)\n\t"
            "sv.q C120, (%2)\n\t"
            :: "r"(newContact), "r"(refA), "r"(refB) : "memory");
    } else {
        goto ret1;
    }
    if (0 < arg3) {
        __asm__ volatile("lv.q C230, (%0)" :: "r"(refA));
        float eps;
        int epsBits = 0x3727c5ac;
        __asm__ volatile("mtc1 %1, %0" : "=f"(eps) : "r"(epsBits));
        __asm__ volatile("mtv %0, S100" :: "r"(eps));
        __asm__ volatile("lv.q C220, (%0)" :: "r"(refB));
        float slotC210[4], slotC200[4];
        __asm__ volatile(
            "lv.q C210, (%0)\n\t"
            "lv.q C200, (%1)"
            :: "r"(slotC210), "r"(slotC200));
        do {
            if (i == arg2) goto advance;
            int flag = *(int *)((char *)contacts + 0x34);
            if (flag & 0xE) {
                if (i < arg2) {
                    int f2 = *(int *)((char *)contacts + 0x34);
                    if (f2 == 8) {
                        __asm__ volatile(
                            "lv.q C210, 0x60(%0)\n\t"
                            "lv.q C200, 0x40(%0)" :: "r"(contacts));
                    } else if (f2 == 4) {
                        __asm__ volatile(
                            "lv.q C210, 0x50(%0)\n\t"
                            "lv.q C200, 0x60(%0)" :: "r"(contacts));
                    } else if (f2 == 2) {
                        __asm__ volatile(
                            "lv.q C210, 0x40(%0)\n\t"
                            "lv.q C200, 0x50(%0)" :: "r"(contacts));
                    }
                    int mask;
                    __asm__ volatile(
                        "vsub.t C120, C230, C200\n\t"
                        "vabs.t C120, C120\n\t"
                        "vpfxt X, X, X, W\n\t"
                        "vcmp.t le, C120, C100\n\t"
                        "vnop\n\t"
                        "mfvc %0, $131" : "=r"(mask));
                    if (((mask & 0x20) != 0) & 0xff) {
                        __asm__ volatile(
                            "vsub.t C120, C220, C210\n\t"
                            "vabs.t C120, C120\n\t"
                            "vpfxt X, X, X, W\n\t"
                            "vcmp.t le, C120, C100\n\t"
                            "vnop\n\t"
                            "mfvc %0, $131" : "=r"(mask));
                        if (((mask & 0x20) != 0) & 0xff) {
                            __asm__ volatile("sv.q C210, (%0)" :: "r"(slotC210) : "memory");
                            __asm__ volatile("sv.q C200, (%0)" :: "r"(slotC200) : "memory");
                            return 0;
                        }
                    }
                    __asm__ volatile(
                        "vsub.t C120, C230, C210\n\t"
                        "vabs.t C120, C120\n\t"
                        "vpfxt X, X, X, W\n\t"
                        "vcmp.t le, C120, C100\n\t"
                        "vnop\n\t"
                        "mfvc %0, $131" : "=r"(mask));
                    if (((mask & 0x20) != 0) & 0xff) {
                        __asm__ volatile(
                            "vsub.t C120, C220, C200\n\t"
                            "vabs.t C120, C120\n\t"
                            "vpfxt X, X, X, W\n\t"
                            "vcmp.t le, C120, C100\n\t"
                            "vnop\n\t"
                            "mfvc %0, $131" : "=r"(mask));
                        if (((mask & 0x20) != 0) & 0xff) {
                            __asm__ volatile("sv.q C210, (%0)" :: "r"(slotC210) : "memory");
                            __asm__ volatile("sv.q C200, (%0)" :: "r"(slotC200) : "memory");
                            return 0;
                        }
                    }
                    goto advance;
                }
            }
            if (flag & 1) {
                int j = 0;
                float tmpEdge[8];
                char *slot40 = (char *)tmpEdge;
                char *slot50 = (char *)tmpEdge + 0x10;
                do {
                    if (j <= 0) {
                        __asm__ volatile(
                            "lv.q C120, 0x40(%0)\n\t"
                            "sv.q C120, 0(%1)\n\t"
                            "lv.q C120, 0x50(%0)\n\t"
                            "sv.q C120, 0(%2)\n\t"
                            :: "r"(contacts), "r"(slot40), "r"(slot50) : "memory");
                    } else if (j < 2) {
                        __asm__ volatile(
                            "lv.q C120, 0x50(%0)\n\t"
                            "sv.q C120, 0(%1)\n\t"
                            "lv.q C120, 0x60(%0)\n\t"
                            "sv.q C120, 0(%2)\n\t"
                            :: "r"(contacts), "r"(slot40), "r"(slot50) : "memory");
                    } else if (j < 3) {
                        __asm__ volatile(
                            "lv.q C120, 0x60(%0)\n\t"
                            "sv.q C120, 0(%1)\n\t"
                            "lv.q C120, 0x40(%0)\n\t"
                            "sv.q C120, 0(%2)\n\t"
                            :: "r"(contacts), "r"(slot40), "r"(slot50) : "memory");
                    }
                    int mask;
                    __asm__ volatile(
                        "lv.q C120, 0(%1)\n\t"
                        "vsub.t C130, C230, C120\n\t"
                        "vabs.t C130, C130\n\t"
                        "vpfxt X, X, X, W\n\t"
                        "vcmp.t le, C130, C100\n\t"
                        "vnop\n\t"
                        "mfvc %0, $131\n\t"
                        "lv.q C130, 0(%2)"
                        : "=r"(mask) : "r"(slot50), "r"(slot40));
                    if (!(((mask & 0x20) != 0) & 0xff)) goto loop_cont;
                    __asm__ volatile(
                        "vsub.t C300, C220, C130\n\t"
                        "vabs.t C300, C300\n\t"
                        "vpfxt X, X, X, W\n\t"
                        "vcmp.t le, C300, C100\n\t"
                        "vnop\n\t"
                        "mfvc %0, $131" : "=r"(mask));
                    if (((mask & 0x20) != 0) & 0xff) {
                        __asm__ volatile("sv.q C210, (%0)" :: "r"(slotC210) : "memory");
                        __asm__ volatile("sv.q C200, (%0)" :: "r"(slotC200) : "memory");
                        return 0;
                    }
                loop_cont:
                    __asm__ volatile(
                        "vsub.t C130, C230, C130\n\t"
                        "vabs.t C130, C130\n\t"
                        "vpfxt X, X, X, W\n\t"
                        "vcmp.t le, C130, C100\n\t"
                        "vnop\n\t"
                        "mfvc %0, $131" : "=r"(mask));
                    if (!(((mask & 0x20) != 0) & 0xff)) { j++; continue; }
                    __asm__ volatile(
                        "vsub.t C120, C220, C120\n\t"
                        "vabs.t C120, C120\n\t"
                        "vpfxt X, X, X, W\n\t"
                        "vcmp.t le, C120, C100\n\t"
                        "vnop\n\t"
                        "mfvc %0, $131" : "=r"(mask));
                    j++;
                    if (((mask & 0x20) != 0) & 0xff) {
                        __asm__ volatile("sv.q C210, (%0)" :: "r"(slotC210) : "memory");
                        __asm__ volatile("sv.q C200, (%0)" :: "r"(slotC200) : "memory");
                        return 0;
                    }
                } while (j < 3);
            }
        advance:
            i++;
            contacts = (eContact *)((char *)contacts + 0x90);
        } while (i < arg3);
        __asm__ volatile("sv.q C210, (%0)" :: "r"(slotC210) : "memory");
        __asm__ volatile("sv.q C200, (%0)" :: "r"(slotC200) : "memory");
    }
ret1:
    return 1;
}
