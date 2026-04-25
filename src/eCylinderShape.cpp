#include "eCylinderShape.h"
#include "mVec3.h"
#include "mOCS.h"

typedef int v4sf_t __attribute__((mode(V4SF)));

class cBase;
class cFile;
class cMemPool;
class eCollisionContactInfo;

extern char eCylinderShapevirtualtable[];

// dcast template, used by AssignCopy (linker-resolved per-type instantiation).
template <class T> T *dcast(const cBase *);

// cType — class registry entry; lazy-initialized via cType::InitializeType.
class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                  const cType *, cBase *(*)(cMemPool *, cBase *),
                                  const char *, const char *, unsigned int);
};

extern const char eCylinderShape_type_name[];
extern const char eCylinderShape_type_desc[];

static cType *type_eCylinderShape_root;    // 0x385DC (shared cBase root string)
static cType *type_eCylinderShape_parent;  // 0x40FE4 (shared eShape parent)
static cType *type_eCylinderShape;         // 0x46BE8 (this class)

// Joint vtable entry — joint cleanup in ~eCylinderShape() dispatches via
// joint->vtable[0x50] passing the joint adjusted-this and a fixed kind=3.
struct JointVtableEntry {
    short adjust;
    short pad;
    void (*fn)(void *self, int kind);
};

// cWriteBlock helper — same layout as used by other shape Write() functions
class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

// Free-function wrappers (resolved via linker; jal is masked)
void eShape_Write_Cylinder(const void *, cFile &);
void eShape_eShape_Cylinder(void *, cBase *);
void eCylinderShape_eCylinderShape(eCylinderShape *, cBase *);

// Pool-layout helpers (mirror eSphereShape.cpp / eCapsuleShape.cpp style)
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

// Virtual-table entry for eShape::Collide(const eCylinderShape*, ...) at offset 0xF8
struct CollideVtableEntry {
    short thisOffset;
    short pad;
    int (*fn)(void *self, const eCylinderShape *other, int a, int b,
              const mOCS *ocs1, const mOCS *ocs2, eCollisionContactInfo *info);
};

// eCylinderShape::Write(cFile &) const — 0x000740e4
#pragma control sched=1
void eCylinderShape::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eShape_Write_Cylinder(this, file);
    wb.Write(mRadius);
    wb.Write(mHalfHeight);
    wb.End();
}
#pragma control sched=2

// eCylinderShape::eCylinderShape(cBase *) — 0x000742d8
#pragma control sched=1
eCylinderShape::eCylinderShape(cBase *parent) {
    eShape_eShape_Cylinder(this, parent);
    *(void **)((char *)this + 4) = eCylinderShapevirtualtable;
    mRadius = 1.0f;
    mHalfHeight = 1.0f;
    __asm__ volatile("" ::: "memory");
    _unk88 = 0;
}
#pragma control sched=2

// eCylinderShape::GetInertialTensor(float, mVec3 *) const — 0x000747b8
// For a solid cylinder (radius r, half-height h):
//   I_xx = I_yy = (m/12) * (3*r^2 + (2*h)^2)
//   I_zz = (m/2)  * r^2
#pragma control sched=1
void eCylinderShape::GetInertialTensor(float mass, mVec3 *out) const {
    float h2 = mHalfHeight * 2.0f;
    float mHalf = mass * 0.5f;
    float rSq = mRadius * mRadius;
    float z = mHalf * rSq;
    float mOver12 = mass * (1.0f / 12.0f);
    float xy = mOver12 * (rSq * 3.0f + h2 * h2);

    int a, b, c;
    __asm__ volatile("mfc1 %0, %1" : "=r"(a) : "f"(xy));
    __asm__ volatile("mfc1 %0, %1" : "=r"(b) : "f"(xy));
    __asm__ volatile("mfc1 %0, %1" : "=r"(c) : "f"(z));
    __asm__ volatile(
        "mtv %0, S120\n"
        "mtv %1, S121\n"
        "mtv %2, S122\n"
        "sv.q C120, 0(%3)\n"
        :: "r"(a), "r"(b), "r"(c), "r"(out)
        : "memory"
    );
}
#pragma control sched=2

// eCylinderShape::Collide(const eShape *, int, int, const mOCS &, const mOCS &, eCollisionContactInfo *) const
// Address: 0x0007470c
// Generic dispatcher: virtual-calls shape's Collide(const eCylinderShape*, ...) at vtable
// offset 0xF8, passing args swapped (a/b swapped, ocs1/ocs2 swapped). Then negates each
// contact normal (first quad of each 0x40-stride contact starting at info+0x20).
#pragma control sched=1
int eCylinderShape::Collide(const eShape *shape, int a, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    char *vtable = *(char **)((char *)shape + 4);
    CollideVtableEntry *entry = (CollideVtableEntry *)(vtable + 0xF8);
    void *adjThis = (char *)shape + entry->thisOffset;
    int hit = entry->fn(adjThis, this, b, a, &ocs2, &ocs1, info);
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
#pragma control sched=2

// eCylinderShape::New(cMemPool *, cBase *) static — 0x0020dbec
#pragma control sched=1
eCylinderShape *eCylinderShape::New(cMemPool *pool, cBase *parent) {
    eCylinderShape *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eCylinderShape *obj = (eCylinderShape *)entry->fn(base, 0x90, 0x10, 0, 0);
    if (obj != 0) {
        eCylinderShape_eCylinderShape(obj, parent);
        result = obj;
    }
    return result;
}
#pragma control sched=2

// eCylinderShape::AssignCopy(const cBase *) — 0x0020db2c
struct ecs_block_18 { int _[6]; };  // 24-byte block at offset 0x54
#pragma control sched=1
void eCylinderShape::AssignCopy(const cBase *src) {
    eCylinderShape *other = dcast<eCylinderShape>(src);
    *(v4sf_t *)((char *)this + 0x40) = *(v4sf_t *)((char *)other + 0x40);
    *(v4sf_t *)((char *)this + 0x10) = *(v4sf_t *)((char *)other + 0x10);
    *(v4sf_t *)((char *)this + 0x20) = *(v4sf_t *)((char *)other + 0x20);
    *(v4sf_t *)((char *)this + 0x30) = *(v4sf_t *)((char *)other + 0x30);
    *(unsigned char *)((char *)this + 0x50) = *(unsigned char *)((char *)other + 0x50);
    __asm__ volatile("" ::: "memory");
    *(ecs_block_18 *)((char *)this + 0x54) = *(ecs_block_18 *)((char *)other + 0x54);
    *(int *)((char *)this + 0x6C) = *(int *)((char *)other + 0x6C);
    *(int *)((char *)this + 0x70) = *(int *)((char *)other + 0x70);
    *(float *)((char *)this + 0x74) = *(float *)((char *)other + 0x74);
    *(float *)((char *)this + 0x78) = *(float *)((char *)other + 0x78);
    mRadius = other->mRadius;
    mHalfHeight = other->mHalfHeight;
    _unk88 = other->_unk88;
}
#pragma control sched=2

// eCylinderShape::GetType(void) const — 0x0020dc68
#pragma control sched=1
const cType *eCylinderShape::GetType(void) const {
    if (!type_eCylinderShape) {
        if (!type_eCylinderShape_parent) {
            if (!type_eCylinderShape_root) {
                type_eCylinderShape_root = cType::InitializeType(
                    eCylinderShape_type_name, eCylinderShape_type_desc, 1, 0, 0, 0, 0, 0);
            }
            type_eCylinderShape_parent = cType::InitializeType(
                0, 0, 0x227, type_eCylinderShape_root, 0, 0, 0, 0);
        }
        type_eCylinderShape = cType::InitializeType(
            0, 0, 0x22B, type_eCylinderShape_parent,
            (cBase *(*)(cMemPool *, cBase *))&eCylinderShape::New, 0, 0, 0);
    }
    return type_eCylinderShape;
}
#pragma control sched=2

// eCylinderShape::NeedsRollingFriction(float *) const — 0x0020de04
#pragma control sched=1
int eCylinderShape::NeedsRollingFriction(float *out) const {
    *out = mRadius;
    __asm__ volatile("" ::: "memory");
    return 1;
}
#pragma control sched=2

// eCylinderShape::GetProjectedMinMax(...) const — 0x0020dd40
//
// Computes [outMin, outMax] = projection of this cylinder onto axis (in world
// space) given its OCS. The cylinder axis is ocs.row2; origin is ocs.position.
// Half-extent along axis ⋅ outAxis comes from h * (ocsZ · axis); radial
// half-extent from r * |axis - ocsZ * (ocsZ · axis)|. Combine: min/max of
// the two cap-center projections ± r * perp.
void eCylinderShape::GetProjectedMinMax(const mVec3 &axis, const mOCS &ocs,
                                        float *outMin, float *outMax) const {
    float perp;
    int   bits;
    float top_proj;
    float bot_proj;
    float r = mRadius;
    float h = mHalfHeight;

    __asm__ volatile(
        "lv.q    C120, 0(%2)\n"
        "lv.q    C130, 0x20(%3)\n"
        "vdot.t  S100, C120, C130\n"
        "mfv     %1, S100\n"
        "mtc1    %1, $f12\n"
        "mfc1    %1, $f12\n"
        "mtv     %1, S100\n"
        "vscl.t  C120, C130, S100\n"
        "lv.q    C200, 0(%2)\n"
        "vsub.t  C120, C200, C120\n"
        "vdot.t  S100, C120, C120\n"
        "vsqrt.s S100, S100\n"
        "mfv     %0, S100\n"
        : "=r"(perp), "=&r"(bits)
        : "r"(&axis), "r"(&ocs)
        : "memory", "$f12"
    );

    float r_perp = perp * r;

    __asm__ volatile(
        "mfc1    %0, %2\n"
        "mtv     %0, S100\n"
        "lv.q    C120, 0x30(%3)\n"
        "vscl.t  C210, C130, S100\n"
        "vadd.t  C210, C120, C210\n"
        "vdot.t  S100, C210, C200\n"
        "mfv     %0, S100\n"
        "mtc1    %0, %1\n"
        : "=&r"(bits), "=f"(top_proj)
        : "f"(h), "r"(&ocs)
        : "memory"
    );

    float topMin = top_proj - r_perp;
    float topMax = top_proj + r_perp;

    __asm__ volatile(
        "neg.s   $f12, %2\n"
        "mfc1    %0, $f12\n"
        "mtv     %0, S100\n"
        "vscl.t  C130, C130, S100\n"
        "vadd.t  C120, C120, C130\n"
        "vdot.t  S100, C120, C200\n"
        "mfv     %0, S100\n"
        "mtc1    %0, %1\n"
        : "=&r"(bits), "=f"(bot_proj)
        : "f"(h)
        : "memory", "$f12"
    );

    float botMin = bot_proj - r_perp;
    float botMax = bot_proj + r_perp;

    float lo = botMin;
    if (topMin < botMin) lo = topMin;
    *outMin = lo;
    float hi = botMax;
    if (!(topMax <= botMax)) hi = topMax;
    *outMax = hi;
}
