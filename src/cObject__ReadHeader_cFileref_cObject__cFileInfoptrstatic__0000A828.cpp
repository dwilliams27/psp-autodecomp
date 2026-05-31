// Split TU for cObject::ReadHeader(cFile &, cObject::cFileInfo *) static.
// Keep local declarations minimal so this match does not perturb the
// existing cObject.cpp translation unit.

class cFile;
class cFileHandle;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cName {
public:
    void Read(cReadBlock &);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern "C" void __0fFcNameEReadR6KcReadBlock(void *, cReadBlock &);
extern "C" void __0fLcFileSystemEReadP6LcFileHandlePvUiT(cFileHandle *, void *,
                                                         unsigned int);

class cObject {
public:
    struct cFileInfo {
        int mField0;    // 0x00
        int mField4;    // 0x04
        char mName[0x18]; // 0x08  (cName)
        int mField20;   // 0x20
        int mField24;   // 0x24
        int mField28;   // 0x28
    };
    static void ReadHeader(cFile &file, cFileInfo *info);
};

static inline void FsRead(int *rb, void *dst, unsigned int n) {
    cFileHandle *h = *(cFileHandle **)(*(void **)&rb[0]);
    __0fLcFileSystemEReadP6LcFileHandlePvUiT(h, dst, n);
}

void cObject::ReadHeader(cFile &file, cFileInfo *info) {
    int rb[5];
    int tmp;
    int zero;

    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);

    FsRead(rb, &tmp, 4);
    zero = 0;
    FsRead(rb, &zero, 4);
    FsRead(rb, &info->mField0, 4);
    FsRead(rb, &info->mField4, 4);
    __0fFcNameEReadR6KcReadBlock(&info->mName, *(cReadBlock *)rb);
    FsRead(rb, &info->mField20, 4);
    FsRead(rb, &info->mField28, 4);
    if ((unsigned int)rb[3] >= 2) {
        FsRead(rb, &info->mField24, 4);
    }

    __0oKcReadBlockdtv(rb, 2);
}
