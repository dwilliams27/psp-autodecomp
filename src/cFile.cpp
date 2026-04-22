#include "cFile.h"
#include "cFileSystem.h"

cFile::cFile() {
    mHandle = 0;
    mMode = 0;
    mField5 = 0;
    mField7 = 0;
    mDepCount = 0;
    mDepArray = 0;
}

unsigned int cFile::GetCurrentPos(void) const {
    return cFileSystem::GetCurrentPos(mHandle);
}

void cFile::SetCurrentPos(unsigned int offset) {
    cFileSystem::SetCurrentPos(mHandle, offset);
}
