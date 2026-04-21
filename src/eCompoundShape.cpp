#include "eCompoundShape.h"
#include "eCollision.h"
#include "mOCS.h"

// ── Base eShape external references (wrapped in C-linkage shims, matching
//    the convention in src/eTriangleShape.cpp).
extern "C" {
    void eShape___ct_eShape_cBaseptr(void *self, cBase *parent);
}

extern char eCompoundShapevirtualtable[];

// ── Constructor (0x00072dd4) ──
// Calls eShape::eShape(parent), overwrites the vtable pointer at +0x04,
// clears the sub-shape array pointer at +0x80, and stores `this` at +0x84.
eCompoundShape::eCompoundShape(cBase *parent) {
    eShape___ct_eShape_cBaseptr(this, parent);
    *(void **)((char *)this + 4) = eCompoundShapevirtualtable;
    *(int *)((char *)this + 0x80) = 0;
    *(eCompoundShape **)((char *)this + 0x84) = this;
}

// ── Already-matched accessors (present in this file for reference context).
// Implemented in src/eCompoundShape_GetSubShape.cpp.

// ── Collide thunks ──
//
// These three-line forwarders dispatch to double-dispatch helpers in
// eCollision.  Each compiles to a 7-insn prologue/jal/epilogue (28B) when
// SNC declines to tail-call-optimize through a cross-module static method.

int eCompoundShape::Collide(const eCompoundShape *shape, int a, int b,
                            const mOCS &ocs1, const mOCS &ocs2,
                            eCollisionContactInfo *info) const {
    return eCollision::CompoundCompound(*this, *shape, a, b, ocs1, ocs2, info);
}

int eCompoundShape::Collide(const eMeshShape *shape, int a, int b,
                            const mOCS &ocs1, const mOCS &ocs2,
                            eCollisionContactInfo *info) const {
    return eCollision::CompoundMesh(*this, *shape, a, b, ocs1, ocs2, info);
}

int eCompoundShape::Collide(const eHeightmapShape *shape, int a, int b,
                            const mOCS &ocs1, const mOCS &ocs2,
                            eCollisionContactInfo *info) const {
    return eCollision::CompoundHeightmap(*this, *shape, a, b, ocs1, ocs2, info);
}
