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

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" void free(void *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct DeleteEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
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
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            DeleteEntry *rec = (DeleteEntry *)(((PoolBlock *)block)->allocTable + 0x30);
            rec->fn(block + rec->offset, p);
        } else {
            free(p);
        }
    }
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

// ── cReadBlock::~cReadBlock(void)  @ 0x00005dbc, 212B ──
cReadBlock::~cReadBlock() {
    cFile *f = mFile;
    int isOpen = ((f->mHandle != 0) & 0xFF);
    if (isOpen) {
        cFile *fr = *(cFile * volatile *)&mFile;
        int dummy = 0;
        unsigned int pos = fr->GetCurrentPos();
        f->SetCurrentPos(((pos + 3) >> 2) << 2);
        int *pd = &dummy;
        __asm__ volatile("" : "+r"(pd));
        cFileSystem::Read(mFile->mHandle, pd, 4);
    }
}
