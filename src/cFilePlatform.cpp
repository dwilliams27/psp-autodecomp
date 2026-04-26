#include "cFile.h"
#include "cFileSystemPlatform.h"

void cFilePlatform::Close(void) {
    cFileSystemPlatform::Close(this);
}
