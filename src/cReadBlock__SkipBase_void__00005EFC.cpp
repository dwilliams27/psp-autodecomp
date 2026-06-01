// cReadBlock::SkipBase(void)  @ 0x00005efc, 116B
// obj: cAll_psp.obj  (split-TU isolated file)

struct cFileHandle;

class cFile {
public:
    cFileHandle *mHandle;

    void SetCurrentPos(unsigned int);
};

class cFileSystem {
public:
    static int Read(cFileHandle *, void *, unsigned int);
};

class cReadBlock {
public:
    cFile *mFile;
    int mStart;
    int mSize;
    int mEnd;
    bool mOwned;

    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock();

    void SkipBase(void);
};

void cReadBlock::SkipBase(void) {
    int tag;
    cFileSystem::Read(this->mFile->mHandle, &tag, 4);
    if (tag != -1) {
        cReadBlock rb(*this->mFile, 0xFFFF, true);
        rb.mFile->SetCurrentPos(rb.mStart);
    }
}
