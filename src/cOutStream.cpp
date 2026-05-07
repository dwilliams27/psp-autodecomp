#include "cOutStream.h"

void cOutStream::Write(int value, int numBits, bool sign) {
    if (sign) {
        if (numBits >= 3) {
            register int bitPos __asm__("$t0") = mBitPos;
            int isShort = (value == 0) | (value == 1);
            unsigned char *base = mData;
            register int bit __asm__("$a3") = bitPos & 7;
            int byteIndex = bitPos >> 3;
            int newBitPos = bitPos + 1;
            __asm__ volatile("" ::: "memory");
            unsigned char *ptr = base + byteIndex;
            unsigned char overflow = mOverflow;
            register int bitValue __asm__("$t1") = isShort & 0xFF;
            mBitPos = newBitPos;
            if (overflow != 0) {
                overflow = mOverflow;
            } else {
                if (mCapacity < ((mBitPos + 7) >> 3)) {
                    mOverflow = 1;
                    overflow = mOverflow;
                }
            }
            int canWrite = (unsigned char)(overflow == 0);
            if (canWrite != 0) {
                int byte = *ptr;
                int mask = 1 << bit;
                bitValue = bitValue != 0;
                byte = (byte & ~mask) | (bitValue << bit);
                *ptr = byte;
            }

            if (isShort != 0) {
                int value2 = value;
                int bitValue2 = (value2 == 1);
                bitValue2 &= 0xFF;
                int bitPos2 = mBitPos;
                unsigned char *base2 = mData;
                register int bit2 __asm__("$a3") = bitPos2 & 7;
                __asm__ volatile("" ::: "memory");
                int byteIndex2 = bitPos2 >> 3;
                int newBitPos2 = bitPos2 + 1;
                unsigned char overflow2 = mOverflow;
                bitValue2 &= 0xFF;
                unsigned char *ptr2 = base2 + byteIndex2;
                mBitPos = newBitPos2;
                if (overflow2 != 0) {
                    overflow2 = mOverflow;
                } else {
                    if (mCapacity < ((mBitPos + 7) >> 3)) {
                        mOverflow = 1;
                        overflow2 = mOverflow;
                    }
                }
                int canWrite2 = (unsigned char)(overflow2 == 0);
                if (canWrite2 != 0) {
                    int byte2 = *ptr2;
                    int mask2 = 1 << bit2;
                    int outBit = (bitValue2 != 0);
                    byte2 = (byte2 & ~mask2) | (outBit << bit2);
                    *ptr2 = byte2;
                }
                return;
            }
        }
    }

    WriteBits(&value, numBits);
}

void cOutStream::Write(float value, int wholeBits, int fracBits, bool sign) {
    int scale = 1 << fracBits;
    float scaled = value * scale;
    int intVal;
    if (scaled < 0.0f) {
        int _tmp_839 = (int)(scaled - 0.5f);
        intVal = _tmp_839;
    } else {
        intVal = (int)(scaled + 0.5f);
    }

    if (sign) {
        int bitPos = mBitPos;
        __asm__ volatile("" ::: "memory");
        unsigned char *base = mData;
        int bit = bitPos & 7;
        unsigned char *ptr = base + (bitPos >> 3);
        int newBitPos = bitPos + 1;

        if (intVal == 0 || intVal == scale) {
            // Write sign bit = 1 (at boundary)
            mBitPos = newBitPos;
            if (!mOverflow) {
                if (mCapacity < ((mBitPos + 7) >> 3)) {
                    mOverflow = 1;
                }
            }
            int notOv = (unsigned char)(!mOverflow);
            int isZero = (intVal == 0);
            if (notOv) {
                unsigned char b = *ptr;
                b = (b & ~(1 << bit)) | (1 << bit);
                *ptr = b;
                base = mData;
                isZero = (intVal == 0);
            }

            // Write second bit
            int bitPos2 = mBitPos;
            isZero = (unsigned char)isZero;
            __asm__ volatile("" ::: "memory");
            int newBitPos2 = bitPos2 + 1;
            int bit2 = bitPos2 & 7;
            isZero = (unsigned char)isZero;
            unsigned char *ptr2 = base + (bitPos2 >> 3);
            __asm__ volatile("" ::: "memory");
            mBitPos = newBitPos2;
            if (!mOverflow) {
                if (mCapacity < ((mBitPos + 7) >> 3)) {
                    mOverflow = 1;
                }
            }
            if (!mOverflow) {
                unsigned char b = *ptr2;
                __asm__ volatile("" ::: "memory");
                b = (b & ~(1 << bit2)) | ((isZero != 0) << bit2);
                *ptr2 = b;
            }
            return;
        }

        // intVal != 0 && intVal != scale: write sign bit = 0
        mBitPos = newBitPos;
        if (!mOverflow) {
            if (mCapacity < ((mBitPos + 7) >> 3)) {
                mOverflow = 1;
            }
        }
        int notOv = (unsigned char)(!mOverflow);
        if (notOv) {
            unsigned char b = *ptr;
            int zero = 0;
            __asm__ volatile("" ::: "memory");
            b = (b & ~(1 << bit)) | (zero << bit);
            *ptr = b;
        }
    }

    Write(intVal, fracBits + wholeBits, false);
}

void cOutStream::WriteBits(const void *data, int numBits) {
    int bitPos = mBitPos;
    const unsigned char *src = (const unsigned char *)data;
    unsigned char *dst = mData;
    __asm__ volatile("" ::: "memory");
    dst += bitPos >> 3;
    mBitPos = bitPos + numBits;
    int startBit = bitPos & 7;

    if (!mOverflow) {
        if (mCapacity < ((mBitPos + 7) >> 3)) {
            mOverflow = 1;
        }
    }

    int canWrite = (unsigned char)(!mOverflow);
    if (!canWrite) {
        return;
    }
    if (numBits <= 0) {
        return;
    }

    int rem = 8 - startBit;
    do {
        int count = numBits;
        if (rem < numBits) {
            count = rem;
        }
        int mask = ((1 << count) - 1) & 0xff;
        *dst = (*dst & (~(mask << startBit) & 0xff)) | ((*src & mask) << startBit);
        dst++;
        if (count < 8 && count < numBits) {
            int left = numBits - count;
            int rem2 = 8 - count;
            if (rem2 < left) {
                left = rem2;
            }
            int mask2 = (((1 << left) - 1) << count) & 0xff;
            *dst = (*dst & (~(mask2 >> count) & 0xff)) | ((*src & mask2) >> count);
            __asm__ volatile("" ::: "memory");
        }
        numBits -= 8;
        src++;
    } while (numBits > 0);
}
