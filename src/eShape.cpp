#include "eShape.h"
#include "cFile.h"
#include "eCollision.h"
#include "eCompoundShape.h"
#include "eMeshShape.h"
#include "eHeightmapShape.h"

class cBase;

class cFileSystemPlatform {
public:
    static int Read(cFilePlatform *, unsigned int, unsigned int, void *, bool);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(int, const float *);
    void End(void);
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class cMemPool;
class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

extern cType *D_000385DC;
extern cType *D_00040FE4;

extern char eShapevirtualtable[];

int eShape::GetNumSubShapes(void) const {
    return 1;
}

const eShape *eShape::GetSubShape(int) const {
    return this;
}

int eShape::IsSubShape(void) const {
    return *((const unsigned char *)this + 0x50);
}

void *eShape::GetSurface(void) const {
    return *(void **)((const char *)this + 0x6C);
}

int eShape::CanSweep(void) const {
    return 0;
}

int eShape::GetCollisionHandler(const mCollideInfo &) const {
    return 0;
}

int eShape::GetCollisionHandler(const mVec3 &) const {
    return 0;
}

void eShape::GetProjectedMinMax(const mVec3 &, const mOCS &, float *min, float *max) const {
    *max = 0.0f;
    *min = 0.0f;
}

int eShape::NeedsRollingFriction(float *out) const {
    *out = 0.0f;
    return 0;
}

int eShape::Collide(const eSphereShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::GenericConvexCollide(this, (const eShape *)shape, ocs1, ocs2, info);
}

int eShape::Collide(const eMultiSphereShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::ShapeMultiSphere(*this, *shape, ocs1, ocs2, info);
}

int eShape::Collide(const eCapsuleShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::GenericConvexCollide(this, (const eShape *)shape, ocs1, ocs2, info);
}

int eShape::Collide(const eCylinderShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::GenericConvexCollide(this, (const eShape *)shape, ocs1, ocs2, info);
}

int eShape::Collide(const eConvexHullShape *shape, int, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::GenericConvexCollide(this, (const eShape *)shape, ocs1, ocs2, info);
}

// eShape::Collide(const eCompoundShape *, ...) — 0x0002bac8
int eShape::Collide(const eCompoundShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::ShapeCompound(*this, *shape, b, ocs1, ocs2, info);
}

// eShape::Collide(const eMeshShape *, ...) — 0x0002baf0
int eShape::Collide(const eMeshShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::ShapeMesh(*this, *shape, b, ocs1, ocs2, info);
}

// eShape::Collide(const eHeightmapShape *, ...) — 0x0002bb18
int eShape::Collide(const eHeightmapShape *shape, int, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::ShapeHeightmap(*this, *shape, b, ocs1, ocs2, info);
}

// eShape::GetType(void) const — 0x001e0f00
const cType *eShape::GetType(void) const {
    if (D_00040FE4 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36CD74, (const char *)0x36CD7C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00040FE4 = cType::InitializeType(0, 0, 0x227, D_000385DC, 0, 0, 0, 0);
    }
    return D_00040FE4;
}

// eShape::Write(cFile &) const — 0x0002b788
void eShape::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cName *)((const char *)this + 0x54))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 0x6C));
    wb.Write(*(const int *)((const char *)this + 0x70));
    wb.Write(3, (const float *)((const char *)this + 0x40));
    wb.Write(3, (const float *)((const char *)this + 0x10));
    wb.Write(3, (const float *)((const char *)this + 0x20));
    wb.Write(3, (const float *)((const char *)this + 0x30));
    wb.End();
}

// cFilePlatform::ReadAsync(void *, unsigned int, unsigned int) — 0x0000e28c
void cFilePlatform::ReadAsync(void *buf, unsigned int offset, unsigned int size) {
    *(unsigned int *)((char *)this + 0x114) = offset;
    *(unsigned int *)((char *)this + 0x118) = size;
    *(void **)((char *)this + 0x11C) = buf;
    cFileSystemPlatform::Read(this, offset, size, buf, true);
}

// eShape::eShape(cBase *) — 0x0002b92c
// Stores cBase* at +0x00, eShape vtable at +0x04, zero-init various fields,
// sets float +0x78 = 0.0f and float +0x74 = -1.0f.
eShape::eShape(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(void **)((char *)this + 4) = eShapevirtualtable;
    *((char *)this + 0x50) = 0;
    *(short *)((char *)this + 0x68) = 0;
    *(short *)((char *)this + 0x6A) = 0;
    *((char *)this + 0x54) = 0;
    *(int *)((char *)this + 0x6C) = 0;
    *(int *)((char *)this + 0x70) = 0;
    *(float *)((char *)this + 0x74) = -1.0f;
    *(float *)((char *)this + 0x78) = 0.0f;
    __asm__ volatile("" ::: "memory");
}

// eShape::~eShape(void) — 0x0002b970
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard
// and the deleting-tail dispatch through operator delete. The body just
// resets the vtable to the cDynamicMemAllocator slot at 0x37E6A8 (which
// SNC stamps for objects whose dtor is currently executing). The deleting
// tail (operator delete with pool-lookup + free fallback) lives in
// src/eShape_dtor.cpp where it can be defined inline alongside a local
// eShape declaration that exposes operator delete to the compiler.
