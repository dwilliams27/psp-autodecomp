class cMemAllocator;

class cMemPool {
public:
    char mPad[0x98];

    cMemPool(cMemAllocator *, const char *);
};

class cFastMemAllocator {
public:
    cFastMemAllocator(const char *, unsigned int, unsigned int, void *);
};

extern char cStaticMemPoolvirtualtable[];

class cStaticMemPool : public cMemPool {
public:
    void *mVtable;
    cFastMemAllocator mAllocator;

    cStaticMemPool(const char *, unsigned int, unsigned int);
};

cStaticMemPool::cStaticMemPool(const char *name, unsigned int allocSize,
                               unsigned int poolSize)
    : cMemPool((cMemAllocator *)&mAllocator, name),
      mVtable(cStaticMemPoolvirtualtable),
      mAllocator((const char *)((char *)this + 4), allocSize, poolSize, 0) {}
