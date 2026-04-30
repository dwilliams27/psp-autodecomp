class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    cFile *file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void WriteBase(const cBase *);
    void End(void);
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class gcValue {
public:
    void Write(cFile &) const;
};

class gcValNavMeshDirection {
public:
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

typedef void (*EmbeddedWriteFn)(void *, cFile *);

struct EmbeddedWriteSlot {
    short offset;
    short pad;
    EmbeddedWriteFn fn;
};

struct EmbeddedTypeInfo {
    char pad[0x28];
    EmbeddedWriteSlot write;
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

void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);

cBase *gcValNavMeshDirection::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValNavMeshDirection *result = 0;
    gcValNavMeshDirection *obj =
        (gcValNavMeshDirection *)entry->fn(base, 0x64, 4, 0, 0);
    if (obj != 0) {
        register void *type0 asm("a0");
        register void *type1 asm("a0");
        register void *helper_desc asm("a0");

        ((void **)obj)[1] = (void *)(0x380000 - 6488);
        __asm__ volatile("lui %0,0x1" : "=r"(type0));
        ((cBase **)obj)[0] = parent;
        __asm__ volatile("addiu %0,%0,-0x6ac0" : "+r"(type0));
        ((void **)obj)[1] = type0;
        char *sub = (char *)obj + 8;
        gcDesiredObject_ctor(sub, obj);
        __asm__ volatile("lui %0,0x0" : "=r"(type1));
        __asm__ volatile("addiu %0,%0,0x338" : "+r"(type1));
        ((void **)obj)[3] = type1;
        gcDesiredEntityHelper_ctor((char *)obj + 0x14, 1, 0, 0);
        __asm__ volatile("lui %0,0x39\n\taddiu %0,%0,-0x75b8"
                         : "=r"(helper_desc));
        ((void **)obj)[8] = (void *)(0x380000 - 6488);
        ((void **)obj)[3] = helper_desc;
        ((char **)obj)[7] = sub;
        ((void **)obj)[8] = (void *)0x388568;
        ((char *)obj)[0x24] = 1;
        ((char *)obj)[0x25] = 0;
        ((int *)obj)[10] = 0;
        int sub_or_1 = (int)sub | 1;
        __asm__ volatile("" : "+r"(sub_or_1) :: "memory");
        ((int *)obj)[11] = 0;
        int obj_or_1 = (int)obj | 1;
        __asm__ volatile("" : "+r"(obj_or_1) :: "memory");
        ((int *)obj)[12] = sub_or_1;
        int field38 = 0xF;
        __asm__ volatile("" : "+r"(field38) :: "memory");
        ((int *)obj)[13] = obj_or_1;
        ((int *)obj)[14] = field38;
        ((int *)obj)[15] = 0;
        ((short *)obj)[0x2A] = 0;
        ((short *)obj)[0x2B] = 0;
        ((char *)obj)[0x40] = 0;
        ((int *)obj)[22] = 0;
        ((int *)obj)[23] = 0;
        ((int *)obj)[24] = 0;
        result = obj;
    }
    return (cBase *)result;
}

void gcValNavMeshDirection::Write(cFile &file_arg) const {
    cWriteBlock wb(file_arg, 0xC);
    ((const gcValue *)this)->Write(file_arg);

    EmbeddedTypeInfo *type0 = *(EmbeddedTypeInfo **)((char *)this + 0xC);
    EmbeddedWriteSlot *slot0 = &type0->write;
    char *base0 = (char *)this + 8;
    slot0->fn(base0 + slot0->offset, wb.file);

    int val0 = *(const int *)((const char *)this + 0x34);
    int flag0 = 0;
    if (val0 & 1) {
        flag0 = 1;
    }
    cBase *ptr0;
    if (flag0 != 0) {
        ptr0 = 0;
    } else {
        ptr0 = (cBase *)val0;
    }
    wb.WriteBase(ptr0);
    wb.Write(*(const int *)((const char *)this + 0x38));
    wb.Write(*(const unsigned int *)((const char *)this + 0x3C));
    ((const cName *)((const char *)this + 0x40))->Write(wb);
    wb.Write(*(const int *)((const char *)this + 0x58));
    wb.Write(*(const int *)((const char *)this + 0x5C));
    wb.Write(*(const int *)((const char *)this + 0x60));
    wb.End();
}
