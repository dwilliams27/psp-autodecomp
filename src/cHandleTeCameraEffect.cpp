class eCameraEffect;
class eSprite;
class eDynamicMeshVisTriList;

template <class T>
class cHandleT {
public:
    int mIndex;
    cHandleT();
};

template <class T>
cHandleT<T>::cHandleT() : mIndex(0) {
}

template cHandleT<eCameraEffect>::cHandleT();
template cHandleT<eSprite>::cHandleT();

class eModelAnimationState {
public:
    int mHandle;
    eModelAnimationState();
};

eModelAnimationState::eModelAnimationState() : mHandle(0) {
}

struct eSimulatedContact {
    eSimulatedContact();
};

eSimulatedContact::eSimulatedContact() {
    *(int *)((char *)this + 0x24) = 0;
}

template <class T>
class cArrayBase {
public:
    int mData;
    cArrayBase();
};

template <class T>
cArrayBase<T>::cArrayBase() : mData(0) {
}

template cArrayBase<eDynamicMeshVisTriList>::cArrayBase();
