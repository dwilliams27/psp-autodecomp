inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cMemPool;

class cObject {
public:
    cObject(cBase *);
    cObject &operator=(const cObject &);
};

template <class T> T *dcast(const cBase *);

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase<T> &operator=(const cArrayBase<T> &);
};

struct eAnimationQuat;
struct eAnimationVec3;
struct eAnimationMorphFrame;

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
    void AssignCopy(const cBase *);
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

void eAnimation::AssignCopy(const cBase *base) {
    eAnimation *other = dcast<eAnimation>(base);
    ((cObject *)this)->operator=(*(const cObject *)other);
    int *src44 = (int *)((char *)other + 0x44);
    int value44 = *src44;
    int *dst44 = (int *)((char *)this + 0x44);
    int *src48 = (int *)((char *)other + 0x48);
    int *dst48 = (int *)((char *)this + 0x48);
    *dst44 = value44;
    int value48 = *src48;
    *dst48 = value48;
    *(short *)((char *)this + 0x4C) = *(const short *)((char *)other + 0x4C);
    *(short *)((char *)this + 0x4E) = *(const short *)((char *)other + 0x4E);
    *(unsigned char *)((char *)this + 0x50) =
        *(const unsigned char *)((char *)other + 0x50);
    *(unsigned char *)((char *)this + 0x51) =
        *(const unsigned char *)((char *)other + 0x51);
    *(unsigned char *)((char *)this + 0x52) =
        *(const unsigned char *)((char *)other + 0x52);
    *(unsigned char *)((char *)this + 0x53) =
        *(const unsigned char *)((char *)other + 0x53);
    *(float *)((char *)this + 0x54) = *(const float *)((char *)other + 0x54);
    *(float *)((char *)this + 0x58) = *(const float *)((char *)other + 0x58);
    ((cArrayBase<unsigned char> *)((char *)this + 0x5C))
        ->operator=(*(const cArrayBase<unsigned char> *)((char *)other + 0x5C));
    ((cArrayBase<eAnimationQuat> *)((char *)this + 0x60))
        ->operator=(*(const cArrayBase<eAnimationQuat> *)((char *)other + 0x60));
    ((cArrayBase<eAnimationVec3> *)((char *)this + 0x64))
        ->operator=(*(const cArrayBase<eAnimationVec3> *)((char *)other + 0x64));
    ((cArrayBase<eAnimationVec3> *)((char *)this + 0x68))
        ->operator=(*(const cArrayBase<eAnimationVec3> *)((char *)other + 0x68));
    ((cArrayBase<unsigned short> *)((char *)this + 0x6C))
        ->operator=(*(const cArrayBase<unsigned short> *)((char *)other + 0x6C));
    ((cArrayBase<unsigned short> *)((char *)this + 0x70))
        ->operator=(*(const cArrayBase<unsigned short> *)((char *)other + 0x70));
    ((cArrayBase<unsigned short> *)((char *)this + 0x74))
        ->operator=(*(const cArrayBase<unsigned short> *)((char *)other + 0x74));
    ((cArrayBase<eAnimationMorphFrame> *)((char *)this + 0x78))
        ->operator=(*(const cArrayBase<eAnimationMorphFrame> *)((char *)other + 0x78));
    ((cArrayBase<unsigned char> *)((char *)this + 0x7C))
        ->operator=(*(const cArrayBase<unsigned char> *)((char *)other + 0x7C));
    ((cArrayBase<short> *)((char *)this + 0x80))
        ->operator=(*(const cArrayBase<short> *)((char *)other + 0x80));
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
