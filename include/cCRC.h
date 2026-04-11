#pragma once

// cCRC: purely static CRC calculation utility.
// Calc (0x440c) takes a data pointer, bit start, bit end, and initial CRC value.
// Uses a 256-entry lookup table for CRC computation.
// Operates on bit-level granularity (bit offsets, not byte offsets).
class cCRC {
public:
    static unsigned int Calc(const void *data, int bitStart, int bitEnd, unsigned int initialCRC);
};
