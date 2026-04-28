class cBase;
class cMemPool;

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcValEntityDistance {
public:
    gcValEntityDistance(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

cBase *gcValEntityDistance::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcValEntityDistance *result = 0;
    gcValEntityDistance *obj =
        (gcValEntityDistance *)rec->fn(base, 0xB8, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValEntityDistance(parent);
        result = obj;
    }
    return (cBase *)result;
}
