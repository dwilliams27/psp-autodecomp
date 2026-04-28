// gcTrigger -- trigger object helpers.

class cBase;
class cMemPool;

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcTrigger {
public:
    gcTrigger(cBase *);

    static cBase *New(cMemPool *, cBase *);
};

// -- gcTrigger::New(cMemPool *, cBase *) static @ 0x00276f70 --
cBase *gcTrigger::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcTrigger *result = 0;
    gcTrigger *obj = (gcTrigger *)rec->fn(base, 0x140, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) gcTrigger(parent);
        result = obj;
    }
    return (cBase *)result;
}
