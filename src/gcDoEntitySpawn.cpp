class cBase;
class cMemPool;

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcDoEntitySpawn {
public:
    gcDoEntitySpawn(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

class gcDoEntitySubGeomOp {
public:
    gcDoEntitySubGeomOp(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

class gcDoSwitch {
public:
    gcDoSwitch(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

cBase *gcDoEntitySpawn::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntitySpawn *result = 0;
    gcDoEntitySpawn *obj =
        (gcDoEntitySpawn *)rec->fn(base, 0x13C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntitySpawn(parent);
        result = obj;
    }
    return (cBase *)result;
}

cBase *gcDoEntitySubGeomOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntitySubGeomOp *result = 0;
    gcDoEntitySubGeomOp *obj =
        (gcDoEntitySubGeomOp *)rec->fn(base, 0xA0, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntitySubGeomOp(parent);
        result = obj;
    }
    return (cBase *)result;
}

cBase *gcDoSwitch::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoSwitch *result = 0;
    gcDoSwitch *obj = (gcDoSwitch *)rec->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoSwitch(parent);
        result = obj;
    }
    return (cBase *)result;
}
