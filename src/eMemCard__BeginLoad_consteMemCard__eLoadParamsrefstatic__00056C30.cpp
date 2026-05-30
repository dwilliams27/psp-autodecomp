#include "eMemCard.h"

void eMemCard::BeginLoad(const eMemCard::eLoadParams &) {
    *(int *)0x37D2C8 = 0;
}
