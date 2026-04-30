// cArray<eHeightmapTile>::Read(cReadBlock &) — eAll_psp.obj

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cReadBlock {
public:
    int _data[5];
};

class cFileSystem {
public:
    static void Read(void *, void *, unsigned int);
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

class cHandle {
public:
    int mIndex;
};

class eHeightmapTile {
public:
    float mF0;
    float mF4;
    cHandle mH8;
    cHandle mHC;
    unsigned int mU10;

    void Read(cReadBlock &);
};

template <class T>
class cArray {
public:
    T *mData;
    void Read(cReadBlock &);
};

template <class T>
void cArray<T>::Read(cReadBlock &rb) {
    T zero;
    int storedCount;
    cFileSystem::Read(*(void **)rb._data[0], &storedCount, 4);

    zero.mF0 = 0.0f;
    int count = storedCount;
    zero.mF4 = 0.0f;
    zero.mH8.mIndex = 0;
    zero.mHC.mIndex = 0;
    zero.mU10 = 0;

    int oldCount = 0;
    T *newData = mData;
    if (newData != 0) {
        oldCount = ((int *)newData)[-1] & 0x3FFFFFFF;
    }

    if (oldCount != count) {
        if (count > 0) {
            cMemPool *pool = cMemPool::GetPoolFromPtr(this);
            void *block = ((void **)pool)[9];
            AllocEntry *entry = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
            short off = entry->offset;
            void *base = (char *)block + off;
            void *mem = entry->fn(base, count * sizeof(T) + 4, 4, 0x36CF0C, 0x112);
            *(int *)mem = count;
            newData = (T *)((char *)mem + 4);
        } else {
            newData = 0;
        }

        int i = 0;
        if (newData == 0 && count > 0) {
            count = storedCount;
            newData = mData;
        } else {
            int offset = 0;
            if (i < count) {
                T *dst = newData;
                do {
                    if (dst != 0) {
                        T *src = &zero;
                        if (i < oldCount) {
                            src = (T *)((char *)mData + offset);
                        }
                        dst->mF0 = src->mF0;
                        dst->mF4 = src->mF4;
                        int *src8 = &src->mH8.mIndex;
                        int *dst8 = &dst->mH8.mIndex;
                        *dst8 = *src8;
                        int *srcC = &src->mHC.mIndex;
                        int *dstC = &dst->mHC.mIndex;
                        *dstC = *srcC;
                        dst->mU10 = src->mU10;
                    }
                    i++;
                    offset += 20;
                    dst++;
                } while (i < count);
            }

            T *oldData = mData;
            int oldLen = 0;
            if (oldData != 0) {
                oldLen = ((int *)oldData)[-1] & 0x3FFFFFFF;
            }

            int n = 0;
            if (n < oldLen) {
                do {
                    n++;
                } while (n < oldLen);
            }

            if (oldData != 0) {
                void *oldMem = (char *)oldData - 4;
                if (oldMem != 0) {
                    cMemPool *pool = cMemPool::GetPoolFromPtr(oldMem);
                    void *block = ((void **)pool)[9];
                    AllocEntry *entry = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x30);
                    short off = entry->offset;
                    void *base = (char *)block + off;
                    void (*fn)(void *, void *) = (void (*)(void *, void *))entry->fn;
                    fn(base, oldMem);
                    mData = 0;
                } else {
                    mData = 0;
                }
            }
            mData = newData;
            count = storedCount;
        }
    }

    int readIndex = 0;
    if (readIndex < count) {
        T *tile = newData;
        do {
            tile->Read(rb);
            readIndex++;
            tile++;
        } while (readIndex < count);
    }
}


template class cArray<eHeightmapTile>;
