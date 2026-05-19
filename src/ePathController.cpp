// ePathController
//   0x0005ac9c  Reset(cHandleT<ePath>, const mVec3 &, float, const mVec3 &, bool, const mVec3 &)
//   0x0005b0a4  Write(cOutStream &) const
//   0x0005b11c  Read(cInStream &)
// Obj: eAll_psp.obj

class cOutStream {
public:
    void Write(float, int, int, bool);
};

class cInStream {
public:
    void Read(float &, int, int, bool);
};

class cHandle {
public:
    int mId;
    void Write(cOutStream &) const;
    void Read(cInStream &);
};

#include "mVec3.h"

template <class T> class cHandleT {
public:
    int mIndex;
};

class ePath {
public:
    char _pad00[0x30];
    int mHandleId;                  // 0x30
    char _pad34[0x10];
    void *mPoints;                  // 0x44
    float mTotalLength;             // 0x48
    float FindClosestPoint(const mVec3 &, float, float, bool, float, float *) const;
    float Units2PathT(float, float, float) const;
};

class ePathController : public cHandle {
public:
    float mField4;
    float mField8;
    float mFieldC;

    void Reset(cHandleT<ePath>, const mVec3 &, float, float, bool, bool);
    float CalcLead(const ePath *, float, float) const;
    void Write(cOutStream &) const;
    void Read(cInStream &);
};

#pragma control sched=1

void ePathController::Reset(cHandleT<ePath> handle, const mVec3 &start, float t1,
                            float t2, bool useFind, bool findFlag) {
    this->mId = handle.mIndex;

    ePath *path;
    if (handle.mIndex == 0) {
        path = 0;
    } else {
        ePath **table = (ePath **)0x38890;
        ePath *entry = table[handle.mIndex & 0xFFFF];
        path = 0;
        if (entry != 0 && entry->mHandleId == handle.mIndex) {
            path = entry;
        }
    }

    if (path == 0) return;

    int empty = 1;
    if (path->mPoints != 0) {
        empty = ((*((int *)path->mPoints - 1)) & 0x3FFFFFFF) == 0;
    }
    if (empty) return;

    float r1 = path->Units2PathT(
        path->mTotalLength * t1, 0.0f,
        path->mPoints != 0
            ? (float)((*((int *)path->mPoints - 1) & 0x3FFFFFFF) - 1)
            : -1.0f);

    this->mField8 = path->Units2PathT(
        path->mTotalLength * t2, 0.0f,
        path->mPoints != 0
            ? (float)((*((int *)path->mPoints - 1) & 0x3FFFFFFF) - 1)
            : -1.0f);

    float sp4 = 0.0f;
    float r3 = r1;
    if (useFind) {
        r3 = path->FindClosestPoint(
            start, 0.0f,
            path->mPoints != 0
                ? (float)((*((int *)path->mPoints - 1) & 0x3FFFFFFF) - 1)
                : -1.0f,
            findFlag, path->mTotalLength, &sp4);
    }
    this->mFieldC = r3;
    this->mField4 = CalcLead((const ePath *)path, 0.1f, sp4);
}

void ePathController::Write(cOutStream &stream) const {
    cHandle::Write(stream);
    stream.Write(mField4, 10, 10, true);
    stream.Write(mField8, 10, 10, true);
    stream.Write(mFieldC, 10, 10, true);
}

void ePathController::Read(cInStream &stream) {
    cHandle::Read(stream);
    stream.Read(mField4, 10, 10, true);
    stream.Read(mField8, 10, 10, true);
    stream.Read(mFieldC, 10, 10, true);
}
