class cBase;
class cFile;
class cMemPool;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class cHandle {
public:
    int mHandle;
    void Write(cWriteBlock &) const;
};

class cObject {
public:
    char _pad[0x44];
    cObject(cBase *);
    cObject &operator=(const cObject &);
};

class eMesh {
public:
    ~eMesh();
    void Write(cFile &) const;
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cMemBlockSuspend {
public:
    int _data[1];
    cMemBlockSuspend(cMemPool *);
    ~cMemBlockSuspend(void);
};

class cFileSystem {
public:
    static void Read(void *, void *, unsigned int);
};

template <class T> T *dcast(const cBase *);

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
    void RemoveAll(void);
};

class eStaticMeshVisLOD;
class eStaticMeshVisData {
public:
    int mData;
    eStaticMeshVisData(void);
    void Read(cReadBlock &, cMemPool *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class eStaticMesh : public cObject {
public:
    eStaticMesh(cBase *);
    ~eStaticMesh();
    void AssignCopy(const cBase *);
    void Free(void);
    const cType *GetType(void) const;
    void PlatformRead(cFile &, cMemPool *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct cGUID {
    int a;
    int b;
};

inline void *operator new(unsigned int, void *p) { return p; }

void cFile_SetCurrentPos(void *, unsigned int);
void cMemPool_GetValue(cMemPool *, const cGUID &, void **)
    __asm__("__0fIcMemPoolIGetValueRC6FcGUIDPPv");
extern "C" void eMesh___dtor_eMesh_void(eMesh *, int)
    __asm__("__0oFeMeshdtv");

extern char eMeshclassdesc[];
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469B8;
extern cType *D_000469BC;

// ── eStaticMesh::eStaticMesh(cBase *) @ 0x000449B4 ──
eStaticMesh::eStaticMesh(cBase *parent) : cObject(parent) {
    register int negOne __asm__("$4") = -1;
    *(int *)((char *)this + 0x44) = 0;
    register float zero __asm__("$f12") = 0.0f;
    *(int *)((char *)this + 0x48) = negOne;
    *(float *)((char *)this + 0x4C) = zero;
    *(unsigned char *)((char *)this + 0x58) = 0;
    *(void **)((char *)this + 0x04) = eMeshclassdesc;
    *(int *)((char *)this + 0x5C) = 0;
    *(int *)((char *)this + 0x60) = 0;
    *(int *)((char *)this + 0x64) = 0;
    *(float *)((char *)this + 0x50) = 128.0f;
    *(float *)((char *)this + 0x54) = 0.0078125f;
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

// ── eStaticMesh::~eStaticMesh(void) @ 0x00044A24 ──
eStaticMesh::~eStaticMesh() {
    *(void **)((char *)this + 4) = (void *)0x380ED8;
    Free();
    cArrayBase<eStaticMeshVisLOD> *lods =
        (cArrayBase<eStaticMeshVisLOD> *)((char *)this + 0x5C);
    if (lods != 0) {
        lods->RemoveAll();
    }
    eMesh___dtor_eMesh_void((eMesh *)this, 0);
}

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
        new (obj) eStaticMesh(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *eStaticMesh::GetType(void) const {
    if (D_000469BC == 0) {
        if (D_000469B8 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        D_000385DC = cType::InitializeType(
                            (const char *)0x36CD74, (const char *)0x36CD7C,
                            1, 0, 0, 0, 0, 0);
                    }
                    D_000385E0 = cType::InitializeType(
                        0, 0, 2, D_000385DC,
                        (cBase *(*)(cMemPool *, cBase *))&cNamed::New,
                        0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            D_000469B8 = cType::InitializeType(0, 0, 0xE, D_000385E4, 0,
                                               (const char *)0x36CE40,
                                               (const char *)0x36CE48, 1);
        }
        D_000469BC = cType::InitializeType(
            0, 0, 0x14, D_000469B8,
            (cBase *(*)(cMemPool *, cBase *))&eStaticMesh::New, 0, 0, 0);
    }
    return D_000469BC;
}

class eStaticMeshVisLOD {
public:
    char _pad[0x14];
    void Write(cWriteBlock &) const;
};

void eStaticMesh::Write(cFile &file) const {
    int var_a0;
    eStaticMeshVisLOD *temp_s2;

    cWriteBlock wb(file, 6);
    ((const eMesh *)this)->Write(file);

    temp_s2 = *(eStaticMeshVisLOD **)((char *)this + 0x5C);
    var_a0 = 0;
    if (temp_s2 != 0) {
        var_a0 = ((int *)temp_s2)[-1] & 0x3FFFFFFF;
    }
    wb.Write(var_a0);

    temp_s2 = *(eStaticMeshVisLOD **)((char *)this + 0x5C);
    int var_s1 = 0;
    if (temp_s2 != 0) {
        var_s1 = ((int *)temp_s2)[-1] & 0x3FFFFFFF;
    }

    eStaticMeshVisLOD *lods = temp_s2;
    int i = 0;
    int offset = 0;
    if (i < var_s1) {
        eStaticMeshVisLOD *lod = lods + offset;
        do {
            lod->Write(wb);
            i++;
            lod++;
        } while (i < var_s1);
    }

    wb.WriteBase(*(cBase *const *)((char *)this + 0x64));
    wb.End();
}

// ── eStaticMesh::PlatformRead(cFile &, cMemPool *) @ 0x00044790 ──
void eStaticMesh::PlatformRead(cFile &file, cMemPool *pool) {
    cGUID guidArg;
    cMemBlockSuspend ms(pool);
    cReadBlock rb(file, 1, true);
    cMemPool *visPool;
    int pad24;
    cGUID guid;
    cGUID guidCopy;
    char pad[8];
    char hasVisData;
    eStaticMeshVisData *result;
    eStaticMeshVisData *obj;
    __asm__ volatile("" : : "m"(pad), "m"(pad24));

    if ((unsigned int)rb._data[3] < 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return;
    }

    if (*(unsigned char *)((char *)this + 0x58) != 0) {
        return;
    }

    visPool = 0;
    guidCopy.a = 0x812D2B72;
    guidCopy.b = 0xB1E8A1F9;
    __asm__ volatile("" : "+m"(guidCopy));
    guid.a = 0x812D2B72;
    guid.b = 0xB1E8A1F9;
    guidArg = guid;
    cMemPool_GetValue(pool, guidArg, (void **)&visPool);

    cFileSystem::Read(*(void **)rb._data[0], &hasVisData, 1);
    unsigned int has = hasVisData != 0;
    if (has != 0) {
        void *block = ((void **)pool)[9];
        AllocRec *rec = (AllocRec *)(((PoolBlock *)block)->allocTable + 0x28);
        short off = rec->offset;
        void *base = (char *)block + off;
        result = 0;
        obj = (eStaticMeshVisData *)rec->fn(base, 4, 0, 0, -1);
        if (obj != 0) {
            new (obj) eStaticMeshVisData;
            result = obj;
        }
        *(eStaticMeshVisData **)((char *)this + 0x60) = result;
        result->Read(rb, visPool);
    }
}
