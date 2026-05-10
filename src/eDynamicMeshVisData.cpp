// ODR-WARNING: split translation unit for eDynamicMeshVisData destructor only.
// Keep local declarations minimal to avoid changing matched users of the class.

#pragma control sched=1

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct eDynamicMeshVisData_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern "C" void free(void *);
extern "C" void __vec_delete(void *, int, int, void (*)(void *), int, int);

class eDynamicMeshObjectVisData {
public:
    char field_0[0x88];

    ~eDynamicMeshObjectVisData(void);
};

class eDynamicMeshVisData : public eDynamicMeshObjectVisData {
public:
    char skinLists[0x80];
    int field_108;
    void *field_10C;
    int field_110;
    void *field_114;
    char morphLists[0x80];

    ~eDynamicMeshVisData(void);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            eDynamicMeshVisData_DeleteRecord *rec =
                (eDynamicMeshVisData_DeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            char *base = block + off;
            __asm__ volatile("" : "+r"(p));
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        } else {
            free(p);
        }
    }
};

eDynamicMeshVisData::~eDynamicMeshVisData(void) {
    void *first = field_10C;
    if (first != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(first);
        char *block = ((char **)pool)[9];
        register volatile eDynamicMeshVisData_DeleteRecord *rec __asm__("a1") =
            (volatile eDynamicMeshVisData_DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void *base = block + off;
        void (*fn)(void *, void *, short, void *) =
            (void (*)(void *, void *, short, void *))rec->fn;
        fn(base, field_10C, off, (void *)fn);
        field_10C = 0;
    }

    void *second = field_114;
    if (second != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(second);
        char *block = ((char **)pool)[9];
        register volatile eDynamicMeshVisData_DeleteRecord *rec __asm__("a1") =
            (volatile eDynamicMeshVisData_DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void *base = block + off;
        void (*fn)(void *, void *, short, void *) =
            (void (*)(void *, void *, short, void *))rec->fn;
        fn(base, field_114, off, (void *)fn);
        field_114 = 0;
    }

    void *morphArray = morphLists;
    __asm__ volatile("" : "+r"(morphArray));
    void (*morphDtor)(void *) = (void (*)(void *))0x21DCA8;
    __asm__ volatile("" : "+r"(morphDtor));
    __vec_delete(morphArray, 0x20, 4, morphDtor, 0, 0);
    void *skinArray = skinLists;
    __asm__ volatile("" : "+r"(skinArray));
    void (*skinDtor)(void *) = (void (*)(void *))0x21DB7C;
    __asm__ volatile("" : "+r"(skinDtor));
    __vec_delete(skinArray, 0x20, 4, skinDtor, 0, 0);
}

#pragma control sched=2
