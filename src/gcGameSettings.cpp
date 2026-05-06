#include "eMemCard.h"
#include "gcMap.h"

inline void *operator new(unsigned int, void *p) { return p; }

extern "C" void *__vec_new(void *, int, int, void (*)(void *));
extern "C" void *memset(void *, int, unsigned int);

class cBase;
class cMemPool;
class cType;
class gcProfile;
class gcString;
class gcStringValue;

template <class T>
class cSubHandleT {
public:
    int mIndex;
    cSubHandleT() {}
    cSubHandleT(const cSubHandleT &o) : mIndex(o.mIndex) {}
    ~cSubHandleT() {}
};

class gcString {
public:
    char pad_00[0x18];
    const char *mText;
};

class gcStringTable {
public:
    gcString *GetSubObject(cSubHandleT<gcString>, int) const;
};
extern void *D_00038890[];

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcGameSettings {
public:
    cBase *mParent;            // 0x00
    void *mVtable;             // 0x04
    void *mRefListA;           // 0x08
    gcGameSettings *mSelfA;    // 0x0C
    void *mRefListB;           // 0x10
    gcGameSettings *mSelfB;    // 0x14
    void *mArray18;            // 0x18
    int mArray1C;              // 0x1C
    gcGameSettings *mSelf20;   // 0x20
    int mArray24;              // 0x24
    gcGameSettings *mSelf28;   // 0x28
    void *mArray2C;            // 0x2C
    int mFlags;                // 0x30
    int mActiveProfileIndex;   // 0x34
    int mField38;              // 0x38
    int mProfileCount;         // 0x3C
    void *mProfiles;           // 0x40 (array of 24-byte profile headers)
    char mProfileHandles[0x10]; // 0x44
    gcProfile **mLoadedProfiles; // 0x54
    gcGameSettings *mSelf58;   // 0x58
    void *mProfileNames;       // 0x5C
    int mProfileNameCount;     // 0x60
    void *mField64;            // 0x64
    void *mField68;            // 0x68
    int mField6C;              // 0x6C
    int mField70;              // 0x70
    int mField74;              // 0x74
    int mField78;              // 0x78
    int mField7C;              // 0x7C
    char mTail80[8];           // 0x80

    gcGameSettings(cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void SaveGameFormat(void);
    int ProfileFind(const gcStringValue *) const;
    bool ProfileExists(const gcStringValue *) const;
    unsigned char ProfileIsCorrupt(const gcStringValue *) const;
    int ProfileHeaderTimePlayed(const gcStringValue *) const;
    void OnProfileLoaded(gcProfile *);
    void HandleSaveGame(void);
    void SaveGameClear(int, int);
    void SaveGameCapture(void);
    void SaveGameRestore(void);
    void SaveGameRestore(int);
    const char *GetAppTitle(void);
};

class gcViewport {
public:
    static void CloseAllDialogs(void);
};

static cType *type_base;
static cType *type_gcGameSettings;

gcGameSettings::gcGameSettings(cBase *parent) {
    mParent = parent;
    mVtable = (void *)0x388010;
    mRefListA = 0;
    mSelfA = this;
    mRefListB = 0;
    mSelfB = this;
    mArray18 = 0;
    mArray1C = 0;
    mSelf20 = this;
    mArray24 = 0;
    mSelf28 = this;
    mArray2C = 0;
    mFlags = 0;
    mActiveProfileIndex = 0;
    mField38 = 0;
    mProfileCount = -1;
    mProfiles = 0;
    __vec_new(mProfileHandles, 2, 8, (void (*)(void *))0x2275F0);
    mLoadedProfiles = 0;
    mSelf58 = this;
    mProfileNames = 0;
    mProfileNameCount = -1;
    mField64 = 0;
    mField68 = 0;
    mField6C = -1;
    mField70 = 0;
    mField74 = 0;
    mField78 = 0;
    mField7C = 0;
    memset(mTail80, 0, 8);
}

cBase *gcGameSettings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcGameSettings *result = 0;
    gcGameSettings *obj = (gcGameSettings *)entry->fn(base, 0x88, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcGameSettings(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcGameSettings::GetType(void) const {
    if (!type_gcGameSettings) {
        if (!type_base) {
            type_base = cType::InitializeType((const char *)0x36D894,
                                              (const char *)0x36D89C,
                                              1, 0, 0, 0, 0, 0);
        }
        type_gcGameSettings = cType::InitializeType(0, 0, 0x1B3, type_base,
                                                    &gcGameSettings::New,
                                                    0, 0, 0);
    }
    return type_gcGameSettings;
}

void gcGameSettings::SaveGameFormat(void) {
    eMemCard::Format();
}

bool gcGameSettings::ProfileExists(const gcStringValue *s) const {
    return ProfileFind(s) >= 0;
}

void gcGameSettings::HandleSaveGame(void) {
    int flags = mFlags;
    if (flags & 1) {
        SaveGameCapture();
    } else if (flags & 2) {
        SaveGameRestore();
    }
}

unsigned char gcGameSettings::ProfileIsCorrupt(const gcStringValue *s) const {
    int idx = ProfileFind(s);
    unsigned char result = 0;
    if (idx >= 0) {
        result = ((unsigned char *)mProfiles)[idx * 24];
    }
    return result;
}

int gcGameSettings::ProfileHeaderTimePlayed(const gcStringValue *s) const {
    int idx = ProfileFind(s);
    if (idx >= 0) {
        char *header = (char *)mProfiles + idx * 24;
        header += 20;
        __asm__ volatile("" : "+r"(header));
        return *(int *)header;
    }
    volatile int result;
    result = 0;
    return result;
}

void gcGameSettings::OnProfileLoaded(gcProfile *profile) {
    gcProfile *loaded = 0;
    int index = mActiveProfileIndex;
    if (index >= 0) {
        int count = 0;
        gcProfile **profiles = mLoadedProfiles;
        if (profiles != 0) {
            count = ((int *)profiles)[-1];
        }
        if (index < count) {
            loaded = profiles[index];
        }
    }
    if (profile == loaded) {
        SaveGameClear(-1, -1);
    }
}

extern gcMap *g_gcMap;  // at 0x37D7FC

const char *gcGameSettings::GetAppTitle(void) {
    int handle = mField70;
    void *resolved;
    if (handle == 0) {
        resolved = 0;
    } else {
        void *cand = D_00038890[handle & 0xFFFF];
        resolved = 0;
        if (cand != 0 && *(int *)((char *)cand + 0x30) == handle) {
            resolved = cand;
        }
    }

    gcString *result = 0;
    if (resolved != 0) {
        result = ((gcStringTable *)resolved)->GetSubObject(
            *(cSubHandleT<gcString> *)((char *)this + 0x74), 0);
    }

    if (result == 0) return (const char *)0x36DC18;
    const char *text = result->mText;
    if (text == 0) return (const char *)0x36DAF4;
    return text;
}

void gcGameSettings::SaveGameRestore(int idx) {
    if (idx < 0) return;

    int count = 0;
    if (mField68 != 0) {
        count = *(int *)((char *)mField68 - 4) & 0x3FFFFFFF;
    }
    if (idx >= count) return;

    if (((unsigned char *)mField64)[idx] == 0) return;

    void *profile = 0;
    int active = mActiveProfileIndex;
    if (active >= 0) {
        int prof_count = 0;
        if (mLoadedProfiles != 0) {
            prof_count = ((int *)mLoadedProfiles)[-1];
        }
        if (active < prof_count) {
            profile = mLoadedProfiles[active];
        }
    }
    if (profile == 0) return;

    gcViewport::CloseAllDialogs();
    mField6C = idx;
    mFlags |= 2;
}

void gcGameSettings::SaveGameClear(int a, int b) {
    gcMap *map = g_gcMap;
    if (map) {
        if (a < 0) {
            map->ResetRegionStates();
        } else {
            map->ClearRegionSetState(a, b);
        }
    }
}
