#ifndef CFILE_H
#define CFILE_H

class cFile {
public:
    void OnCreated(void);
    void OnClosed(void);
};

class cListSubscriber {
public:
    char pad[0x20];
    int mNumVisible;

    void SetNumVisible(int numVisible);
    bool IsModifiable(void) const;
};

class cLanguage {
public:
    static int PlatformInitialize(void);
};

class cFilePlatform {
public:
    char _pad[0x104];
    int mSize;
    int mHandle;
    char _pad2[0x14];

    void Close(void);
    void ReadAsync(void *buf, unsigned int offset, unsigned int size);
};

class cBufferedFile {
public:
    char pad0[0x04];
    unsigned int mBufferSize;
    void *mBufferPtr[2];
    char pad1[0x04];
    int mCurrentBuffer;
    int mBufPos;
    unsigned int mFilePos[2];
    cFilePlatform mPlatform;
    int mAllocHandle;

    void Close(void);
    void FillBuffer(int bufIndex);
    void WaitForFill(void);
    void NextBuffer(void);
    int GetFilePos(void) const;
    int GetSize(void) const;
    bool IsOpen(void) const;
};

#endif
