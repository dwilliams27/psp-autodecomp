// gcDesiredCamera.cpp - gcAll_psp.obj

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cFileHandle;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDesiredCamera {
public:
    gcDesiredCamera(cBase *);

    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    void GetText(char *) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

struct GetTextRec {
    short offset;
    short _pad;
    void (*fn)(void *, char *);
};

void cStrCat(char *, const char *);

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

struct ReadRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFileHandle *, cMemPool *);
};

extern cType *D_000385DC;
extern cType *D_0009F408;

// 0x00121a90 - gcDesiredCamera::GetText(char *) const
void gcDesiredCamera::GetText(char *buf) const {
    int val = *(const int *)((const char *)this + 8);
    if (val == 0) {
        char *typeInfo = *(char **)((const char *)this + 16);
        GetTextRec *rec = (GetTextRec *)(typeInfo + 0x78);
        short off = rec->offset;
        void *base = (char *)this + 12;
        rec->fn((char *)base + off, buf);
    } else {
        cStrCat(buf, (const char *)0x36DAF0);
    }
    cStrCat(buf, (const char *)0x36DADC);
    cStrCat(buf, (const char *)0x36DE70);
}

// 0x00121490 - gcDesiredCamera::Write(cFile &) const
void gcDesiredCamera::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.Write(*(int *)((const char *)this + 8));

    char *typeInfo = *(char **)((const char *)this + 16);
    WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
    short off = rec->offset;
    void *base = (char *)this + 12;
    rec->fn((char *)base + off, *(cFile **)&wb);

    wb.End();
}

// 0x001214f0 - gcDesiredCamera::Read(cFile &, cMemPool *)
int gcDesiredCamera::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 8, 4);
    char *typeInfo = *(char **)((char *)this + 0x10);
    char *base = (char *)this + 0x0C;
    ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
    short offset = rec->offset;
    cFileHandle *handle = *(cFileHandle **)&rb[0];
    char *adjusted = base + offset;
    cMemPool *childPool = cMemPool::GetPoolFromPtr(base);
    void (*fn)(void *, cFileHandle *, cMemPool *) = rec->fn;
    fn(adjusted, handle, childPool);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// 0x00262AC4 - gcDesiredCamera::New(cMemPool *, cBase *) static
cBase *gcDesiredCamera::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDesiredCamera *result = 0;
    gcDesiredCamera *obj = (gcDesiredCamera *)rec->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDesiredCamera(parent);
        result = obj;
    }
    return (cBase *)result;
}

// 0x00262B40 - gcDesiredCamera::GetType(void) const
const cType *gcDesiredCamera::GetType(void) const {
    if (D_0009F408 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_0009F408 = cType::InitializeType(0, 0, 0xA6, D_000385DC,
                                           &gcDesiredCamera::New, 0, 0, 0);
    }
    return D_0009F408;
}
