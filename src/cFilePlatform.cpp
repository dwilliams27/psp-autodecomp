#include "cFile.h"
#include "cFileSystemPlatform.h"

void cFilePlatform::Close(void) {
    cFileSystemPlatform::Close(this);
}

void cFilePlatform::PollAsync(void) {
    cFileSystemPlatform::PollAsync(this);
}
