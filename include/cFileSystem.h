#ifndef CFILESYSTEM_H
#define CFILESYSTEM_H

#include "cFile.h"

struct cFileHandle {
    cBufferedFile mBufferedFile;
    unsigned int mDataStart;
    unsigned int mField14C;
    unsigned int mCurrentPos;
    unsigned char mFlag;
};

class cFileSystemPlatform;

class cFileSystem {
public:
    static void SetCurrentPos(cFileHandle *handle, unsigned int offset);
    static unsigned int GetCurrentPos(cFileHandle *handle);
    static int Exists(const char *name);
    static int GetFileStatus(void);
    static void Close(cFileHandle *handle);
};

#endif
