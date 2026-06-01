// eRoom::AddVolume(eVolume *, const mSphere &)  @ 0x0003f3ac

class eVolume;

struct mSphere {
    float x, y, z;   // 0x0
    float r;         // 0xC
};

template <class T> class cHandleT {
public:
    unsigned int mHandle;
    cHandleT() : mHandle(0) {}
};

class eRoom;

class eRoom {
public:
    void ClearRoomVolumeList(eVolume *);
    bool BuildRoomVolumeList(unsigned int, eVolume *, const mSphere &,
                             cHandleT<eRoom>, unsigned int *);
    bool AddVolume(eVolume *, const mSphere &);
};

class eWorld {
public:
    static unsigned int GetNextCullId(void);
};

bool eRoom::AddVolume(eVolume *vol, const mSphere &sphere) {
    if (((unsigned char *)vol)[0x74] & 1) {
        if (!(sphere.r < 0.0f)) {
            ClearRoomVolumeList(vol);
            unsigned int count = 0;
            BuildRoomVolumeList(eWorld::GetNextCullId(), vol, sphere,
                                cHandleT<eRoom>(), &count);
            if (count >= 4) {
                ClearRoomVolumeList(vol);
                return false;
            }
            *(eRoom **)((char *)vol + 0x24) = this;
        }
    }
    return true;
}
