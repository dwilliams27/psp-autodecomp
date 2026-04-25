#include "cFileSystem.h"
#include "cFileSystemPlatform.h"

void cFileSystem::SetCurrentPos(cFileHandle *handle, unsigned int offset) {
    handle->mCurrentPos = handle->mDataStart + offset;
}

unsigned int cFileSystem::GetCurrentPos(cFileHandle *handle) {
    return handle->mCurrentPos - handle->mDataStart;
}

int cFileSystem::Exists(const char *name) {
    return cFileSystemPlatform::FileExists(name);
}

int cFileSystem::GetFileStatus(void) {
    return cFileSystemPlatform::GetFileStatus();
}

int cFileSystem::Write(cFileHandle *handle, const void *data, unsigned int size) {
    return 0;
}

void cFileSystem::Close(cFileHandle *handle) {
    if (!handle->mFlag) {
        handle->mBufferedFile.Close();
    }
    handle->mDataStart = 0;
    handle->mField14C = 0;
    handle->mCurrentPos = 0;
    handle->mFlag = 0;
}

void cFile::OnCreated(void) {
}
