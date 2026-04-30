#include "cFile.h"
#include "cFileSystem.h"

class nwMsg;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int mOffset;
};

class gcMsgRequestLoadedState {
public:
    static nwMsg *New(nwMsgBuffer &);
};

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

void cFile::OnClosed(void) {
}

bool cFile::AddDependency(const cObject *obj) {
    const cObject **deps = mDepArray;
    if (deps != 0) {
        int i = 0;
        int count = mDepCount;
        for (; i < count; i++) {
            if (deps[i] == obj) {
                return false;
            }
        }
        deps[mDepCount++] = obj;
        return true;
    }
    return false;
}

nwMsg *gcMsgRequestLoadedState::New(nwMsgBuffer &buf) {
    buf.mOffset += 4;
    char *p = (char *)&buf + buf.mOffset;
    nwMsg *result = 0;
    if (p) {
        *(int *)p = 0x389050;
        result = (nwMsg *)p;
    }
    return result;
}
