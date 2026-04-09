#include "cFile.h"

void cFile::OnCreated(void) {
}

void cFile::OnClosed(void) {
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
