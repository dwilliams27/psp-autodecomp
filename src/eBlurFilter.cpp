// Functions:
//   0x0007fc10 eBlurFilter::Write(cFile &) const               eAll_psp.obj
//   0x0007fdcc eBlurFilter::~eBlurFilter(void)                  eAll_psp.obj

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned int);
    void Write(float);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class eTextureFilter {
public:
    eTextureFilter(cBase *);
    ~eTextureFilter();
    void Write(cFile &) const;
};

class eBlurFilter : public eTextureFilter {
public:
    eBlurFilter(cBase *);
    ~eBlurFilter();
    void Write(cFile &) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern char eBlurFiltervirtualtable[];

#pragma control sched=1
void eBlurFilter::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    eTextureFilter::Write(file);
    wb.Write(*(float *)((char *)this + 0x10));
    wb.Write(*(float *)((char *)this + 0x14));
    wb.Write(*(unsigned int *)((char *)this + 0x18));
    wb.End();
}

eBlurFilter::~eBlurFilter() {
    *(void **)((char *)this + 4) = eBlurFiltervirtualtable;
}
#pragma control sched=2
