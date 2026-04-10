#include "eBipedController.h"

void eBipedController::OnSnappedTo(void) {
    *(int *)((char *)this + 0x44) = 0;
    *(int *)((char *)this + 0x48) = 0;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x90) = 0.0f;
    *(int *)((char *)this + 0x120) = 0;
}
