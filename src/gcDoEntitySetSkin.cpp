// gcDoEntitySetSkin.cpp - gcAll_psp.obj

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cMemPool;

class gcDoEntitySetSkin {
public:
    gcDoEntitySetSkin(cBase *);

    static cBase *New(cMemPool *, cBase *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

cBase *gcDoEntitySetSkin::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntitySetSkin *result = 0;
    gcDoEntitySetSkin *obj = (gcDoEntitySetSkin *)rec->fn(base, 0x8C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntitySetSkin(parent);
        result = obj;
    }
    return (cBase *)result;
}
