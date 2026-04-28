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

    void Write(bool);
    void Write(char);
    void Write(unsigned char);
    void Write(short);
    void Write(unsigned short);
    void Write(wchar_t);
    void Write(float);
    void Write(int, const bool *);
    void Write(int, const char *);
    void Write(int, const unsigned char *);
    void Write(int, const int *);
    void Write(int, const short *);
    void Write(int, const unsigned short *);
    void Write(int, const wchar_t *);
};

extern unsigned char gByteSwap;

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

void cWriteBlock::Write(short data) {
    short tmp;
    if (gByteSwap) {
        tmp = (short)(((data & 0xff00) >> 8) | ((data & 0xff) << 8));
    } else {
        tmp = data;
    }
    data = tmp;
    cFileSystem::Write(mFile->mHandle, &data, 2);
}

void cWriteBlock::Write(unsigned short data) {
    unsigned short tmp;
    if (gByteSwap) {
        int val = data & 0xffff;
        tmp = (unsigned short)(((val & 0xff00) >> 8) | ((val & 0xff) << 8));
    } else {
        tmp = data;
    }
    data = tmp;
    cFileSystem::Write(mFile->mHandle, &data, 2);
}

void cWriteBlock::Write(wchar_t data) {
    wchar_t tmp;
    if (gByteSwap) {
        int val = data & 0xffff;
        tmp = (wchar_t)(((val & 0xff00) >> 8) | ((val & 0xff) << 8));
    } else {
        tmp = data;
    }
    data = tmp;
    cFileSystem::Write(mFile->mHandle, &data, 2);
}

void cWriteBlock::Write(float data) {
    unsigned int u = *(unsigned int *)&data;
    if (gByteSwap) {
        unsigned int hi = ((u & 0xFF000000) >> 24) | ((u & 0xFF0000) >> 8);
        unsigned int lo = ((u & 0xFF00) << 8) | ((u & 0xFF) << 24);
        u = hi | lo;
    }
    int bits = (int)u;
    cFileSystem::Write(mFile->mHandle, &bits, 4);
}

void cWriteBlock::Write(int count, const int *data) {
    if (!gByteSwap) {
        cFileSystem::Write(mFile->mHandle, data, count * 4);
    } else {
        while (count > 0) {
            int val = *data;
            int hi = ((val & 0xFF000000) >> 24) | ((val & 0xFF0000) >> 8);
            int mid = (val & 0xFF00) << 8;
            hi = hi | mid;
            int lo = (val & 0xFF) << 24;
            int tmp = hi | lo;
            cFileSystem::Write(mFile->mHandle, &tmp, 4);
            count--;
            data++;
        }
    }
}

void cWriteBlock::Write(int count, const short *data) {
    if (!gByteSwap) {
        cFileSystem::Write(mFile->mHandle, data, count * 2);
    } else {
        while (count > 0) {
            int val = ((int)*data << 16) >> 16;
            short tmp = (short)(((val & 0xff00) >> 8) | ((val & 0xff) << 8));
            cFileSystem::Write(mFile->mHandle, &tmp, 2);
            count--;
            data++;
        }
    }
}

void cWriteBlock::Write(int count, const bool *data) {
    for (int i = 0; i < count; i++) {
        Write(data[i]);
    }
}
