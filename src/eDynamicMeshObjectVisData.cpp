class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct eDynamicMeshObjectVisData_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern "C" void free(void *);
void vec_delete6(void *, int, int, void (*)(void *), int, int) asm("__vec_delete");

#pragma control sched=1
class eDynamicMeshObjectVisData {
public:
    char field_0[0x80];
    int field_80;
    int field_84;

    eDynamicMeshObjectVisData(void);
    ~eDynamicMeshObjectVisData(void);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            register volatile eDynamicMeshObjectVisData_DeleteRecord *rec __asm__("a1") =
                (volatile eDynamicMeshObjectVisData_DeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            void *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        } else {
            free(p);
        }
    }
};

eDynamicMeshObjectVisData::~eDynamicMeshObjectVisData(void) {
    void *displayList = (void *)field_84;
    if (displayList != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(displayList);
        char *block = ((char **)pool)[9];
        register volatile eDynamicMeshObjectVisData_DeleteRecord *rec __asm__("a1") =
            (volatile eDynamicMeshObjectVisData_DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void *base = block + off;
        void (*fn)(void *, void *, short, void *) =
            (void (*)(void *, void *, short, void *))rec->fn;
        fn(base, *(void **)((char *)this + 0x84), off, (void *)fn);
        field_84 = 0;
    }
    void (*dtor)(void *) = (void (*)(void *))0x21DA38;
    __asm__ volatile("" : "+r"(dtor));
    vec_delete6(this, 0x20, 4, dtor, 0, 0);
}
#pragma control sched=2
