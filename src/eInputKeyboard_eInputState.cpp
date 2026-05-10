#include "eInputKeyboard.h"

extern "C" void *memset(void *, int, unsigned int);

#pragma control sched=2

void eInputKeyboard::eInputState::Reset() {
    memset(this, 0, 0xA08);
    asciiKey = 0;
    present = 1;
    changed = 0;
    int *p = &timeAccum;
    *p = 0;
}
