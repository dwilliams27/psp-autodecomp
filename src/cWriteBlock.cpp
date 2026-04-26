struct cFileHandle;

class cFile {
public:
    cFileHandle *mHandle;
};

class cFileSystem {
public:
    static int Write(cFileHandle *, const void *, unsigned int);
};

class cWriteBlock {
public:
    cFile *mFile;

    void Write(char);
    void Write(unsigned char);
    void Write(int, const char *);
    void Write(int, const unsigned char *);
    void Write(int, const short *);
    void Write(int, const unsigned short *);
    void Write(int, const wchar_t *);
};

void cWriteBlock::Write(char data) {
    cFileSystem::Write(mFile->mHandle, &data, 1);
}

void cWriteBlock::Write(unsigned char data) {
    cFileSystem::Write(mFile->mHandle, &data, 1);
}

void cWriteBlock::Write(int count, const char *data) {
    cFileSystem::Write(mFile->mHandle, data, count);
}

void cWriteBlock::Write(int count, const unsigned char *data) {
    cFileSystem::Write(mFile->mHandle, data, count);
}

void cWriteBlock::Write(int count, const unsigned short *data) {
    Write(count, (const short *)data);
}

void cWriteBlock::Write(int count, const wchar_t *data) {
    Write(count, (const short *)data);
}
