typedef char *va_list;

extern "C" int vsnprintf(char *, unsigned int, const char *, va_list);
extern "C" int sceKernelCreateMutex(const char *, int, int, int);

class cFilename {
public:
    char mName[0x100];

    cFilename(const char *, ...);
};

class cMemAllocator {
public:
    void *mHead;
    void *mTail;
    void *mBlock;
    unsigned int mSize;
    const char *mName;
    float mScale;
    int mMutex;
    void *mVTable;

    cMemAllocator(const char *, unsigned int, float);
};

cFilename::cFilename(const char *fmt, ...) {
    register va_list args asm("a3");

    mName[0] = 0;
    if (fmt == 0) {
        mName[0] = 0;
    } else {
        __asm__ volatile("lui $4, 0x0");
        __asm__ volatile("addiu $4, $4, 0x10");
        __asm__ volatile("andi $4, $4, 0xffff");
        __asm__ volatile("addu $7, $sp, $4");
        __asm__ volatile("addiu %0, $7, 0x8" : "=r"(args));
        vsnprintf(mName, 0x100, fmt, args);
        mName[0xff] = 0;
    }
}

cMemAllocator::cMemAllocator(const char *name, unsigned int size, float scale) {
    mVTable = (void *)0x37e590;
    mHead = 0;
    mTail = 0;
    mBlock = 0;
    mSize = size;
    mName = name;
    mScale = scale;
    mMutex = sceKernelCreateMutex((const char *)0x36c87c, 0, 0, 0);
}
