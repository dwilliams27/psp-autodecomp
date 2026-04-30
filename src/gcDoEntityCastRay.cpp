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

extern "C" void gcAction_ctor(void *, cBase *)
    __asm__("gcAction__gcAction_cBaseptr__0012F4C8");
extern "C" void gcEvent_ctor(void *, cBase *, const char *)
    __asm__("gcEvent__gcEvent_cBaseptr_constcharptr__000D60F0");
extern "C" void gcDesiredObject_ctor(void *, cBase *)
    __asm__("gcDesiredObject__gcDesiredObject_cBaseptr__0011B6F4");
extern "C" void gcDesiredEntityHelper_ctor(void *, int, int, int)
    __asm__("gcDesiredEntityHelper__gcDesiredEntityHelper_gcDesiredEntityHelper__gcPrimary_gcDesiredEntityHelper__gcRelationship_gcDesiredEntityHelper__gcRelationship__0011B714");
extern "C" void gcDesiredCamera_ctor(void *, cBase *)
    __asm__("gcDesiredCamera__gcDesiredCamera_cBaseptr__001215D0");

extern const char gcDoEntityCastRay_base_name[];
extern const char gcDoEntityCastRay_base_desc[];
extern char D_00000338[];
extern char D_00002428[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityCastRay;

gcDoEntityCastRay::gcDoEntityCastRay(cBase *parent) {
    gcAction_ctor(this, parent);

    *(void **)((char *)this + 0x04) = D_00002428;
    *(short *)((char *)this + 0x20) = 0;
    *(short *)((char *)this + 0x22) = 0;
    *(unsigned char *)((char *)this + 0x0C) = 0;
    *(short *)((char *)this + 0x38) = 0;
    *(short *)((char *)this + 0x3A) = 0;
    *(unsigned char *)((char *)this + 0x24) = 0;

    gcEvent_ctor((char *)this + 0x3C, (cBase *)this, (const char *)0x36E5D4);

    char *desired = (char *)this + 0x58;
    gcDesiredObject_ctor(desired, (cBase *)this);

    *(void **)((char *)this + 0x5C) = D_00000338;

    void *helper = (char *)this + 0x64;
    int one = 1;
    gcDesiredEntityHelper_ctor(helper, 1, 0, 0);
    *(void **)((char *)this + 0x5C) = (void *)0x388A48;
    *(void **)((char *)this + 0x6C) = desired;
    void *desiredVTable = (void *)0x388568;
    *(void **)((char *)this + 0x70) = desiredVTable;
    *(unsigned char *)((char *)this + 0x74) = one;
    *(unsigned char *)((char *)this + 0x75) = 0;
    *(int *)((char *)this + 0x78) = 0;
    *(int *)((char *)this + 0x7C) = 0;
    *(int *)((char *)this + 0x80) = (int)desired | 1;

    gcDesiredCamera_ctor((char *)this + 0x84, (cBase *)this);

    int encoded = (int)this | 1;
    *(int *)((char *)this + 0xBC) = one;
    *(int *)((char *)this + 0xC0) = encoded;
    *(int *)((char *)this + 0xC4) = encoded;
    *(int *)((char *)this + 0xC8) = encoded;
    *(int *)((char *)this + 0xCC) = encoded;
    *(int *)((char *)this + 0xD0) = encoded;
    *(int *)((char *)this + 0xD4) = 0;
    *(int *)((char *)this + 0xD8) = encoded;
    *(int *)((char *)this + 0xDC) = encoded;
    *(int *)((char *)this + 0xE0) = encoded;
    *(int *)((char *)this + 0xE4) = encoded;
    *(int *)((char *)this + 0xE8) = encoded;
    *(int *)((char *)this + 0xEC) = encoded;
    *(int *)((char *)this + 0xF0) = -1;
    *(unsigned char *)((char *)this + 0xF4) = 0;
    *(unsigned char *)((char *)this + 0xF5) = 0;
    *(unsigned char *)((char *)this + 0xF6) = 0;
    *(void **)((char *)this + 0xF8) = this;
    *(void **)((char *)this + 0xFC) = desiredVTable;
    *(unsigned char *)((char *)this + 0x100) = one;
    *(unsigned char *)((char *)this + 0x101) = 0;
    *(int *)((char *)this + 0x104) = 0;
    *(int *)((char *)this + 0x108) = 0;
    *(int *)((char *)this + 0x10C) = encoded;
    *(int *)((char *)this + 0x110) = encoded;
    *(int *)((char *)this + 0x114) = encoded;
    *(int *)((char *)this + 0x118) = encoded;
    *(int *)((char *)this + 0x11C) = encoded;
    *(unsigned char *)((char *)this + 0x120) = 0;
}

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
