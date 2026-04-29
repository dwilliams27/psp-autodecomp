// eFrameBufferTexture.cpp — decompiled from eAll_psp.obj
// Parent: eVirtualTexture. Methods Write/dtor are in the sched=1 zone.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void End(void);
};

class cMemPool_shim {
public:
    static void *GetPoolFromPtr(const void *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

inline void *operator new(unsigned int, void *p) { return p; }

class eVirtualTexture {
public:
    eVirtualTexture(cBase *);
    ~eVirtualTexture(void);
    void Write(cFile &) const;
};

class cObject {
public:
    cObject &operator=(const cObject &);
};

struct cHandleVal {
    int mIndex;
};

template <class T> T *dcast(const cBase *);

class eFrameBufferTexture : public eVirtualTexture {
public:
    eFrameBufferTexture(cBase *);
    ~eFrameBufferTexture(void);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        void *pool = cMemPool_shim::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern char eFrameBufferTextureclassdesc[];

// ── eFrameBufferTexture::Write(cFile &) const @ 0x00081588 ──
#pragma control sched=1
void eFrameBufferTexture::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    eVirtualTexture::Write(file);
    wb.Write(*(bool *)((char *)this + 0x50));
    wb.End();
}

// ── eFrameBufferTexture::~eFrameBufferTexture(void) @ 0x00081754 ──
eFrameBufferTexture::~eFrameBufferTexture(void) {
    *(void **)((char *)this + 4) = eFrameBufferTextureclassdesc;
}

// ── eFrameBufferTexture::AssignCopy(const cBase *) @ 0x0021842c ──
void eFrameBufferTexture::AssignCopy(const cBase *base) {
    eFrameBufferTexture *other = dcast<eFrameBufferTexture>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    ((signed char *)this)[0x44] = ((const signed char *)other)[0x44];
    ((signed char *)this)[0x45] = ((const signed char *)other)[0x45];
    ((signed char *)this)[0x46] = ((const signed char *)other)[0x46];
    ((unsigned char *)this)[0x47] = ((const unsigned char *)other)[0x47];
    ((short *)this)[0x48 / 2] = ((const short *)other)[0x48 / 2];
    ((short *)this)[0x4A / 2] = ((const short *)other)[0x4A / 2];
    __asm__ volatile("" ::: "memory");
    *(cHandleVal *)((char *)this + 0x4C) =
        *(cHandleVal *)((char *)other + 0x4C);
    ((unsigned char *)this)[0x50] = ((const unsigned char *)other)[0x50];
}

// ── eFrameBufferTexture::New(cMemPool *, cBase *) static @ 0x002184b4 ──
cBase *eFrameBufferTexture::New(cMemPool *pool, cBase *parent) {
    eFrameBufferTexture *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eFrameBufferTexture *obj = (eFrameBufferTexture *)rec->fn(base, 0x54, 4, 0, 0);
    if (obj != 0) {
        new (obj) eFrameBufferTexture(parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2
