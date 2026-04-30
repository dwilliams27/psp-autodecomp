class cBase;
class cMemPool;
class cType;
class cFile;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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

class eConfigBase {
public:
    eConfigBase(cBase *);
    ~eConfigBase();

    void Write(cFile &) const;
    const char *GetImageFileFormatName(void) const;
    int GetPhysicsMemPoolSize(void) const;
    const cType *GetType(void) const;
    unsigned int GetAudioFrequency(bool) const;
    unsigned int GetAudioBytesPerSecond(unsigned int) const;
    static int FindResolution(int, int);
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

class cWriteBlock {
public:
    int _data[2];

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(unsigned char);
    void Write(int);
    void Write(float);
    void End(void);
};

extern "C" {
    int ePhysics__GetPhysicsMemPoolSize_voidstatic(void);
    void eConfigBase__eConfigBase_cBaseptr(void *self, cBase *parent);
    int cGetCurrentPlatform(void);
}

extern cType *D_000385DC;
extern cType *D_00040E78;

void eConfigBase::Write(cFile &file) const {
    cWriteBlock wb(file, 0xB);
    wb.Write(*(const int *)((const char *)this + 8));
    wb.Write(*(const int *)((const char *)this + 0xC));
    wb.Write(*(const int *)((const char *)this + 0x10));
    wb.Write(*(const bool *)((const char *)this + 0x14));
    wb.Write(*(const int *)((const char *)this + 0x18));
    wb.Write(*(const int *)((const char *)this + 0x1C));
    wb.Write(*(const int *)((const char *)this + 0x20));
    wb.Write(*(const int *)((const char *)this + 0x24));
    wb.Write(*(const bool *)((const char *)this + 0x28));
    wb.Write(*(const bool *)((const char *)this + 0x29));
    wb.Write(*(const bool *)((const char *)this + 0x2A));
    wb.Write(*(const int *)((const char *)this + 0x2C));
    wb.Write(*(const int *)((const char *)this + 0x30));
    wb.Write(*(const float *)((const char *)this + 0x34));
    wb.Write(*(const float *)((const char *)this + 0x38));
    wb.Write(*(const float *)((const char *)this + 0x3C));
    wb.Write(*(const float *)((const char *)this + 0x40));
    wb.Write(*(const float *)((const char *)this + 0x44));
    wb.Write(*(const unsigned char *)((const char *)this + 0x48));
    wb.Write(*(const unsigned char *)((const char *)this + 0x49));
    wb.Write(*(const float *)((const char *)this + 0x4C));
    wb.Write(*(const float *)((const char *)this + 0x50));
    wb.Write(*(const int *)((const char *)this + 0x54));
    wb.Write(*(const int *)((const char *)this + 0x58));
    wb.Write(*(const int *)((const char *)this + 0x5C));
    wb.Write(*(const int *)((const char *)this + 0x60));
    wb.Write(*(const int *)((const char *)this + 0x64));
    wb.Write(*(const int *)((const char *)this + 0x68));
    wb.End();
}

// ── eConfigBase::GetPhysicsMemPoolSize(void) const @ 0x0001cb18 ──
int eConfigBase::GetPhysicsMemPoolSize(void) const {
    return ePhysics__GetPhysicsMemPoolSize_voidstatic();
}

// ── eConfigBase::FindResolution(int, int) static @ 0x0001cabc ──
int eConfigBase::FindResolution(int width, int height) {
    const int *w = (const int *)0x37D070;
    const int *h = (const int *)0x37D09C;
    int i = 0;
    do {
        if (*w == width && *h == height) return i;
        ++w;
        ++h;
        ++i;
    } while (i < 11);
    return 0;
}

// ── eConfigBase::GetAudioBytesPerSecond(unsigned int) const @ 0x001df540 ──
unsigned int eConfigBase::GetAudioBytesPerSecond(unsigned int mode) const {
    unsigned int result;
    switch (mode) {
    default: return 0x15888;
    case 1: return 0x80;
    case 2: return 0x6108;
    case 3: return 0x6270;
    case 4: return 0x6270;
    case 5: return 0x15888;
    case 6: return 0x6270;
    case 7: return 0x6108;
    case 8:
        result = 0x4800;
        __asm__ volatile("" ::: "memory");
        return result;
    }
}

// ── eConfigBase::New(cMemPool *, cBase *) static @ 0x001df384 ──
cBase *eConfigBase::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eConfigBase *result = 0;
    eConfigBase *obj = (eConfigBase *)rec->fn(base, 0x6C, 4, 0, 0);
    if (obj != 0) {
        eConfigBase__eConfigBase_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── eConfigBase::~eConfigBase(void) @ 0x0001c918 ──
eConfigBase::~eConfigBase() {
    *(void **)((char *)this + 4) = (void *)0x37F8B0;
    *(eConfigBase **)0x37D0C8 = 0;
    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}

// ── eConfigBase::GetType(void) const @ 0x001df400 ──
const cType *eConfigBase::GetType(void) const {
    if (D_00040E78 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                               (const char *)0x36CD7C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00040E78 = cType::InitializeType(0, 0, 0x28B, D_000385DC,
                                           &eConfigBase::New, 0, 0, 0);
    }
    return D_00040E78;
}

// ── eConfigBase::GetAudioFrequency(bool) const @ 0x001df4a0 ──
unsigned int eConfigBase::GetAudioFrequency(bool high) const {
    int platform = cGetCurrentPlatform();
    unsigned int flag = (unsigned char)high;
    flag = (unsigned char)flag;
    unsigned int result;
    switch (platform) {
    default:
        result = 0xAC44;
        break;
    case 1:
        result = 0x7D00;
        break;
    case 2:
        result = 0xAC44;
        break;
    case 3:
        result = 0xAC44;
        break;
    case 4:
        result = 0xAC44;
        break;
    case 5:
        result = 0xAC44;
        break;
    case 6:
        result = flag ? 0xBB80 : 0xAC44;
        break;
    case 7:
        result = 0xAC44;
        break;
    case 8:
        result = 0x7D00;
        break;
    }
    return result;
}
