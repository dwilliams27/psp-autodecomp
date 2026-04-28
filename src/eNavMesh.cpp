#include "eNavMesh.h"

#pragma control sched=1

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

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
