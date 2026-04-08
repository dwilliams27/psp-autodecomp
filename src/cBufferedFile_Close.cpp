#include "cFile.h"

void cBufferedFile::Close(void) {
    mPlatform.Close();
}
