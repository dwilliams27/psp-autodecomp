// gcMap::PlayCinematic(gcCinematic *, cHandle, float, bool) static
// Address: 0x000f41b8, Size: 236B
// Obj: gcAll_psp.obj
// Split-TU: classes declared locally (PlayCinematic absent from include/gcMap.h).

#define NULL 0

template <class T>
class cHandleT {
public:
    int mIndex;
};

class cHandle {
public:
    int mIndex;
};

class gcCinematic {
public:
    cHandleT<gcCinematic> GetHandle(void) const;  // member at 0x30
    char pad_000[0x30];
    cHandleT<gcCinematic> mHandle;  // 0x30
};

class gcCinematicInstance {
public:
    void Chain(cHandleT<gcCinematic>, cHandle, float);
    void Play(cHandle, float);
    static gcCinematicInstance *FindPlayingCinematic(cHandle, bool);
};

class gcCinematic_ext {
public:
    static gcCinematicInstance *CreateDynamicInstance(gcCinematic *);
};

extern "C" gcCinematicInstance *__0fNgcCinematicInstanceVFindPlayingCinematic6HcHandlebT(cHandle handle, bool b);
extern "C" void __0fNgcCinematicInstanceFChain6McHandleTYZ1Z11gcCinematic6HcHandlef(gcCinematicInstance *self, cHandleT<gcCinematic> cinematic, cHandle handle, float time);
extern "C" void __0fNgcCinematicInstanceDPlay6HcHandlef(gcCinematicInstance *self, cHandle handle, float time);
extern "C" gcCinematicInstance *__0fLgcCinematicVCreateDynamicInstancev(gcCinematic *self);

class gcMap {
public:
    static gcCinematicInstance *PlayCinematic(gcCinematic *, cHandle, float, bool);
};

gcCinematicInstance *gcMap::PlayCinematic(gcCinematic *cinematic, cHandle handle, float time, bool b) {
    gcCinematicInstance *inst;

    if (!b) {
        inst = __0fNgcCinematicInstanceVFindPlayingCinematic6HcHandlebT(handle, b);
        if (inst != NULL) {
            cHandleT<gcCinematic> ch;
            ch.mIndex = cinematic ? cinematic->mHandle.mIndex : 0;
            __0fNgcCinematicInstanceFChain6McHandleTYZ1Z11gcCinematic6HcHandlef(inst, ch, handle, time);
            return inst;
        }
    }
    inst = __0fLgcCinematicVCreateDynamicInstancev(cinematic);
    if (inst != NULL) {
        __0fNgcCinematicInstanceDPlay6HcHandlef(inst, handle, time);
        return inst;
    }
    return NULL;
}
