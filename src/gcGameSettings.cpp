#include "eMemCard.h"
#include "gcMap.h"

class gcStringValue;

class gcGameSettings {
public:
    char pad_000[0x30];
    int mFlags;                // 0x30
    char pad_034[0xC];         // 0x34
    void *mProfiles;           // 0x40 (array of 24-byte profile headers)

    static void SaveGameFormat(void);
    int ProfileFind(const gcStringValue *) const;
    bool ProfileExists(const gcStringValue *) const;
    unsigned char ProfileIsCorrupt(const gcStringValue *) const;
    void HandleSaveGame(void);
    void SaveGameClear(int, int);
    void SaveGameCapture(void);
    void SaveGameRestore(void);
};

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
