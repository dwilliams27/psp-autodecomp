#include "eCylinderShape.h"
#include "mVec3.h"
#include "mOCS.h"

typedef int v4sf_t __attribute__((mode(V4SF)));

class cBase;
class cFile;
class cMemPool;
class eCollisionContactInfo;

extern char eCylinderShapevirtualtable[];

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
