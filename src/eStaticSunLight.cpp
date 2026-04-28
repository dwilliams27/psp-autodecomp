// Functions:
//   0x0005f9f4 eStaticSunLight::Write(cFile &) const                eAll_psp.obj
//   0x0005fbb4 eStaticSunLight::~eStaticSunLight(void)              eAll_psp.obj

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(float);
    void End(void);
};

class cMemPool_helper {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cHandle {
public:
    int mIndex;
    void Write(cWriteBlock &) const;
};

// ============================================================
// eStaticSunLight class declaration
// ============================================================

class eStaticLight {
public:
    eStaticLight(cBase *);
    ~eStaticLight();
    void Write(cFile &) const;
};

class eStaticSunLight : public eStaticLight {
public:
    ~eStaticSunLight();
    void Write(cFile &) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool_helper::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern char eStaticSunLightvirtualtable[];

// ============================================================
// 0x0005f9f4 — eStaticSunLight::Write(cFile &) const
// 0x0005fbb4 — eStaticSunLight::~eStaticSunLight(void)
// ============================================================

#pragma control sched=1
void eStaticSunLight::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    eStaticLight::Write(file);
    wb.Write(*(float *)((char *)this + 0x90));
    wb.Write(*(float *)((char *)this + 0x94));
    ((const cHandle *)((char *)this + 0x98))->Write(wb);
    wb.End();
}

eStaticSunLight::~eStaticSunLight() {
    *(void **)((char *)this + 4) = eStaticSunLightvirtualtable;
}
#pragma control sched=2
