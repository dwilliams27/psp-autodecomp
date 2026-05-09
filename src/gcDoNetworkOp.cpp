class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cType;
class gcDoNetworkOp;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

class gcExpression {
};

class gcAction : public gcExpression {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    int Read(cFile &, cMemPool *);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
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
    short pad;
    const cType *(*getType)(void *);
};

extern "C" void gcAction_gcAction(void *, cBase *);
void gcAction_Write(const gcDoNetworkOp *, cFile &);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern const char gcDoNetworkOp_base_name[];
extern const char gcDoNetworkOp_base_desc[];
extern char gcDoNetworkOpvirtualtable[];

class gcDoNetworkOp : public gcAction {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    const cType *GetType(void) const;
    gcDoNetworkOp &operator=(const gcDoNetworkOp &);
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoNetworkOp;

cBase *gcDoNetworkOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoNetworkOp *result = 0;
    gcDoNetworkOp *obj = (gcDoNetworkOp *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        *(void **)((char *)obj + 4) = gcDoNetworkOpvirtualtable;
        *(int *)((char *)obj + 0xC) = 0;
        *(int *)((char *)obj + 0x10) = (int)obj | 1;
        *(int *)((char *)obj + 0x14) = (int)obj | 1;
        result = obj;
    }
    return (cBase *)result;
}

void gcDoNetworkOp::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoNetworkOp) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894, (const char *)0x36D89C, 1,
                            0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoNetworkOp = cType::InitializeType(
                0, 0, 0x1A8, type_action, gcDoNetworkOp::New, 0, 0, 0);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoNetworkOp;
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
    *this = *(const gcDoNetworkOp *)copy;
}

void gcDoNetworkOp::Write(cFile &file) const {
    cWriteBlock wb(file, 3);
    gcAction_Write(this, file);
    wb.Write(((int *)this)[3]);
    ((gcDesiredValue *)((char *)this + 0x10))->Write(wb);

    int val = ((int *)this)[5];
    int flag = 0;
    if (val & 1) {
        flag = 1;
    }
    cBase *ptr;
    if (flag != 0) {
        ptr = 0;
    } else {
        ptr = (cBase *)val;
    }
    wb.WriteBase(ptr);
    wb.End();
}

int gcDoNetworkOp::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);

    if ((unsigned int)rb[3] >= 4 || (unsigned int)rb[3] < 2 ||
        ((gcAction *)this)->Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);
    ((gcDesiredValue *)((char *)this + 0x10))->Read(*(cReadBlock *)rb);

    if ((unsigned int)rb[3] >= 3) {
        int sp14;
        int value = *(int *)((char *)this + 0x14);
        int tag = value & 1;
        int flag = 0;
        if (tag != 0) {
            flag = 1;
        }

        int outValue;
        if (flag != 0) {
            outValue = 0;
            goto out_done;
        }
        outValue = value;
    out_done:
        sp14 = outValue;

        int flag2 = 0;
        if (tag != 0) {
            flag2 = 1;
        }

        int base;
        if (flag2 != 0) {
            base = value & ~1;
        } else {
            base = *(int *)value;
        }

        cMemPool *childPool = cMemPool::GetPoolFromPtr((char *)this + 0x14);
        ((cReadBlock *)rb)->ReadBase(childPool, (cBase *)base, *(cBase **)&sp14);

        int newValue;
        if (sp14 == 0) {
            newValue = base | 1;
        } else {
            newValue = sp14;
        }
        *(int *)((char *)this + 0x14) = newValue;
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

const cType *gcDoNetworkOp::GetType(void) const {
    if (!type_gcDoNetworkOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoNetworkOp_base_name,
                                                      gcDoNetworkOp_base_desc,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoNetworkOp = cType::InitializeType(0, 0, 0x1A8, type_action,
                                                   gcDoNetworkOp::New, 0, 0, 0);
    }
    return type_gcDoNetworkOp;
}
