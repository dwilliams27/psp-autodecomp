// cConfigBase — runtime config block (paired strings + typed arrays)

class cBase;
class cFile;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(int, const bool *);
    void Write(int, const int *);
    void End(void);
};

class cStr {
public:
    char _data[256];
    void Write(cWriteBlock &) const;
};

extern "C" void cStr_ctor(void *, const char *, ...) __asm__("__0oEcStrctPCce");

struct DeleteRec {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class cConfigBase {
public:
    cConfigBase(cBase *);
    ~cConfigBase();

    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRec *rec = (DeleteRec *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

extern char cListSubscribervirtualtable__0037E788[];
extern char cBaseclassdesc[];
extern cType *D_000385DC;
extern cType *D_00038888;

template <class T> T *dcast(const cBase *);

// ── cConfigBase::cConfigBase(cBase *) @ 0x000086b4 ──
cConfigBase::cConfigBase(cBase *parent) {
    *(cBase **)this = parent;
    *(void **)((char *)this + 4) = (void *)0x37E7C8;
    cStr_ctor((char *)this + 8, (const char *)0x36CA2C);
    cStr_ctor((char *)this + 0x108, (const char *)0x36CA44);
    *(cConfigBase **)0x37C074 = this;
    int i = 0;
    do {
        char *byte = (char *)this + i;
        *(int *)((char *)this + 0x214 + i * 4) = i;
        *(char *)(byte + 0x208) = 0;
        __asm__ volatile("" ::: "memory");
        i += 1;
    } while (i < 12);
    *((char *)this + 0x208) = 1;
}

// ── cConfigBase::~cConfigBase(void) @ 0x0000873c ──
cConfigBase::~cConfigBase() {
    *(void **)((char *)this + 4) = (void *)0x37E7C8;
    *(cConfigBase **)0x37C074 = 0;
    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}

// ── cConfigBase::Write(cFile &) const @ 0x000084d8 ──
void cConfigBase::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cStr *)((char *)this + 8))->Write(wb);
    ((const cStr *)((char *)this + 0x108))->Write(wb);
    wb.Write(0xC);
    wb.Write(0xC, (const bool *)((char *)this + 0x208));
    wb.Write(0xC, (const int *)((char *)this + 0x214));
    wb.End();
}

// ── cConfigBase::AssignCopy(const cBase *) @ 0x001c676c ──
void cConfigBase::AssignCopy(const cBase *src) {
    cConfigBase *other = dcast<cConfigBase>(src);

    char *srcFirst = (char *)other + 8;
    __asm__ volatile("" :: "r"(srcFirst));
    *(cStr *)((char *)this + 8) = *(cStr *)((char *)other + 8);
    *(cStr *)((char *)this + 0x108) = *(cStr *)((char *)other + 0x108);

    int i = 0;
    do {
        *((unsigned char *)this + (0x208 + i)) = *((unsigned char *)other + (0x208 + i));
        i += 1;
    } while (i < 12);

    int j = 0;
    cConfigBase *srcObj = other;
    cConfigBase *dstObj = this;
    do {
        j += 1;
        *(int *)((char *)dstObj + 0x214) = *(int *)((char *)srcObj + 0x214);
        srcObj = (cConfigBase *)((char *)srcObj + 4);
        dstObj = (cConfigBase *)((char *)dstObj + 4);
    } while (j < 12);
}

// ── cConfigBase::New(cMemPool *, cBase *) static @ 0x001c6848 ──
cBase *cConfigBase::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    cConfigBase *result = 0;
    cConfigBase *obj = (cConfigBase *)rec->fn(base, 0x244, 4, 0, 0);
    if (obj != 0) {
        new (obj) cConfigBase(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── cConfigBase::GetType(void) const @ 0x001c68c4 ──
const cType *cConfigBase::GetType(void) const {
    if (D_00038888 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36C830,
                                               (const char *)0x36C838,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00038888 = cType::InitializeType(0, 0, 0x28A, D_000385DC,
                                           &cConfigBase::New, 0, 0, 0);
    }
    return D_00038888;
}
