#pragma once

class cBase;
class cFile;
class cMemPool;
class cName;
class cType;
class eCamera;
class eDynamicGeom;
class mFrustum;
class mOCS;
class eCameraBins;
class eMaterial;
template <class T> class cHandleT;

// eGeom: intermediate base class (inherits cObject).
// eGeom ctor (0x2c538) sets:
//   +0x00: cBase* (parent)
//   +0x04: vtable
//   +0x60..0x8C: various geom fields
//   +0x5C: float = -1.0
//   +0x74: float = -1.0
//   +0x78: float = 1.0
//   +0x8C: byte = 0xE0
//   +0x8D: byte = 0
//   +0x10..0x50: VFPU-zeroed (identity OCS matrices)
// cObject fields: offsets 0x00..0x43 from cObject ctor (0x9d28)

// eDynamicGeom ctor (0x44c44) calls eGeom::eGeom then sets:
//   +0x04: vtable (eDynamicGeom's)
//   +0xD0: short = -1
//   +0xD2: byte = 0x10
//   +0xD4: float = 1.0 (opacity)
//   +0xD8: int = -1
//   +0xDC..0xE8: zeroed (5 ints)
//   +0x90..0xC0: VFPU-zeroed (local-to-world OCS, 4x mVec3)
// Struct size: at least 0xCC (from last VFPU store at +0xC0)
class eGeom {
public:
    eGeom(cBase *);
    char _eGeomPad[0x90];
};

class eDynamicGeom : public eGeom {
public:
    eDynamicGeom(cBase *);
    ~eDynamicGeom();

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void SetGeomFlagsOnOff(unsigned int, unsigned int);
    void SetDynamicGeomFlagsOnOff(unsigned int, unsigned int);
    float GetOpacity(const eCamera &, float) const;
    void AddChild(eDynamicGeom *, int);
    void Detach();
    void UpdateLocalToWorldRecurse();
    void CullAttached(const cType *, unsigned int, const eCamera &, const mFrustum &, eCameraBins *, unsigned int, int, const cHandleT<eMaterial> *, float) const;
    void UpdateLocalToWorld();
    void ApplyLocalToWorld(const mOCS &);
    void DetachChildren();
    void UpdateChildFlags(eDynamicGeom *, unsigned int, unsigned int);
    void InvalidateChildrenLocalToWorld();
    int GetSubObjectIndex(const cName &, int) const;
    void GetSubObjectToWorld(int, mOCS *) const;

    // VFPU OCS at +0x90..0xCF (local-to-world, 4x 16-byte vectors)
    char _padOCS[0x40];         // 0x90 (localToWorld OCS, set by VFPU)
    short mGeomIndex;           // 0xD0
    unsigned char mGeomType;    // 0xD2
    char _padD3;                // 0xD3
    float mOpacity;             // 0xD4
    int mParentIndex;           // 0xD8
    int mFieldDC;               // 0xDC
    int mFieldE0;               // 0xE0
    int mFieldE4;               // 0xE4
    int mFieldE8;               // 0xE8
};
