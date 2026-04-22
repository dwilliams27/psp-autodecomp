#include "eContactCollector.h"

void eContactCollector::ClearAllContacts(void) {
    int i = 0;
    do {
        *(int *)((char *)this + i*4 + 0x4860) = 0;
        i++;
    } while (i < 4);
}
