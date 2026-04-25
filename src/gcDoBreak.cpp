class cBase;
class cFile;
class cMemPool;
class cType;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" {
    void cStrCat(char *, const char *);
    void *cMemPool_GetPoolFromPtr(const void *);
    void gcAction_gcAction(void *, cBase *);
    void gcAction_Write(const void *, cFile &);
}

class gcAction {
public:
    cBase *mOwner;
    void *mVTable;
    unsigned int mNext;

    ~gcAction();
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *, cBase *(*)(cMemPool *, cBase *), const char *, const char *, unsigned int);
};

class gcDoBreak : public gcAction {
public:
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    float Evaluate(void) const;
    void GetText(char *) const;
    ~gcDoBreak();
    static void operator delete(void *p) {
        void *pool = cMemPool_GetPoolFromPtr(p);
        void *block = *(void **)((char *)pool + 0x24);
        DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
        short off = rec->offset;
        rec->fn((char *)block + off, p);
    }
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

gcDoBreak *dcast(const cBase *);

extern char gcDoBreakvirtualtable[];
extern const char gcDoBreak_base_name[];
extern const char gcDoBreak_base_desc[];

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoBreak;

struct gcDoBreakRoot {
    char pad[0x10];
    int *flags;
};

extern gcDoBreakRoot *g_gcDoBreak_root;

// 0x00294318, 72B
void gcDoBreak::AssignCopy(const cBase *other) {
    gcDoBreak *src = dcast(other);
    int v = ((int *)this)[2] & ~3;
    ((int *)this)[2] = v;
    ((int *)this)[2] = v | (((int *)src)[2] & 3);
}

// 0x00294360, 136B — static
cBase *gcDoBreak::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoBreak *result = 0;
    gcDoBreak *obj = (gcDoBreak *)entry->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        gcAction_gcAction(obj, parent);
        ((void **)obj)[1] = gcDoBreakvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// 0x002943e8, 280B
const cType *gcDoBreak::GetType(void) const {
    if (!type_gcDoBreak) {
        if (!type_action) {
            if (!type_expression) {
                if (!type_base) {
                    type_base = cType::InitializeType(gcDoBreak_base_name, gcDoBreak_base_desc, 1, 0, 0, 0, 0, 0);
                }
                type_expression = cType::InitializeType(0, 0, 0x6A, type_base, 0, 0, 0, 0);
            }
            type_action = cType::InitializeType(0, 0, 0x6B, type_expression, 0, 0, 0, 0);
        }
        type_gcDoBreak = cType::InitializeType(0, 0, 0xF5, type_action, gcDoBreak::New, 0, 0, 0);
    }
    return type_gcDoBreak;
}

// 0x00294500, 76B
void gcDoBreak::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcAction_Write(this, file);
    wb.End();
}

// 0x0029454c, 188B
int gcDoBreak::Read(cFile &file, cMemPool *pool) {
    register int ok = 1;
    int one = 1;
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != one || !gcAction::Read(file, pool)) {
        ((cFile *)rb._data[0])->SetCurrentPos((unsigned int)rb._data[1]);
        return 0;
    }
    return ok;
}

// 0x00294608, 44B
float gcDoBreak::Evaluate(void) const {
    int *flags = 0;
    gcDoBreakRoot *root = g_gcDoBreak_root;
    if (root != 0) {
        flags = root->flags;
    }
    *flags |= 2;
    return 1.0f;
}

// 0x00294634, 40B
void gcDoBreak::GetText(char *buf) const {
    cStrCat(buf, "break");
}

// 0x0029465c, 124B
gcDoBreak::~gcDoBreak() {
    ((void **)this)[1] = gcDoBreakvirtualtable;
}
