// eSoundData.cpp — decompiled from eAll_psp.obj
// Parent: cObject. Vtable at 0x37F9B0 (eSoundDatavirtualtable).

class cBase;
class cFile;
class cFilename;
class cMemPool;

class cObject {
public:
    cObject(cBase *);
    ~cObject(void);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class eSoundWaveData;
extern "C" void eSoundWaveData___dtor_eSoundWaveData_void(eSoundWaveData *, int);

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

class cStr {
public:
    char _data[256];
    void Set(const char *, ...);
};

class cLanguage {
public:
    enum cLanguages {
        kLang0 = 0,
    };
    static int GetSupportedVoiceLanguage(cLanguages);
};

extern "C" int cGetCurrentPlatform(void);

extern int gSomePlatformDefault;  // address 0x37C06C

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

struct VtblSlot20 {
    short offset;
    short pad;
    void (*fn)(void *self);
};

extern "C" void cObject___dtor_cObject_void(void *, int);
extern "C" void *cMemPool_GetPoolFromPtr(const void *);

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
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

extern const char eSoundData_cBase_name[];      // 0x36CD74 = "cBase"
extern const char eSoundData_cBase_desc[];      // 0x36CD7C = "Base"
extern const char eSoundData_kind_name[];       // 0x36CD84 = "Sounds"
extern const char eSoundData_kind_desc[];       // 0x36CD90 = "snd"

static cType *type_cBase;          // 0x385DC
static cType *type_cNamed;         // 0x385E0
static cType *type_eSoundData_3;   // 0x385E4
static cType *type_eSoundData;     // 0x40F6C

class eSoundData : public cObject {
public:
    char _padA[0x28];          // cObject members to offset 0x2C
    unsigned short mFlags28;   // 0x28
    char _padB[0x1A];          // to 0x44
    int mField44;              // 0x44
    int mField48;              // 0x48
    float mField4C;            // 0x4C
    bool mField50;             // 0x50
    bool mField51;             // 0x51
    bool mField52;             // 0x52
    char _pad53;               // 0x53
    float mField54;            // 0x54
    float mField58;            // 0x58
    unsigned char mField5C;    // 0x5C
    char _pad5D[3];            // 0x5D
    eSoundWaveData *mWaveData; // 0x60
    int mField64;              // 0x64

    eSoundData(cBase *);
    ~eSoundData(void);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
    void PlatformFree(void);
    void Write(cFile &) const;
    void GetExternalDependency(int, cFilename *) const;
    cStr GetRelativeFilename(unsigned int, cLanguage::cLanguages) const;
    bool IsVoiceOver(void) const;
    int GetNumExternalDependencies(void) const;
    const cType *GetType(void) const;
    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
};

extern char eSoundDatavirtualtable[];

// ── PlatformFree @ 0x00020cc8 ──
void eSoundData::PlatformFree(void) {
    if (mWaveData != 0) {
        eSoundWaveData___dtor_eSoundWaveData_void(mWaveData, 3);
        mWaveData = 0;
    }
}

// ── Constructor @ 0x00020d00 ──
eSoundData::eSoundData(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = eSoundDatavirtualtable;
    mField44 = 0;
    mField48 = 0;
    mField4C = 1.0f;
    mField50 = 0;
    mField51 = 0;
    mField52 = 0;
    mField54 = 10.0f;
    mField58 = 2.5f;
    mField5C = 0;
    mWaveData = 0;
    mField64 = 0;
}

// ── Destructor @ 0x00020d78 ──
// Canonical C++ destructor; SNC ABI auto-generates the (this != 0) guard,
// the ~cObject() base-chain call (with flags=0), and the deleting-tail
// `if (flags & 1) cMemPool_dispatch(this)` epilogue.
eSoundData::~eSoundData(void) {
    *(void **)((char *)this + 4) = eSoundDatavirtualtable;
    PlatformFree();
}

// ── IsVoiceOver @ 0x001dfe84 ──
bool eSoundData::IsVoiceOver(void) const {
    return false;
}

// ── GetNumExternalDependencies @ 0x001dfe70 ──
// Reads mFlags-equivalent short at this+0x1C (decimal 28), inside the
// cObject base. Equivalent to: !((flags1c == 0) & 0xFF), i.e. flags1c != 0.
// SNC C++ frontend would fold `(bool)(x == 0) == 0` to `x != 0` (3 insns);
// the original keeps the redundant bool→byte→bool round-trip (5 insns)
// — match it via an explicit unsigned-char truncation.
int eSoundData::GetNumExternalDependencies(void) const {
    return ((unsigned char)(*(const short *)((const char *)this + 28) == 0)) == 0;
}

extern "C" void *cMemPool_GetPoolFromPtr(const void *);

// ── Write @ 0x00020690 ──
void eSoundData::Write(cFile &file) const {
    cWriteBlock wb(file, 5);
    cObject::Write(file);
    wb.Write(mField64);
    wb.Write(mField44);
    wb.Write(mField48);
    wb.Write(mField4C);
    wb.Write(mField50);
    wb.Write(mField51);
    wb.Write(mField52);
    wb.Write(mField54);
    wb.Write(mField58);
    wb.Write(mField5C);
    wb.End();
}

// ── GetType @ 0x001dfd4c ──
// Standard cType static-init chain. 4 levels: cBase(1) → cNamed(2 + factory)
// → unnamed(3) → eSoundData(0x26 + name/desc strings + flag=1).
const cType *eSoundData::GetType(void) const {
    if (!type_eSoundData) {
        if (!type_eSoundData_3) {
            if (!type_cNamed) {
                if (!type_cBase) {
                    type_cBase = cType::InitializeType(
                        eSoundData_cBase_name, eSoundData_cBase_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_cNamed = cType::InitializeType(
                    0, 0, 2, type_cBase,
                    (cBase *(*)(cMemPool *, cBase *))&cNamed::New,
                    0, 0, 0);
            }
            type_eSoundData_3 = cType::InitializeType(
                0, 0, 3, type_cNamed, 0, 0, 0, 0);
        }
        type_eSoundData = cType::InitializeType(
            0, 0, 0x26, type_eSoundData_3, 0,
            eSoundData_kind_name, eSoundData_kind_desc, 1);
    }
    return type_eSoundData;
}

// ── Read @ 0x00020754 ──
// Versioned binary read (versions 2..5). Reads 4 ints (mField64, mField44,
// mField48, mField4C), then 1-byte bools, then mField54, then a version-
// gated mField58 (or computed = mField54*0.25f when v<4), then optional
// mField5C at v>=3, then a virtual call at vtbl[20], then PlatformRead.
int eSoundData::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 5, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));

    if ((unsigned int)rb._data[3] >= 6) goto fail;
    if ((unsigned int)rb._data[3] < 2) goto fail;
    if (!cObject::Read(file, pool)) goto fail;

    cFileSystem::Read(*(void **)rb._data[0], &mField64, 4);
    cFileSystem::Read(*(void **)rb._data[0], &mField44, 4);
    cFileSystem::Read(*(void **)rb._data[0], &mField48, 4);
    cFileSystem::Read(*(void **)rb._data[0], &mField4C, 4);
    {
        char tmp50;
        cFileSystem::Read(*(void **)rb._data[0], &tmp50, 1);
        mField50 = tmp50 != 0;
    }
    if ((unsigned int)rb._data[3] >= 5) goto read_51;
    goto read_52;

fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;

read_51:
    {
        char tmp51;
        cFileSystem::Read(*(void **)rb._data[0], &tmp51, 1);
        mField51 = tmp51 != 0;
    }

read_52:
    {
        char tmp52;
        cFileSystem::Read(*(void **)rb._data[0], &tmp52, 1);
        mField52 = tmp52 != 0;
    }
    cFileSystem::Read(*(void **)rb._data[0], &mField54, 4);
    if ((unsigned int)rb._data[3] >= 4) {
        cFileSystem::Read(*(void **)rb._data[0], &mField58, 4);
    } else {
        mField58 = mField54 * 0.25f;
    }
    if ((unsigned int)rb._data[3] >= 3) {
        cFileSystem::Read(*(void **)rb._data[0], &mField5C, 1);
    }
    // Virtual dispatch via fixed vtable slot at offset 160 (slot 20).
    {
        VtblSlot20 *slot = (VtblSlot20 *)((char *)*(void **)((char *)this + 4) + 160);
        slot->fn((char *)this + slot->offset);
    }
    PlatformRead(file, pool);
    return result;
}

// ── GetExternalDependency @ 0x00020dfc ──
void eSoundData::GetExternalDependency(int, cFilename *out) const {
    cStr filename;
    int b1 = ((this->mFlags28 & 2) != 0) & 0xFF;
    int platform = 9;
    if (b1 != 0) {
        platform = cGetCurrentPlatform();
    }
    int b2 = ((this->mFlags28 & 0x100) != 0) & 0xFF;
    int lang = 0xC;
    if (b2 != 0) {
        lang = cLanguage::GetSupportedVoiceLanguage((cLanguage::cLanguages)gSomePlatformDefault);
    }
    filename = GetRelativeFilename((unsigned int)platform, (cLanguage::cLanguages)lang);
    ((cStr *)out)->Set((const char *)&filename);
}
