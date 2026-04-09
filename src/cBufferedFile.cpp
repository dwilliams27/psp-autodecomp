#include "cFile.h"

bool cBufferedFile::IsOpen(void) const {
    return mPlatform.mHandle >= 0;
}
