class cBase;
class cFile;
class cMemPool;
class cType;

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
};

class gcDesiredValue {
public:
    void Write(cWriteBlock &) const;
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
    const cType *GetType(void) const;
    void Write(cFile &) const;
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

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcAction_Write(const void *, cFile &);
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
