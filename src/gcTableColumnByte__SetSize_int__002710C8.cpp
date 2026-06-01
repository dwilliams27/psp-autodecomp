// gcTableColumnByte::SetSize(int)
// 0x002710c8, 384B, gcAll_psp.obj
// Byte-element analogue of cBaseArray::SetSize (pointer array). The element
// type has no destructor, so the per-element "free" loop degenerates into an
// empty counting loop, and Allocate is inlined as a pool dispatch.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

typedef void *(*cAllocFn)(void *, int, int, int, int);

struct cAllocEntry {
    short offset;
    short _pad;
    cAllocFn func;
};

typedef void (*cFreeFn)(void *, void *);

struct cFreeEntry {
    short offset;
    short _pad;
    cFreeFn func;
};

struct cPoolBlock {
    char _pad[0x1c];
    char *allocTable;
};

class gcTableColumnByte {
public:
    void *mOwner;
    void *mClassDesc;
    unsigned char *mData;

    void SetSize(int size);
};

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0fRgcTableColumnByteHSetSizei, 0x180\n");

void gcTableColumnByte::SetSize(int size) {
    int count = 0;
    if (mData != 0) {
        count = ((int *)mData)[-1] & 0x3FFFFFFF;
    }

    if (count != size) {
        unsigned char *newData;
        if (size <= 0) {
            newData = 0;
        } else {
            cMemPool *pool = cMemPool::GetPoolFromPtr(&mData);
            char *obj = ((char **)pool)[9];
            cAllocEntry *entry = (cAllocEntry *)(((cPoolBlock *)obj)->allocTable + 0x28);
            cAllocFn fn = entry->func;
            short adjust = entry->offset;
            int *raw = (int *)fn(obj + adjust, size + 4, 1, 0x36DA98, 0x112);
            *raw = size;
            newData = (unsigned char *)raw + 4;
        }

        int copyIndex = 0;
        if (newData != 0) {
            goto copy;
        }
        if (size > 0) {
            goto done;
        }
        __asm__ volatile("" ::: "memory");
        copyIndex = 0;
copy:
        while (copyIndex < size) {
            unsigned char *dst = newData + copyIndex;
            if (dst != 0) {
                unsigned char value = 0;
                if (copyIndex < count) {
                    value = mData[copyIndex];
                }
                *dst = value;
            }
            copyIndex += 1;
        }

        unsigned char *cur = mData;
        int oldCount = 0;
        if (cur != 0) {
            oldCount = ((int *)cur)[-1] & 0x3FFFFFFF;
        }

        int k = 0;
        while (k < oldCount) {
            k += 1;
        }

        if (cur != 0) {
            unsigned char *oldData = cur - 4;
            if (oldData != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(oldData);
                char *obj = ((char **)pool)[9];
                cFreeEntry *entry = (cFreeEntry *)(((cPoolBlock *)obj)->allocTable + 0x30);
                cFreeFn fn = entry->func;
                short adjust = entry->offset;
                fn(obj + adjust, oldData);
            }
            mData = 0;
        }
        mData = newData;
    }
done:
    ;
}
