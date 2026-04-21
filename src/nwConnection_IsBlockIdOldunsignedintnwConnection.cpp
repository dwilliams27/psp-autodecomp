#include "nwConnection.h"

int nwConnection::IsBlockIdOld(unsigned int blockId, nwSendMethod method) const {
    unsigned int diff = (blockId - mBlocks[method].m41C) & 0x3FF;
    return (diff == 0) | (diff >= 0x100);
}
