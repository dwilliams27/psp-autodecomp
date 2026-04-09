#include "cFile.h"

void cBufferedFile::FillBuffer(int bufIndex) {
    unsigned int filePos = mFilePos[bufIndex];
    unsigned int fileSize = mPlatform.mSize;
    if (filePos < fileSize) {
        unsigned int remaining = fileSize - filePos;
        unsigned int readSize = remaining;
        if (readSize >= mBufferSize) {
            readSize = mBufferSize;
        }
        mPlatform.ReadAsync(mBufferPtr[bufIndex], filePos, readSize);
    }
}
