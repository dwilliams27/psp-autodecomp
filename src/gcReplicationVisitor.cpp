#include "gcCinematic.h"
#include "gcReplicationVisitor.h"

void gcReplicationVisitor::EndRead(void) {
    mReadActive = 0;
}

gcRegionBase &gcRegionBase::operator=(const gcRegionBase &other) {
    return *this;
}

gcLoadingScreen &gcLoadingScreen::operator=(const gcLoadingScreen &other) {
    return *this;
}

gcMap &gcMap::operator=(const gcMap &other) {
    return *this;
}

gcGame &gcGame::operator=(const gcGame &other) {
    return *this;
}
