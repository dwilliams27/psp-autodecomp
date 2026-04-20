#include "cFile.h"
#include "cFileSystem.h"

unsigned char cFile::s_bSwapEndianess;

cFile::cFile()
    : mHandle(0), mMode(false), mField5(false), mField7(false),
      mDepCount(0), mDepArray(0) {
}

void cFile::OnCreated(void) {
}

void cFile::OnClosed(void) {
}

unsigned int cFile::GetCurrentPos(void) const {
    return cFileSystem::GetCurrentPos(mHandle);
}

void cFile::SetCurrentPos(unsigned int offset) {
    cFileSystem::SetCurrentPos(mHandle, offset);
}

bool cFile::AddDependency(const cObject *obj) {
    const cObject **arr = mDepArray;
    if (arr == 0) return false;
    for (int i = 0; i < mDepCount; i++) {
        if (arr[i] == obj) return false;
    }
    arr[mDepCount++] = obj;
    return true;
}

void cFile::Close(bool flag) {
    int readBuf;
    int writeMarker;
    if (mHandle != 0) {
        int isWrite = (unsigned char)!mMode;
        if (!isWrite) {
            if (flag) {
                readBuf = -1;
                cFileSystem::Read(mHandle, &readBuf, 4);
            }
        } else {
            writeMarker = s_bSwapEndianess ? 0xEFFEEFFE : 0xFEEFFEEF;
            cFileSystem::Write(mHandle, &writeMarker, 4);
        }
        cFileSystem::Close(mHandle);
        OnClosed();
        mHandle = 0;
    }
}

void cListSubscriber::SetNumVisible(int numVisible) {
    mNumVisible = numVisible;
}

int cLanguage::PlatformInitialize(void) {
    return 1;
}

int cBufferedFile::GetSize(void) const {
    return mPlatform.mSize;
}
