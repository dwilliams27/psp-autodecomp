// gcValEntityDirection -- entity direction value helpers.

class cBase;
class cMemPool;

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcValEntityDirection {
public:
    gcValEntityDirection(cBase *);

    static cBase *New(cMemPool *, cBase *);
};

// -- gcValEntityDirection::New(cMemPool *, cBase *) static @ 0x00328904 --
cBase *gcValEntityDirection::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcValEntityDirection *result = 0;
    gcValEntityDirection *obj =
        (gcValEntityDirection *)rec->fn(base, 0x108, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValEntityDirection(parent);
        result = obj;
    }
    return (cBase *)result;
}
