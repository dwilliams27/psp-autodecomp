// gcViewport — viewport class. Instances live in a contiguous global array
// based at *(0x37D840); each is 0x1390 bytes wide. GetName formats the
// instance's 1-based index into the supplied buffer.

class cBase;
class cMemPool;

class gcCamera {
public:
    gcCamera &operator=(const gcCamera &);
};

class gcViewport {
public:
    void GetName(char *) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

template <class T> T *dcast(const cBase *);

extern "C" int cStrFormat(char *, const char *, ...);
extern "C" void gcViewport__gcViewport_cBaseptr(void *self, cBase *parent);

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

// ── gcViewport::GetName(char *) const @ 0x002492E8 ──
void gcViewport::GetName(char *buf) const {
    cStrFormat(buf, (const char *)0x36DBCC,
               ((int)((const char *)this - *(char **)0x37D840) / 0x1390) + 1);
}

// ── gcViewport::AssignCopy(const cBase *) @ 0x0024917c ──
void gcViewport::AssignCopy(const cBase *base) {
    gcViewport *other = dcast<gcViewport>(base);
    const gcCamera &src = *(const gcCamera *)(other + 0x10);
    *(gcCamera *)((char *)this + 0x10) = src;
    *(unsigned char *)((char *)this + 0x1380) = *(const unsigned char *)((char *)other + 0x1380);
    *(unsigned char *)((char *)this + 0x1381) = *(const unsigned char *)((char *)other + 0x1381);
}

// ── gcViewport::New(cMemPool *, cBase *) static @ 0x002491cc ──
cBase *gcViewport::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcViewport *result = 0;
    gcViewport *obj = (gcViewport *)rec->fn(base, 0x1390, 0x10, 0, 0);
    if (obj != 0) {
        gcViewport__gcViewport_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
