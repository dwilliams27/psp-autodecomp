// gcTrigger -- trigger object helpers.

class cBase;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
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
extern cType *D_0009F4A4;

class gcTrigger {
public:
    gcTrigger(cBase *);
    const cType *GetType(void) const;

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

const cType *gcTrigger::GetType(void) const {
    if (D_0009F4A4 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_0009F4A4 = cType::InitializeType(0, 0, 0x9B, D_000385E4,
                                           &gcTrigger::New,
                                           (const char *)0x36D9EC,
                                           (const char *)0x36D9F8, 0);
    }
    return D_0009F4A4;
}
