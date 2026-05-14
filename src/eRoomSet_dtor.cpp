// eRoomSet_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares eRoomSet locally with only the destructor;
// the rest of the class lives in src/eRoomSet.cpp.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" void cObject_dtor(void *, int) asm("__0oHcObjectdtv");

inline void operator delete(void *p) {
    if (p != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
}

class eRoomSet {
public:
    ~eRoomSet();
};

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oIeRoomSetdtv, 0x240\n");

eRoomSet::~eRoomSet() {
    *(void **)((char *)this + 4) = (void *)0x380A08;

    void *arr4CAddr = (char *)this + 0x4C;
    char *arr44Addr = (char *)this + 0x44;
    if (arr4CAddr != 0) {
        void *entries = *(void **)((char *)this + 0x4C);
        int count = 0;
        if (entries != 0) {
            count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (entries != 0) {
            char *basePtr = (char *)entries - 4;
            if (basePtr != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                char *block = ((char **)pool)[9];
                DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
                short off = rec->offset;
                char *base = block + off;
                void (*fn)(void *, void *) = rec->fn;
                fn(base, basePtr);
            }
            *(void **)((char *)this + 0x4C) = 0;
        }
    }

    if ((void *)arr44Addr != 0) {
        void *arr48Addr = (char *)this + 0x48;
        if (arr48Addr != 0) {
            void *entries = *(void **)((char *)this + 0x48);
            int count = 0;
            if (entries != 0) {
                count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
            }
            int i = 0;
            if (i < count) {
                do {
                    i++;
                } while (i < count);
            }
            if (entries != 0) {
                char *basePtr = (char *)entries - 4;
                if (basePtr != 0) {
                    cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                    char *block = ((char **)pool)[9];
                    DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
                    short off = rec->offset;
                    char *base = block + off;
                    void (*fn)(void *, void *) = rec->fn;
                    fn(base, basePtr);
                }
                *(void **)((char *)this + 0x48) = 0;
            }
        }

        if ((void *)arr44Addr != 0) {
            void *entries = *(void **)((char *)this + 0x44);
            int count = 0;
            if (entries != 0) {
                count = *(int *)((char *)entries - 4) & 0x3FFFFFFF;
            }
            int i = 0;
            if (i < count) {
                do {
                    i++;
                } while (i < count);
            }
            if (entries != 0) {
                char *basePtr = (char *)entries -
                                (((unsigned int)*(int *)((char *)entries - 4) >> 30) * 4) -
                                4;
                if (basePtr != 0) {
                    cMemPool *pool = cMemPool::GetPoolFromPtr(basePtr);
                    char *block = ((char **)pool)[9];
                    DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
                    short off = rec->offset;
                    char *base = block + off;
                    void (*fn)(void *, void *) = rec->fn;
                    fn(base, basePtr);
                }
                *(void **)((char *)this + 0x44) = 0;
            }
        }
    }

    cObject_dtor(this, 0);
}
