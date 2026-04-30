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

class gcViewport {
public:
    static int PausesGame(void);
};

struct gcMapObjectLoad {
    int mA;
    int mB;
    int mC;
    int mD;
};

struct gcThreadLoadDispatcher {
    short offset;
    short pad;
    int (*fn)(void *);
};

struct gcThreadLoadManager {
    int pad;
    char *dispatchBase;
};

class cThread {
public:
    enum cThreadPriority {};
    cThread(cThreadPriority, unsigned int, void *, unsigned int);
    ~cThread(void);
    void Start(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern void *__vec_new(void *, int, int, void (*)(void *));
extern void *memset(void *, int, unsigned int);
extern "C" void free(void *);

class gcBackgroundLoader : public cThread {
public:
    char pad_00[4];                             // 0x000
    volatile int mFlags;                        // 0x004
    void *mVtable;                              // 0x008
    int mField0C;                               // 0x00C
    unsigned char mField10;                     // 0x010
    unsigned char mField11;                     // 0x011
    char pad_12[0x14 - 0x12];                   // 0x012
    cGUIDT<gcMap> mMapGuid;                     // 0x014
    cGUIDT<gcLoadingScreen> mLoadingScreenGuid; // 0x01C
    char pad_24[0x34 - 0x24];                   // 0x024
    cGUIDT<gcStreamedCinematic> mCinematicGuid; // 0x034
    gcMapObjectLoad mObjectLoad;                // 0x03C
    char pad_4C[0x80C - 0x4C];                  // 0x04C
    int mLoadMapResult;                         // 0x80C
    int mField810;                              // 0x810
    int mField814;                              // 0x814
    int mField818;                              // 0x818
    int mField81C;                              // 0x81C
    int mLoadTarget;                            // 0x820
    char pad_824[8];                            // 0x824
    unsigned char mLoadFailed;                  // 0x82C
    unsigned char mField82D;                    // 0x82D

    bool IsLoadingObject(void) const;
    int ExitRenderLoop(void) const;
    void LoadLoadingScreen(void);
    void LoadGameThread(void);
    void LoadObjects(void);
    void LoadMap(void);
    void PreLoad(void);
    void Load(const cGUIDT<gcStreamedCinematic> &guid);
    static int LoadObjectsStatic(int, gcMapObjectLoad *);
    gcBackgroundLoader(void);
    ~gcBackgroundLoader(void);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
            rec->fn(block + rec->offset, p);
        } else {
            free(p);
        }
    }
};

gcBackgroundLoader::gcBackgroundLoader()
    : cThread((cThread::cThreadPriority)0, 0x1B800, 0, 0) {
    mVtable = (void *)0x387E00;
    mField0C = 0;
    mField10 = 0;
    mField11 = 0;
    mMapGuid.mA = 0;
    mMapGuid.mB = 0;
    mLoadingScreenGuid.mA = 0;
    *(volatile int *)&mLoadingScreenGuid.mB = 0;
    void *objectLoads = (char *)this + 0x24;
    void (*objectLoadCtor)(void *) = (void (*)(void *))0x245578;
    __vec_new(objectLoads, 2, 8, objectLoadCtor);
    mCinematicGuid.mA = 0;
    *(volatile int *)&mCinematicGuid.mB = 0;
    void *mapObjectLoads = (char *)this + 0x3C;
    void (*mapObjectLoadCtor)(void *) = (void (*)(void *))0x24767C;
    __vec_new(mapObjectLoads, 0x64, 0x14, mapObjectLoadCtor);
    mLoadMapResult = 0;
    mField818 = 0;
    mField81C = 0;
    mLoadTarget = 0;
    mLoadFailed = 0;
    mField82D = 0;
    memset(objectLoads, 0, 0x10);
    memset((char *)this + 0x824, 0, 8);
    memset(mapObjectLoads, 0, 0x7D0);
    memset((char *)this + 0x810, 0, 8);
}

gcBackgroundLoader::~gcBackgroundLoader() {
    *(void **)((char *)this + 8) = (void *)0x387E00;
}

bool gcBackgroundLoader::IsLoadingObject(void) const {
    if (((mFlags & 0x1000) != 0) & 0xFF) {
        return mLoadTarget >= 0;
    }
    return false;
}

int gcBackgroundLoader::ExitRenderLoop(void) const {
    int result = 0;
    if ((((mFlags & 0x1000) != 0) & 0xFF) == 0) {
        if (gcViewport::PausesGame() == 0) {
            result = 1;
        }
    }
    return result & 0xFF;
}

void gcBackgroundLoader::LoadGameThread(void) {
    gcThreadLoadManager *mgr = *(gcThreadLoadManager **)0x37D854;
    gcThreadLoadDispatcher *dispatch =
        (gcThreadLoadDispatcher *)(mgr->dispatchBase + 0x80);
    short offset = dispatch->offset;
    int (*fn)(void *) = dispatch->fn;
    if (fn((char *)mgr + offset) == 0) {
        mLoadFailed = 1;
    }
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
