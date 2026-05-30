// gcBackgroundLoader::IsLoadingMap(bool) const  @ 0x000f7c1c
// Split-TU: local declarations only; symbol __0fSgcBackgroundLoaderMIsLoadingMapbK

class gcMap;

template <class T>
struct cGUIDT {
    int mA;
    int mB;
};

class cThread {
public:
    enum cThreadPriority {};
    cThread(cThreadPriority, unsigned int, void *, unsigned int);
};

struct gcConfiguration {
    char pad[0x544];        // 0x000
    cGUIDT<gcMap> mCurMap;  // 0x544
};

extern gcConfiguration *gConfigInstance;  // 0x0037D854

class gcBackgroundLoader : public cThread {
public:
    char pad_00[4];                  // 0x000
    volatile int mFlags;             // 0x004
    char pad_08[0x14 - 0x08];        // 0x008
    cGUIDT<gcMap> mMapGuid;          // 0x014

    int IsLoadingMap(bool checkCurrent) const;
};

int gcBackgroundLoader::IsLoadingMap(bool checkCurrent) const {
    int result;
    if ((((mFlags & 0x1000) != 0) & 0xFF) == 0) {
        return 0;
    }
    char loading = 0;
    if (mMapGuid.mA != 0 || mMapGuid.mB != 0) {
        loading = 1;
    }
    if ((loading & 0xFF) == 0) {
        return 0;
    }
    result = 0;
    if (!checkCurrent) {
        goto setone;
    }
    {
        char match = 0;
        if (mMapGuid.mA == gConfigInstance->mCurMap.mA &&
            mMapGuid.mB == gConfigInstance->mCurMap.mB) {
            match = 1;
        }
        if ((match & 0xFF) == 0) {
            goto ret;
        }
    }
setone:
    result = 1;
ret:
    return result & 0xFF;
}
