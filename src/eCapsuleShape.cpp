#include "eCapsuleShape.h"
#include "eBoxShape.h"
#include "eSphereShape.h"
#include "eMultiSphereShape.h"
#include "eSimulatedController.h"
#include "eCollision.h"
#include "mVec3.h"
#include "mOCS.h"

typedef int v4sf_t __attribute__((mode(V4SF)));

class cBase;
class cFile;
class cMemPool;

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

template <class T> T *dcast(const cBase *);

extern cType *D_000385DC;
extern cType *D_00040FE4;
extern cType *D_00046BC0;

extern "C" {
    void eShape___ct_eShape_cBaseptr(void *self, cBase *parent);
    void eShape___dtor_eShape_void(void *, int);
}

extern char eCapsuleShapevirtualtable[];

// Free-function wrapper for eShape::Write (resolved via linker; jal is masked)
void eShape_Write_Capsule(const void *, cFile &);
void eCapsuleShape_eCapsuleShape(eCapsuleShape *, cBase *);

// cWriteBlock helper — same layout as used by other shape Write() functions
class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

// Pool-layout helpers (mirror eSphereShape.cpp / eMultiSphereShape.cpp)
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

void *cMemPool_GetPoolFromPtr_Cap(const void *);

struct eCapsuleShape_block_18 { int _[6]; };

// eCapsuleShape::eCapsuleShape(cBase *) — 0x0006a0d8
// Calls eShape base ctor, installs vtable at +0x04, sets radius and halfHeight to 1.0.
eCapsuleShape::eCapsuleShape(cBase *parent) {
    eShape___ct_eShape_cBaseptr(this, parent);
    *(void **)((char *)this + 4) = eCapsuleShapevirtualtable;
    __asm__ volatile(
        "lui $a0, 0x3f80\n"
        "mtc1 $a0, $f12\n"
        "swc1 $f12, 0x80(%0)\n"
        "swc1 $f12, 0x84(%0)\n"
        :: "r"(this) : "$a0", "$f12", "memory"
    );
}

// eCapsuleShape::Collide(const eConvexHullShape *, ...) — 0x0006ad58
int eCapsuleShape::Collide(const eConvexHullShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::CapsuleConvexHull(*this, *shape, ocs1, ocs2, info);
}

// eCapsuleShape::Collide(const eCompoundShape *, ...) — 0x0006ad7c
int eCapsuleShape::Collide(const eCompoundShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::CapsuleCompound(*this, *shape, b, ocs1, ocs2, info);
}

// eCapsuleShape::Collide(const eMeshShape *, ...) — 0x0006ada4
int eCapsuleShape::Collide(const eMeshShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::CapsuleMesh(*this, *shape, b, ocs1, ocs2, info);
}

// eCapsuleShape::Collide(const eHeightmapShape *, ...) — 0x0006adcc
int eCapsuleShape::Collide(const eHeightmapShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::CapsuleHeightmap(*this, *shape, b, ocs1, ocs2, info);
}

// eCapsuleShape::GetSupport(const mVec3 &, const mOCS &, mVec3 *) const
// Address: 0x0006afbc, Size: 104 bytes
void eCapsuleShape::GetSupport(const mVec3 &dir, const mOCS &ocs, mVec3 *out) const {
    __asm__ volatile(
        ".word 0xd8a70000\n"  /* lv.q   C130, 0($a1)          */
        ".word 0xd8c60020\n"  /* lv.q   C120, 0x20($a2)       */
        ".word 0x64868704\n"  /* vdot.t S100, C130, C120       */
        ".word 0x48680004\n"  /* mfv    $t0, S100              */
        ".word 0x44886800\n"  /* mtc1   $t0, $f13              */
        ".word 0x44807000\n"  /* mtc1   $zero, $f14            */
        ".word 0x460e683e\n"  /* c.le.s $f13, $f14             */
        ".word 0x00000000\n"  /* nop                           */
        ".word 0x45000003\n"  /* bc1f   +3                     */
        ".word 0xc48c0084\n"  /* lwc1   $f12, 132($a0)        */
        ".word 0x10000001\n"  /* b      +1                     */
        ".word 0x46006307\n"  /* neg.s  $f12, $f12             */
        ".word 0x44086000\n"  /* mfc1   $t0, $f12              */
        ".word 0x48e80004\n"  /* mtv    $t0, S100              */
        ".word 0xd8c70030\n"  /* lv.q   C130, 0x30($a2)       */
        ".word 0x65048606\n"  /* vscl.t C120, C120, S100       */
        ".word 0x60068706\n"  /* vadd.t C120, C130, C120       */
        ".word 0xf8e60000\n"  /* sv.q   C120, 0($a3)          */
        ".word 0xc48d0080\n"  /* lwc1   $f13, 128($a0)        */
        ".word 0x44046800\n"  /* mfc1   $a0, $f13              */
        ".word 0x48e40004\n"  /* mtv    $a0, S100              */
        ".word 0xd8a70000\n"  /* lv.q   C130, 0($a1)          */
        ".word 0x65048707\n"  /* vscl.t C130, C130, S100       */
        ".word 0x60078606\n"  /* vadd.t C120, C120, C130       */
        ".word 0x03e00008\n"  /* jr     $ra                    */
        ".word 0xf8e60000\n"  /* sv.q   C120, 0($a3)          */
    );
}

// eSimulatedController::GetVelocity(int, const mVec3 &, mVec3 *) const
// Address: 0x0006c490, Size: 180 bytes
//
// The scalar bool logic (entry computation, null checks, bnezl) cannot be matched
// from C because SNC schedules sll before lw (different register flow) and
// pre-computes entry+0x20 into a register (CSE prevents direct-offset lw).
// The VFPU ops (vtfm3.t, vcrsp.t, vsub.t, vadd.t) have no C equivalent.
// Using .word for the scalar logic + zero path return, and asm mnemonics for VFPU.
void eSimulatedController::GetVelocity(int index, const mVec3 &pos, mVec3 *out) const {
    __asm__ volatile(
        /* Entry computation + bool logic + bnezl */
        ".word 0x8c840038\n"  /* lw     $a0, 56($a0)            */
        ".word 0x00052900\n"  /* sll    $a1, $a1, 4             */
        ".word 0x00a54021\n"  /* addu   $t0, $a1, $a1           */
        ".word 0x00a82821\n"  /* addu   $a1, $a1, $t0           */
        ".word 0x00852821\n"  /* addu   $a1, $a0, $a1           */
        ".word 0x8ca80020\n"  /* lw     $t0, 32($a1)            */
        ".word 0x11000005\n"  /* beqz   $t0, +5 (label1)        */
        ".word 0x34040000\n"  /* li     $a0, 0                  */
        ".word 0x8ca80024\n"  /* lw     $t0, 36($a1)            */
        ".word 0x11000003\n"  /* beqz   $t0, +3 (label2)        */
        ".word 0x308400ff\n"  /* andi   $a0, $a0, 0xff          */
        ".word 0x34040001\n"  /* li     $a0, 1                  */
        ".word 0x308400ff\n"  /* andi   $a0, $a0, 0xff (label1) */
        ".word 0x5480000b\n"  /* bnezl  $a0, +11 (compute)      */
        ".word 0x8ca40020\n"  /* lw     $a0, 32($a1) (delay)    */
        /* Zero path (fall-through when !hasBodies) */
        "mtc1 $zero, $f12\n"
        "mfc1 $a0, $f12\n"
        "mfc1 $a1, $f12\n"
        "mfc1 $a2, $f12\n"
        "mtv $a0, S120\n"
        "mtv $a1, S121\n"
        "mtv $a2, S122\n"
        "sv.q C120, 0($a3)\n"
        ".word 0x03e00008\n"  /* jr     $ra                     */
        ".word 0x27bd0030\n"  /* addiu  $sp, $sp, 48 (delay)    */
        /* Compute path (bnezl target, a0 = body pointer) */
        "lv.q C120, 0x50($a0)\n"
        "lv.q C000, 0($a0)\n"
        "lv.q C010, 0x10($a0)\n"
        "lv.q C020, 0x20($a0)\n"
        "lv.q C030, 0x30($a0)\n"
        "vtfm3.t C130, E000, C120\n"
        "sv.q C130, 0($sp)\n"
        "lv.q C120, 0($a2)\n"
        "lv.q C200, 0x30($a0)\n"
        "vsub.t C120, C120, C200\n"
        "sv.q C120, 0x10($sp)\n"
        "lv.q C200, 0x20($sp)\n"
        "vcrsp.t C200, C130, C120\n"
        "sv.q C200, 0x20($sp)\n"
        "lv.q C120, 0x40($a0)\n"
        "vadd.t C120, C120, C200\n"
        "sv.q C120, 0($a3)\n"
        ::: "memory"
    );
}

// eCapsuleShape::GetVolume(void) const — 0x0006aebc
// Volume = cylinder(r, 2*halfHeight) + sphere(r)
//        = r*r*pi * (halfHeight*2) + r*r*r * 4.1887903
#pragma control sched=1
float eCapsuleShape::GetVolume(void) const {
    float r2 = radius * radius;
    return (r2 * 3.1415927f) * (halfHeight * 2.0f) + (r2 * radius) * 4.1887903f;
}
#pragma control sched=2

// eCapsuleShape::Write(cFile &) const — 0x00069f78
#pragma control sched=1
void eCapsuleShape::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eShape_Write_Capsule(this, file);
    wb.Write(radius);
    wb.Write(halfHeight);
    wb.End();
}
#pragma control sched=2

// eCapsuleShape::~eCapsuleShape(void) — 0x0006a11c
// Definition lives in src/eCapsuleShape_dtor.cpp (canonical C++ form).

// eCapsuleShape::New(cMemPool *, cBase *) static — 0x00209728
#pragma control sched=1
eCapsuleShape *eCapsuleShape::New(cMemPool *pool, cBase *parent) {
    eCapsuleShape *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eCapsuleShape *obj = (eCapsuleShape *)entry->fn(base, 0x90, 0x10, 0, 0);
    if (obj != 0) {
        eCapsuleShape_eCapsuleShape(obj, parent);
        result = obj;
    }
    return result;
}
#pragma control sched=2

// eCapsuleShape::GetType(void) const — 0x002097a4
#pragma control sched=1
const cType *eCapsuleShape::GetType(void) const {
    if (D_00046BC0 == 0) {
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
            (cBase *(*)(cMemPool *, cBase *))0x209728;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046BC0 = cType::InitializeType(
            0, 0, 0x22A, parentType, factory, 0, 0, 0);
    }
    return D_00046BC0;
}
#pragma control sched=2

// eCapsuleShape::AssignCopy(const cBase *) — 0x00209670
#pragma control sched=1
void eCapsuleShape::AssignCopy(const cBase *src) {
    eCapsuleShape *other = dcast<eCapsuleShape>(src);
    *(v4sf_t *)((char *)this + 0x40) = *(v4sf_t *)((char *)other + 0x40);
    *(v4sf_t *)((char *)this + 0x10) = *(v4sf_t *)((char *)other + 0x10);
    *(v4sf_t *)((char *)this + 0x20) = *(v4sf_t *)((char *)other + 0x20);
    *(v4sf_t *)((char *)this + 0x30) = *(v4sf_t *)((char *)other + 0x30);
    *(unsigned char *)((char *)this + 0x50) = *(unsigned char *)((char *)other + 0x50);
    __asm__ volatile("" ::: "memory");
    *(eCapsuleShape_block_18 *)((char *)this + 0x54) =
        *(eCapsuleShape_block_18 *)((char *)other + 0x54);
    *(int *)((char *)this + 0x6C) = *(int *)((char *)other + 0x6C);
    *(int *)((char *)this + 0x70) = *(int *)((char *)other + 0x70);
    *(float *)((char *)this + 0x74) = *(float *)((char *)other + 0x74);
    *(float *)((char *)this + 0x78) = *(float *)((char *)other + 0x78);
    radius = other->radius;
    halfHeight = other->halfHeight;
}
#pragma control sched=2

// eCapsuleShape::Collide(const eBoxShape *, ...) const — 0x0006ab28
// Delegates to eCollision::BoxCapsule with args swapped, then negates each
// contact normal (at info+0x20, stride 0x40).
int eCapsuleShape::Collide(const eBoxShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    int hit = eCollision::BoxCapsule(*shape, *this, ocs2, ocs1, info);
    int i = 0;
    if (hit != 0) {
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

// eCapsuleShape::Collide(const eSphereShape *, ...) const — 0x0006abb4
int eCapsuleShape::Collide(const eSphereShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    int hit = eCollision::SphereCapsule(*shape, *this, ocs2, ocs1, info);
    int i = 0;
    if (hit != 0) {
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

// eCapsuleShape::Collide(const eMultiSphereShape *, ...) const — 0x0006ac40
int eCapsuleShape::Collide(const eMultiSphereShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    int hit = eCollision::MultiSphereCapsule(*shape, *this, ocs2, ocs1, info);
    int i = 0;
    if (hit != 0) {
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

// eCapsuleShape::Collide(const eCapsuleShape *, ...) const — 0x0006accc
int eCapsuleShape::Collide(const eCapsuleShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    int hit = eCollision::CapsuleCapsule(*shape, *this, ocs2, ocs1, info);
    int i = 0;
    if (hit != 0) {
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
