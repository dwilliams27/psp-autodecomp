inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cMemPool;

class cObject {
public:
    cObject(cBase *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class eAnimation : public cObject {
public:
    eAnimation(cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_000469C4;

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

const cType *eAnimation::GetType(void) const {
    if (D_000469C4 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36CD74,
                                                       (const char *)0x36CD7C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_000469C4 = cType::InitializeType(0, 0, 0x2E, D_000385E4,
                                           &eAnimation::New,
                                           (const char *)0x36CE4C,
                                           (const char *)0x36CE58, 1);
    }
    return D_000469C4;
}
