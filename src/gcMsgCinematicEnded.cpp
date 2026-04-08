#include "gcMsgCinematicEnded.h"
#include "gcMsgRequestLoadedState.h"
#include "gcEntityTemplate.h"
#include "gcValNumber.h"
#include "gcUIWidgetList.h"

void gcMsgCinematicEnded::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
}

void gcMsgRequestLoadedState::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
}

void gcEntityTemplate::CalcInstanceSize(void) {
}

float gcValNumber::Evaluate(void) const {
    return mValue;
}

void gcUIWidgetList::FillCell(gcUICell, int) {
}
