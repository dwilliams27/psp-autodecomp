// cArrayBase<cHandleT<gcEnumeration>>::operator=(const cArrayBase<cHandleT<gcEnumeration>>&)
// @ 0x00268344, gcAll_psp.obj

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcEnumeration;

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cArrayBase {
public:
    T *mData;
    cArrayBase<T> &operator=(const cArrayBase<T> &);
};

template <class T>
cArrayBase<T> &cArrayBase<T>::operator=(const cArrayBase<T> &other) {
    int otherCap = 0;
    if (other.mData != 0) {
        otherCap = ((int *)other.mData)[-1] & 0x3FFFFFFF;
    }
    int thisCap = 0;
    if (mData != 0) {
        thisCap = ((int *)mData)[-1] & 0x3FFFFFFF;
    }

    if (otherCap != thisCap) {
        int destroyCount = 0;
        __asm__ volatile("" ::: "memory");
        if (mData != 0) {
            destroyCount = ((int *)mData)[-1] & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < destroyCount) {
            do {
                i++;
            } while (i < destroyCount);
        }

        T *origData = mData;
        if (origData != 0) {
            int *alloc = (int *)origData - 1;
            if (alloc != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(alloc);
                char *block = ((char **)pool)[9];
                DtorDeleteRecord *rec =
                    (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
                short off = rec->offset;
                void (*fn)(void *, void *) = rec->fn;
                fn(block + off, alloc);
            }
            mData = 0;
        }

        int newCap = 0;
        if (other.mData != 0) {
            newCap = ((int *)other.mData)[-1] & 0x3FFFFFFF;
        }
        int allocCap = newCap;
        T *newData = 0;
        if (allocCap > 0) {
            cMemPool *pool = cMemPool::GetPoolFromPtr(this);
            char *block = ((char **)pool)[9];
            AllocEntry *entry =
                (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
            int *raw = (int *)entry->fn(block + entry->offset,
                                        allocCap * 4 + 4, 4,
                                        0x36DA98, 0x112);
            *raw = allocCap;
            newData = (T *)(raw + 1);
        }
        mData = newData;

        int j = 0;
        int off2 = 0;
        while (true) {
            int cap = 0;
            if (other.mData != 0) {
                cap = ((int *)other.mData)[-1] & 0x3FFFFFFF;
            }
            if (j >= cap) break;
            int *dst = (int *)((char *)mData + off2);
            if (dst != 0) {
                *dst = *(int *)((char *)other.mData + off2);
            }
            j++;
            off2 += 4;
        }
    } else {
        int j = 0;
        int off2 = 0;
        while (true) {
            int cap = 0;
            if (other.mData != 0) {
                cap = ((int *)other.mData)[-1] & 0x3FFFFFFF;
            }
            if (j >= cap) break;
            *(int *)((char *)mData + off2) =
                *(int *)((char *)other.mData + off2);
            j++;
            off2 += 4;
        }
    }
    return *this;
}

template cArrayBase<cHandleT<gcEnumeration> > &
cArrayBase<cHandleT<gcEnumeration> >::operator=(
    const cArrayBase<cHandleT<gcEnumeration> > &);
