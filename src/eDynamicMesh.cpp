class cBase;
class cFile;
class cMemPool;
class cName;
class cType;
class eDynamicMeshNode;
class eDynamicMeshBone;
class eDynamicMeshLookAt;
class eDynamicMeshVisData;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRec {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cObject {
public:
    char _pad[0x44];
    cObject(cBase *);
    cObject &operator=(const cObject &);
};

class eDynamicMesh : public cObject {
public:
    eDynamicMesh(cBase *);
    ~eDynamicMesh();

    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void PlatformRead(cFile &, cMemPool *);
    void PlatformFree();
    int GetNodeIndex(const cName &, int) const;
    int GetCollisionShapeIndex(const cName &) const;
    int HasSkin() const;
    void Free();
    void Reset(cMemPool *, bool);

    void AssignCopy(const cBase *);
    static eDynamicMesh *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        if (p != 0) {
            cMemPool *pool = cMemPool::GetPoolFromPtr(p);
            char *block = ((char **)pool)[9];
            register volatile DeleteRec *rec __asm__("a1") =
                (volatile DeleteRec *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            char *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        }
    }
};

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

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469B8;
extern cType *D_00046A00;

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
    void RemoveAll(void);
};

eDynamicMesh *dcast(const cBase *);

extern "C" void eDynamicMeshVisData___dtor_eDynamicMeshVisData_void(void *, int);
extern "C" void eMesh___dtor_eMesh_void(eDynamicMesh *, int)
    __asm__("__0oFeMeshdtv");

void eDynamicMesh_eDynamicMesh(eDynamicMesh *, cBase *);

#pragma control sched=1
const cType *eDynamicMesh::GetType(void) const {
    if (D_00046A00 == 0) {
        if (D_000469B8 == 0) {
            if (D_000385E4 == 0) {
                if (D_000385E0 == 0) {
                    if (D_000385DC == 0) {
                        const char *name = (const char *)0x36CD74;
                        const char *desc = (const char *)0x36CD7C;
                        __asm__ volatile("" : "+r"(name), "+r"(desc));
                        D_000385DC = cType::InitializeType(name, desc,
                                                           1, 0, 0, 0, 0, 0);
                    }
                    const cType *parentType = D_000385DC;
                    cBase *(*factory)(cMemPool *, cBase *) =
                        (cBase *(*)(cMemPool *, cBase *))0x1C3C58;
                    __asm__ volatile("" : "+r"(parentType), "+r"(factory));
                    D_000385E0 = cType::InitializeType(0, 0, 2, parentType,
                                                       factory, 0, 0, 0);
                }
                D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                                   0, 0, 0, 0);
            }
            const cType *parentType = D_000385E4;
            const char *name = (const char *)0x36CE40;
            const char *desc = (const char *)0x36CE48;
            __asm__ volatile("" : "+r"(parentType), "+r"(name),
                             "+r"(desc));
            D_000469B8 = cType::InitializeType(0, 0, 0xE, parentType,
                                               0, name, desc, 1);
        }
        const cType *parentType = D_000469B8;
        cBase *(*factory)(cMemPool *, cBase *) =
            (cBase *(*)(cMemPool *, cBase *))0x1F3B7C;
        __asm__ volatile("" : "+r"(parentType), "+r"(factory));
        D_00046A00 = cType::InitializeType(0, 0, 0x1C, parentType,
                                           factory, 0, 0, 0);
    }
    return D_00046A00;
}
#pragma control sched=2

// ── HasSkin ──

int eDynamicMesh::HasSkin() const {
    void *vd = *(void **)((char *)this + 0x7C);
    if (vd == 0) return 0;
    return *(int *)((char *)vd + 0x108) > 0;
}

// ── PlatformFree ──

#pragma control sched=1
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
    eDynamicMesh *dstBarrier = this;
    eDynamicMesh *srcBarrier = other;
    __asm__ volatile("" : "+r"(dstBarrier), "+r"(srcBarrier));
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

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

// ── Destructor ──

eDynamicMesh::~eDynamicMesh() {
    *(void **)((char *)this + 4) = (void *)0x381740;

    Free();

    cBaseArray *shapes = (cBaseArray *)((char *)this + 0x74);
    cArrayBase<eDynamicMeshLookAt> *lookAts =
        (cArrayBase<eDynamicMeshLookAt> *)((char *)this + 0x70);
    cArrayBase<eDynamicMeshBone> *bones =
        (cArrayBase<eDynamicMeshBone> *)((char *)this + 0x6C);
    cArrayBase<eDynamicMeshNode> *nodes =
        (cArrayBase<eDynamicMeshNode> *)((char *)this + 0x68);

    if (shapes != 0) {
        shapes->RemoveAll();
    }

    if (lookAts != 0) {
        void *entries = *(void **)((char *)this + 0x70);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries -
                            (((unsigned int)*(int *)((char *)entries - 4) >> 30) * 4) -
                            4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                register volatile DeleteRec *rec __asm__("a1") =
                    (volatile DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x70) = 0;
        }
    }

    if (bones != 0) {
        void *entries = *(void **)((char *)this + 0x6C);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries -
                            (((unsigned int)*(int *)((char *)entries - 4) >> 30) * 4) -
                            4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                register volatile DeleteRec *rec __asm__("a1") =
                    (volatile DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x6C) = 0;
        }
    }

    if (nodes != 0) {
        void *entries = *(void **)((char *)this + 0x68);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries -
                            (((unsigned int)*(int *)((char *)entries - 4) >> 30) * 4) -
                            4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                register volatile DeleteRec *rec __asm__("a1") =
                    (volatile DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x68) = 0;
        }
    }

    eMesh___dtor_eMesh_void(this, 0);
}
