class ePortal;
class eRoom;
class eTexture;

template <class T>
class cHandleT {
public:
    int mIndex;
    cHandleT();
};

template <class T>
cHandleT<T>::cHandleT() : mIndex(0) {
}

template cHandleT<ePortal>::cHandleT();
template cHandleT<eRoom>::cHandleT();
template cHandleT<eTexture>::cHandleT();

class mVec3;
class mOCS;
class mPlane;
class eDragAreaUtil;

class eShape {
public:
    void GetVolumeUnderPlane(eDragAreaUtil *, const mPlane &, const mOCS &, float *, mVec3 *) const;
};

void eShape::GetVolumeUnderPlane(eDragAreaUtil *, const mPlane &, const mOCS &, float *volume, mVec3 *) const {
    *volume = 0.0f;
}
