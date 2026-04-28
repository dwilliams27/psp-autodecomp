// gcEvent::operator=(const gcEvent &) — 0x000d61d0

class cBase;
class cMemPool;

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcExpressionList {
public:
    gcExpressionList &operator=(const gcExpressionList &other);
};

class gcEvent {
public:
    char _pad0[8];
    gcExpressionList mExprList;     // +0x08
    char _pad1[4];                  // pad so mField10 lands at +0x10
    int mField10;                   // +0x10

    gcEvent(cBase *, const char *);
    gcEvent &operator=(const gcEvent &other);
    static cBase *New(cMemPool *, cBase *);
};

gcEvent &gcEvent::operator=(const gcEvent &other) {
    mExprList = other.mExprList;
    mField10 = other.mField10;
    return *this;
}

cBase *gcEvent::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcEvent *result = 0;
    gcEvent *obj = (gcEvent *)rec->fn(base, 0x1C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcEvent(parent, 0);
        result = obj;
    }
    return (cBase *)result;
}
