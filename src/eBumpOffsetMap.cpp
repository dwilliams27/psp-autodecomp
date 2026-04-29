#include "eTextureMap.h"

class cObject {
public:
    cObject &operator=(const cObject &);
};

template <class T> T *dcast(const cBase *);

class cFilename;

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
};

extern "C" int cGetCurrentPlatform(void);
extern int gSomePlatformDefault;
extern "C" void eTexture___dtor_eTexture_void(void *, int);
extern "C" void eBumpOffsetMap__GetRelativeFilename_unsignedint_cLanguage__cLanguagesconst(
    cStr *, const eBumpOffsetMap *, unsigned int, cLanguage::cLanguages);

// ── GetNumExternalDependencies ──

int eBumpOffsetMap::GetNumExternalDependencies(void) const {
    return ((*(short *)((char *)this + 0x1C) == 0) & 0xFF) == 0;
}

// ── Constructor ──

extern char eBumpOffsetMapclassdesc[];

eBumpOffsetMap::eBumpOffsetMap(cBase *parent) : eTexture(parent) {
    ((void **)this)[1] = eBumpOffsetMapclassdesc;
    __asm__ volatile("" ::: "memory");
    ((unsigned char *)this)[0x4C] = 1;
    *(float *)((char *)this + 0x50) = 0.1f;
    *(int *)((char *)this + 0x54) = 0;
}

// ── Write ──

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(float);
    void End(void);
};

void eBumpOffsetMap::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    eTexture::Write(file);
    wb.Write(field_4C);
    wb.Write(field_50);
    wb.End();
}

// ── AssignCopy ──

void eBumpOffsetMap::AssignCopy(const cBase *base) {
    eBumpOffsetMap *other = dcast<eBumpOffsetMap>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    ((signed char *)this)[0x44] = ((const signed char *)other)[0x44];
    ((signed char *)this)[0x45] = ((const signed char *)other)[0x45];
    ((signed char *)this)[0x46] = ((const signed char *)other)[0x46];
    ((unsigned char *)this)[0x47] = ((const unsigned char *)other)[0x47];
    ((short *)this)[0x48 / 2] = ((const short *)other)[0x48 / 2];
    ((short *)this)[0x4A / 2] = ((const short *)other)[0x4A / 2];
    ((unsigned char *)this)[0x4C] = ((const unsigned char *)other)[0x4C];
    ((float *)this)[0x50 / 4] = ((const float *)other)[0x50 / 4];
    ((int *)this)[0x54 / 4] = ((const int *)other)[0x54 / 4];
}

// ── New ──

void eBumpOffsetMap_eBumpOffsetMap(eBumpOffsetMap *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

eBumpOffsetMap *eBumpOffsetMap::New(cMemPool *pool, cBase *parent) {
    eBumpOffsetMap *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eBumpOffsetMap *obj = (eBumpOffsetMap *)entry->fn(base, 0x58, 4, 0, 0);
    if (obj != 0) {
        eBumpOffsetMap_eBumpOffsetMap(obj, parent);
        result = obj;
    }
    return result;
}

// ── Destructor ──

extern char eBumpOffsetMapvirtualtable[];

eBumpOffsetMap::~eBumpOffsetMap(void) {
    *(void **)((char *)this + 4) = eBumpOffsetMapvirtualtable;
    PlatformFree();
    eTexture___dtor_eTexture_void(this, 0);
}

// ── GetExternalDependency ──

void eBumpOffsetMap::GetExternalDependency(int, cFilename *out) const {
    cStr filename;
    cStr temp;
    cStr *ret = &temp;
    __asm__ volatile("" : : "r"(ret) : "memory");
    unsigned int platform = 9;
    int usePlatform = ((*(const unsigned short *)((const char *)this + 0x28) & 2) != 0) & 0xFF;
    if (usePlatform != 0) {
        platform = (unsigned int)cGetCurrentPlatform();
    }
    int lang = 0xC;
    int useLang = ((*(const unsigned short *)((const char *)this + 0x28) & 0x100) != 0) & 0xFF;
    if (useLang != 0) {
        lang = gSomePlatformDefault;
    }
    eBumpOffsetMap__GetRelativeFilename_unsignedint_cLanguage__cLanguagesconst(
        ret, this, platform, (cLanguage::cLanguages)lang);
    filename = temp;
    ((cStr *)out)->Set((const char *)&filename);
}

// ── Stubs (already matched) ──

void eBumpOffsetMap::PlatformFree(void) {
}

void eBumpOffsetMap::CreateData(void) {
}

void eBumpOffsetMap::Apply(int, const eCamera *, const eWorld *) const {
}
