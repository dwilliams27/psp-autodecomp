class cBase;

class eVirtualTexture {
public:
    eVirtualTexture(cBase *);
};

class eFilteredTexture : public eVirtualTexture {
public:
    char _pad[0x50 - sizeof(eVirtualTexture)];
    int field_50;
    void *field_54;

    eFilteredTexture(cBase *);
};

#pragma control sched=1
eFilteredTexture::eFilteredTexture(cBase *parent) : eVirtualTexture(parent) {
    *(void **)((char *)this + 4) = (void *)0x3852D0;
    field_50 = 0;
    field_54 = this;
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
