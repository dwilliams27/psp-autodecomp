// eSprite.cpp -- decompiled from eAll_psp.obj.
//
// Functions:
//   0x00038230 eSprite::Write(cFile &) const
//   0x00038470 eSprite::eSprite(cBase *)
//   0x00038ce8 eSprite::SetTexCoord1(mVec2)
//   0x00038d44 eSprite::SetTexCoord2(mVec2)
//   0x001e6f84 eSprite::New(cMemPool *, cBase *) static
//   0x001e7428 eSprite::~eSprite(void)

class cBase;
class cFile;
class cMemPool;
class cType;
struct mVec2 {
    float x;
    float y;

    mVec2(const mVec2 &);
};

class cWriteBlock {
public:
    int _data[2];

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(int, const float *);
    void End(void);
};

class cHandle {
public:
    int _data;

    void Write(cWriteBlock &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

inline void *operator new(unsigned int, void *p) {
    return p;
}

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class cObject {
public:
    char _pad[0x44];

    cObject(cBase *);
    ~cObject();
    cObject &operator=(const cObject &);
    void Write(cFile &) const;
};

class eSprite : public cObject {
public:
    int mField44;
    cHandle mField48;
    float mField4C;
    float mField50;
    float mField54;
    float mField58;
    float mField5C;
    float mField60;
    float mField64;
    float mField68;

    eSprite(cBase *);
    ~eSprite();
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void SetTexCoord1(mVec2);
    void SetTexCoord2(mVec2);
    bool GetTextureSize(mVec2 *) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

template <class T> T *dcast(const cBase *);

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00041110;

struct CopyWord {
    int value;
};

// -- eSprite::eSprite(cBase *) @ 0x00038470 --
eSprite::eSprite(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x3806A8;
    mField44 = 0;
    mField48._data = 0;
    mField4C = 32.0f;
    mField50 = 32.0f;
    mField54 = 0.0f;
    mField58 = 0.0f;
    mField5C = 1.0f;
    mField60 = 1.0f;
    mField64 = 0.0f;
    mField68 = 0.0f;
}

// -- eSprite::Write(cFile &) const @ 0x00038230 --
void eSprite::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    ((const cObject *)this)->Write(file);
    wb.Write(mField44);
    wb.Write(2, &mField4C);
    mField48.Write(wb);
    wb.Write(2, &mField54);
    wb.Write(2, &mField5C);
    wb.Write(2, &mField64);
    wb.End();
}

// -- eSprite::SetTexCoord1(mVec2) @ 0x00038ce8 --
void eSprite::SetTexCoord1(mVec2 texCoord) {
    float textureSize[2];

    if (GetTextureSize((mVec2 *)textureSize)) {
        mField54 = texCoord.x / textureSize[0];
        mField58 = texCoord.y / textureSize[1];
    }
}

// -- eSprite::SetTexCoord2(mVec2) @ 0x00038d44 --
void eSprite::SetTexCoord2(mVec2 texCoord) {
    float textureSize[2];

    if (GetTextureSize((mVec2 *)textureSize)) {
        mField5C = texCoord.x / textureSize[0];
        mField60 = texCoord.y / textureSize[1];
    }
}

// -- eSprite::AssignCopy(const cBase *) @ 0x001e6eec --
void eSprite::AssignCopy(const cBase *src) {
    eSprite *other = dcast<eSprite>(src);
    ((cObject *)this)->operator=(*(const cObject *)other);
    *(int *)((char *)this + 0x44) = *(const int *)((char *)other + 0x44);
    *(CopyWord *)((char *)this + 0x48) = *(CopyWord *)((char *)other + 0x48);
    *(float *)((char *)this + 0x4C) = *(const float *)((char *)other + 0x4C);
    *(float *)((char *)this + 0x50) = *(const float *)((char *)other + 0x50);
    *(float *)((char *)this + 0x54) = *(const float *)((char *)other + 0x54);
    *(float *)((char *)this + 0x58) = *(const float *)((char *)other + 0x58);
    *(float *)((char *)this + 0x5C) = *(const float *)((char *)other + 0x5C);
    *(float *)((char *)this + 0x60) = *(const float *)((char *)other + 0x60);
    *(float *)((char *)this + 0x64) = *(const float *)((char *)other + 0x64);
    *(float *)((char *)this + 0x68) = *(const float *)((char *)other + 0x68);
}

// -- eSprite::New(cMemPool *, cBase *) static @ 0x001e6f84 --
cBase *eSprite::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eSprite *result = 0;
    eSprite *obj = (eSprite *)rec->fn(base, 0x6C, 4, 0, 0);
    if (obj != 0) {
        new (obj) eSprite(parent);
        result = obj;
    }
    return (cBase *)result;
}

// -- eSprite::GetType(void) const @ 0x001e7000 --
const cType *eSprite::GetType(void) const {
    if (D_00041110 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_00041110 = cType::InitializeType(0, 0, 0x3D, D_000385E4,
                                           &eSprite::New,
                                           (const char *)0x36CDDC,
                                           (const char *)0x36CDE4, 5);
    }
    return D_00041110;
}

// -- eSprite::~eSprite(void) @ 0x001e7428 --
eSprite::~eSprite() {
    *(void **)((char *)this + 4) = (void *)0x3806A8;
}
