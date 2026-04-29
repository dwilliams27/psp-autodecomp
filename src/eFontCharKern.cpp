class cFile {
public:
    void *mHandle;
};

class cWriteBlock {
public:
    cFile *mFile;

    void Write(char);
    void Write(unsigned char);
    void Write(unsigned short);
    void Write(wchar_t);
};

class cReadBlock {
public:
    int _data[5];
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class eFontCharKern {
public:
    wchar_t mChar;                // 0x0
    short mField2;                // 0x2
    short mField4;                // 0x4
    unsigned char mField6;        // 0x6
    unsigned char mField7;        // 0x7
    char mField8;                 // 0x8
    char mField9;                 // 0x9
    unsigned char mFieldA;        // 0xA
    char mFieldB;                 // 0xB
    unsigned char mFieldC;        // 0xC

    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

void eFontCharKern::Write(cWriteBlock &wb) const {
    wb.Write(mChar);
    wb.Write((unsigned short)(mField2 & 0xFFF));
    wb.Write((unsigned short)(mField4 & 0xFFF));
    wb.Write(mField6);
    wb.Write(mField7);
    wb.Write(mField8);
    wb.Write(mField9);
    wb.Write(mFieldA);
    wb.Write(mFieldB);
    wb.Write(mFieldC);
}

void eFontCharKern::Read(cReadBlock &rb) {
    if ((unsigned int)rb._data[3] >= 7) {
        if ((unsigned int)rb._data[3] >= 8) {
            cFileSystem::Read(*(void **)rb._data[0], this, 2);
        }

        unsigned short bits2;
        cFileSystem::Read(*(void **)rb._data[0], &bits2, 2);
        mField2 = (short)((mField2 & ~0xFFF) | (bits2 & 0xFFF));

        unsigned short bits4;
        cFileSystem::Read(*(void **)rb._data[0], &bits4, 2);
        mField4 = (short)((mField4 & ~0xFFF) | (bits4 & 0xFFF));

        cFileSystem::Read(*(void **)rb._data[0], &mField6, 1);
        cFileSystem::Read(*(void **)rb._data[0], &mField7, 1);
        cFileSystem::Read(*(void **)rb._data[0], &mField8, 1);
        cFileSystem::Read(*(void **)rb._data[0], &mField9, 1);
        cFileSystem::Read(*(void **)rb._data[0], &mFieldA, 1);
        cFileSystem::Read(*(void **)rb._data[0], &mFieldB, 1);

        if ((unsigned int)rb._data[3] >= 8) {
            cFileSystem::Read(*(void **)rb._data[0], &mFieldC, 1);
        }
    } else if ((unsigned int)rb._data[3] >= 6) {
        float old8;
        float old9;
        float oldA;
        float oldB;

        cFileSystem::Read(*(void **)rb._data[0], &old8, 4);
        cFileSystem::Read(*(void **)rb._data[0], &old9, 4);
        cFileSystem::Read(*(void **)rb._data[0], &oldA, 4);
        cFileSystem::Read(*(void **)rb._data[0], &oldB, 4);

        mField8 = (char)(int)old8;
        mField9 = (char)(int)old9;
        mFieldA = (unsigned char)(int)oldA;
        mFieldB = (char)(int)oldB;
    } else {
        cFileSystem::Read(*(void **)rb._data[0], &mField8, 1);
        cFileSystem::Read(*(void **)rb._data[0], &mField9, 1);
        cFileSystem::Read(*(void **)rb._data[0], &mFieldA, 1);
        cFileSystem::Read(*(void **)rb._data[0], &mFieldB, 1);
    }
}
