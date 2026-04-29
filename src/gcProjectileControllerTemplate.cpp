// gcProjectileControllerTemplate — gcAll_psp.obj
//
// Functions:
//   gcProjectileControllerTemplate::New(cMemPool *, cBase *) static @ 0x0031EC28 (124B)

class cBase;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcProjectileControllerTemplate {
public:
    gcProjectileControllerTemplate(cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

extern cType *D_000385DC;
extern cType *D_0009A400;
extern cType *D_0009F7A8;

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

const cType *gcProjectileControllerTemplate::GetType(void) const {
    if (D_0009F7A8 == 0) {
        if (D_0009A400 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009A400 = cType::InitializeType(0, 0, 0x9A, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F7A8 = cType::InitializeType(0, 0, 0xF1, D_0009A400,
                                           &gcProjectileControllerTemplate::New,
                                           0, 0, 0);
    }
    return D_0009F7A8;
}
