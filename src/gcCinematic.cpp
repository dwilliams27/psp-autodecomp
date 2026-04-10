#include "gcCinematic.h"

void gcCinematic::Reset(cMemPool *, bool) {
}

void gcCinematicInstance::Reset(cMemPool *, bool) {
}

void gcCinematicInstance::UpdateProfile(void) {
}

void gcReplicationVisitor::SetNetConnection(int connection) {
    mNetConnection = connection;
}

void gcReplicationVisitor::SetMemCardStream(cInStream *stream) {
    mInStream = stream;
}
