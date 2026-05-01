#include "eHeightmapShape.h"
#include "eCollision.h"
#include "mOCS.h"

typedef int v4sf_t __attribute__((mode(V4SF)));

class cBase;
class cFile;
class cMemPool;
class eShape;

extern "C" {
    void eShape___ct_eShape_cBaseptr(void *self, cBase *parent);
}

extern char eHeightmapShapevirtualtable[];

// cWriteBlock helper — same layout as other shape Write()s
class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(int, const float *);
    void End(void);
};

// cReadBlock helper
class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

// cFile::SetCurrentPos wrapper (free-function form; resolved via linker)
void cFile_SetCurrentPos(void *, unsigned int);

// eShape base class — declared locally to call Write/Read via member syntax
class eShape {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

// cArray<float> / cArrayBase<float> template wrappers for linker resolution
template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

template <class T>
class cArray {
public:
    void Read(cReadBlock &);
};

// dcast helper (same template pattern as eHeightmap.cpp)
template <class T> T *dcast(const cBase *);

// Pool-layout helpers (mirror other shape New() implementations)
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

// vtable entry for shape-specific Collide dispatch
struct CollideVtableEntry {
    short thisOffset;
    short pad;
    int (*fn)(void *self, const eHeightmapShape *other, int a, int b,
              const mOCS *ocs1, const mOCS *ocs2, eCollisionContactInfo *info);
};

extern "C" void eHeightmapShape_eHeightmapShape(eHeightmapShape *, cBase *);

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                  const cType *, cBase *(*)(cMemPool *, cBase *),
                                  const char *, const char *, unsigned int);
};

extern const char eHeightmapShape_type_name[];
extern const char eHeightmapShape_type_desc[];

static cType *type_eHeightmapShape_root;    // 0x385DC (shared base)
static cType *type_eHeightmapShape_parent;  // 0x40FE4
static cType *type_eHeightmapShape;         // 0x46A08

float eHeightmapShape::GetVolume(void) const {
    return 0.0f;
}

// eHeightmapShape::GetProjectedMinMax — 0x001f4d94 (8B stub)
void eHeightmapShape::GetProjectedMinMax(const mVec3 &, const mOCS &, float *, float *) const {
}

#pragma control sched=1

// eHeightmapShape::GetType(void) const — 0x001f4cbc
const cType *eHeightmapShape::GetType(void) const {
    if (!type_eHeightmapShape) {
        if (!type_eHeightmapShape_parent) {
            if (!type_eHeightmapShape_root) {
                type_eHeightmapShape_root = cType::InitializeType(
                    eHeightmapShape_type_name, eHeightmapShape_type_desc, 1, 0, 0, 0, 0, 0);
            }
            type_eHeightmapShape_parent = cType::InitializeType(
                0, 0, 0x227, type_eHeightmapShape_root, 0, 0, 0, 0);
        }
        type_eHeightmapShape = cType::InitializeType(
            0, 0, 0x2E7, type_eHeightmapShape_parent,
            (cBase *(*)(cMemPool *, cBase *))&eHeightmapShape::New, 0, 0, 0);
    }
    return type_eHeightmapShape;
}

#pragma control sched=2

// eHeightmapShape::eHeightmapShape(cBase *) — 0x00050684
eHeightmapShape::eHeightmapShape(cBase *parent) {
    eShape___ct_eShape_cBaseptr(this, parent);
    *(void **)((char *)this + 4) = eHeightmapShapevirtualtable;
    *(int *)((char *)this + 0x80) = 0;
    *(int *)((char *)this + 0x84) = 0;
}

#pragma control sched=1

// eHeightmapShape::Collide(const eBoxShape *, ...) — 0x000513b8
int eHeightmapShape::Collide(const eBoxShape *shape, int a, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::BoxHeightmap(*shape, *this, a, ocs2, ocs1, info);
}

// eHeightmapShape::Collide(const eSphereShape *, ...) — 0x000513e4
int eHeightmapShape::Collide(const eSphereShape *shape, int a, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::SphereHeightmap(*shape, *this, a, ocs2, ocs1, info);
}

// eHeightmapShape::Collide(const eMultiSphereShape *, ...) — 0x00051410
int eHeightmapShape::Collide(const eMultiSphereShape *shape, int a, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::MultiSphereHeightmap(*shape, *this, a, ocs2, ocs1, info);
}

// eHeightmapShape::Collide(const eCapsuleShape *, ...) — 0x0005143c
int eHeightmapShape::Collide(const eCapsuleShape *shape, int a, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::CapsuleHeightmap(*shape, *this, a, ocs2, ocs1, info);
}

// eHeightmapShape::Collide(const eCylinderShape *, ...) — 0x00051468
int eHeightmapShape::Collide(const eCylinderShape *shape, int a, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::ShapeHeightmap(*(const eShape *)shape, *this, a, ocs2, ocs1, info);
}

// eHeightmapShape::Collide(const eConvexHullShape *, ...) — 0x00051494
int eHeightmapShape::Collide(const eConvexHullShape *shape, int a, int, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::ConvexHullHeightmap(*shape, *this, a, ocs2, ocs1, info);
}

// eHeightmapShape::Collide(const eCompoundShape *, ...) — 0x000514c0
int eHeightmapShape::Collide(const eCompoundShape *shape, int a, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    return eCollision::CompoundHeightmap(*shape, *this, a, b, ocs2, ocs1, info);
}

// eHeightmapShape::Collide(const eShape *, ...) — 0x0005136c
// Dispatches via shape's vtable at offset 0x118 (the eHeightmapShape-variant
// Collide), swapping ocs1/ocs2 and a/b. No contact-normal fix-up.
int eHeightmapShape::Collide(const eShape *shape, int a, int b, const mOCS &ocs1, const mOCS &ocs2, eCollisionContactInfo *info) const {
    __asm__ volatile("" ::: "memory");
    CollideVtableEntry *entry = (CollideVtableEntry *)(*(char **)((char *)shape + 4) + 0x118);
    __asm__ volatile("" ::: "memory");
    return entry->fn((char *)shape + entry->thisOffset, this, b, a, &ocs2, &ocs1, info);
}

// eHeightmapShape::Write(cFile &) const — 0x00050524
void eHeightmapShape::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eShape *)this)->Write(file);
    int size = 0;
    int *arr = *(int **)((char *)this + 0x80);
    if (arr) size = arr[-1] & 0x3FFFFFFF;
    wb.Write(size);
    size = 0;
    arr = *(int **)((char *)this + 0x80);
    if (arr) size = arr[-1] & 0x3FFFFFFF;
    wb.Write(size, (const float *)arr);
    wb.End();
}

// eHeightmapShape::Read(cFile &, cMemPool *) — 0x000505bc
int eHeightmapShape::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && ((eShape *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    ((cArray<float> *)((char *)this + 0x80))->Read(rb);
    return result;
}

// eHeightmapShape::AssignCopy(const cBase *) — 0x001f4b70
struct ehs_block_24 { int _[6]; };
struct ehs_block_4  { int _[1]; };
void eHeightmapShape::AssignCopy(const cBase *src) {
    eHeightmapShape *other = dcast<eHeightmapShape>(src);
    *(v4sf_t *)((char *)this + 0x40) = *(v4sf_t *)((char *)other + 0x40);
    *(v4sf_t *)((char *)this + 0x10) = *(v4sf_t *)((char *)other + 0x10);
    *(v4sf_t *)((char *)this + 0x20) = *(v4sf_t *)((char *)other + 0x20);
    *(v4sf_t *)((char *)this + 0x30) = *(v4sf_t *)((char *)other + 0x30);
    *(unsigned char *)((char *)this + 0x50) = *(unsigned char *)((char *)other + 0x50);
    __asm__ volatile("" ::: "memory");
    *(ehs_block_24 *)((char *)this + 0x54) = *(ehs_block_24 *)((char *)other + 0x54);
    *(int *)((char *)this + 0x6C) = *(int *)((char *)other + 0x6C);
    *(int *)((char *)this + 0x70) = *(int *)((char *)other + 0x70);
    *(float *)((char *)this + 0x74) = *(float *)((char *)other + 0x74);
    *(float *)((char *)this + 0x78) = *(float *)((char *)other + 0x78);
    ((cArrayBase<float> *)((char *)this + 0x80))->operator=(*(const cArrayBase<float> *)((char *)other + 0x80));
    *(ehs_block_4 *)((char *)this + 0x84) = *(ehs_block_4 *)((char *)other + 0x84);
}

// eHeightmapShape::New(cMemPool *, cBase *) static — 0x001f4c40
#pragma control sched=1
eHeightmapShape *eHeightmapShape::New(cMemPool *pool, cBase *parent) {
    eHeightmapShape *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eHeightmapShape *obj = (eHeightmapShape *)entry->fn(base, 0x90, 0x10, 0, 0);
    if (obj != 0) {
        eHeightmapShape_eHeightmapShape(obj, parent);
        result = obj;
    }
    return result;
}
#pragma control sched=2
