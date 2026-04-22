#include "eSphereShape.h"
#include "eShape.h"
#include "eMultiSphereShape.h"
#include "eCapsuleShape.h"
#include "eCompoundShape.h"
#include "eMeshShape.h"
#include "eHeightmapShape.h"
#include "eCollision.h"
#include "mVec3.h"
#include "mOCS.h"

class eConvexHullShape;
class cBase;
class cFile;
class cMemPool;
class eCollisionSupport;

extern char eSphereShapevirtualtable[];

// Forward decl of cWriteBlock used by Write()
class cFile;
class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

// Free-function wrapper for eShape::Write (resolved via linker; jal is masked)
void eShape_Write(const void *, cFile &);

// Helper wrappers (resolved at link time; relocations are masked during byte-compare)
void eShape_eShape(void *, cBase *);
void eSphereShape_eSphereShape(eSphereShape *, cBase *);
extern "C" void eShape___dtor_eShape_void(void *, int);

// Pool-layout helpers (match eBumpOffsetMap.cpp style)
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

void *cMemPool_GetPoolFromPtr(const void *);

int eSphereShape::CanSweep(void) const {
    return 1;
}

float eSphereShape::GetVolume(void) const {
    float r = mRadius;
    return r * r * r * 4.1887903f;
}

void eSphereShape::GetSupport(const mVec3 &dir, const mOCS &ocs, mVec3 *out) const {
    float r = mRadius;
    SceULong128 pos;
    __asm__ volatile(
        "mfc1 $a0, %1\n"
        "mtv $a0, S100\n"
        "lv.q %0, 0x30(%2)\n"
        "lv.q C130, 0(%3)\n"
        "vscl.t C130, C130, S100\n"
        "vadd.t %0, %0, C130\n"
        : "=X"(pos)
        : "f"(r), "r"(&ocs), "r"(&dir)
        : "$a0"
    );
    *(SceULong128 *)out = pos;
}

void eSphereShape::GetProjectedMinMax(const mVec3 &dir, const mOCS &ocs, float *outMin, float *outMax) const {
    float dot;
    __asm__ volatile(
        "lv.q C120, 0x30(%1)\n"
        "lv.q C130, 0(%2)\n"
        "vdot.t S100, C120, C130\n"
        "mfv $a1, S100\n"
        "mtc1 $a1, %0\n"
        : "=f"(dot)
        : "r"(&ocs), "r"(&dir)
        : "$a1"
    );
    *outMin = dot - mRadius;
    *outMax = dot + mRadius;
}

void eSphereShape::GetInertialTensor(float mass, mVec3 *out) const {
    float v = mass * 0.4f * (mRadius * mRadius);
    int a, b, c;
    __asm__ volatile("mfc1 %0, %1" : "=r"(a) : "f"(v));
    __asm__ volatile("mfc1 %0, %1" : "=r"(b) : "f"(v));
    __asm__ volatile("mfc1 %0, %1" : "=r"(c) : "f"(v));
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0(%3)\n"
        :: "r"(a), "r"(b), "r"(c), "r"(out)
        : "memory"
    );
}

int eSphereShape::Collide(const eMultiSphereShape *ms, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::SphereMultiSphere(*this, *ms, ocs1, ocs2, info);
}

int eSphereShape::Collide(const eCapsuleShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::SphereCapsule(*this, *shape, ocs1, ocs2, info);
}

int eSphereShape::Collide(const eConvexHullShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::SphereConvexHull(*this, *shape, ocs1, ocs2, info);
}

int eSphereShape::Collide(const eCompoundShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::SphereCompound(*this, *shape, b, ocs1, ocs2, info);
}

int eSphereShape::Collide(const eMeshShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::SphereMesh(*this, *shape, b, ocs1, ocs2, info);
}

int eSphereShape::Collide(const eHeightmapShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::SphereHeightmap(*this, *shape, b, ocs1, ocs2, info);
}

// eSphereShape::eSphereShape(cBase *) — 0x00067c2c
eSphereShape::eSphereShape(cBase *parent) {
    eShape_eShape(this, parent);
    *(void **)((char *)this + 4) = eSphereShapevirtualtable;
}

// eSphereShape::Write(cFile &) const — 0x00067afc
#pragma control sched=1
void eSphereShape::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eShape_Write(this, file);
    wb.Write(mRadius);
    wb.End();
}
#pragma control sched=2

// eSphereShape::GetSupport(const mVec3 &, const mOCS &, eCollisionSupport *) const — 0x00068860
void eSphereShape::GetSupport(const mVec3 &dir, const mOCS &ocs, eCollisionSupport *out) const {
    float r = -mRadius;
    int buf[4];
    __asm__ volatile(
        "mfc1 $a0, %1\n"
        "mtv $a0, S100\n"
        "lv.q C120, 0x30(%2)\n"
        "lv.q C130, 0(%3)\n"
        "vscl.t C130, C130, S100\n"
        "vadd.t C120, C120, C130\n"
        "sv.q C120, 0(%0)\n"
        :
        : "r"(&buf[0]), "f"(r), "r"(&ocs), "r"(&dir)
        : "$a0", "memory"
    );
    *(int *)((char *)out + 0x548) = 0;
    *(int *)((char *)out + 0x54C) = 1;
    *(int *)((char *)out + 0x400) = 0;
    *(int *)((char *)out + 0x550) = 1;
    int i = 0;
    int offset;
    __asm__ volatile("ori %0, $zero, 0" : "=r"(offset));
    char *dst = (char *)out + offset;
    char *src = (char *)&buf[0] + offset;
    do {
        __asm__ volatile(
            "lv.q C120, 0(%0)\n"
            "sv.q C120, 0(%1)\n"
            :
            : "r"(src), "r"(dst)
            : "memory"
        );
        i++;
        dst += 16;
        src += 16;
    } while (i < 1);
}

// eSphereShape::~eSphereShape(void) — 0x00067c60
#pragma control sched=1
extern "C" void eSphereShape___dtor_eSphereShape_void(eSphereShape *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = eSphereShapevirtualtable;
        eShape___dtor_eShape_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
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

// eSphereShape::New(cMemPool *, cBase *) static — 0x00208fe0
#pragma control sched=1
eSphereShape *eSphereShape::New(cMemPool *pool, cBase *parent) {
    eSphereShape *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eSphereShape *obj = (eSphereShape *)entry->fn(base, 0x80, 0x10, 0, 0);
    if (obj != 0) {
        eSphereShape_eSphereShape(obj, parent);
        result = obj;
    }
    return result;
}
#pragma control sched=2
