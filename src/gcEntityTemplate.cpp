inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cMemPool;
class cType;

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

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009F448;

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcEntityTemplate {
public:
    gcEntityTemplate(cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
};

class gcExternalVariable {
public:
    gcExternalVariable(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

class gcGeomCurveConfig {
public:
    gcGeomCurveConfig(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

// gcEntityTemplate::GetType(void) const @ 0x002688a8
const cType *gcEntityTemplate::GetType(void) const {
    if (D_0009F448 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New,
                                                   0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_0009F448 = cType::InitializeType(0, 0, 0x8E, D_000385E4,
                                           &gcEntityTemplate::New,
                                           (const char *)0x36D9B8,
                                           (const char *)0x36D9C8,
                                           5);
    }
    return D_0009F448;
}

// gcEntityTemplate::New(cMemPool *, cBase *) static @ 0x0026882c
cBase *gcEntityTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcEntityTemplate *result = 0;
    gcEntityTemplate *obj = (gcEntityTemplate *)rec->fn(base, 0xD0, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcEntityTemplate(parent);
        result = obj;
    }
    return (cBase *)result;
}

// gcExternalVariable::New(cMemPool *, cBase *) static @ 0x0027c0b4
cBase *gcExternalVariable::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcExternalVariable *result = 0;
    gcExternalVariable *obj = (gcExternalVariable *)rec->fn(base, 0x48, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcExternalVariable(parent);
        result = obj;
    }
    return (cBase *)result;
}

// gcGeomCurveConfig::New(cMemPool *, cBase *) static @ 0x0031bd08
cBase *gcGeomCurveConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcGeomCurveConfig *result = 0;
    gcGeomCurveConfig *obj = (gcGeomCurveConfig *)rec->fn(base, 0xA0, 0x10, 0, 0);
    if (obj != 0) {
        new (obj) gcGeomCurveConfig(parent);
        result = obj;
    }
    return (cBase *)result;
}
