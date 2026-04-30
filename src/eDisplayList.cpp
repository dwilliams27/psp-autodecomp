class cMemAllocator;

class cMemPool {
public:
    cMemPool(cMemAllocator *, const char *);
};

class cFastMemAllocator {
public:
    cFastMemAllocator(const char *, unsigned int, unsigned int, void *);
};

extern char LS_4504_rindex__0036CF48[];
extern char eDisplayListvirtualtable[];

extern "C" void cMemPool_ctor(cMemPool *, cMemAllocator *, const char *)
    asm("__0oIcMemPoolctP6NcMemAllocatorPCc");
extern "C" void cFastMemAllocator_ctor(cFastMemAllocator *, const char *,
                                       unsigned int, unsigned int, void *)
    asm("__0oRcFastMemAllocatorctPCcUiTCPv");

class eDisplayList {
public:
    char field_0[0x20];

    eDisplayList(void);
};

eDisplayList::eDisplayList(void) {
    eDisplayList *self = this;
    *(int *)((char *)this + 0x08) = 0;
    *(int *)((char *)this + 0x0C) = 0;
    *(int *)((char *)this + 0x10) = 0;
    *(int *)((char *)this + 0x14) = 0;
    *(int *)((char *)this + 0x18) = 0;
    *(unsigned char *)((char *)this + 0x1C) = 0;
    cFastMemAllocator *allocator = (cFastMemAllocator *)((char *)this + 0xBC);
    cMemPool_ctor((cMemPool *)((char *)self + 0x20),
                  (cMemAllocator *)allocator, LS_4504_rindex__0036CF48);
    *(void **)((char *)this + 0xB8) = eDisplayListvirtualtable;
    cFastMemAllocator_ctor(allocator, (const char *)((char *)this + 0x24), 0,
                           1, 0);
    *(int *)((char *)this + 0x04) = 0;
    *(int *)((char *)this + 0x00) = 0;
}

extern "C" void *__vec_new(void *, int, int, void (*)(void *));

class eDynamicMeshObjectVisData {
public:
    char field_0[0x88];

    eDynamicMeshObjectVisData(void);
};

class eDynamicMeshVisData : public eDynamicMeshObjectVisData {
public:
    char field_88[0x80];
    int field_108;
    int field_10C;
    int field_110;
    int field_114;
    char field_118[0x80];

    eDynamicMeshVisData(void);
};

#pragma control sched=1
eDynamicMeshVisData::eDynamicMeshVisData(void) {
    void *skinArray = (char *)this + 0x88;
    void (*skinCtor)(void *) = (void (*)(void *))0x21DB70;
    __asm__ volatile("" : "+r"(skinCtor));
    __vec_new(skinArray, 0x20, 4, skinCtor);
    field_108 = 0;
    field_10C = 0;
    field_110 = 0;
    field_114 = 0;
    void *morphArray = (unsigned char *)this + 0x118;
    void (*morphCtor)(void *) = (void (*)(void *))0x21DB64;
    __asm__ volatile("" : "+r"(morphCtor));
    __vec_new(morphArray, 0x20, 4, morphCtor);
}
#pragma control sched=2
