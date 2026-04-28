inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cMemPool;

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcEntityTemplate {
public:
    gcEntityTemplate(cBase *);
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
