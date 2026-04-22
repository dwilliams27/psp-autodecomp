#include "nwConnection.h"

bool nwConnection::IsBlockIdOld(unsigned int blockId, nwSendMethod method) const {
    unsigned int delta = (blockId - mBlocks[method].mLastSeq) & 0x3FF;
    return (delta == 0) | (delta >= 256);
}
