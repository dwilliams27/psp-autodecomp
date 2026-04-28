class cBase;
class cMemPool;

class cObject {
public:
    cObject &operator=(const cObject &);
};

template <class T> T *dcast(const cBase *);

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class eStaticMeshVisLOD;

class eStaticMesh : public cObject {
public:
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" void eStaticMesh__eStaticMesh_cBaseptr(void *, cBase *);

// ── eStaticMesh::AssignCopy(const cBase *) @ 0x001ed82c ──
void eStaticMesh::AssignCopy(const cBase *src) {
    eStaticMesh &other = *dcast<eStaticMesh>(src);
    ((cObject *)this)->operator=(*(const cObject *)&other);
    *(int *)((char *)this + 0x44) = *(int *)((char *)&other + 0x44);
    *(int *)((char *)this + 0x48) = *(int *)((char *)&other + 0x48);
    *(float *)((char *)this + 0x4C) = *(float *)((char *)&other + 0x4C);
    *(float *)((char *)this + 0x50) = *(float *)((char *)&other + 0x50);
    *(float *)((char *)this + 0x54) = *(float *)((char *)&other + 0x54);
    *(unsigned char *)((char *)this + 0x58) = *(unsigned char *)((char *)&other + 0x58);
    ((cArrayBase<eStaticMeshVisLOD> *)((char *)this + 0x5C))->operator=(*(cArrayBase<eStaticMeshVisLOD> *)((char *)&other + 0x5C));
    *(int *)((char *)this + 0x60) = *(int *)((char *)&other + 0x60);
    *(int *)((char *)this + 0x64) = *(int *)((char *)&other + 0x64);
}

// ── eStaticMesh::New(cMemPool *, cBase *) static @ 0x001ed8b8 ──
cBase *eStaticMesh::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eStaticMesh *result = 0;
    eStaticMesh *obj = (eStaticMesh *)rec->fn(base, 0x68, 4, 0, 0);
    if (obj != 0) {
        eStaticMesh__eStaticMesh_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
