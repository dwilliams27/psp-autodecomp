// gcMap::~gcMap(void) @ 0x000f1180
//
// Split TU per AGENTS.md SPLIT-TU rule. The main gcMap definition lives in
// include/gcMap.h / src/gcMap.cpp, but that header does not declare the dtor.
//
// ODR-WARNING: local redeclarations expose only the destructor, delete path,
// base methods, and field offsets touched here.

class cBaseArray {
public:
    void RemoveAll(void);
};

class cFactory {
public:
    void DeleteGroups(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class eWorld {
public:
    ~eWorld(void);
};

class gcEvent {
public:
    ~gcEvent(void);
};

class gcRegionBase {
public:
    ~gcRegionBase(void);
};

extern "C" void __vec_delete(void *, int, unsigned int, void (*)(void *), int, int);
extern "C" void __0oMgcRegionBasedtv(gcRegionBase *, int);

class gcMap : public cFactory {
public:
    static void operator delete(void *);
    void UnloadAllRegions(void);
    ~gcMap(void);
};

extern char cObjectvirtualtable[];
extern int D_0037D7FC;
extern "C" void gcRegionBase__gcRegionBase(void *);

inline void gcMap::operator delete(void *ptr) {
    if (ptr != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *slot = (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
        slot->fn(block + slot->offset, ptr);
    }
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oFgcMapdtv, 0x2d8\n");

gcMap::~gcMap(void) {
    *(void **)((char *)this + 4) = (void *)0x387D20;

    UnloadAllRegions();
    cFactory::DeleteGroups();

    void *firstOwned = *(void **)((char *)this + 0x1F4);
    char *sub = (char *)this + 0x3EC;
    char *array = (char *)this + 0x398;
    char *slot204 = (char *)this + 0x204;
    char *slot200 = (char *)this + 0x200;

    if (firstOwned != 0) {
        char *typeInfo = *(char **)((char *)firstOwned + 4);
        DtorDeleteRecord *rec = (DtorDeleteRecord *)(typeInfo + 0x50);
        rec->fn((char *)firstOwned + rec->offset, (void *)3);
        *(int *)((char *)this + 0x1F4) = 0;
    }

    void *secondOwned = *(void **)((char *)this + 0x1FC);
    if (secondOwned != 0) {
        char *typeInfo = *(char **)((char *)secondOwned + 4);
        DtorDeleteRecord *rec = (DtorDeleteRecord *)(typeInfo + 0x50);
        rec->fn((char *)secondOwned + rec->offset, (void *)3);
        *(int *)((char *)this + 0x1FC) = 0;
    }

    eWorld *world = *(eWorld **)((char *)this + 0x50);
    if (world != 0) {
        delete world;
        *(int *)((char *)this + 0x50) = 0;
    }

    *(int *)0x37D7FC = 0;

    if ((void *)sub != 0) {
        *(void **)((char *)this + 0x3F0) = (void *)0x37E6A8;
    }

    if ((void *)array != 0) {
        ((cBaseArray *)array)->RemoveAll();
    }

    __vec_delete((char *)this + 0x218, 2, 0xC0, (void (*)(void *))0x1C766C, 0, 0);

    if ((void *)slot204 != 0) {
        char *data = *(char **)((char *)this + 0x204);
        int count = 0;
        if (data != 0) {
            count = ((int *)data)[-1] & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }

        if (data != 0) {
            data -= 4;
            if (data != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(data);
                char *block = ((char **)pool)[9];
                DtorDeleteRecord *rec =
                    (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
                rec->fn(block + rec->offset, data);
            }
            *(int *)((char *)this + 0x204) = 0;
        }
    }

    if ((void *)slot200 != 0) {
        char *data = *(char **)((char *)this + 0x200);
        int count = 0;
        if (data != 0) {
            count = ((int *)data)[-1] & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }

        if (data != 0) {
            data -= 4;
            if (data != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(data);
                char *block = ((char **)pool)[9];
                DtorDeleteRecord *rec =
                    (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
                rec->fn(block + rec->offset, data);
            }
            *(int *)((char *)this + 0x200) = 0;
        }
    }

    ((gcEvent *)((char *)this + 0x120))->~gcEvent();
    ((gcEvent *)((char *)this + 0x104))->~gcEvent();
    ((gcEvent *)((char *)this + 0x0E8))->~gcEvent();
    ((gcEvent *)((char *)this + 0x0CC))->~gcEvent();
    ((gcEvent *)((char *)this + 0x0B0))->~gcEvent();
    ((gcEvent *)((char *)this + 0x094))->~gcEvent();
    ((gcEvent *)((char *)this + 0x078))->~gcEvent();
    __0oMgcRegionBasedtv((gcRegionBase *)this, 0);
}
