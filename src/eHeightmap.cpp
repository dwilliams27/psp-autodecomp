#include "eHeightmap.h"
#include "eMemCard.h"
#include "eMovie.h"

void eHeightmap::PlatformReset(cMemPool *, bool) {
}

int eMemCard::Reset(void) {
    return 1;
}

void eMemCard::EndLoad(const eMemCard::eLoadParams &) {
}

void eMemCard::EndSave(const eMemCard::eSaveParams &) {
}

void eMovie::PlatformPause(void) {
}
