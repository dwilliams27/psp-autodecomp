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

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct cTypeNode {
    char pad[0x1C];
    cTypeNode *parent;
};

struct VTableSlot {
    short offset;
    short _pad;
    const cType *(*getType)(void *);
};

class gcDoEntitySpawn {
public:
    gcDoEntitySpawn(cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    gcDoEntitySpawn &operator=(const gcDoEntitySpawn &);
};

class gcDoEntitySubGeomOp {
public:
    gcDoEntitySubGeomOp(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

class gcDoSwitch {
public:
    gcDoSwitch(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_action asm("D_000385D4");
static cType *type_gcDoEntitySpawn asm("D_0009F668");

cBase *gcDoEntitySpawn::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntitySpawn *result = 0;
    gcDoEntitySpawn *obj =
        (gcDoEntitySpawn *)rec->fn(base, 0x13C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntitySpawn(parent);
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoEntitySpawn::GetType(void) const {
    if (!type_gcDoEntitySpawn) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A,
                                                        type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoEntitySpawn = cType::InitializeType(
            0, 0, 0xE0, type_action, gcDoEntitySpawn::New, 0, 0, 0);
    }
    return type_gcDoEntitySpawn;
}

void gcDoEntitySpawn::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoEntitySpawn) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType((const char *)0x36D894,
                                                          (const char *)0x36D89C,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(0, 0, 0x6A,
                                                            type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                    0, 0, 0, 0);
            }
            type_gcDoEntitySpawn = cType::InitializeType(
                0, 0, 0xE0, type_action, gcDoEntitySpawn::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoEntitySpawn;
        VTableSlot *slot = (VTableSlot *)((char *)vt + 8);
        short voff = slot->offset;
        const cType *(*getType)(void *) = slot->getType;
        const cType *type = getType((char *)other + voff);
        int ok;

        if (myType == 0) {
            ok = 0;
            goto done;
        }
        if (type != 0) {
        loop:
            if (type == myType) {
                ok = 1;
                goto done;
            }
            type = (const cType *)((cTypeNode *)type)->parent;
            if (type != 0) {
                goto loop;
            }
        }
        ok = 0;
    done:
        if (ok != 0) {
            copy = other;
        }
    }
    *this = *(const gcDoEntitySpawn *)copy;
}

cBase *gcDoEntitySubGeomOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntitySubGeomOp *result = 0;
    gcDoEntitySubGeomOp *obj =
        (gcDoEntitySubGeomOp *)rec->fn(base, 0xA0, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntitySubGeomOp(parent);
        result = obj;
    }
    return (cBase *)result;
}

cBase *gcDoSwitch::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoSwitch *result = 0;
    gcDoSwitch *obj = (gcDoSwitch *)rec->fn(base, 0x20, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoSwitch(parent);
        result = obj;
    }
    return (cBase *)result;
}
