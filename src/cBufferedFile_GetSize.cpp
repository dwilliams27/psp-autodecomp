#include "cFile.h"

int cBufferedFile::GetSize(void) const {
    return mPlatform.mSize;
}
