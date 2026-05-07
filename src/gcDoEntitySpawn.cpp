class cBase;
class cFile;
class cMemPool;
class cName;
class cType;
class gcDesiredValue;

class cWriteBlock {
public:
    cFile *mFile;
    unsigned int mOffset;

    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void End(void);
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
};

class gcAction {
public:
    void Write(cFile &) const;
};

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

struct WriteRec {
    short offset;
    short _pad;
    void (*fn)(void *, cFile *);
};

class gcDoEntitySpawn {
public:
    gcDoEntitySpawn(cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
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

void gcDoEntitySpawn::Write(cFile &file) const {
    cWriteBlock wb(file, 0x11);
    ((const gcAction *)this)->Write(file);

    {
        char *typeInfo = *(char **)((const char *)this + 0x10);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x0C;
        rec->fn(base + rec->offset, wb.mFile);
    }

    wb.Write(*(const int *)((const char *)this + 0x24));

    {
        char *typeInfo = *(char **)((const char *)this + 0x2C);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x28;
        rec->fn(base + rec->offset, wb.mFile);
    }

    {
        char *typeInfo = *(char **)((const char *)this + 0x40);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x3C;
        rec->fn(base + rec->offset, wb.mFile);
    }

    ((const cName *)((const char *)this + 0x6C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xD8))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x84))->Write(wb);

    {
        char *typeInfo = *(char **)((const char *)this + 0x8C);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x88;
        rec->fn(base + rec->offset, wb.mFile);
    }

    ((const cName *)((const char *)this + 0xB4))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xCC))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xD0))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xD4))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0xDC));
    wb.Write(*(const bool *)((const char *)this + 0xDD));
    wb.Write(*(const bool *)((const char *)this + 0xDE));
    wb.Write(*(const int *)((const char *)this + 0xE4));
    ((const gcDesiredValue *)((const char *)this + 0xE8))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xEC))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 0xF0));
    ((const gcDesiredValue *)((const char *)this + 0xF4))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xF8))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0xFC))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x100))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 0x104));

    {
        char *typeInfo = *(char **)((const char *)this + 0x10C);
        WriteRec *rec = (WriteRec *)(typeInfo + 0x28);
        char *base = (char *)this + 0x108;
        rec->fn(base + rec->offset, wb.mFile);
    }

    wb.Write(*(const bool *)((const char *)this + 0xDF));
    wb.Write(*(const bool *)((const char *)this + 0xE0));
    ((const gcDesiredValue *)((const char *)this + 0x11C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x120))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x124))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x128))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x12C))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x130))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x134))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x138))->Write(wb);
    wb.Write(*(const bool *)((const char *)this + 0xE1));
    wb.End();
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
