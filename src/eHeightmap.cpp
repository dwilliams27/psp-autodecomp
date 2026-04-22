#include "eHeightmap.h"
#include "eMemCard.h"
#include "eMovie.h"
#include "ePath.h"
#include "cBase.h"

typedef int v4sf_t __attribute__((mode(V4SF)));

extern char eHeightmapvirtualtable[];
extern char eGeomvirtualtable[];
extern "C" void eHeightmapData___dtor_eHeightmapData_void(void *, int);
extern "C" void eGeom___dtor_eGeom_void(void *, int);
extern "C" void eHeightmap_eHeightmap(eHeightmap *, cBase *);

class cFile;
class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int, const float *);
    void End(void);
};

void eStaticGeom_Write(const void *, cFile &);
void *cMemPool_GetPoolFromPtr(const void *);

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

template <class T> T *dcast(const cBase *);

void eHeightmap::PlatformReset(cMemPool *, bool) {
}

eHeightmap::eHeightmap(cBase *base) : eGeom(base) {
    *(void **)((char *)this + 4) = eHeightmapvirtualtable;
    *(void **)((char *)this + 0x90) = 0;
}

void eHeightmap::PlatformFree(void) {
    void *data = *(void **)((char *)this + 0x90);
    if (data != 0) {
        eHeightmapData___dtor_eHeightmapData_void(data, 3);
        *(void **)((char *)this + 0x90) = 0;
    }
}

int eHeightmap::GetSurface(int idx) const {
    char *arr = *(char **)((char *)*(void **)((char *)this + 0x60) + 0x7C);
    char *entry = arr + idx * 20 + 12;
    return *(int *)entry;
}

void eHeightmap::GetEmbedContacts(const eCollisionInfo &info, int idx, const mSphere *sphere, eContactCollector *) const {
    char *shape = ((char **)&info)[1];
    int *entry = (int *)(((char **)shape)[1] + 0xB0);
    ((void (*)(char *, int, const mSphere *, const eCollisionInfo &))entry[1])(shape + *(short *)entry, idx, sphere, info);
}

void eHeightmap::GetSweptContacts(const eCollisionInfo &info, int idx, const mSphere *sphere, const mCollideInfo *ci, eContactCollector *) const {
    char *shape = ((char **)&info)[1];
    int *entry = (int *)(((char **)shape)[1] + 0xA8);
    ((void (*)(char *, int, const mSphere *, const mCollideInfo *, const eCollisionInfo &))entry[1])(shape + *(short *)entry, idx, sphere, ci, info);
}

int eMemCard::Reset(void) {
    return 1;
}

void eMemCard::EndLoad(const eMemCard::eLoadParams &) {
}

void eMemCard::EndSave(const eMemCard::eSaveParams &) {
}

void eMovie::PlatformPause(void) {
}

void eHeightmap::CastSphere(const eCollisionInfo &info, const mRay &ray, float radius, mCollideHit *hit) const {
    // Body is VFPU ray processing + vtable dispatch.
    // Written as inline asm with embedded prologue/epilogue since the compiler
    // cannot infer them from .word VFPU instructions.
    __asm__ volatile(
        ".set noreorder\n"
        "addiu $sp, $sp, -96\n"
        "sw $ra, 80($sp)\n"
        "move $t3, $a1\n"
        "lv.q C120, 0($a2)\n"
        "sv.q C120, 0($sp)\n"
        "swc1 $f12, 12($sp)\n"
        "lwc1 $f12, 32($a2)\n"
        "sv.q C120, 0x10($sp)\n"
        "addiu $v0, $sp, 16\n"
        "lv.q C130, 0x10($a2)\n"
        "sv.q C130, 0x30($sp)\n"
        "swc1 $f12, 64($sp)\n"
        "mfc1 $a0, $f12\n"
        "mtv $a0, S100\n"
        "vscl.t C130, C130, S100\n"
        "vadd.t C120, C120, C130\n"
        "sv.q C120, 0x20($sp)\n"
        "lw $a0, 4($t3)\n"
        "lw $a1, 4($a0)\n"
        "addiu $a1, $a1, 160\n"
        "lh $a2, 0($a1)\n"
        "addu $a0, $a0, $a2\n"
        "addiu $t0, $a3, 16\n"
        "addiu $t1, $a3, 32\n"
        "addiu $t2, $a3, 48\n"
        "lw $v1, 4($a1)\n"
        "move $a1, $sp\n"
        "move $a2, $v0\n"
        "jalr $v1\n"
        "move $a3, $t3\n"
        "lw $ra, 80($sp)\n"
        "jr $ra\n"
        "addiu $sp, $sp, 96\n"
        ".set reorder\n"
    );
}

// ── eHeightmap::CastRay — 0x00052b68 ──
void eHeightmap::CastRay(const eCollisionInfo &info, const mRay &ray, mCollideHit *hit) const {
    char *shape = ((char **)&info)[1];
    int *entry = (int *)(((char **)shape)[1] + 0x98);
    ((void (*)(char *, const mRay &, const eCollisionInfo &, mCollideHit *, mCollideHit *, mCollideHit *))entry[1])(
        shape + *(short *)entry, ray, info,
        (mCollideHit *)((char *)hit + 16),
        (mCollideHit *)((char *)hit + 32),
        (mCollideHit *)((char *)hit + 48));
}

// ── eHeightmap::New — 0x001f5df4 ──
#pragma control sched=1
eHeightmap *eHeightmap::New(cMemPool *pool, cBase *parent) {
    eHeightmap *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eHeightmap *obj = (eHeightmap *)entry->fn(base, 0xA0, 0x10, 0, 0);
    if (obj != 0) {
        eHeightmap_eHeightmap(obj, parent);
        result = obj;
    }
    return result;
}

#pragma control sched=2
// ── eHeightmap::Write — 0x00052724 ──
void eHeightmap::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eStaticGeom_Write(this, file);
    wb.Write(3, (const float *)((char *)this + 0x40));
    wb.Write(3, (const float *)((char *)this + 0x10));
    wb.Write(3, (const float *)((char *)this + 0x20));
    wb.Write(3, (const float *)((char *)this + 0x30));
    wb.End();
}

// ── eHeightmap::~eHeightmap — 0x0005293c ──
#pragma control sched=1
extern "C" void eHeightmap___dtor_eHeightmap_void(eHeightmap *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = eHeightmapvirtualtable;
        self->PlatformFree();
        *(void **)((char *)self + 4) = eGeomvirtualtable;
        eGeom___dtor_eGeom_void(self, 0);
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
// ── eHeightmap::AssignCopy — 0x001f5d2c ──
void eHeightmap::AssignCopy(const cBase *src) {
    eHeightmap *other = dcast<eHeightmap>(src);
    *(v4sf_t *)((char *)this + 0x40) = *(v4sf_t *)((char *)other + 0x40);
    *(v4sf_t *)((char *)this + 0x10) = *(v4sf_t *)((char *)other + 0x10);
    *(v4sf_t *)((char *)this + 0x20) = *(v4sf_t *)((char *)other + 0x20);
    *(v4sf_t *)((char *)this + 0x30) = *(v4sf_t *)((char *)other + 0x30);
    float t5C = *(float *)((char *)other + 0x5C);
    *(v4sf_t *)((char *)this + 0x50) = *(v4sf_t *)((char *)other + 0x50);
    *(float *)((char *)this + 0x5C) = t5C;
    *(int *)((char *)this + 0x60) = *(int *)((char *)other + 0x60);
    *(int *)((char *)this + 0x64) = *(int *)((char *)other + 0x64);
    *(int *)((char *)this + 0x68) = *(int *)((char *)other + 0x68);
    *(int *)((char *)this + 0x6C) = *(int *)((char *)other + 0x6C);
    *(int *)((char *)this + 0x70) = *(int *)((char *)other + 0x70);
    *(float *)((char *)this + 0x74) = *(float *)((char *)other + 0x74);
    *(float *)((char *)this + 0x78) = *(float *)((char *)other + 0x78);
    *(int *)((char *)this + 0x7C) = *(int *)((char *)other + 0x7C);
    *(int *)((char *)this + 0x80) = *(int *)((char *)other + 0x80);
    *(int *)((char *)this + 0x84) = *(int *)((char *)other + 0x84);
    *(int *)((char *)this + 0x88) = *(int *)((char *)other + 0x88);
    *(unsigned char *)((char *)this + 0x8C) = *(unsigned char *)((char *)other + 0x8C);
    *(unsigned char *)((char *)this + 0x8D) = *(unsigned char *)((char *)other + 0x8D);
    *(int *)((char *)this + 0x90) = *(int *)((char *)other + 0x90);
}

float ePath::PathT2Units(float t) const {
    int startIdx = (int)t;
    float endT = t + 1.0f;
    ePathPoint *points = mPoints;
    float maxIdx;

    if (points != 0) {
        maxIdx = (float)((((int *)points)[-1] & 0x3FFFFFFF) - 1);
    } else {
        maxIdx = -1.0f;
    }

    int endIdx;
    if (endT <= 0.0f) {
        endIdx = (int)0.0f;
    } else {
        if (maxIdx <= endT) {
            endT = maxIdx;
        }
        endIdx = (int)endT;
    }

    float startDist = points[startIdx].mDistance;
    float endDist = points[endIdx].mDistance;
    return startDist + (t - (float)startIdx) * (endDist - startDist);
}
