// ODR-WARNING: split translation unit for eStaticModel::~eStaticModel only.
// The destructor body inlines two levels of cArrayBase teardown plus the
// auto-generated eGeom base destructor call and deleting-destructor trailer.
// Isolate it from src/eStaticModel.cpp to keep the matched siblings safe.
// DO NOT add `#include "eStaticModel.h"`.
#include "eDynamicGeom.h"

struct _StaticModelDelRec {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

extern char eStaticModelvirtualtable[];
extern char eGeomvirtualtable[];

class eStaticModel : public eGeom {
public:
    void PlatformFree(void);
    ~eStaticModel();
    static void operator delete(void *p) {
        if (p != 0) {
            cMemPool *pool = cMemPool::GetPoolFromPtr(p);
            char *block = ((char **)pool)[9];
            _StaticModelDelRec *rec = (_StaticModelDelRec *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            char *base = block + off;
            void (*fn)(void *, void *) = rec->fn;
            fn(base, p);
        }
    }
};

#pragma control sched=2

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oMeStaticModeldtv, 0x1e4\n");

eStaticModel::~eStaticModel() {
    *(void **)((char *)this + 4) = eStaticModelvirtualtable;
    PlatformFree();

    void *arrAddr = (char *)this + 0x94;
    if (arrAddr != 0) {
        void *entries = *(void **)((char *)this + 0x94);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            int byteOff = 0;
            do {
                void **slot = (void **)((char *)entries + byteOff);
                if (slot != 0) {
                    void *innerData = *slot;
                    int innerCount = 0;
                    if (innerData != 0) {
                        innerCount = *(int *)((char *)innerData - 4) & 0x3FFFFFFF;
                    }
                    int j = 0;
                    if (j < innerCount) {
                        do {
                            j++;
                        } while (j < innerCount);
                    }
                    if (innerData != 0) {
                        char *innerBase = (char *)innerData - 4;
                        if (innerBase != 0) {
                            cMemPool *pool = cMemPool::GetPoolFromPtr(innerBase);
                            char *block = ((char **)pool)[9];
                            _StaticModelDelRec *rec = (_StaticModelDelRec *)(((char **)block)[7] + 0x30);
                            short off = rec->offset;
                            char *base = block + off;
                            void (*fn)(void *, void *) = rec->fn;
                            fn(base, innerBase);
                        }
                        *slot = 0;
                    }
                }
                i++;
                byteOff += 4;
                entries = *(void **)((char *)this + 0x94);
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries - 4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                _StaticModelDelRec *rec = (_StaticModelDelRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x94) = 0;
        }
    }

    *(void **)((char *)this + 4) = eGeomvirtualtable;
}
