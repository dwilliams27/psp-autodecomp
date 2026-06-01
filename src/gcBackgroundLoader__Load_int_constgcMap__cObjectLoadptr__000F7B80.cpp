// gcBackgroundLoader::Load(int, const gcMap::cObjectLoad *)  @ 0x000f7b80
// Split-TU: local declarations only; symbol
//   __0fSgcBackgroundLoaderELoadiPC65FgcMapLcObjectLoad

class cThread {
public:
    enum cThreadPriority {};
    cThread(cThreadPriority, unsigned int, void *, unsigned int);
    void Start(void);
};

class gcMap {
public:
    struct cObjectLoad {
        int mField0;
        int mField4;
        int mField8;
        int mFieldC;
        int mField10;
    };
};

class gcBackgroundLoader : public cThread {
public:
    char pad_00[0x3C];                       // 0x000
    gcMap::cObjectLoad mObjects[1];          // 0x03C
    char pad_xx[0x820 - 0x50];               // 0x050
    int mObjectCount;                        // 0x820

    void PreLoad(void);
    void Load(int count, const gcMap::cObjectLoad *objects);
};

void gcBackgroundLoader::Load(int count, const gcMap::cObjectLoad *objects) {
    PreLoad();
    for (int i = 0; i < count; i++) {
        mObjects[i] = objects[i];
    }
    mObjectCount = count;
    Start();
}
