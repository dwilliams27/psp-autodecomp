#include "eNavMesh.h"
#include "cObject.h"

#pragma control sched=1

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

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

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_00046A30;
extern int D_00037C128;
extern int D_0001AF48[];

// -- eNavMesh::GetType(void) const @ 0x00203244 --
const cType *eNavMesh::GetType(void) const {
    if (D_00046A30 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    const char *baseName = (const char *)0x36CD74;
                    const char *baseDesc = (const char *)0x36CD7C;
                    __asm__ volatile("" : "+r"(baseName), "+r"(baseDesc));
                    D_000385DC = cType::InitializeType(baseName, baseDesc,
                                                       1, 0, 0, 0, 0, 0);
                }
                const cType *parentType = D_000385DC;
                cBase *(*namedFactory)(cMemPool *, cBase *) = &cNamed::New;
                __asm__ volatile("" : "+r"(parentType), "+r"(namedFactory));
                D_000385E0 = cType::InitializeType(0, 0, 2, parentType,
                                                   namedFactory, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        const cType *parentType = D_000385E4;
        cBase *(*factory)(cMemPool *, cBase *) = &eNavMesh::New;
        const char *kindName = (const char *)0x36CE94;
        const char *kindDesc = (const char *)0x36CEA0;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory),
                         "+r"(kindName), "+r"(kindDesc));
        D_00046A30 = cType::InitializeType(0, 0, 0xC4, parentType,
                                           factory, kindName, kindDesc, 4);
    }
    return D_00046A30;
}

#pragma control sched=2

// -- eNavMesh::eNavMesh(cBase *) @ 0x00057704 --
eNavMesh::eNavMesh(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x381ED0;
    *(int *)((char *)this + 0x44) = 0;
    *(int *)((char *)this + 0x48) = 0;
    *(int *)((char *)this + 0x4C) = 0;
    *(int *)((char *)this + 0x80) = 0;
    *(unsigned char *)((char *)this + 0x84) = 0;
    if (D_00037C128 < 0) {
        int i = 0;
        __asm__ volatile("" : "+r"(i));
        int *p = D_0001AF48;
        do {
            *p = 0;
            i += 1;
            p += 1;
        } while (i < 64);
        D_00037C128 = 0;
    }
}

#pragma control sched=1

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
