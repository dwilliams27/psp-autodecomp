// Local type definitions — the shared header only declares the nested
// gcObjectLoad struct. Full class layout is kept here so we don't disturb
// other translation units that include gcBackgroundLoader.h.

template <class T>
struct cGUIDT {
    int mA;
    int mB;
};

class gcLoadingScreen;
class gcMap;
class gcStreamedCinematic;

class gcGame {
public:
    int LoadMap(const cGUIDT<gcMap> &, bool);
    void LoadLoadingScreen(const cGUIDT<gcLoadingScreen> &);
};

extern gcGame *gGameInstance;

struct gcMapObjectLoad {
    int mA;
    int mB;
    int mC;
    int mD;
};

class cThread {
public:
    void Start(void);
};

class gcBackgroundLoader : public cThread {
public:
    char pad_00[4];                             // 0x000
    int mFlags;                                 // 0x004
    char pad_08[0x14 - 0x08];                   // 0x008
    cGUIDT<gcMap> mMapGuid;                     // 0x014
    cGUIDT<gcLoadingScreen> mLoadingScreenGuid; // 0x01C
    char pad_24[0x34 - 0x24];                   // 0x024
    cGUIDT<gcStreamedCinematic> mCinematicGuid; // 0x034
    gcMapObjectLoad mObjectLoad;                // 0x03C
    char pad_4C[0x80C - 0x4C];                  // 0x04C
    int mLoadMapResult;                         // 0x80C
    char pad_810[0x820 - 0x810];                // 0x810
    int mLoadTarget;                            // 0x820
    char pad_824[8];                            // 0x824
    unsigned char mLoadFailed;                  // 0x82C

    bool IsLoadingObject(void) const;
    void LoadLoadingScreen(void);
    void LoadObjects(void);
    void LoadMap(void);
    void PreLoad(void);
    void Load(const cGUIDT<gcStreamedCinematic> &guid);
    static int LoadObjectsStatic(int, gcMapObjectLoad *);
};

bool gcBackgroundLoader::IsLoadingObject(void) const {
    if (((mFlags & 0x1000) != 0) & 0xFF) {
        return mLoadTarget >= 0;
    }
    return false;
}

void gcBackgroundLoader::LoadLoadingScreen(void) {
    gGameInstance->LoadLoadingScreen(mLoadingScreenGuid);
}

void gcBackgroundLoader::LoadObjects(void) {
    if (LoadObjectsStatic(mLoadTarget, &mObjectLoad) == 0) {
        mLoadFailed = 1;
    }
}

void gcBackgroundLoader::LoadMap(void) {
    int result = gGameInstance->LoadMap(mMapGuid, true);
    mLoadMapResult = result;
    if (result == 0) {
        mLoadFailed = 1;
    }
}

void gcBackgroundLoader::Load(const cGUIDT<gcStreamedCinematic> &guid) {
    PreLoad();
    mCinematicGuid = guid;
    Start();
}
