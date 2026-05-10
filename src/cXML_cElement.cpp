// cXML_cElement.cpp - destructor for cXML::cElement.
// ODR-WARNING: this TU re-declares cXML::cElement and cXML::cNode locally
// with just enough members + destructor + operator delete to drive codegen.
// Keep the layout assumptions in sync with src/cXML.cpp and
// src/cXML_cNode.cpp.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" void free(void *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct DeleteEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct DispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

class cXML {
public:
    class cNode {
    public:
        int _pad0;
        int _pad4;
        char *mText;
        int _padC;
        int _pad10;
        void *mVtable;

        ~cNode();
    };

    class cElement : public cNode {
    public:
        void *mList18;
        void *mList1C;
        void **mChildren;

        ~cElement(void);
    };
};

inline void operator delete(void *p) {
    if (p != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            DeleteEntry *entry =
                (DeleteEntry *)(((PoolBlock *)block)->allocTable + 0x30);
            entry->fn(block + entry->offset, p);
        } else {
            free(p);
        }
    }
}

__asm__(".word 0x1000ffff\n"
        ".word 0x00000000\n"
        ".size __0o5EcXMLIcElementdtv, 0x1BC\n");

cXML::cElement::~cElement(void) {
    mVtable = (void *)0x37EB20;

    // Walk list at this+0x1C
    void *cur = mList1C;
    if (cur != 0) {
        do {
            void *next = *(void **)((char *)cur + 0xC);
            if (cur != 0) {
                void *vt = *(void **)((char *)cur + 0x14);
                DispatchEntry *entry = (DispatchEntry *)((char *)vt + 8);
                entry->fn((char *)cur + entry->offset, 3);
            }
            cur = next;
        } while (cur != mList1C);
    }

    // Walk list at this+0x18
    void *cur2 = mList18;
    if (cur2 != 0) {
        do {
            void *next = *(void **)((char *)cur2 + 0xC);
            if (cur2 != 0) {
                void *vt = *(void **)((char *)cur2 + 0x14);
                DispatchEntry *entry = (DispatchEntry *)((char *)vt + 8);
                entry->fn((char *)cur2 + entry->offset, 3);
            }
            cur2 = next;
        } while (cur2 != mList18);
    }

    // Children array at this+0x20
    if ((char *)&mChildren != 0) {
        void **arr = mChildren;
        int count = 0;
        if (arr != 0) {
            count = ((int *)arr)[-1] & 0x3FFFFFFF;
        }
        int i = 0;
        if (i < count) {
            do {
                i++;
            } while (i < count);
        }
        if (arr != 0) {
            int *alloc = (int *)arr - 1;
            if (alloc != 0) {
                cMemPool *pool = cMemPool::GetPoolFromPtr(alloc);
                char *block = ((char **)pool)[9];
                DeleteEntry *entry =
                    (DeleteEntry *)(((PoolBlock *)block)->allocTable + 0x30);
                entry->fn(block + entry->offset, alloc);
            }
            mChildren = 0;
        }
    }
}
