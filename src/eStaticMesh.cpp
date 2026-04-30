class cBase;
class cFile;
class cMemPool;
class cType;

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
    cObject &operator=(const cObject &);
};

class eMesh : public cObject {
public:
    void Write(cFile &) const;
};

template <class T> T *dcast(const cBase *);

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class eStaticMeshVisLOD;

class eStaticMesh : public eMesh {
public:
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" void eStaticMesh__eStaticMesh_cBaseptr(void *, cBase *);
extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469B8;
extern cType *D_000469BC;

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
