// gcDesiredEntityHelper::Read(cReadBlock &) @ 0x0011d1c4
// Obj: gcAll_psp.obj
// Symbol: __0fVgcDesiredEntityHelperEReadR6KcReadBlock
//
// Split TU: declare everything locally so this Read match does not perturb
// the existing gcDesiredEntityHelper.cpp translation unit. Mirrors the
// sibling Write (0x0011d15c) struct layout and the gcDesiredCustomAnimation
// cReadBlock read idiom.

class cFile;
class cFileHandle;
class cMemPool;
class cReadBlock;

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cHandle {
public:
    int mId;
    void Read(cReadBlock &, cMemPool *);
};

class cReadBlock {
public:
    cFile *_file;
    int _data[4];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct gcDesiredEntityHelper {
    char mField0;
    char mField1;
    char mField2;
    cHandle mField4;
    void Read(cReadBlock &);
};

// 0x0011d1c4 — gcDesiredEntityHelper::Read(cReadBlock &), 164B
void gcDesiredEntityHelper::Read(cReadBlock &outer) {
    cReadBlock rb(*outer._file, 2, true);

    cFileSystem::Read(*(cFileHandle **)rb._file, &mField0, 1);
    cFileSystem::Read(*(cFileHandle **)rb._file, &mField1, 1);
    cFileSystem::Read(*(cFileHandle **)rb._file, &mField2, 1);

    mField4.mId = 0;
    mField4.Read(rb, cMemPool::GetPoolFromPtr(&mField4));
}
