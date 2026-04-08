#include "cFile.h"

bool cBufferedFile::IsOpen(void) const {
    return mHandle >= 0;
}
