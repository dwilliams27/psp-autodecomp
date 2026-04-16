#include "cOutStream.h"

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
