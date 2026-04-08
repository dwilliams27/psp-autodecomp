#include "eMemCard.h"

void eMemCard::SetSlot(int) {
    *(int *)0x37D2CC = 0;
}

float eMemCard::GetMinSaveTime(void) {
    return 1.0f;
}

float eMemCard::GetMinLoadTime(void) {
    return 1.0f;
}

void eMemCard::Format(void) {
    *(int *)0x37D2C8 = 0;
}

int eMemCard::GetSaveSize(int a, int b, int c) {
    int x = c;
    int y = b;
    return a + x * y;
}
