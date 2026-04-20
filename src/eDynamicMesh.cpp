#include "eDynamicMesh.h"

class cFile;
class cMemPool;
class cName;
class eDynamicMeshNode;
class eDynamicMeshBone;
class eDynamicMeshLookAt;
class eDynamicMeshVisData;

// ── Forward decls / helper types ──

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class cBaseArray {
public:
    void *mData;
    cBaseArray &operator=(const cBaseArray &);
};

eDynamicMesh *dcast(const cBase *);

extern "C" void eDynamicMeshVisData___dtor_eDynamicMeshVisData_void(void *, int);

void eDynamicMesh_eDynamicMesh(eDynamicMesh *, cBase *);

// ── HasSkin ──

int eDynamicMesh::HasSkin() const {
    void *vd = *(void **)((char *)this + 0x7C);
    if (vd == 0) return 0;
    return *(int *)((char *)vd + 0x108) > 0;
}

// ── PlatformFree ──

void eDynamicMesh::PlatformFree() {
    void *vd = *(void **)((char *)this + 0x7C);
    if (vd != 0) {
        eDynamicMeshVisData___dtor_eDynamicMeshVisData_void(vd, 3);
        *(void **)((char *)this + 0x7C) = 0;
    }
}

// ── New (static) ──

eDynamicMesh *eDynamicMesh::New(cMemPool *pool, cBase *parent) {
    eDynamicMesh *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eDynamicMesh *obj = (eDynamicMesh *)entry->fn(base, 0x84, 4, 0, 0);
    if (obj != 0) {
        eDynamicMesh_eDynamicMesh(obj, parent);
        result = obj;
    }
    return result;
}

// ── AssignCopy ──

void eDynamicMesh::AssignCopy(const cBase *base) {
    eDynamicMesh *other = dcast(base);
    this->cObject::operator=(*(cObject *)other);
    ((int *)this)[0x44 / 4] = ((int *)other)[0x44 / 4];
    ((int *)this)[0x48 / 4] = ((int *)other)[0x48 / 4];
    ((float *)this)[0x4C / 4] = ((float *)other)[0x4C / 4];
    ((float *)this)[0x50 / 4] = ((float *)other)[0x50 / 4];
    ((float *)this)[0x54 / 4] = ((float *)other)[0x54 / 4];
    ((unsigned char *)this)[0x58] = ((unsigned char *)other)[0x58];
    ((short *)this)[0x5C / 2] = ((short *)other)[0x5C / 2];
    ((short *)this)[0x5E / 2] = ((short *)other)[0x5E / 2];
    ((short *)this)[0x60 / 2] = ((short *)other)[0x60 / 2];
    ((short *)this)[0x62 / 2] = ((short *)other)[0x62 / 2];
    ((short *)this)[0x64 / 2] = ((short *)other)[0x64 / 2];
    ((short *)this)[0x66 / 2] = ((short *)other)[0x66 / 2];
    ((cArrayBase<eDynamicMeshNode> *)((char *)this + 0x68))->operator=(*(cArrayBase<eDynamicMeshNode> *)((char *)other + 0x68));
    ((cArrayBase<eDynamicMeshBone> *)((char *)this + 0x6C))->operator=(*(cArrayBase<eDynamicMeshBone> *)((char *)other + 0x6C));
    ((cArrayBase<eDynamicMeshLookAt> *)((char *)this + 0x70))->operator=(*(cArrayBase<eDynamicMeshLookAt> *)((char *)other + 0x70));
    ((cBaseArray *)((char *)this + 0x74))->operator=(*(cBaseArray *)((char *)other + 0x74));
    ((int *)this)[0x7C / 4] = ((int *)other)[0x7C / 4];
    ((int *)this)[0x80 / 4] = ((int *)other)[0x80 / 4];
}

// ── GetCollisionShapeIndex ──
// NOTE: unmatched. Algorithm is correct but exact branch/register layout
// differs from original. Original inlines a cName comparison with specific
// scheduling that requires either source restructuring or a helper function.

int eDynamicMesh::GetCollisionShapeIndex(const cName &name) const {
    if (*(short *)((char *)&name + 0x14) == 0) return -1;

    void **array = *(void ***)((char *)this + 0x74);
    int count = (array != 0) ? *(int *)((char *)array - 4) : 0;

    for (int v0 = 0; v0 < count; v0++, array++) {
        short nameLen = *(short *)((char *)&name + 0x14);
        char *sn = *(char **)array + 0x54;
        short shapeLen = *(short *)(sn + 0x14);

        int matched;
        if (shapeLen == 0 && nameLen == 0) {
            matched = 1;
        } else if (shapeLen != nameLen) {
            matched = 0;
        } else if (*(unsigned short *)(sn + 0x16) != *(unsigned short *)((char *)&name + 0x16)) {
            matched = 0;
        } else {
            int aligned = ((nameLen + 3) >> 2) << 2;
            char *end = sn + aligned;
            if (sn == end) {
                matched = 1;
            } else {
                const char *p = (const char *)&name;
                matched = 1;
                while (sn != end) {
                    if (*(int *)sn != *(int *)p) { matched = 0; break; }
                    sn += 4;
                    p += 4;
                }
            }
        }
        if (matched) return v0;
    }
    return -1;
}
