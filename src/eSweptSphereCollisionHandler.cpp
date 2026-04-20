#include "mVec3.h"
#include "mOCS.h"

typedef unsigned int SceULong128 __attribute__((mode(TI)));
typedef int v16sf_t __attribute__((mode(V16SF)));

struct eCollisionInfo {
    void *obj;
};

struct eCollisionObj {
    int unk0;
    void *vt;
};

class eSweptVolumeCollisionHandler {
public:
    void SetLocalToWorld(const mOCS &ocs);
};

class eSweptSphereCollisionHandler : public eSweptVolumeCollisionHandler {
public:
    void MoveVolume(const mVec3 &offset);
    void SetLocalToWorld(const mOCS &ocs);
    void ProcessCollision(const eCollisionInfo &info);
};

// 0x0020878c, 28B, leaf: translate world-sphere center by offset, preserve radius.
void eSweptSphereCollisionHandler::MoveVolume(const mVec3 &offset) {
    SceULong128 c = *(SceULong128 *)((char *)this + 0x4950);
    __asm__ volatile(
        "lv.q C130, 0(%1)\n"
        "vadd.t %0, %0, C130\n"
        : "+X"(c)
        : "r"(&offset)
    );
    float r = *(float *)((char *)this + 0x495c);
    *(SceULong128 *)((char *)this + 0x4950) = c;
    *(float *)((char *)this + 0x495c) = r;
}

// 0x00064714, 64B: transform world sphere to local space via OCS, then call base.
void eSweptSphereCollisionHandler::SetLocalToWorld(const mOCS &ocs) {
    float r = *(float *)((char *)this + 0x495c);
    SceULong128 c = *(SceULong128 *)((char *)this + 0x4950);
    __asm__ volatile(
        "lv.q C000, 0x0(%1)\n"
        "lv.q C010, 0x10(%1)\n"
        "lv.q C020, 0x20(%1)\n"
        "lv.q C030, 0x30(%1)\n"
        "vsub.t %0, %0, C030\n"
        "vtfm3.t C130, M000, %0\n"
        "sv.q C130, 0x4960(%2)\n"
        : "+X"(c)
        : "r"(&ocs), "r"(this)
        : "memory"
    );
    *(float *)((char *)this + 0x496c) = r;
    eSweptVolumeCollisionHandler::SetLocalToWorld(ocs);
}

// 0x000646d0, 68B, leaf: virtual interface dispatch to shape's collision method.
void eSweptSphereCollisionHandler::ProcessCollision(const eCollisionInfo &info) {
    eCollisionObj *obj = (eCollisionObj *)info.obj;
    char *vt = (char *)obj->vt;
    short *slot = (short *)(vt + 0x88);
    void *adjusted = (char *)obj + *slot;
    typedef void (*Method)(void *, const eCollisionInfo &, int, void *, void *, void *);
    Method m = *(Method *)((char *)slot + 4);
    m(adjusted, info, 1,
      (char *)this + 0x4960,
      (char *)this + 0x80,
      (char *)this + 0xd0);
}

// ---- gcTableColumnShort — placed in this file per orchestrator target ----

struct cArrayShort {
    short *mData;
};

class gcTableColumnShort {
public:
    char _pad[0x8];
    cArrayShort mValues;

    void Set(int row, float value);
    float Get(int row) const;
};

// 0x0027186c, 28B, leaf.
void gcTableColumnShort::Set(int row, float value) {
    mValues.mData[row] = (short)(int)value;
}

// 0x00271888, 28B, leaf.
float gcTableColumnShort::Get(int row) const {
    return (float)mValues.mData[row];
}
