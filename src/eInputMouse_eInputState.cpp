#include "eInputMouse.h"

extern "C" void *memset(void *, int, unsigned int);

#pragma control sched=2

void eInputMouse::eInputState::Reset() {
    memset(this, 0, 0x78);
    *(int *)((char *)this + 0x78) = 0;
    *((char *)this + 0x7C) = 1;
    *((char *)this + 0x7D) = 0;
    *((char *)this + 0x7E) = 0;
    *((char *)this + 0x7F) = 0;
    int *p = (int *)((char *)this + 0x88);
    *p = 0;
    *(int *)((char *)this + 0x8C) = 0;
    *(int *)((char *)this + 0x90) = 0;
    *(int *)((char *)this + 0x94) = 0;
    *(int *)((char *)this + 0x98) = 0;
    *(float *)((char *)this + 0xA0) = 0.0f;
    *(float *)((char *)this + 0xA4) = 0.0f;
    *(float *)((char *)this + 0xA8) = 0.0f;
    *(float *)((char *)this + 0xAC) = 0.0f;
    int *q = (int *)((char *)this + 0xB0);
    *q = 0xff00ff00;
    *(int *)((char *)this + 0xB4) = 0;
}
