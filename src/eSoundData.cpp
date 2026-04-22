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

extern "C" void cObject___dtor_cObject_void(void *, int);
extern "C" void *cMemPool_GetPoolFromPtr(const void *);

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
    void PlatformFree(void);
    void Write(cFile &) const;
    void GetExternalDependency(int, cFilename *) const;
    cStr GetRelativeFilename(unsigned int, cLanguage::cLanguages) const;
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
extern "C" void eSoundData___dtor_eSoundData_void(eSoundData *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = eSoundDatavirtualtable;
        self->PlatformFree();
        cObject___dtor_cObject_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
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
