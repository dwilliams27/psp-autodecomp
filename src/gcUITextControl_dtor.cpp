// gcUITextControl_dtor.cpp - split TU for the destructor.
// ODR-WARNING: this TU re-declares gcUITextControl locally with only the
// destructor + operator delete. Do not promote this into a shared header
// without checking matched siblings in src/gcUITextControl.cpp.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcUITextControl {
public:
    static void operator delete(void *);
    ~gcUITextControl(void);
};

extern "C" void gcUIControl_dtor(void *, int) asm("__0oLgcUIControldtv");

inline void pool_delete_no_guard(void *ptr) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DeleteRecord *rec = (DeleteRecord *)(entries + 0x30);
    rec->fn((char *)block + rec->offset, ptr);
}

inline void gcUITextControl::operator delete(void *ptr) {
    if (ptr != 0) {
        pool_delete_no_guard(ptr);
    }
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oPgcUITextControldtv, 0x35c\n");

gcUITextControl::~gcUITextControl(void) {
    char *pDC = (char *)this + 0xDC;
    char *pD8 = (char *)this + 0xD8;
    char *pD4 = (char *)this + 0xD4;
    char *pD0 = (char *)this + 0xD0;

    *(void **)((char *)this + 4) = (void *)0x388E90;

    if ((void *)pDC != 0) {
        char *data = *(char **)pDC;
        int count = 0;
        if (data != 0) {
            count = *(int *)(data - 4) & 0x3FFFFFFF;
        }

        int i = 0;
        if (i < count) {
            do {
                i += 1;
            } while (i < count);
        }

        if (data != 0) {
            char *header = data - 4;
            if (header != 0) {
                pool_delete_no_guard(header);
            }
            *(int *)pDC = 0;
        }
    }

    if ((void *)pD8 != 0) {
        char *data = *(char **)((char *)this + 0xD8);
        int count = 0;
        if (data != 0) {
            count = *(int *)(data - 4) & 0x3FFFFFFF;
        }

        int i = 0;
        if (i < count) {
            int offset = 0;
            const int mask = 0x3FFFFFFF;
            do {
                char **slot = (char **)(data + offset);
                if ((void *)slot != 0) {
                    char *inner = *slot;
                    int innerCount = 0;
                    if (inner != 0) {
                        innerCount = *(int *)(inner - 4) & mask;
                    }

                    int j = 0;
                    if (j < innerCount) {
                        do {
                            j += 1;
                        } while (j < innerCount);
                    }

                    if (inner != 0) {
                        char *header = inner - 4;
                        if (header != 0) {
                            pool_delete_no_guard(header);
                        }
                        *slot = 0;
                    }
                }
                i += 1;
                offset += 4;
            } while (i < count);
        }

        if (data != 0) {
            char *header = data - 4;
            if (header != 0) {
                pool_delete_no_guard(header);
            }
            *(int *)((char *)this + 0xD8) = 0;
        }
    }

    if ((void *)pD4 != 0) {
        char *data = *(char **)((char *)this + 0xD4);
        int count = 0;
        if (data != 0) {
            count = *(int *)(data - 4) & 0x3FFFFFFF;
        }

        int i = 0;
        if (i < count) {
            do {
                i += 1;
            } while (i < count);
        }

        if (data != 0) {
            char *header = data - 4;
            if (header != 0) {
                pool_delete_no_guard(header);
            }
            *(int *)((char *)this + 0xD4) = 0;
        }
    }

    if ((void *)pD0 != 0) {
        char *data = *(char **)((char *)this + 0xD0);
        int count = 0;
        if (data != 0) {
            count = *(int *)(data - 4) & 0x3FFFFFFF;
        }

        int i = 0;
        if (i < count) {
            do {
                i += 1;
            } while (i < count);
        }

        if (data != 0) {
            char *header = data - 4;
            if (header != 0) {
                pool_delete_no_guard(header);
            }
            *(int *)((char *)this + 0xD0) = 0;
        }
    }
    gcUIControl_dtor(this, 0);
}
