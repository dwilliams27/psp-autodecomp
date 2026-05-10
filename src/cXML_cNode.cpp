// ODR-WARNING: split translation unit with local class redeclarations.
// Keep declarations minimal to avoid perturbing canonical class headers.

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" void free(void *);
extern char cXML__cNodevirtualtable[];

int cStrLength(const char *);
void cStrCopy(char *, const char *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
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
        int SetText(const char *);
        static void operator delete(void *p) {
            cMemPool *pool = cMemPool::GetPoolFromPtr(p);
            if (pool != 0) {
                char *block = ((char **)pool)[9];
                DeleteEntry *entry = (DeleteEntry *)(((PoolBlock *)block)->allocTable + 0x30);
                entry->fn(block + entry->offset, p);
            } else {
                free(p);
            }
        }
    };
};

cXML::cNode::~cNode() {
    mVtable = cXML__cNodevirtualtable;
    if (mText != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(mText);
        char *block = ((char **)pool)[9];
        DeleteEntry *entry = (DeleteEntry *)(((PoolBlock *)block)->allocTable + 0x30);
        entry->fn(block + entry->offset, mText);
        mText = 0;
    }
}

int cXML::cNode::SetText(const char *text) {
    if (mText != 0) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(mText);
        char *block = ((char **)pool)[9];
        DeleteEntry *entry = (DeleteEntry *)(((PoolBlock *)block)->allocTable + 0x30);
        entry->fn(block + entry->offset, mText);
        mText = 0;
    }

    if (text == 0 || *text == 0) {
        return 1;
    }

    {
        cMemPool *pool = cMemPool::GetPoolFromPtr(this);
        char *block = ((char **)pool)[9];
        AllocEntry *entry = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
        char *newText = (char *)entry->fn(block + entry->offset, cStrLength(text) + 1, 0, 0, 0);
        mText = newText;
        if (newText != 0) {
            cStrCopy(mText, text);
            return 1;
        }
    }

    return 0;
}

struct cGUID {
    int mA;
    int mB;
};

class gcTableColumn {
public:
    void *mOwner;
    void *mClassDesc;
};

class gcTableColumnGUID : public gcTableColumn {
public:
    cGUID *mValues;

    void SetSize(int);
};

void gcTableColumnGUID::SetSize(int size) {
    cGUID blank = {0, 0};
    int count = 0;
    if (mValues != 0) {
        count = ((int *)mValues)[-1] & 0x3FFFFFFF;
    }

    if (count != size) {
        cGUID *newData;
        if (size > 0) {
            goto allocate;
        }
        newData = 0;
        goto after_allocate;

allocate:
        {
            cMemPool *pool = cMemPool::GetPoolFromPtr(&mValues);
            char *block = ((char **)pool)[9];
            int allocSize = size * 8;
            char *table = ((PoolBlock *)block)->allocTable;
            allocSize += 4;
            AllocEntry *entry = (AllocEntry *)(table + 0x28);
            short adjust = entry->offset;
            void *(*fn)(void *, int, int, int, int) = entry->fn;
            int *alloc = (int *)fn(block + adjust, allocSize, 4, 0x36DA98, 0x112);
            alloc[0] = size;
            newData = (cGUID *)(alloc + 1);
        }

after_allocate:
        int copyIndex;
        if (newData == 0) {
            if (size > 0) {
                goto done_resize;
            }
        }
        copyIndex = 0;
        {
            register int shouldCopy asm("a1") = copyIndex < size;
            if (shouldCopy) {
                int offset = 0;
                __asm__ volatile("" : "+r"(offset));
                cGUID *dstIter = (cGUID *)((char *)newData + offset);
                do {
                    cGUID *dst = dstIter;
                    if (dst != 0) {
                        cGUID *src = (copyIndex < count)
                            ? (cGUID *)((char *)mValues + offset)
                            : &blank;
                        dst->mA = src->mA;
                        dst->mB = src->mB;
                    }
                    copyIndex++;
                    offset += 8;
                    dstIter = (cGUID *)((char *)dstIter + 8);
                } while (copyIndex < size);
            }

            register cGUID *oldData asm("s1") = mValues;
            __asm__ volatile("" : "+r"(oldData));
            register int oldCount asm("a0") = 0;
            if (oldData != 0) {
                oldCount = ((int *)oldData)[-1] & 0x3FFFFFFF;
            }

            register int i asm("a1") = 0;
            if (i < oldCount) {
                do {
                    i++;
                } while (i < oldCount);
            }

            int *oldAlloc = (int *)oldData - 1;
            if (oldData != 0) {
                if (oldAlloc != 0) {
                    cMemPool *pool = cMemPool::GetPoolFromPtr(oldAlloc);
                    char *block = ((char **)pool)[9];
                    DeleteEntry *entry = (DeleteEntry *)(((PoolBlock *)block)->allocTable + 0x30);
                    entry->fn(block + entry->offset, oldAlloc);
                }
                mValues = 0;
            }
            mValues = newData;
        }
done_resize:
        ;
    }
}
