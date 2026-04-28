// gcProjectileControllerTemplate — gcAll_psp.obj
//
// Functions:
//   gcProjectileControllerTemplate::New(cMemPool *, cBase *) static @ 0x0031EC28 (124B)

class cBase;
class cMemPool;

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcProjectileControllerTemplate {
public:
    gcProjectileControllerTemplate(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

// ── gcProjectileControllerTemplate::New(cMemPool *, cBase *) static @ 0x0031EC28 ──
cBase *gcProjectileControllerTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcProjectileControllerTemplate *result = 0;
    gcProjectileControllerTemplate *obj = (gcProjectileControllerTemplate *)rec->fn(base, 0x70, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) gcProjectileControllerTemplate(parent);
        result = obj;
    }
    return (cBase *)result;
}
