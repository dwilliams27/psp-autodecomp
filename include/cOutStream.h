#pragma once

// cOutStream: bitwise output stream (24 bytes, 0x18)
// Confirmed from cOutStream::Write (0x000065f0) and cOutStream::WriteBits (0x000068c4):
//   offset 0x00: lw — pointer to data buffer (unsigned char *)
//   offset 0x04: lw — capacity in bytes
//   offset 0x08: lw/sw — current bit position
//   offset 0x0C: lw/sw — CRC accumulator
// Confirmed from cOutStream::GetCRC (0x00006118):
//   offset 0x0C: CRC value
//   offset 0x10: lbu — dirty flag (CRC needs recalc)
//   offset 0x14: lw — saved bit position for CRC range
// Confirmed from cOutStream::WriteRaw (0x000067e8):
//   offset 0x12: lbu/sb — overflow flag
struct cOutStream {
    unsigned char *mData;    // offset 0x00 — data buffer
    int mCapacity;           // offset 0x04 — buffer size in bytes
    int mBitPos;             // offset 0x08 — current write position in bits
    int mCRC;                // offset 0x0C — CRC accumulator
    unsigned char mDirty;    // offset 0x10 — CRC dirty flag
    char _pad11;             // offset 0x11
    unsigned char mOverflow; // offset 0x12 — overflow flag
    char _pad13;             // offset 0x13
    int mCRCBitPos;          // offset 0x14 — bit position for CRC calc

    int GetCRC(void) const;
    void Write(int, int, bool);
    void Write(unsigned int, int, bool);
    void Write(float, bool);
    void Write(float, int, int, bool);
    void WriteRaw(int, const void *);
    void WriteBits(const void *, int);
};
