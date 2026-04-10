class cMemPool {
public:
    static void *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void free(void *);

struct eDynamicMeshMorphVert;

class eDynamicMeshMorphTarget {
public:
    int mCount;
    float mWeight;
    eDynamicMeshMorphVert *mVerts;
};

extern "C" {

void eDynamicMeshMorphTarget___dtor_eDynamicMeshMorphTarget_void(eDynamicMeshMorphTarget *self, int flags) {
    if (self != 0) {
        if (self->mVerts != 0) {
            void *pool = cMemPool::GetPoolFromPtr(self->mVerts);
            void *block = *(void **)((char *)pool + 0x24);
            __asm__ volatile("" ::: "memory");
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self->mVerts);
            self->mVerts = 0;
        }
        if (flags & 1) {
            void *pool = cMemPool::GetPoolFromPtr(self);
            if (pool != 0) {
                void *block = *(void **)((char *)pool + 0x24);
                __asm__ volatile("" ::: "memory");
                DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
                short off = rec->offset;
                rec->fn((char *)block + off, self);
            } else {
                free(self);
            }
        }
    }
}

}
