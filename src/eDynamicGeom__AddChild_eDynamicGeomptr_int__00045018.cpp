// eDynamicGeom::AddChild(eDynamicGeom *, int)
// Address: 0x00045018, Size: 188B, Obj: eAll_psp.obj
//
// ODR-WARNING: split-TU local redeclaration of eDynamicGeom. Only the members
// referenced here are declared. Do NOT add these to include/eDynamicGeom.h.

class eDynamicGeom {
public:
    char        _pad0[0x8C];
    unsigned char mFlags;       // 0x8C
    char        _pad8D[0x43];
    short       mIndex;         // 0xD0
    char        _padD2[0x0A];
    eDynamicGeom *mParent;      // 0xDC
    eDynamicGeom *mFirstChild;  // 0xE0
    eDynamicGeom *mNext;        // 0xE4
    eDynamicGeom *mPrev;        // 0xE8

    void AddChild(eDynamicGeom *child, int index);
    void Detach();
    void UpdateChildFlags(eDynamicGeom *child, unsigned int on, unsigned int off);
};

void eDynamicGeom::AddChild(eDynamicGeom *child, int index) {
    if (child->mParent != this) {
        if (child->mParent != 0) {
            child->Detach();
        }
        eDynamicGeom *first = mFirstChild;
        if (first != 0) {
            child->mNext = first->mNext;
            child->mPrev = mFirstChild;
            child->mNext->mPrev = child;
            child->mPrev->mNext = child;
        } else {
            mFirstChild = child;
            child->mPrev = child;
            child->mNext = child;
        }
        child->mParent = this;
        child->mIndex = (short)index;
        unsigned char f = mFlags;
        UpdateChildFlags(child, (f & ~0xE0u) | 4, ~f & ~0x14u & 0xFF);
    }
}
