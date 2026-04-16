#include "eMultiSphereShape.h"
#include "eCapsuleShape.h"
#include "eSimulatedController.h"
#include "eTriangleShape.h"
#include "gcStreamedCinematic.h"
#include "mVec3.h"
#include "mOCS.h"
#include "cRedBlackTree.h"
#include "eCollisionPair.h"

int eMultiSphereShape::CanSweep(void) const {
    return 1;
}

// eMultiSphereShape::GetProjectedMinMax
// Projects two sphere centers (center ± halfLength along ocs.row2) onto dir,
// expands by ±radius, returns overall min/max.
// 13-byte mismatch: SNC inline asm allocator puts dot1 in f14 instead of f13.
// Tried: output reordering, clobbers, builtins, split asm, permuter (2000+ candidates).
void eMultiSphereShape::GetProjectedMinMax(const mVec3 &dir, const mOCS &ocs, float *outMin, float *outMax) const {
    float offset = mHalfLength;

    float dot1;
    __asm__ volatile(
        "mfc1 $t1, %1\n"
        "mtv $t1, S100\n"
        "lv.q C120, 0x30(%2)\n"
        "lv.q C130, 0x20(%2)\n"
        "vscl.t C200, C130, S100\n"
        "vadd.t C200, C120, C200\n"
        "lv.q C210, 0(%3)\n"
        "vdot.t S100, C200, C210\n"
        "mfv $a1, S100\n"
        "mtc1 $a1, %0\n"
        : "=f"(dot1)
        : "f"(offset), "r"(&ocs), "r"(&dir)
        : "$a1", "$t1"
    );

    float radius = mRadius;
    float min1 = dot1 - radius;
    float max1 = dot1 + radius;
    float negOffset = -offset;

    float dot2;
    __asm__ volatile(
        "mfc1 $a0, %1\n"
        "mtv $a0, S100\n"
        "vscl.t C130, C130, S100\n"
        "vadd.t C120, C120, C130\n"
        "vdot.t S100, C120, C210\n"
        "mfv $a0, S100\n"
        "mtc1 $a0, %0\n"
        : "=f"(dot2)
        : "f"(negOffset)
        : "$a0"
    );

    float min2 = dot2 - radius;
    float max2 = dot2 + radius;

    if (min1 < min2)
        *outMin = min1;
    else
        *outMin = min2;

    if (!(max1 <= max2))
        *outMax = max1;
    else
        *outMax = max2;
}

int eCapsuleShape::CanSweep(void) const {
    return 1;
}

int eSimulatedController::GetCollisionMask(void) const {
    return collisionMask;
}

int eTriangleShape::CanSweep(void) const {
    return 0;
}

int gcStreamedCinematic::AllowParentCreate(void) const {
    return 0;
}

typedef cRedBlackTreeNode<eCollisionPair> PairNode;
typedef cFixedSizeAllocatorBase<PairNode> PairAlloc;
typedef cRedBlackTree<eCollisionPair, PairAlloc, eCollisionPair, cRedBlackTreeCompare> PairTree;

static inline int pairLessAB(const PairAlloc &a, const PairNode *b) {
    unsigned int va, vb;
    va = *(unsigned int *)((char *)&a + 0x10);
    vb = *(unsigned int *)((char *)b + 0x10);
    if (va < vb) return 1;
    if (vb < va) return 0;
    va = *(unsigned int *)((char *)&a + 0x14);
    vb = *(unsigned int *)((char *)b + 0x14);
    if (va < vb) return 1;
    if (vb < va) return 0;
    va = *(unsigned int *)((char *)&a + 0x18);
    vb = *(unsigned int *)((char *)b + 0x18);
    if (va < vb) return 1;
    if (vb < va) return 0;
    va = *(unsigned int *)((char *)&a + 0x38);
    vb = *(unsigned int *)((char *)b + 0x38);
    if (va < vb) return 1;
    if (vb < va) return 0;
    va = *(unsigned int *)((char *)&a + 0x3C);
    vb = *(unsigned int *)((char *)b + 0x3C);
    if (va < vb) return 1;
    if (vb < va) return 0;
    return 0;
}

static inline int pairLessBA(const PairNode *a, const PairAlloc &b) {
    unsigned int va, vb;
    va = *(unsigned int *)((char *)a + 0x10);
    vb = *(unsigned int *)((char *)&b + 0x10);
    if (va < vb) return 1;
    if (vb < va) return 0;
    va = *(unsigned int *)((char *)a + 0x14);
    vb = *(unsigned int *)((char *)&b + 0x14);
    if (va < vb) return 1;
    if (vb < va) return 0;
    va = *(unsigned int *)((char *)a + 0x18);
    vb = *(unsigned int *)((char *)&b + 0x18);
    if (va < vb) return 1;
    if (vb < va) return 0;
    va = *(unsigned int *)((char *)a + 0x38);
    vb = *(unsigned int *)((char *)&b + 0x38);
    if (va < vb) return 1;
    if (vb < va) return 0;
    va = *(unsigned int *)((char *)a + 0x3C);
    vb = *(unsigned int *)((char *)&b + 0x3C);
    if (va < vb) return 1;
    if (vb < va) return 0;
    return 0;
}

template<>
PairNode *PairTree::FindNode(const PairAlloc &key) const {
    PairNode *node = mRoot;
    if (!node) return 0;

    do {
        int cmp;
        if (pairLessAB(key, node)) {
            cmp = -1;
        } else {
            int nodeLess = pairLessBA(node, key);
            cmp = 0;
            if (nodeLess) cmp = 1;
        }

        if (cmp < 0) {
            node = (PairNode *)node->mLeft;
        } else if (cmp > 0) {
            node = (PairNode *)node->mRight;
        } else {
            return node;
        }
    } while (node);

    return 0;
}
