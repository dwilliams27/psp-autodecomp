class cMemAllocator;

class cMemPool {
public:
    cMemPool(cMemAllocator *, const char *);
    static cMemPool *GetPoolFromPtr(const void *);
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

extern "C" void cMemPool_dtor(void *, int) asm("__0oIcMemPooldtv");
extern "C" void cFastMemAllocator_dtor(void *, int)
    asm("__0oRcFastMemAllocatordtv");
extern "C" void free(void *);

struct DelRec_eDisplayList {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class eDisplayList {
public:
    char field_0[0x20];

    eDisplayList(void);
    ~eDisplayList(void);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            DelRec_eDisplayList *rec =
                (DelRec_eDisplayList *)(*(char **)((char *)block + 0x1C) +
                                        0x30);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn((char *)block + off, p);
        } else {
            free(p);
        }
    }
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

eDisplayList::~eDisplayList(void) {
    void *cMemPoolSub = (char *)this + 0x20;
    if (cMemPoolSub != 0) {
        *(void **)((char *)this + 0xB8) = eDisplayListvirtualtable;
        cFastMemAllocator_dtor((char *)this + 0xBC, 2);
        cMemPool_dtor(cMemPoolSub, 0);
    }
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
