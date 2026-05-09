// gcDoMouseOp - gcAll_psp.obj
//   0x002e7814 gcDoMouseOp::New(cMemPool *, cBase *) static
//   0x002e79f4 gcDoMouseOp::Write(cFile &) const

class cBase;
class cFile;
class cMemPool;
class cReadBlock;

class cReadBlock {
public:
    cFile *file;
    unsigned int _pos;
    int _pad[3];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class gcExpression {
public:
    void Write(cFile &) const;
};

class gcAction : public gcExpression {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class cHandle {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &, cMemPool *);
};

struct cTypeMethod {
    short offset;
    short pad;
    void *fn;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
    char pad[0x28];
    cTypeMethod write_m;
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

typedef void (*WriteFn)(void *, cFile *);
typedef void (*ReadFn)(void *, cFile *, void *);
typedef void (*TextFn)(void *, char *);

struct PoolDeleteSlot {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct DtorSlot {
    short offset;
    short pad;
    void (*fn)(void *, int);
};

extern "C" void gcAction_gcAction(void *, cBase *);
extern "C" void gcDesiredObject_gcDesiredObject(void *, void *);
extern "C" void gcAction___dtor_gcAction_void(void *, int);
extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void cFileSystem_Read(void *, void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
void *cMemPool_GetPoolFromPtr(const void *);
void cStrAppend(char *, const char *, ...);

extern char gcDoMouseOpvirtualtable[];
extern char D_000006F8[];
extern char D_003898A0[];
extern const char gcDoMouseOp_base_name[] asm("D_0036D894");
extern const char gcDoMouseOp_base_desc[] asm("D_0036D89C");

static cType *type_action asm("D_000385D4");
static cType *type_expression asm("D_000385D8");
static cType *type_base asm("D_000385DC");
static cType *type_gcDoMouseOp asm("D_0009F6A8");

class gcDoMouseOp : public gcAction {
public:
    int mMode;
    unsigned int mFlags;
    cHandle mHandle;

    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *);
    const cType *GetType(void) const;
    void GetText(char *) const;
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
    ~gcDoMouseOp(void);
};

inline void gcDoMouseOp::operator delete(void *ptr) {
    void *pool = cMemPool_GetPoolFromPtr(ptr);
    void *block = *(void **)((char *)pool + 0x24);
    char *entries = *(char **)((char *)block + 0x1C);
    PoolDeleteSlot *slot = (PoolDeleteSlot *)(entries + 0x30);
    slot->fn((char *)block + slot->offset, ptr);
}

// 0x002e7814 - gcDoMouseOp::New(cMemPool *, cBase *) static
cBase *gcDoMouseOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoMouseOp *result = 0;
    gcDoMouseOp *obj = (gcDoMouseOp *)entry->fn(base, 0x2C, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoMouseOpvirtualtable;
        ((int *)obj)[3] = 0;
        ((int *)obj)[4] = -1;
        ((int *)obj)[5] = 0;
        gcDesiredObject_gcDesiredObject((char *)obj + 0x18, obj);
        ((void **)obj)[7] = D_000006F8;
        ((int *)obj)[9] = 7;
        ((int *)obj)[10] = 0;
        ((void **)obj)[7] = D_003898A0;
        result = obj;
    }
    return (cBase *)result;
}

// 0x002e78dc - gcDoMouseOp::GetType(void) const
const cType *gcDoMouseOp::GetType(void) const {
    if (!type_gcDoMouseOp) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(
                        gcDoMouseOp_base_name, gcDoMouseOp_base_desc,
                        1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(
                    0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(
                0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoMouseOp = cType::InitializeType(
            0, 0, 0x20C, type_action, gcDoMouseOp::New, 0, 0, 0);
    }
    return type_gcDoMouseOp;
}

// 0x002e79f4 - gcDoMouseOp::Write(cFile &) const
void gcDoMouseOp::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    gcAction::Write(file);
    wb.Write(*(int *)((const char *)this + 0x0C));
    wb.Write(*(unsigned int *)((const char *)this + 0x10));
    ((const cHandle *)((const char *)this + 0x14))->Write(wb);

    const cTypeMethod *entry =
        &((const cType *)*(void *const *)((const char *)this + 0x1C))->write_m;
    char *base = (char *)this + 0x18;
    ((WriteFn)entry->fn)(base + entry->offset, *(cFile **)&wb._data[0]);

    wb.End();
}

// 0x002e7a84 - gcDoMouseOp::Read(cFile &, cMemPool *)
int gcDoMouseOp::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    int tag = rb[3];
    if ((unsigned int)tag >= 3 || (unsigned int)tag < 1 ||
        gcAction::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }
    cFileSystem_Read(*(void **)rb[0], (char *)this + 0x0C, 4);
    cFileSystem_Read(*(void **)rb[0], (char *)this + 0x10, 4);
    cHandle *handle = (cHandle *)((char *)this + 0x14);
    *(int *)((char *)this + 0x14) = 0;
    handle->Read(*(cReadBlock *)rb, (cMemPool *)cMemPool_GetPoolFromPtr(handle));
    if ((unsigned int)rb[3] >= 2) {
        char *typeInfo = *(char **)((char *)this + 0x1C);
        void *base = (char *)this + 0x18;
        cTypeMethod *slot = (cTypeMethod *)(typeInfo + 0x30);
        short off = slot->offset;
        ((ReadFn)slot->fn)((char *)base + off, *(cFile **)&rb[0],
                           cMemPool_GetPoolFromPtr(base));
    }
    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// 0x002e7cd8 - gcDoMouseOp::GetText(char *) const
void gcDoMouseOp::GetText(char *buf) const {
    cStrAppend(buf, (const char *)0x36ECB8, (const char *)0x36DAF0);
    if (*(const int *)((const char *)this + 0x0C) == 3) {
        const cTypeMethod *entry =
            (const cTypeMethod *)((const char *)*(void *const *)((const char *)this + 0x1C) + 0x78);
        char *base = (char *)this + 0x18;
        ((TextFn)entry->fn)(base + entry->offset, buf);
        cStrAppend(buf, (const char *)0x36E440);
    }
    cStrAppend(buf, (const char *)0x36EBE4);
}

// Original object keeps this dead branch tail inside the destructor symbol.
__asm__(".word 0x1000ffff\n");
__asm__(".word 0x00000000\n");

gcDoMouseOp::~gcDoMouseOp(void) {
    *(void **)((char *)this + 4) = gcDoMouseOpvirtualtable;

    if ((void *)((char *)this + 0x18) != 0) {
        *(void * volatile *)((char *)this + 0x1C) = D_003898A0;
        *(void * volatile *)((char *)this + 0x1C) = D_000006F8;
        *(void * volatile *)((char *)this + 0x1C) = (void *)0x3889A8;
        if ((void *)((char *)this + 0x20) != 0) {
            int owned = 1;
            int val = *(int *)((char *)this + 0x20);
            if (val & 1) {
                owned = 0;
            }
            if (owned != 0 && val != 0) {
                char *typeInfo = *(char **)(val + 4);
                DtorSlot *slot = (DtorSlot *)(typeInfo + 0x50);
                slot->fn((char *)val + slot->offset, 3);
                *(int *)((char *)this + 0x20) = 0;
            }
        }
        *(void **)((char *)this + 0x1C) = (void *)0x37E6A8;
    }

    gcAction___dtor_gcAction_void(this, 0);
}
