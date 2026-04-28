// gcDoEntitySetGeomCurveTarget.cpp - gcAll_psp.obj

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cMemPool;

class gcDoEntitySetGeomCurveTarget {
public:
    gcDoEntitySetGeomCurveTarget(cBase *);

    static cBase *New(cMemPool *, cBase *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

cBase *gcDoEntitySetGeomCurveTarget::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntitySetGeomCurveTarget *result = 0;
    gcDoEntitySetGeomCurveTarget *obj =
        (gcDoEntitySetGeomCurveTarget *)rec->fn(base, 0x98, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntitySetGeomCurveTarget(parent);
        result = obj;
    }
    return (cBase *)result;
}
