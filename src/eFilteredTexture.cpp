class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cBaseArray {
public:
    void Write(cWriteBlock &) const;
    cBaseArray &operator=(const cBaseArray &);
};

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
};

class eVirtualTexture : public cObject {
public:
    eVirtualTexture(cBase *);
    void Write(cFile &) const;
};

class eFilteredTexture : public eVirtualTexture {
public:
    char _pad[0x50 - sizeof(eVirtualTexture)];
    int field_50;
    void *field_54;

    eFilteredTexture(cBase *);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static eFilteredTexture *New(cMemPool *, cBase *);
};

template <class T> T *dcast(const cBase *);

template <class T>
class cHandleT {
public:
    int mIndex;
    cHandleT &operator=(const cHandleT &rhs) { mIndex = rhs.mIndex; return *this; }
};

class eTexture;

#pragma control sched=1
eFilteredTexture::eFilteredTexture(cBase *parent) : eVirtualTexture(parent) {
    *(void **)((char *)this + 4) = (void *)0x3852D0;
    field_50 = 0;
    field_54 = this;
}

// ── eFilteredTexture::Write @ 0x00080fe8 ──
void eFilteredTexture::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    eVirtualTexture::Write(file);
    ((const cBaseArray *)((char *)this + 0x50))->Write(wb);
    wb.End();
}
#pragma control sched=2

#pragma control sched=1
// ── eFilteredTexture::New @ 0x00217f9c ──
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

extern "C" void eFilteredTexture_eFilteredTexture(void *, cBase *);

eFilteredTexture *eFilteredTexture::New(cMemPool *pool, cBase *parent) {
    eFilteredTexture *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eFilteredTexture *obj = (eFilteredTexture *)entry->fn(base, 0x58, 4, 0, 0);
    if (obj != 0) {
        eFilteredTexture_eFilteredTexture(obj, parent);
        result = obj;
    }
    return result;
}

// ── eFilteredTexture::AssignCopy @ 0x00217f10 ──
void eFilteredTexture::AssignCopy(const cBase *src) {
    eFilteredTexture &other = *dcast<eFilteredTexture>(src);
    ((cObject *)this)->operator=(*(const cObject *)&other);
    ((signed char *)this)[0x44] = ((signed char *)&other)[0x44];
    ((signed char *)this)[0x45] = ((signed char *)&other)[0x45];
    ((signed char *)this)[0x46] = ((signed char *)&other)[0x46];
    ((unsigned char *)this)[0x47] = ((unsigned char *)&other)[0x47];
    *(short *)((char *)this + 0x48) = *(short *)((char *)&other + 0x48);
    *(short *)((char *)this + 0x4A) = *(short *)((char *)&other + 0x4A);
    __asm__ volatile("" ::: "memory");
    ((cHandleT<eTexture> *)((char *)this + 0x4C))->operator=(*(cHandleT<eTexture> *)((char *)&other + 0x4C));
    __asm__ volatile("" ::: "memory");
    ((cBaseArray *)((char *)this + 0x50))->operator=(*(cBaseArray *)((char *)&other + 0x50));
}
#pragma control sched=2

extern "C" int sceKernelFreePartitionMemory(int);
extern "C" int D_00034EC4;

extern "C" void __free_heap(void) {
    int heap = D_00034EC4;
    if (heap != 0) {
        sceKernelFreePartitionMemory(heap);
        D_00034EC4 = 0;
    }
}

class ePhysicsControllerConfig {
public:
    ePhysicsControllerConfig(cBase *);
};

class eBipedControllerConfig : public ePhysicsControllerConfig {
public:
    char _pad[0x20 - sizeof(ePhysicsControllerConfig)];
    float field_20;

    eBipedControllerConfig(cBase *);
};

eBipedControllerConfig::eBipedControllerConfig(cBase *parent)
    : ePhysicsControllerConfig(parent) {
    *(void **)((char *)this + 4) = (void *)0x382BF0;
    field_20 = 1.0f;
    __asm__ volatile("" ::: "memory");
}

extern "C" void *__vec_new(void *, int, int, void (*)(void *));

class eDynamicMeshObjectVisData {
public:
    char field_0[0x80];
    int field_80;
    int field_84;

    eDynamicMeshObjectVisData(void);
};

#pragma control sched=1
eDynamicMeshObjectVisData::eDynamicMeshObjectVisData(void) {
    void (*ctor)(void *) = (void (*)(void *))0x21DA2C;
    __asm__ volatile("" : "+r"(ctor));
    __vec_new(this, 0x20, 4, ctor);
    field_80 = 0;
    field_84 = 0;
}
#pragma control sched=2
