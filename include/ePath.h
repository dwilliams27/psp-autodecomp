#pragma once

class cBase;
class cFile;
class cMemPool;
class mBasis;
class mVec3;

// ePathPoint: point along a path, 32 bytes (stride 0x20 in PathT2Units).
// PathPoint fields accessed: +0x14 = float (distance/param).
struct ePathPoint {
    char _pad00[0x14];          // 0x00
    float mDistance;            // 0x14
    char _pad18[0x08];         // 0x18
    // Total: 0x20 (32 bytes)
};

// ePath: path object (inherits cObject).
// ePath ctor (0x5b5a0) calls cObject::cObject then sets:
//   +0x04: vtable
//   +0x44: int = 0 (points array pointer)
//   +0x48: float = 0.0 (total length)
// PathT2Units (0x5bab0) reads:
//   +0x44: pointer to ePathPoint array
//   Array count from *(array - 4) & 0x3FFFFFFF
//   Indexes array with stride 0x20 (32 bytes)
//   Reads float at pathpoint+0x14
// cObject base: 0x00..0x43
// Struct size: at least 0x4C
class ePath {
public:
    ePath(cBase *);
    ~ePath();

    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void Reset(cMemPool *, bool);
    void FindClosestPoint(const mVec3 &, float, float, bool, float, float *) const;
    float PathT2Units(float) const;
    float Units2PathT(float, float, float) const;
    void GetPosAt(float, mVec3 *) const;
    void GetBasisAt(float, mBasis *, bool) const;
    void AssignCopy(const cBase *);
    static ePath *New(cMemPool *, cBase *);

    // cObject base fields: 0x00..0x43
    char _padBase[0x44];            // 0x00
    ePathPoint *mPoints;            // 0x44
    float mTotalLength;             // 0x48
};
