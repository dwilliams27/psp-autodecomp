#include "eNavMesh.h"
#include "cObject.h"

#pragma control sched=1

template <class T> T *dcast(const cBase *);

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class eNavTri;
class eNavVert;
class eNavPortal;

typedef int v4sf_t __attribute__((mode(V4SF)));

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

// -- eNavMesh::AssignCopy(const cBase *) @ 0x00203138 --
void eNavMesh::AssignCopy(const cBase *base) {
    eNavMesh *other = dcast<eNavMesh>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    ((cArrayBase<eNavTri> *)((char *)this + 0x44))->operator=(
        *(const cArrayBase<eNavTri> *)((char *)other + 0x44));
    ((cArrayBase<eNavVert> *)((char *)this + 0x48))->operator=(
        *(const cArrayBase<eNavVert> *)((char *)other + 0x48));
    ((cArrayBase<unsigned char> *)((char *)this + 0x4C))->operator=(
        *(const cArrayBase<unsigned char> *)((char *)other + 0x4C));
    *(v4sf_t *)((char *)this + 0x50) =
        *(const v4sf_t *)((char *)other + 0x50);
    *(v4sf_t *)((char *)this + 0x60) =
        *(const v4sf_t *)((char *)other + 0x60);
    *(v4sf_t *)((char *)this + 0x70) =
        *(const v4sf_t *)((char *)other + 0x70);
    __asm__ volatile("" ::: "memory");
    ((cArrayBase<eNavPortal> *)((char *)this + 0x80))->operator=(
        *(const cArrayBase<eNavPortal> *)((char *)other + 0x80));
    *(unsigned char *)((char *)this + 0x84) =
        *(const unsigned char *)((char *)other + 0x84);
}

extern "C" {
    void eNavMesh__eNavMesh_cBaseptr(void *self, cBase *parent);
}

// -- eNavMesh::New(cMemPool *, cBase *) static @ 0x002031c8 --
cBase *eNavMesh::New(cMemPool *pool, cBase *parent) {
    eNavMesh *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eNavMesh *obj = (eNavMesh *)rec->fn(base, 0x90, 0x10, 0, 0);
    if (obj != 0) {
        eNavMesh__eNavMesh_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// -- eNavMesh::FindNavMeshFromPoint(eNavMeshPos *) static @ 0x0005979c --
int eNavMesh::FindNavMeshFromPoint(eNavMeshPos *pos) {
    __asm__ volatile("" ::: "memory");
    eNavMesh *mesh = FindNavMeshFromPoint(
        (const mVec3 *)((char *)pos + 0x10),
        *(unsigned char *)((char *)pos + 0x20));
    if (mesh != 0) {
        char *current = (char *)mesh + 0x30;
        *(int *)pos = *(int *)current;
        __asm__ volatile("" ::: "memory");
        return 1;
    }
    return 0;
}
