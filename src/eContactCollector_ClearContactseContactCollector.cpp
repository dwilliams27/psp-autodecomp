#include "eContactCollector.h"
#include "eKeyframedController.h"
#include "gcExpressionList.h"
#include "gcReplicationVisitor.h"
#include "gcStreamedCinematic.h"

void eContactCollector::ClearContacts(eContactType type) {
    *(int *)((char *)this + (int)type * 4 + 0x4860) = 0;
}

void eKeyframedController::Reset(cMemPool *, bool) {
    *(char **)((char *)this + 0x10) = *(char **)((char *)this + 0xC) + 0x110;
}

gcExpressionList::gcExpressionList(cBase *parent) {
    *(int *)((char *)this + 0) = 0;
    *(cBase **)((char *)this + 4) = parent;
}

void gcReplicationVisitor::SetMemCardStream(cOutStream *stream) {
    *(cOutStream **)((char *)this + 4) = stream;
    char zero = 0;
    *(char *)((char *)stream + 0x11) = zero;
}

void gcStreamedCinematic::NeedsDelete(void) {
    *(char *)((char *)this + 0x141) = 1;
    *(char *)((char *)this + 0x140) = 1;
}
