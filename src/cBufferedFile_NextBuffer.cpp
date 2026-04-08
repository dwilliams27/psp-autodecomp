#include "cFile.h"

void cBufferedFile::NextBuffer(void) {
    WaitForFill();
    int curBuf = mCurrentBuffer;
    int newBuf = !curBuf;
    unsigned int pos = mFilePos[newBuf];
    mCurrentBuffer = newBuf;
    int otherBuf = !newBuf;
    pos += mBufferSize;
    mBufPos = 0;
    mFilePos[otherBuf & 0xFF] = pos;
    FillBuffer(otherBuf);
}
