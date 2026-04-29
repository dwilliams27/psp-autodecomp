class cType;
class cMemPool;

class cBase {
public:
    cBase *mParent;
    void *mClassDesc;
};

class cType {
public:
    char _pad[0x1C];
    cType *mParent;

    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct VtableEntry {
    short thisAdjust;
    short pad;
    const cType *(*fn)(const void *);
};

class gcDoEntityCastRay {
public:
    gcDoEntityCastRay &operator=(const gcDoEntityCastRay &);
    gcDoEntityCastRay(cBase *);
    const cType *GetType(void) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
};

class gcDoEntityMoveToPoint {
public:
    gcDoEntityMoveToPoint(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

class gcDoEntityPartialBodyAttack {
public:
    gcDoEntityPartialBodyAttack(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

extern const char gcDoEntityCastRay_base_name[];
extern const char gcDoEntityCastRay_base_desc[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityCastRay;

cBase *gcDoEntityCastRay::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntityCastRay *result = 0;
    gcDoEntityCastRay *obj =
        (gcDoEntityCastRay *)rec->fn(base, 0x124, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntityCastRay(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoEntityCastRay::GetType(void) const {
    if (!type_gcDoEntityCastRay) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoEntityCastRay_base_name,
                        gcDoEntityCastRay_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoEntityCastRay = cType::InitializeType(
            0, 0, 0x119, type_action, gcDoEntityCastRay::New, 0, 0, 0x80);
    }
    return type_gcDoEntityCastRay;
}

void gcDoEntityCastRay::AssignCopy(const cBase *base) {
    const gcDoEntityCastRay *other = 0;

    if (base != 0) {
        if (!type_gcDoEntityCastRay) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            gcDoEntityCastRay_base_name,
                            gcDoEntityCastRay_base_desc,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoEntityCastRay = cType::InitializeType(
                0, 0, 0x119, type_action, gcDoEntityCastRay::New, 0, 0, 0x80);
        }

        const cType *wantedType = type_gcDoEntityCastRay;
        VtableEntry *entry = (VtableEntry *)((char *)((cBase *)base)->mClassDesc + 8);
        const cType *type = entry->fn((const char *)base + entry->thisAdjust);
        int ok;

        if (wantedType == 0) {
            ok = 0;
        } else if (type != 0) {
            while (type != 0) {
                if (type == wantedType) {
                    ok = 1;
                    goto done;
                }
                type = type->mParent;
            }
            ok = 0;
        } else {
            ok = 0;
        }
done:
        if (ok != 0) {
            other = (const gcDoEntityCastRay *)base;
        }
    }

    operator=(*other);
}

cBase *gcDoEntityMoveToPoint::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntityMoveToPoint *result = 0;
    gcDoEntityMoveToPoint *obj =
        (gcDoEntityMoveToPoint *)rec->fn(base, 0x108, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntityMoveToPoint(parent);
        result = obj;
    }
    return (cBase *)result;
}

cBase *gcDoEntityPartialBodyAttack::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntityPartialBodyAttack *result = 0;
    gcDoEntityPartialBodyAttack *obj =
        (gcDoEntityPartialBodyAttack *)rec->fn(base, 0x88, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntityPartialBodyAttack(parent);
        result = obj;
    }
    return (cBase *)result;
}
