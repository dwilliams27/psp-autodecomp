class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];

    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void WriteBase(const cBase *);
    void End(void);
};

class cName {
public:
    void Write(cWriteBlock &) const;
    void Read(class cReadBlock &);
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
    void Read(class cReadBlock &);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class gcAction {
public:
    int Read(cFile &, cMemPool *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcDoQueryNavMesh {
public:
    static cBase *New(cMemPool *, cBase *);
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    gcDoQueryNavMesh &operator=(const gcDoQueryNavMesh &);
    void Write(cFile &) const;
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
    short _pad;
    const cType *(*getType)(void *);
};

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcAction_Write(const void *, cFile &);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern char gcDoQueryNavMeshvirtualtable[];
extern const char gcDoQueryNavMesh_base_name[] asm("D_0036D894");
extern const char gcDoQueryNavMesh_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoQueryNavMesh asm("D_0009F6D8");

cBase *gcDoQueryNavMesh::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoQueryNavMesh *result = 0;
    gcDoQueryNavMesh *obj = (gcDoQueryNavMesh *)entry->fn(base, 0x50, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoQueryNavMeshvirtualtable;
        ((int *)obj)[3] = (int)obj | 1;
        ((int *)obj)[4] = (int)obj | 1;
        ((int *)obj)[5] = (int)obj | 1;
        ((int *)obj)[6] = (int)obj | 1;
        ((short *)obj)[24] = 0;
        ((short *)obj)[25] = 0;
        *((unsigned char *)obj + 0x1C) = 0;
        ((short *)obj)[36] = 0;
        ((short *)obj)[37] = 0;
        *((unsigned char *)obj + 0x34) = 0;
        ((int *)obj)[19] = 0;
        result = obj;
    }
    return (cBase *)result;
}

const cType *gcDoQueryNavMesh::GetType(void) const {
    if (!type_gcDoQueryNavMesh) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoQueryNavMesh_base_name,
                                                      gcDoQueryNavMesh_base_desc,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression,
                                                0, 0, 0, 0);
        }
        type_gcDoQueryNavMesh = cType::InitializeType(
            0, 0, 0x19D, type_action, gcDoQueryNavMesh::New, 0, 0, 0x80);
    }
    return type_gcDoQueryNavMesh;
}

void gcDoQueryNavMesh::AssignCopy(const cBase *other) {
    const cBase *copy = 0;
    if (other != 0) {
        if (!type_gcDoQueryNavMesh) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(gcDoQueryNavMesh_base_name,
                                                          gcDoQueryNavMesh_base_desc,
                                                          1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoQueryNavMesh = cType::InitializeType(
                0, 0, 0x19D, type_action, gcDoQueryNavMesh::New, 0, 0, 0x80);
        }
        void *vt = ((void **)other)[1];
        const cType *myType = type_gcDoQueryNavMesh;
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
    *this = *(const gcDoQueryNavMesh *)copy;
}

void gcDoQueryNavMesh::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);

    int value0 = *(const int *)((const char *)this + 0x0C);
    int flag0 = 0;
    if (value0 & 1) {
        flag0 = 1;
    }
    cBase *ptr0;
    if (flag0 != 0) {
        ptr0 = 0;
    } else {
        ptr0 = (cBase *)value0;
    }
    wb.WriteBase(ptr0);

    int value1 = *(const int *)((const char *)this + 0x10);
    int flag1 = 0;
    if (value1 & 1) {
        flag1 = 1;
    }
    cBase *ptr1;
    if (flag1 != 0) {
        ptr1 = 0;
    } else {
        ptr1 = (cBase *)value1;
    }
    wb.WriteBase(ptr1);

    ((const gcDesiredValue *)((const char *)this + 0x14))->Write(wb);
    ((const gcDesiredValue *)((const char *)this + 0x18))->Write(wb);
    ((const cName *)((const char *)this + 0x1C))->Write(wb);
    ((const cName *)((const char *)this + 0x34))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 0x4C));
    wb.End();
}

int gcDoQueryNavMesh::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);
    if (rb[3] != 1 || ((gcAction *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    int sp14;
    int value0 = *(int *)((char *)this + 0x0C);
    int tag0 = value0 & 1;
    int flag0 = 0;
    if (tag0 != 0) {
        flag0 = 1;
    }

    int out0;
    if (flag0 != 0) {
        out0 = 0;
        goto out_done0;
    }
    out0 = value0;
out_done0:
    sp14 = out0;

    int flag0b = 0;
    if (tag0 != 0) {
        flag0b = 1;
    }

    int base0;
    if (flag0b != 0) {
        base0 = value0 & ~1;
    } else {
        base0 = *(int *)value0;
    }

    ((cReadBlock *)rb)->ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x0C),
                                 (cBase *)base0, *(cBase **)&sp14);

    int sp18;
    int value1 = *(int *)((char *)this + 0x10);
    int tag1 = value1 & 1;
    int new0;
    if (sp14 == 0) {
        new0 = base0 | 1;
    } else {
        new0 = sp14;
    }
    *(int *)((char *)this + 0x0C) = new0;

    int flag1 = 0;
    if (tag1 != 0) {
        flag1 = 1;
    }

    int out1;
    if (flag1 != 0) {
        out1 = 0;
        goto out_done1;
    }
    out1 = value1;
out_done1:
    sp18 = out1;

    int flag1b = 0;
    if (tag1 != 0) {
        flag1b = 1;
    }

    int base1;
    if (flag1b != 0) {
        base1 = value1 & ~1;
    } else {
        base1 = *(int *)value1;
    }

    ((cReadBlock *)rb)->ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x10),
                                 (cBase *)base1, *(cBase **)&sp18);

    int new1;
    if (sp18 == 0) {
        new1 = base1 | 1;
    } else {
        new1 = sp18;
    }
    *(int *)((char *)this + 0x10) = new1;

    ((gcDesiredValue *)((char *)this + 0x14))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x18))->Read(*(cReadBlock *)rb);
    ((cName *)((char *)this + 0x1C))->Read(*(cReadBlock *)rb);
    ((cName *)((char *)this + 0x34))->Read(*(cReadBlock *)rb);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x4C, 4);

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
