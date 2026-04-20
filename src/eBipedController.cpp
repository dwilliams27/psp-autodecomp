#include "eBipedController.h"

void eBipedController::InvalidateCacheEntries(eGeom *geom) {
    if (geom == *(eGeom **)((char *)this + 0x44)) {
        *(int *)((char *)this + 0x44) = 0;
        *(int *)((char *)this + 0x48) = 0;
    }
}

void eBipedController::EnableWallWalk(bool enable) {
    *(unsigned char *)((char *)this + 0xE0) = enable;
    if (!enable) {
        int a, b, c;
        float z = 0.0f;
        __asm__ volatile("mfc1 %0, %1" : "=r"(a) : "f"(z));
        __asm__ volatile("mfc1 %0, %1" : "=r"(b) : "f"(z));
        float neg = -1.0f;
        __asm__ volatile("mfc1 %0, %1" : "=r"(c) : "f"(neg));
        __asm__ volatile(
            "mtv %0, S120\n"
            "mtv %1, S121\n"
            "mtv %2, S122\n"
            "sv.q C120, 0x20(%3)\n"
            :: "r"(a), "r"(b), "r"(c), "r"(this)
        );
    }
}

void eBipedController::CollectContactCallBack(eContactCollector *coll, void *biped, eContact *contact, int idx) {
    if (biped != 0) {
        ((eBipedController *)biped)->CollectContact(coll, contact, idx);
    }
}

void eBipedController::ProcessContactCallBack(eContactCollector *coll, void *biped, eContact *contact, int idx) {
    if (biped != 0) {
        ((eBipedController *)biped)->ProcessContact(coll, contact, idx);
    }
}

void eBipedController::ProcessGroundOnlyContactCallBack(eContactCollector *coll, void *biped, eContact *contact, int idx) {
    if (biped != 0) {
        ((eBipedController *)biped)->ProcessGroundOnlyContact(coll, contact, idx);
    }
}
