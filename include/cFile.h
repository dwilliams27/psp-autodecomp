#ifndef CFILE_H
#define CFILE_H

struct cFileHandle;
class cObject;

class cFile {
public:
    cFileHandle *mHandle;    // 0x00
    bool mMode;              // 0x04
    bool mField5;            // 0x05
    unsigned char _pad6;     // 0x06 (not initialized in ctor)
    bool mField7;            // 0x07
    char _pad[0x100];        // 0x08..0x107
    int mDepCount;           // 0x108
    const cObject **mDepArray; // 0x10C

    static unsigned char s_bSwapEndianess;

    cFile();
    void OnCreated(void);
    void OnClosed(void);
    void Close(bool flag);
    unsigned int GetCurrentPos(void) const;
    void SetCurrentPos(unsigned int offset);
    bool AddDependency(const cObject *obj);
};

class cBase;

class cListSubscriber {
public:
    cBase *mOwner;            // 0x00
    void *mTypeInfo;          // 0x04
    void *mNext;              // 0x08
    void *mPrev;              // 0x0C
    unsigned char mAttached;  // 0x10
    // 3 bytes padding
    int mFirst;               // 0x14
    int mLast;                // 0x18
    int mCount;               // 0x1C
    int mNumVisible;          // 0x20

    cListSubscriber(cBase *owner);
    void SetNumVisible(int numVisible);
    bool IsModifiable(void) const;
    void OnCleared(void);
    void Attach(void);
    void Detach(void);
    void OnAdded(void *);
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

    cBufferedFile(unsigned int shift);
    void Close(void);
    void FillBuffer(int bufIndex);
    void WaitForFill(void);
    void NextBuffer(void);
    int GetFilePos(void) const;
    int GetSize(void) const;
    bool IsOpen(void) const;
};

#endif
