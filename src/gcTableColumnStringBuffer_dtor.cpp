// gcTableColumnStringBuffer_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcTableColumnStringBuffer locally with only
// the destructor + operator delete; the rest of the class lives in
// src/gcTableColumnStringBuffer.cpp (Set/AssignCopy/Compare/Get/New/GetType).
// Adding a destructor declaration to that class would perturb the matched
// siblings' vtable handling.

class cBase;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

class gcTableColumnStringBuffer {
public:
    static void operator delete(void *);
    ~gcTableColumnStringBuffer(void);
};

inline void gcTableColumnStringBuffer::operator delete(void *p) {
    if (p != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DtorDeleteRecord *rec =
            (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
}

__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");
__asm__(".size __0oZgcTableColumnStringBufferdtv, 0x1d0\n");

gcTableColumnStringBuffer::~gcTableColumnStringBuffer(void) {
    *(char **)((char *)this + 4) = (char *)0x389DC0;
    char *slot = (char *)this + 0x0C;
    if (slot != 0) {
        char *data = *(char **)((char *)this + 0x0C);
        int count = 0;
        if (data != 0) {
            count = ((int *)data)[-1] & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            int byteOff = 0;
            do {
                char **elem = (char **)(*(char **)((char *)this + 0x0C) + byteOff);
                if (elem != 0) {
                    char *inner = *elem;
                    int innerCount = 0;
                    if (inner != 0) {
                        innerCount = ((int *)inner)[-1] & 0x3FFFFFFF;
                    }
                    int j = 0;
                    if (j < innerCount) {
                        do {
                            j++;
                        } while (j < innerCount);
                    }
                    if (inner != 0) {
                        inner -= 4;
                        if (inner != 0) {
                            cMemPool *pool = cMemPool::GetPoolFromPtr(inner);
                            char *block = ((char **)pool)[9];
                            DtorDeleteRecord *rec =
                                (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
                            short off = rec->offset;
                            void (*fn)(void *, void *) = rec->fn;
                            fn(block + off, inner);
                        }
                        *(int *)elem = 0;
                    }
                }
                i++;
                byteOff += 4;
            } while (i < count);
        }
        data = *(char **)((char *)this + 0x0C);
        if (data != 0) {
            data -= 4;
            if (data != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(data);
                char *block = ((char **)pool)[9];
                DtorDeleteRecord *rec =
                    (DtorDeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
                short off = rec->offset;
                void (*fn)(void *, void *) = rec->fn;
                fn(block + off, data);
            }
            *(int *)((char *)this + 0x0C) = 0;
        }
    }
    if (this != 0) {
        *(char **)((char *)this + 4) = (char *)0x37E6A8;
    }
}
