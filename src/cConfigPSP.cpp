class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cReadBlock;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void Write(int, const int *);
    void Write(int, const char *);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cReadBlock {
public:
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class cConfigBase {
public:
    cConfigBase(cBase *);
    ~cConfigBase();
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

class cConfigPSP : public cConfigBase {
public:
    cConfigPSP(cBase *);
    ~cConfigPSP();

    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char cConfigBasevirtualtable[];
extern cType *D_000385DC;
extern cType *D_00038888;
extern cType *D_0003888C;

void cStrCopy(char *, const char *, int);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

// cConfigPSP::New(cMemPool *, cBase *) static @ 0x001c6bf8
cBase *cConfigPSP::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    cConfigPSP *result = 0;
    cConfigPSP *obj = (cConfigPSP *)rec->fn(base, 0x2BC, 4, 0, 0);
    if (obj != 0) {
        new (obj) cConfigPSP(parent);
        result = obj;
    }
    return (cBase *)result;
}

// cConfigPSP::Write(cFile &) const @ 0x000087b4
void cConfigPSP::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    cConfigBase::Write(file);
    wb.Write(8);
    wb.Write(10);
    int i = 0;
    const char *name = (const char *)this + 0x244;
    do {
        wb.Write(10, name);
        i += 1;
        name += 10;
    } while (i < 8);
    wb.Write(8, (const int *)((const char *)this + 0x294));
    wb.Write(*(const int *)((const char *)this + 0x2B4));
    wb.Write(false);
    wb.End();
}

// cConfigPSP::Read(cFile &, cMemPool *) @ 0x00008870
int cConfigPSP::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    int count;
    int width;
    char enabled;

    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);
    if ((unsigned int)rb[3] >= 4 || (unsigned int)rb[3] < 1 ||
        ((cConfigBase *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], &count, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], &width, 4);

    int i = 0;
    char *name = (char *)this + 0x244;
    if (i < count) {
        do {
            if (width > 0) {
                cFileSystem::Read(*(cFileHandle **)rb[0], name, width);
            }
            i += 1;
            name += 10;
        } while (i < count);
    }

    if (count > 0) {
        cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x294,
                          count << 2);
    }

    if ((unsigned int)rb[3] >= 2) {
        cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x2B4, 4);
    }

    if ((unsigned int)rb[3] >= 3) {
        cFileSystem::Read(*(cFileHandle **)rb[0], &enabled, 1);
        *(unsigned char *)((char *)this + 0x2B8) = enabled != 0;
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// cConfigPSP::cConfigPSP(cBase *) @ 0x00008a28
cConfigPSP::cConfigPSP(cBase *parent) : cConfigBase(parent) {
    *(void **)((char *)this + 4) = (void *)0x37E838;
    *(int *)((char *)this + 0x2B4) = 0xDE;
    *(unsigned char *)((char *)this + 0x2B8) = 1;
    int i = 0;
    char *name = (char *)this;
    const char *src = (const char *)0x36CA58;
    int value = 3;
    name += 0x244;
    void *self = this;
    do {
        cStrCopy(name, src, 10);
        *(int *)((char *)self + 0x294) = value;
        i += 1;
        name += 10;
        self = (char *)self + 4;
    } while (i < 8);
}

// cConfigPSP::GetType(void) const @ 0x001c6c74
const cType *cConfigPSP::GetType(void) const {
    if (D_0003888C == 0) {
        if (D_00038888 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36C830,
                                                   (const char *)0x36C838,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00038888 = cType::InitializeType(0, 0, 0x28A, D_000385DC,
                                               &cConfigBase::New, 0, 0, 0);
        }
        D_0003888C = cType::InitializeType(0, 0, 0x291, D_00038888,
                                           &cConfigPSP::New, 0, 0, 0);
    }
    return D_0003888C;
}

// cConfigPSP::~cConfigPSP(void) @ 0x001c6d58
cConfigPSP::~cConfigPSP() {
    *(void **)((char *)this + 4) = cConfigBasevirtualtable;
}
