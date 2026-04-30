#include "eSphereShape.h"
#include "eBoxShape.h"
#include "eMultiSphereShape.h"
#include "eCapsuleShape.h"
#include "eCompoundShape.h"
#include "eMeshShape.h"
#include "eHeightmapShape.h"
#include "eCollision.h"
#include "mVec3.h"
#include "mOCS.h"

typedef int v4sf_t __attribute__((mode(V4SF)));

class eConvexHullShape;
class cBase;
class cFile;
class cMemPool;
class eCollisionSupport;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

template <class T> T *dcast(const cBase *);

extern char eSphereShapevirtualtable[];
extern cType *D_000385DC;
extern cType *D_00040FE4;
extern cType *D_00046BB8;

// Forward decl of cWriteBlock used by Write()
class cFile;
class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

void cFile_SetCurrentPos(void *, unsigned int);

// Free-function wrapper for eShape::Write (resolved via linker; jal is masked)
void eShape_Write(const void *, cFile &);
int eShape_Read(void *, cFile &, cMemPool *);
void eShape_ComputeMinAxis(void *);

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

struct eSphereShape_block_18 { int _[6]; };

int eSphereShape::CanSweep(void) const {
    return 1;
}

// eSphereShape::NeedsRollingFriction(float *) const — 0x0020913c
// Trivial leaf. Volatile-store on *out forces SNC to keep the swc1 immediately
// after the lwc1, putting `li v0, 1` in the jr delay slot rather than between
// the FPU load and store.
int eSphereShape::NeedsRollingFriction(float *out) const {
    *(volatile float *)out = mRadius;
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

// eSphereShape::Read(cFile &, cMemPool *) — 0x00067b54
int eSphereShape::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && eShape_Read(this, file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    void *h = *(void **)rb._data[0];
    __asm__ volatile("" : "+r"(h));
    cFileSystem::Read(h, &mRadius, 4);
    eShape_ComputeMinAxis(this);
    return result;
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
// Defined canonically (with class-local inheritance from eShape and an
// `operator delete` for the deleting-tail dispatch) in src/eSphereShape_dtor.cpp.

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
#pragma control sched=1

// eSphereShape::AssignCopy(const cBase *) — 0x00208f38
void eSphereShape::AssignCopy(const cBase *src) {
    eSphereShape *other = dcast<eSphereShape>(src);
    *(v4sf_t *)((char *)this + 0x40) = *(v4sf_t *)((char *)other + 0x40);
    *(v4sf_t *)((char *)this + 0x10) = *(v4sf_t *)((char *)other + 0x10);
    *(v4sf_t *)((char *)this + 0x20) = *(v4sf_t *)((char *)other + 0x20);
    *(v4sf_t *)((char *)this + 0x30) = *(v4sf_t *)((char *)other + 0x30);
    *(unsigned char *)((char *)this + 0x50) = *(unsigned char *)((char *)other + 0x50);
    __asm__ volatile("" ::: "memory");
    *(eSphereShape_block_18 *)((char *)this + 0x54) =
        *(eSphereShape_block_18 *)((char *)other + 0x54);
    *(int *)((char *)this + 0x6C) = *(int *)((char *)other + 0x6C);
    *(int *)((char *)this + 0x70) = *(int *)((char *)other + 0x70);
    *(float *)((char *)this + 0x74) = *(float *)((char *)other + 0x74);
    *(float *)((char *)this + 0x78) = *(float *)((char *)other + 0x78);
}

// eSphereShape::GetType(void) const — 0x0020905c
const cType *eSphereShape::GetType(void) const {
    if (D_00046BB8 == 0) {
        if (D_00040FE4 == 0) {
            if (D_000385DC == 0) {
                const char *name = (const char *)0x36CD74;
                const char *desc = (const char *)0x36CD7C;
                __asm__ volatile("" : "+r"(name), "+r"(desc));
                D_000385DC = cType::InitializeType(
                    name, desc, 1, 0, 0, 0, 0, 0);
            }
            D_00040FE4 = cType::InitializeType(
                0, 0, 0x227, D_000385DC, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *parentType = D_00040FE4;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x208FE0;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046BB8 = cType::InitializeType(
            0, 0, 0x228, parentType, factory, 0, 0, 0);
    }
    return D_00046BB8;
}

// eSphereShape::Collide(const eBoxShape *, ...) const — 0x000683c0
// Delegates to eCollision::BoxSphere with args swapped (box-perspective),
// then negates each contact normal at info+0x20 (stride 0x40) since the
// returned normals point box→sphere and we need sphere→box.
int eSphereShape::Collide(const eBoxShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    if (eCollision::BoxSphere(*shape, *this, ocs2, ocs1, info) != 0) {
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

// eSphereShape::Collide(const eSphereShape *, ...) const — 0x0006844c
// Same shape as Collide(eBoxShape*) but routes to SphereSphere. The arg
// swap is structural (consistent with the box overload) even though both
// shapes are spheres — the OCS pair is swapped accordingly.
int eSphereShape::Collide(const eSphereShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    if (eCollision::SphereSphere(*shape, *this, ocs2, ocs1, info) != 0) {
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
// eSphereShape::GetAABB(mBox *, const mOCS &) const — 0x00068280
// AABB of a sphere = [translation - r, translation + r] per axis.
// The translation is OCS[0x30] (xyz). Each axis is extracted via VFPU
// vdot.t against (1,0,0)/(0,1,0)/(0,0,1) (the prefix register zeros W),
// then mfv'd to an integer reg and reinterpreted as float so SNC emits
// mtc1 (not lwc1 from a stack slot).
void eSphereShape::GetAABB(mBox *box, const mOCS &ocs) const {
    int bx, by, bz;
    __asm__ volatile(
        ".word 0xdc0070c1\n"  // vpfxs 1, 0, 0, W
        "vmov.t C120, C000\n"
        "lv.q C130, 0x30(%1)\n"
        "vdot.t S100, C130, C120\n"
        "mfv %0, S100\n"
        : "=r"(bx) : "r"(&ocs)
    );
    float tx = *(float *)&bx;
    *(float *)((char *)box + 0x00) = tx - mRadius;
    *(float *)((char *)box + 0x10) = tx + mRadius;
    __asm__ volatile(
        ".word 0xdc0070c4\n"  // vpfxs 0, 1, 0, W
        "vmov.t C120, C000\n"
        "lv.q C130, 0x30(%1)\n"
        "vdot.t S100, C130, C120\n"
        "mfv %0, S100\n"
        : "=r"(by) : "r"(&ocs)
    );
    float ty = *(float *)&by;
    *(float *)((char *)box + 0x04) = ty - mRadius;
    *(float *)((char *)box + 0x14) = ty + mRadius;
    __asm__ volatile(
        ".word 0xdc0070d0\n"  // vpfxs 0, 0, 1, W
        "vmov.t C120, C000\n"
        "lv.q C130, 0x30(%1)\n"
        "vdot.t S100, C130, C120\n"
        "mfv %0, S100\n"
        : "=r"(bz) : "r"(&ocs)
    );
    float tz = *(float *)&bz;
    *(float *)((char *)box + 0x08) = tz - mRadius;
    *(float *)((char *)box + 0x18) = tz + mRadius;
}
