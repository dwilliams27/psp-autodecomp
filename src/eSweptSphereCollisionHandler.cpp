#include "mVec3.h"
#include "mVec2.h"
#include "mOCS.h"
#include "eMultiSphereShape.h"
#include "eTextureMap.h"
#include "eInputMouse.h"

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

// ---- F2: eBroadphase::UpdateRigidBody — 0x00035c64, 68B ----
// Calls template static eDynamicAABBTreeNode<eRigidBodyState>::UpdateObject
// with (body->mTreeNode, body, this->mAllocator), then this->UpdatePairs(body).
// The caller's a1 register already holds `body`, so SNC doesn't emit a
// redundant move a1 setup before the first jal.

class eRigidBodyState;

class eBroadphase {
public:
    char _pad0[0x10];
    void *mAllocator;   // +0x10

    void UpdateRigidBody(eRigidBodyState *body);
    void UpdatePairs(eRigidBodyState *body);
};

extern "C" {
    void eBroadphase_helper_TreeNodeUpdateObject(
        void *node, eRigidBodyState *body, void *allocator);
}

void eBroadphase::UpdateRigidBody(eRigidBodyState *body) {
    void *treeNode = *(void **)((char *)body + 0xB8);
    eBroadphase_helper_TreeNodeUpdateObject(treeNode, body, mAllocator);
    UpdatePairs(body);
}

// ---- F4: eMultiSphereShape::eMultiSphereShape(cBase *) — 0x00068b74, 68B ----
// Transition zone (sched=2 default). Pattern identical to eCapsuleShape ctor:
// call eShape base ctor (via extern "C" shim), install vtable at +0x04,
// init radius (+0x80) and halfLength (+0x84) to 1.0f via inline asm.

extern "C" {
    void eShape___ct_eShape_cBaseptr_swept(void *self, cBase *parent);
}

extern char eMultiSphereShapevirtualtable[];

eMultiSphereShape::eMultiSphereShape(cBase *parent) {
    eShape___ct_eShape_cBaseptr_swept(this, parent);
    *(void **)((char *)this + 4) = eMultiSphereShapevirtualtable;
    __asm__ volatile(
        "lui $a0, 0x3f80\n"
        "mtc1 $a0, $f12\n"
        "swc1 $f12, 0x80(%0)\n"
        "swc1 $f12, 0x84(%0)\n"
        :: "r"(this) : "$a0", "$f12", "memory"
    );
}

// ---- F5: eProjectedDynamicMtl::eProjectedDynamicMtl(cBase *) — 0x0008310c, 68B ----
// Sched=1 zone (need #pragma). Calls eGeomMtl base ctor, installs vtable at
// +0x04, sets int at +0x68 to -1, then calls this->CreateData().

extern "C" {
    void eGeomMtl___ct_eGeomMtl_cBaseptr_swept(void *self, cBase *parent);
}

extern char eProjectedDynamicMtlvirtualtable[];

#pragma control sched=1
eProjectedDynamicMtl::eProjectedDynamicMtl(cBase *parent) {
    eGeomMtl___ct_eGeomMtl_cBaseptr_swept(this, parent);
    ((void **)this)[0x4 / 4] = eProjectedDynamicMtlvirtualtable;
    __asm__ volatile("" ::: "memory");
    ((int *)this)[0x68 / 4] = -1;
    CreateData();
}
#pragma control sched=2
