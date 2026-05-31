// gcMap::LoadBackground(const cGUIDT<gcMap> &, const cGUIDT<gcLoadingScreen> &, unsigned int) static @ 0x000f1768
// Split-TU: LoadBackground is not declared in include/gcMap.h, declare class locally.

class gcMap;
class gcLoadingScreen;

template <class T>
struct cGUIDT {
    int mA;
    int mB;
};

struct gcLoadScreen {
    char pad_000[0x140];
    int mFlags;   // 0x140
    int mUiVal;   // 0x144
};

struct gcBackgroundLoader {
    char pad_000[0xC];
    int mFieldC;          // 0x0C
    char pad_010[0x80C - 0x10];
    gcLoadScreen *mScreen; // 0x80C
    void Load(const cGUIDT<gcMap> &, const cGUIDT<gcLoadingScreen> &);
    gcBackgroundLoader();
};

extern void __record_needed_destruction(void *);
extern void gcBackgroundLoader_ctor(void *);   // gcBackgroundLoader::gcBackgroundLoader @ 0x000f73bc

static gcBackgroundLoader gLoader;          // 0x00099048
static int gLoaderInit;                     // 0x00099878

class gcMap {
public:
    static gcLoadScreen *LoadBackground(const cGUIDT<gcMap> &, const cGUIDT<gcLoadingScreen> &, unsigned int);
};

gcLoadScreen *gcMap::LoadBackground(const cGUIDT<gcMap> &mapGuid,
                                    const cGUIDT<gcLoadingScreen> &screenGuid,
                                    unsigned int uiVal) {
    char valid = 0;
    if (mapGuid.mA != 0 || mapGuid.mB != 0) {
        valid = 1;
    }
    if ((valid & 0xFF) == 0) {
        return 0;
    }

    if (gLoaderInit == 0) {
        gLoaderInit = 1;
        gcBackgroundLoader_ctor(&gLoader);
        __record_needed_destruction((void *)0x0037D8C8);
    }
    gLoader.Load(mapGuid, screenGuid);

    gcLoadScreen *screen = gLoader.mScreen;
    gLoader.mFieldC = 0;
    gLoader.mScreen = 0;
    if (screen != 0) {
        screen->mUiVal = uiVal;
        screen->mFlags = screen->mFlags & ~0x100;
    }
    return screen;
}
