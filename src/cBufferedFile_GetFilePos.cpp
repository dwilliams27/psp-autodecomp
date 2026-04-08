#include "cFile.h"

int cBufferedFile::GetFilePos(void) const {
    return mFilePos[mCurrentBuffer] + mBufPos;
}
