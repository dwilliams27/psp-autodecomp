struct cFileHandle;

class cFile {
public:
    cFileHandle *mHandle;

    unsigned int GetCurrentPos(void) const;
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
};

cReadBlock::cReadBlock(cFile &file, unsigned int, bool owned) {
    register cReadBlock *self __asm__("$16") = this;
    register cFile *filePtr __asm__("$17") = &file;

    self->mFile = filePtr;
    cFile *source = *(cFile * volatile *)&self->mFile;
    self->mOwned = owned;
    filePtr->SetCurrentPos((source->GetCurrentPos() + 3) >> 2 << 2);

    self->mEnd = -1;
    self->mStart = -1;

    int tag = 0;
    cFileSystem::Read(self->mFile->mHandle, &tag, 4);
    cFileSystem::Read(self->mFile->mHandle, &self->mStart, 4);
    cFileSystem::Read(self->mFile->mHandle, &self->mEnd, 4);

    self->mSize = self->mStart - self->mFile->GetCurrentPos();
}
