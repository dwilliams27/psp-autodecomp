#include "eMultiSphereShape.h"
#include "eBoxShape.h"
#include "eCapsuleShape.h"
#include "eCompoundShape.h"
#include "eMeshShape.h"
#include "eHeightmapShape.h"
#include "eSimulatedController.h"
#include "eTriangleShape.h"
#include "gcStreamedCinematic.h"
#include "eCollision.h"
#include "mVec3.h"
#include "mOCS.h"
#include "cRedBlackTree.h"
#include "eCollisionPair.h"

class eConvexHullShape;
class cFile;
class cBase;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

extern char eMultiSphereShapevirtualtable[];

void eShape_Write_MS(const void *, cFile &);
void eShape_eShape_MS(void *, cBase *);
void eMultiSphereShape_eMultiSphereShape(eMultiSphereShape *, cBase *);
extern "C" void eShape___dtor_eShape_void(void *, int);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    unsigned int (*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

void *cMemPool_GetPoolFromPtr_MS(const void *);

int eMultiSphereShape::CanSweep(void) const {
    return 1;
}

#pragma control sched=1
float eMultiSphereShape::GetVolume(void) const {
    float r2 = mRadius * mRadius;
    float r2pi = r2 * 3.1415927f;
    float doubled = mHalfLength * 2.0f;
    float cyl = r2pi * doubled;
    float r3 = r2 * mRadius;
    return cyl + r3 * 4.1887903f;
}
#pragma control sched=2

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

// eMultiSphereShape::Write(cFile &) const — 0x000688d0
#pragma control sched=1
void eMultiSphereShape::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eShape_Write_MS(this, file);
    wb.Write(mRadius);
    wb.Write(mHalfLength);
    wb.End();
}
#pragma control sched=2

// eMultiSphereShape::~eMultiSphereShape(void) — 0x00068bb8
#pragma control sched=1
extern "C" void eMultiSphereShape___dtor_eMultiSphereShape_void(eMultiSphereShape *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = eMultiSphereShapevirtualtable;
        eShape___dtor_eShape_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr_MS(self);
            void *block = *(void **)((char *)pool + 0x24);
            char *allocTable = *(char **)((char *)block + 0x1C);
            DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
            short off = rec->offset;
            __asm__ volatile("" ::: "memory");
            void *base = (char *)block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, self);
        }
    }
}
#pragma control sched=2

// eMultiSphereShape::New(cMemPool *, cBase *) static — 0x0020936c
#pragma control sched=1
eMultiSphereShape *eMultiSphereShape::New(cMemPool *pool, cBase *parent) {
    eMultiSphereShape *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eMultiSphereShape *obj = (eMultiSphereShape *)entry->fn(base, 0xD0, 0x10, 0, 0);
    if (obj != 0) {
        eMultiSphereShape_eMultiSphereShape(obj, parent);
        result = obj;
    }
    return result;
}
#pragma control sched=2

// eMultiSphereShape::Collide(const eBoxShape *, ...) — 0x000695e0
#pragma control sched=1
int eMultiSphereShape::Collide(const eBoxShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    if (eCollision::BoxMultiSphere(*shape, *this, ocs2, ocs1, info) != 0) {
        int i = 0;
        if (i < *(int *)((char *)info + 0x14)) {
            char *p = (char *)info + 0x20;
            do {
                __asm__ volatile(
                    "lv.q C120, 0(%0)\n"
                    "vneg.t C120, C120\n"
                    "sv.q C120, 0(%0)\n"
                    :: "r"(p) : "memory"
                );
                i++;
                p += 0x40;
            } while (i < *(int *)((char *)info + 0x14));
        }
        return 1;
    }
    return 0;
}
#pragma control sched=2

int eMultiSphereShape::Collide(const eCapsuleShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::MultiSphereCapsule(*this, *shape, ocs1, ocs2, info);
}

int eMultiSphereShape::Collide(const eConvexHullShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::MultiSphereConvexHull(*this, *shape, ocs1, ocs2, info);
}

int eMultiSphereShape::Collide(const eCompoundShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::MultiSphereCompound(*this, *shape, b, ocs1, ocs2, info);
}

int eMultiSphereShape::Collide(const eMeshShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::MultiSphereMesh(*this, *shape, b, ocs1, ocs2, info);
}

int eMultiSphereShape::Collide(const eHeightmapShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::MultiSphereHeightmap(*this, *shape, b, ocs1, ocs2, info);
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
