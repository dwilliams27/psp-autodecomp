// Split TU for cFactory::LoadLocalized(const char *) @ 0x0000b70c.
// Local declarations only; does not perturb cFactory.cpp.

class cFile;
class cFileHandle;
class cMemPool;
class cReadBlock;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cFile {
public:
    cFileHandle *mHandle;
    char _pad[0x10c];
    cFile(void);
    int Open(const char *);
    void Close(bool);
};

class cReadBlock {
public:
    cFile *mFile;
    int _data[4];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cGroup {
public:
    static void ReadLocalized(cFile &, cMemPool *);
};

class cObject {
public:
    char _pad[0x30];
};

class cFactory : public cObject {
public:
    void LoadLocalized(const char *);
};

void cFactory::LoadLocalized(const char *name) {
    cFile file;

    if (file.Open(name) == 0) {
        file.Close(true);
        return;
    }
    {
        cReadBlock block(file, 2, true);
        cMemPool *pool = cMemPool::GetPoolFromPtr(this);
        struct { char pad; char flag; } buf;

        for (;;) {
            cFileSystem::Read(block.mFile->mHandle, &buf.flag, 1);
            bool more = buf.flag != 0;
            if (!more) {
                break;
            }
            cGroup::ReadLocalized(file, pool);
        }
    }
    file.Close(true);
}
