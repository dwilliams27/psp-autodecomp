class cBase;
class cMemPool;

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcDoEntityCastRay {
public:
    gcDoEntityCastRay(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

class gcDoEntityMoveToPoint {
public:
    gcDoEntityMoveToPoint(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

class gcDoEntityPartialBodyAttack {
public:
    gcDoEntityPartialBodyAttack(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

cBase *gcDoEntityCastRay::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntityCastRay *result = 0;
    gcDoEntityCastRay *obj =
        (gcDoEntityCastRay *)rec->fn(base, 0x124, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntityCastRay(parent);
        result = obj;
    }
    return (cBase *)result;
}

cBase *gcDoEntityMoveToPoint::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntityMoveToPoint *result = 0;
    gcDoEntityMoveToPoint *obj =
        (gcDoEntityMoveToPoint *)rec->fn(base, 0x108, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntityMoveToPoint(parent);
        result = obj;
    }
    return (cBase *)result;
}

cBase *gcDoEntityPartialBodyAttack::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntityPartialBodyAttack *result = 0;
    gcDoEntityPartialBodyAttack *obj =
        (gcDoEntityPartialBodyAttack *)rec->fn(base, 0x88, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntityPartialBodyAttack(parent);
        result = obj;
    }
    return (cBase *)result;
}
