inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cMemPool;

class cObject {
public:
    cObject(cBase *);
};

class eAnimation : public cObject {
public:
    eAnimation(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

eAnimation::eAnimation(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x381068;
    *(int *)((char *)this + 0x44) = 0;
    *(int *)((char *)this + 0x48) = 0;
    *(short *)((char *)this + 0x4C) = 0;
    *(short *)((char *)this + 0x4E) = 0;
    *(unsigned char *)((char *)this + 0x50) = 1;
    *(unsigned char *)((char *)this + 0x51) = 0;
    *(unsigned char *)((char *)this + 0x52) = 0;
    *(unsigned char *)((char *)this + 0x53) = 0;
    *(float *)((char *)this + 0x54) = 0.0f;
    *(float *)((char *)this + 0x58) = 0.0f;
    *(int *)((char *)this + 0x5C) = 0;
    *(int *)((char *)this + 0x60) = 0;
    *(int *)((char *)this + 0x64) = 0;
    *(int *)((char *)this + 0x68) = 0;
    *(int *)((char *)this + 0x6C) = 0;
    *(int *)((char *)this + 0x70) = 0;
    *(int *)((char *)this + 0x74) = 0;
    *(int *)((char *)this + 0x78) = 0;
    *(int *)((char *)this + 0x7C) = 0;
    *(int *)((char *)this + 0x80) = 0;
}

cBase *eAnimation::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eAnimation *result = 0;
    eAnimation *obj = (eAnimation *)rec->fn(base, 0x84, 4, 0, 0);
    if (obj != 0) {
        new (obj) eAnimation(parent);
        result = obj;
    }
    return (cBase *)result;
}
