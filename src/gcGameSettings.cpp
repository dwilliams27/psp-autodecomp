#include "eMemCard.h"
#include "gcMap.h"

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cMemPool;
class cType;
class gcProfile;
class gcStringValue;

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
    char pad_000[0x30];
    int mFlags;                // 0x30
    int mActiveProfileIndex;   // 0x34
    char pad_038[0x8];         // 0x38
    void *mProfiles;           // 0x40 (array of 24-byte profile headers)
    char pad_044[0x10];        // 0x44
    gcProfile **mLoadedProfiles; // 0x54

    gcGameSettings(cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    static void SaveGameFormat(void);
    int ProfileFind(const gcStringValue *) const;
    bool ProfileExists(const gcStringValue *) const;
    unsigned char ProfileIsCorrupt(const gcStringValue *) const;
    void OnProfileLoaded(gcProfile *);
    void HandleSaveGame(void);
    void SaveGameClear(int, int);
    void SaveGameCapture(void);
    void SaveGameRestore(void);
};

static cType *type_base;
static cType *type_gcGameSettings;

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
