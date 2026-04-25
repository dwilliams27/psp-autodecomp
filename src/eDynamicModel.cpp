#include "eDynamicModel.h"
#include "eDynamicMesh.h"
#include "ePath.h"
#include "mOCS.h"

typedef int v4sf_t __attribute__((mode(V4SF)));

template <class T> class cHandleT {
public:
    int mHandle;
};

class cTimeValue {
public:
    int mValue;
};

class cFile;
class cBase;
class cMemPool;

extern "C" {
    void eDynamicModel__eDynamicModel_cBaseptr(void *self, cBase *parent);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

void eDynamicGeom_Write(const void *, cFile &);

class cWriteBlock {
public:
    char _data[264];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void End(void);
};

void *eDynamicModel::GetCurrentPhysicsController(void) const {
    int idx = *(int *)((char *)this + 0x120);
    if (idx != -1) {
        return ((void **)*(char **)((char *)this + 0x118))[idx];
    }
    return 0;
}

void *eDynamicModel::GetCurrentPhysicsController(void) {
    int idx = *(int *)((char *)this + 0x120);
    if (idx != -1) {
        return ((void **)*(char **)((char *)this + 0x118))[idx];
    }
    return 0;
}

void eDynamicModel::GetColliderToWorld(int idx, mOCS *out) const {
    v4sf_t *src = (v4sf_t *)(*(char **)((char *)this + 0x110) + idx * 192);
    *(v4sf_t *)((char *)out + 0x30) = src[3];
    *(v4sf_t *)((char *)out + 0x00) = src[0];
    *(v4sf_t *)((char *)out + 0x10) = src[1];
    *(v4sf_t *)((char *)out + 0x20) = src[2];
}

#pragma control sched=2

void eDynamicModel::GetEmbedContacts(const eCollisionInfo &info, int idx, const mSphere *sphere, eContactCollector *cc) const {
    char *shape = ((char **)&info)[1];
    int *entry = (int *)(((char **)shape)[1] + 0xB0);
    ((void (*)(char *, int, const mSphere *, const eCollisionInfo &, eContactCollector *))entry[1])(shape + *(short *)entry, idx, sphere, info, cc);
}

void eDynamicModel::GetSweptContacts(const eCollisionInfo &info, int idx, const mSphere *sphere, const mCollideInfo *ci, eContactCollector *cc) const {
    char *shape = ((char **)&info)[1];
    int *entry = (int *)(((char **)shape)[1] + 0xA8);
    ((void (*)(char *, int, const mSphere *, const mCollideInfo *, const eCollisionInfo &, eContactCollector *))entry[1])(shape + *(short *)entry, idx, sphere, ci, info, cc);
}

void eDynamicModel::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    eDynamicGeom_Write(this, file);
    wb.Write(*(unsigned int *)((char *)this + 0xF4));
    wb.End();
}

void eDynamicModel::SetSkin(cHandleT<eSkin> skin1, int type, int skin2, cTimeValue time) {
    *(cHandleT<eSkin> *)((char *)this + 0xFC) = skin1;
    SetMaterialSet(type, time);
    SetSurfaceSet(skin2);
}

#pragma control sched=2

int eDynamicModel::GetSkinIndex(void) const {
    void *obj = *(void **)((char *)this + 0x60);
    if (obj == 0) return -1;
    int i = 0;
    __asm__ volatile("" ::: "memory");
    int *arr = *(int **)((char *)obj + 0x4C);
    int byteOff = 0;
    while (1) {
        int len = 0;
        if (arr != 0) len = *((int *)((char *)arr - 4)) & 0x3FFFFFFF;
        if (i >= len) return -1;
        if ((unsigned char)(*(int *)((char *)arr + byteOff) == *(int *)((char *)this + 0xFC))) {
            return i;
        }
        ++i;
        byteOff += 4;
    }
}

#pragma control sched=1

void *eDynamicModel::GetAnimationState(void) const {
    void *p = *(void **)((char *)this + 0x124);
    int flag = 1;
    if (p != 0) {
        flag = (*(int *)((char *)p - 4) & 0x3FFFFFFF) == 0;
    }
    if (flag != 0) return 0;
    void *ret = 0;
    if (*(signed char *)((char *)this + 0x13C) > 0) {
        ret = p;
    }
    return ret;
}

void eDynamicModel::CastRay(const eCollisionInfo &info, const mRay &ray, mCollideHit *hit) const {
    char *shape = ((char **)&info)[1];
    int *entry = (int *)(((char **)shape)[1] + 0x98);
    ((void (*)(char *, const mRay &, const eCollisionInfo &, void *, void *, void *))entry[1])(
        shape + *(short *)entry, ray, info,
        (char *)hit + 0x10, (char *)hit + 0x20, (char *)hit + 0x30);
}

#pragma control sched=1

void eDynamicModel::ClearPartialAnimationController(int idx) {
    if (idx < 0) return;
    char *ptr = *(char **)((char *)this + 0x128);
    int count = 0;
    if (ptr != 0) {
        count = *(int *)(ptr - 4) & 0x3FFFFFFF;
    }
    if (idx < count) {
        ptr[idx * 0x44 + 64] = 0;
    }
}

bool eDynamicModel::NeedsSkinning(const eDynamicMesh *mesh, int count, int *indices) const {
    for (int i = 0; i < count; i++) {
        int idx = indices[i];
        idx <<= 2;
        char *nodes = *(char **)((char *)mesh + 0x7C);
        int n = 0;
        char *ptr = *(char **)((unsigned int)nodes + idx + 0x88);
        if (ptr != 0) {
            n = *(int *)(ptr - 4) & 0x3FFFFFFF;
        }
        if (n > 0) {
            char *table = *(char **)((char *)this + 0x148);
            unsigned int val = *(unsigned int *)((unsigned int)table + idx);
            unsigned int gval = *(unsigned int *)0x0037D124;
            if ((unsigned char)((val >> 16) == (gval & 0xFFFF))) {
                return false;
            }
            goto ret;
        }
    }
ret:
    return true;
}

float ePath::PathT2Units(float t) const {
    int intPart = (int)t;
    float nextT = t + 1.0f;
    ePathPoint *points = mPoints;
    float maxIndex;
    if (points != 0) {
        int count = *(int *)((char *)points - 4) & 0x3FFFFFFF;
        maxIndex = (float)(count - 1);
    } else {
        maxIndex = -1.0f;
    }
    int nextIdx;
    if (nextT <= 0.0f) {
        nextIdx = (int)0.0f;
    } else {
        if (maxIndex <= nextT) {
            nextT = maxIndex;
        }
        nextIdx = (int)nextT;
    }
    float curDist = points[intPart].mDistance;
    float nextDist = points[nextIdx].mDistance;
    float frac = t - (float)intPart;
    return curDist + frac * (nextDist - curDist);
}

extern char eDynamicMeshclassdesc[];

eDynamicMesh::eDynamicMesh(cBase *parent) : cObject(parent) {
    ((int *)this)[0x44 / 4] = 0;
    __asm__ volatile("" ::: "memory");
    ((int *)this)[0x48 / 4] = -1;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x4C) = 0.0f;
    __asm__ volatile("" ::: "memory");
    ((unsigned char *)this)[0x58] = 0;
    __asm__ volatile("" ::: "memory");
    ((void **)this)[1] = eDynamicMeshclassdesc;
    *(short *)((char *)this + 0x5C) = 0;
    *(short *)((char *)this + 0x5E) = 0;
    *(short *)((char *)this + 0x60) = 0;
    *(short *)((char *)this + 0x62) = 0;
    *(short *)((char *)this + 0x64) = 0;
    *(short *)((char *)this + 0x66) = 0;
    ((int *)this)[0x68 / 4] = 0;
    ((int *)this)[0x6C / 4] = 0;
    ((int *)this)[0x70 / 4] = 0;
    ((int *)this)[0x74 / 4] = 0;
    *(void **)((char *)this + 0x78) = (void *)this;
    ((int *)this)[0x7C / 4] = 0;
    ((int *)this)[0x80 / 4] = 0;
    *(float *)((char *)this + 0x50) = 1024.0f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x54) = 0.0009765625f;
}

#pragma control sched=2

cBase *eDynamicModel::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eDynamicModel *obj = (eDynamicModel *)rec->fn(base, 0x150, 0x10, 0, 0);
    eDynamicModel *result = 0;
    if (obj != 0) {
        eDynamicModel__eDynamicModel_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
