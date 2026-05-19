// gcUIWidgetGroup_dtor.cpp - split TU for the destructor.
// ODR-WARNING: gcUIWidgetGroup is also defined in src/gcUIWidgetGroup.cpp.
// This TU redeclares only the destructor, DeleteAllWidgets, and operator delete
// so the already-matched methods keep their current codegen.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class gcUIWidgetGroup {
public:
    static void operator delete(void *);
    void DeleteAllWidgets(bool);
    ~gcUIWidgetGroup(void);
};

extern "C" void gcUIWidget_dtor(void *, int) asm("__0oLgcUIWidgetdtv");

inline void pool_delete_no_guard(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    DeleteRec *rec = (DeleteRec *)(entries + 0x30);
    rec->fn((char *)block + rec->offset, p);
}

inline void gcUIWidgetGroup::operator delete(void *p) {
    if (p != 0) {
        pool_delete_no_guard(p);
    }
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0oPgcUIWidgetGroupdtv, 0x274\n");

gcUIWidgetGroup::~gcUIWidgetGroup(void) {
    *(void **)((char *)this + 4) = (void *)0x387058;
    DeleteAllWidgets(false);

    char *pC8 = (char *)this + 0xC8;
    char *pC4 = (char *)this + 0xC4;
    char *pC0 = (char *)this + 0xC0;

    if (*(void **)((char *)this + 0xBC) != 0) {
        cMemPool *pool =
            cMemPool::GetPoolFromPtr(*(void **)((char *)this + 0xBC));
        void *block = *(void **)((char *)pool + 0x24);
        char *entries = *(char **)((char *)block + 0x1C);
        DeleteRec *rec = (DeleteRec *)(entries + 0x30);
        rec->fn((char *)block + rec->offset,
                *(void **)((char *)this + 0xBC));
        *(int *)((char *)this + 0xBC) = 0;
    }

    if ((void *)pC8 != 0) {
        char *data = *(char **)((char *)this + 0xC8);
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
            *(int *)((char *)this + 0xC8) = 0;
        }
    }

    if ((void *)pC4 != 0) {
        char *data = *(char **)((char *)this + 0xC4);
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
            *(int *)((char *)this + 0xC4) = 0;
        }
    }

    if ((void *)pC0 != 0) {
        char *data = *(char **)((char *)this + 0xC0);
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
            *(int *)((char *)this + 0xC0) = 0;
        }
    }

    gcUIWidget_dtor(this, 0);
}
