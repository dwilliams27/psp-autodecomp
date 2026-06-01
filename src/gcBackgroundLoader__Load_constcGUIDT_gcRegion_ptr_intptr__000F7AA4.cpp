// gcBackgroundLoader::Load(const cGUIDT<gcRegion> *, int *)  @ 0x000f7aa4
// Split-TU: local declarations only; symbol
//   __0fSgcBackgroundLoaderELoadPC6GcGUIDT76IgcRegion_Pi

class gcRegion;

template <class T>
struct cGUIDT {
    int mA;
    int mB;
};

class cThread {
public:
    enum cThreadPriority {};
    cThread(cThreadPriority, unsigned int, void *, unsigned int);
    void Start(void);
};

class gcBackgroundLoader : public cThread {
public:
    char pad_00[0x24];                  // 0x000
    cGUIDT<gcRegion> mRegionGuids[2];   // 0x024
    char pad_34[0x824 - 0x34];          // 0x034
    int mRegionTargets[2];              // 0x824

    void PreLoad(void);
    void Load(const cGUIDT<gcRegion> *guids, int *targets);
};

void gcBackgroundLoader::Load(const cGUIDT<gcRegion> *guids, int *targets) {
    PreLoad();
    for (int i = 0; i < 2; i++) {
        mRegionGuids[i] = guids[i];
        mRegionTargets[i] = targets[i];
    }
    Start();
}
