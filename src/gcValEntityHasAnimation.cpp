class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    cFile *_file;
    int _pos;
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

typedef void (*EmbeddedWriteFn)(cBase *, cFile *);

struct EmbeddedWriteSlot {
    short offset;
    short _pad;
    EmbeddedWriteFn fn;
};

struct EmbeddedTypeInfo {
    char _pad[0x28];
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

class gcValEntityHasAnimation {
public:
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    void Write(cFile &) const;
};

void gcValue_Write(const void *, cFile &);
void gcDesiredObject_ctor(void *, void *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);
extern char D_00007108[];
extern char D_00000338[];

static cType *type_base asm("D_000385DC");
static cType *type_expression asm("D_000385D8");
static cType *type_value asm("D_0009F3E8");
static cType *type_gcValEntityHasAnimation asm("D_0009F808");

const cType *gcValEntityHasAnimation::GetType(void) const {
    if (!type_gcValEntityHasAnimation) {
        if (!type_value) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType((const char *)0x36D894,
                                                      (const char *)0x36D89C,
                                                      1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base,
                                                        0, 0, 0, 0);
            }
            type_value = cType::InitializeType(0, 0, 0x6C, type_expression,
                                               0, 0, 0, 0x80);
        }
        type_gcValEntityHasAnimation = cType::InitializeType(
            0, 0, 0x111, type_value, gcValEntityHasAnimation::New, 0, 0, 0);
    }
    return type_gcValEntityHasAnimation;
}

cBase *gcValEntityHasAnimation::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcValEntityHasAnimation *result = 0;
    gcValEntityHasAnimation *obj =
        (gcValEntityHasAnimation *)entry->fn(base, 0x4C, 4, 0, 0);
    if (obj != 0) {
        register void *type0 asm("a0");
        register void *type1 asm("a0");
        register void *helper_desc asm("a0");

        ((void **)obj)[1] = (void *)(0x380000 - 6488);
        __asm__ volatile("lui %0,0x0" : "=r"(type0));
        ((cBase **)obj)[0] = parent;
        __asm__ volatile("addiu %0,%0,0x7108" : "+r"(type0));
        ((void **)obj)[1] = type0;
        char *sub = (char *)obj + 8;
        gcDesiredObject_ctor(sub, obj);
        __asm__ volatile("lui %0,0x0\n\taddiu %0,%0,0x338" : "=r"(type1));
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
        ((int *)obj)[11] = 0;
        int sub_bits = (int)sub | 1;
        ((void **)obj)[14] = (void *)(0x380000 - 6488);
        ((int *)obj)[12] = sub_bits;
        ((gcValEntityHasAnimation **)obj)[13] = obj;
        ((void **)obj)[14] = (void *)0x388568;
        ((char *)obj)[0x3C] = 1;
        ((char *)obj)[0x3D] = 0;
        ((int *)obj)[16] = 0;
        int self_bits = (int)obj | 1;
        ((int *)obj)[17] = 0;
        ((int *)obj)[18] = self_bits;
        result = obj;
    }
    return (cBase *)result;
}

void gcValEntityHasAnimation::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcValue_Write(this, file);

    EmbeddedTypeInfo *type0 = *(EmbeddedTypeInfo **)((char *)this + 0xC);
    EmbeddedWriteSlot *slot0 = &type0->write;
    cBase *embedded0 = (cBase *)((char *)this + 8);
    slot0->fn((cBase *)((char *)embedded0 + slot0->offset), wb._file);

    EmbeddedTypeInfo *type1 = *(EmbeddedTypeInfo **)((char *)this + 0x38);
    EmbeddedWriteSlot *slot1 = &type1->write;
    cBase *embedded1 = (cBase *)((char *)this + 0x34);
    slot1->fn((cBase *)((char *)embedded1 + slot1->offset), wb._file);

    wb.End();
}
