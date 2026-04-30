struct cFileHandle;

struct cGUID {
    int a;
    int b;
};

class cFile {
public:
    cFileHandle *mHandle;

    unsigned int GetCurrentPos(void) const;
    void SetCurrentPos(unsigned int);
};

class cFileSystem {
public:
    static int Write(cFileHandle *, const void *, unsigned int);
};

class cWriteBlock {
public:
    cFile *mFile;
    unsigned int mOffset;

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(char);
    void Write(unsigned char);
    void Write(short);
    void Write(unsigned short);
    void Write(wchar_t);
    void Write(int);
    void Write(unsigned int);
    void Write(float);
    void Write(const cGUID &);
    void Write(int, const bool *);
    void Write(int, const char *);
    void Write(int, const unsigned char *);
    void Write(int, const int *);
    void Write(int, const short *);
    void Write(int, const unsigned short *);
    void Write(int, const wchar_t *);
};

extern unsigned char gByteSwap;

cWriteBlock::cWriteBlock(cFile &file, unsigned int type) : mFile(&file) {
    unsigned int pos = file.GetCurrentPos() + 3;
    pos >>= 2;
    mOffset = pos << 2;
    mFile->SetCurrentPos(mOffset);
    Write((int)0xBBBBBBBB);
    mOffset += 4;
    Write(0);
    Write(type);
}

void cWriteBlock::Write(bool data) {
    unsigned char tmp = (unsigned int)data;
    cFileSystem::Write(mFile->mHandle, &tmp, 1);
}

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

void cWriteBlock::Write(int data) {
    int tmp;
    if (gByteSwap) {
        int hi = ((data & 0xFF000000) >> 24) | ((data & 0xFF0000) >> 8);
        int mid = (data & 0xFF00) << 8;
        hi = hi | mid;
        int lo = (data & 0xFF) << 24;
        tmp = hi | lo;
    } else {
        tmp = data;
    }
    data = tmp;
    cFileSystem::Write(mFile->mHandle, &data, 4);
}

void cWriteBlock::Write(unsigned int data) {
    unsigned int tmp;
    if (gByteSwap) {
        unsigned int hi = ((data & 0xFF000000) >> 24) | ((data & 0xFF0000) >> 8);
        unsigned int mid = (data & 0xFF00) << 8;
        unsigned int lo = (data & 0xFF) << 24;
        lo = mid | lo;
        tmp = hi | lo;
    } else {
        tmp = data;
    }
    data = tmp;
    cFileSystem::Write(mFile->mHandle, &data, 4);
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

void cWriteBlock::Write(const cGUID &data) {
    unsigned char swap = gByteSwap;
    __asm__ volatile("" ::: "memory");
    unsigned int w0 = (unsigned int)data.a;
    __asm__ volatile("" : "+r"(w0) :: "memory");
    unsigned int w1 = (unsigned int)data.b;
    if (swap) {
        unsigned int hi = ((w0 & 0xFF000000) >> 24) | ((w0 & 0xFF0000) >> 8);
        unsigned int mid = (w0 & 0xFF00) << 8;
        hi = hi | mid;
        unsigned int lo = (w0 & 0xFF) << 24;
        w0 = hi | lo;

        hi = ((w1 & 0xFF000000) >> 24) | ((w1 & 0xFF0000) >> 8);
        mid = (w1 & 0xFF00) << 8;
        hi = hi | mid;
        lo = (w1 & 0xFF) << 24;
        w1 = hi | lo;
    }
    unsigned int a = w0;
    unsigned int b = w1;
    cFileSystem::Write(mFile->mHandle, &a, 4);
    cFileSystem::Write(mFile->mHandle, &b, 4);
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
