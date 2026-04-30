// gcDoEntityFindAttached.cpp — decompiled from gcAll_psp.obj
//
//   0x002b0b00  Write(cFile &) const                  — 88B
//   0x002b0d50  GetText(char *) const                 — 104B
//   0x002b0db8  ~gcDoEntityFindAttached(void)         — 124B
//
// Derives from gcDoEntityFindAttachedBase. Vtable at offset 0x4. Same
// pattern as gcDoEntityForEachAttached / gcValEntityPrimaryController.

class cBase;
class cFile;
class cFileHandle;
class cMemPool;

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
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcDoEntityFindAttachedBase {
public:
    char _padToC[0xC];
    int mFieldC;    // 0x0C — accessed by derived GetText
    char _padCto64[0x54];
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void GetText(char *, char *, bool) const;
    ~gcDoEntityFindAttachedBase(void);
};

class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcDesiredEntityTemplate {
public:
    gcDesiredEntityTemplate &operator=(const gcDesiredEntityTemplate &);
};

class gcDesiredObject {
public:
    gcDesiredObject(cBase *);
};

class gcDesiredEntityHelper {
public:
    enum gcPrimary {
        kPrimary = 1
    };

    enum gcRelationship {
        kRelationship0 = 0
    };

    gcDesiredEntityHelper(gcPrimary, gcRelationship, gcRelationship);
};

class gcEnumeration;
class gcEnumerationEntry;

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cSubHandleT {
public:
    int mIndex;
};

template <class T, class U>
class cHandlePairT {
public:
    cHandleT<T> mHandle;
    U mSubHandle;
};

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class gcDoEntityFindAttached : public gcDoEntityFindAttachedBase {
public:
    int mField64;   // 0x64
    void AssignCopy(const cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void GetText(char *) const;
    ~gcDoEntityFindAttached(void);
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

void cStrAppend(char *, const char *, ...);

extern char gcDoEntityFindAttachedvirtualtable[];
extern char D_00000338[];
extern const char gcDoEntityFindAttached_text_fmt[];
extern const char gcDoEntityFindAttached_text_arg[];
extern const char gcDoEntityFindAttached_text_label[];

template <class T> T *dcast(const cBase *);

extern "C" {
void gcAction_ctor_cBase(void *, cBase *);
void gcDesiredObject_ctor_cBase(void *, cBase *);
void gcDesiredEntityHelper_ctor(void *, int, int, int);
}

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

static cType *type_base;
static cType *type_expression;
static cType *type_action;
static cType *type_gcDoEntityFindAttachedBase;
static cType *type_gcDoEntityFindAttached;

// ── AssignCopy @ 0x002b07b8 ────────────────────────────────────────────
void gcDoEntityFindAttached::AssignCopy(const cBase *src) {
    gcDoEntityFindAttached *other = dcast<gcDoEntityFindAttached>(src);
    int flags = *(int *)((char *)this + 8) & ~3;
    *(int *)((char *)this + 8) = flags;
    *(int *)((char *)this + 8) = flags | (*(int *)((char *)other + 8) & 3);
    *(int *)((char *)this + 0x0C) = *(int *)((char *)other + 0x0C);
    ((gcDesiredEntity *)((char *)this + 0x10))->operator=(
        *(const gcDesiredEntity *)((char *)other + 0x10));
    *(int *)((char *)this + 0x3C) = *(int *)((char *)other + 0x3C);
    *(int *)((char *)this + 0x40) = *(int *)((char *)other + 0x40);
    *(int *)((char *)this + 0x44) = *(int *)((char *)other + 0x44);
    ((gcDesiredEntityTemplate *)((char *)this + 0x48))->operator=(
        *(const gcDesiredEntityTemplate *)((char *)other + 0x48));
    ((cArrayBase<cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > > *)((char *)this + 0x60))->operator=(
        *(const cArrayBase<cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > > *)((char *)other + 0x60));
    mField64 = other->mField64;
}

// ── Write @ 0x002b0b00 ─────────────────────────────────────────────────
void gcDoEntityFindAttached::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcDoEntityFindAttachedBase::Write(file);
    wb.Write(mField64);
    wb.End();
}

// ── Read @ 0x002b0b58 ──────────────────────────────────────────────────
int gcDoEntityFindAttached::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1) goto fail;
    if (gcDoEntityFindAttachedBase::Read(file, pool) != 0) goto success;
fail:
    ((cFile *)rb._data[0])->SetCurrentPos((unsigned int)rb._data[1]);
    return 0;
success:
    cFileSystem::Read(*(cFileHandle **)rb._data[0], &mField64, 4);
    return result;
}

// ── GetText @ 0x002b0d50 ───────────────────────────────────────────────
void gcDoEntityFindAttached::GetText(char *buf) const {
    cStrAppend(buf,
               gcDoEntityFindAttached_text_fmt,
               gcDoEntityFindAttached_text_arg);
    gcDoEntityFindAttachedBase::GetText(buf,
                                        (char *)gcDoEntityFindAttached_text_label,
                                        mFieldC == 1);
}

// ── GetType @ 0x002b09ac ───────────────────────────────────────────────
const cType *gcDoEntityFindAttached::GetType(void) const {
    if (!type_gcDoEntityFindAttached) {
        if (!type_gcDoEntityFindAttachedBase) {
            if (!type_action) {
                if (!type_expression) {
                    if (!type_base) {
                        type_base = cType::InitializeType(
                            (const char *)0x36D894,
                            (const char *)0x36D89C,
                            1, 0, 0, 0, 0, 0);
                    }
                    type_expression = cType::InitializeType(
                        0, 0, 0x6A, type_base, 0, 0, 0, 0);
                }
                type_action = cType::InitializeType(
                    0, 0, 0x6B, type_expression, 0, 0, 0, 0);
            }
            type_gcDoEntityFindAttachedBase = cType::InitializeType(
                0, 0, 0x157, type_action, 0, 0, 0, 0);
        }
        type_gcDoEntityFindAttached = cType::InitializeType(
            0, 0, 0x159, type_gcDoEntityFindAttachedBase,
            gcDoEntityFindAttached::New, 0, 0, 0);
    }
    return type_gcDoEntityFindAttached;
}

// ── New @ 0x002b0858 ───────────────────────────────────────────────────
cBase *gcDoEntityFindAttached::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcDoEntityFindAttached *result = 0;
    gcDoEntityFindAttached *obj =
        (gcDoEntityFindAttached *)entry->fn(base, 0x68, 4, 0, 0);
    if (obj != 0) {
        gcAction_ctor_cBase(obj, parent);

        int *obj_i = (int *)obj;
        obj_i[1] = 0x38BCA8;

        void *desobj = (char *)obj + 0x10;
        obj_i[3] = 0;
        gcDesiredObject_ctor_cBase(desobj, (cBase *)obj);

        obj_i[5] = (int)D_00000338;

        gcDesiredEntityHelper_ctor((char *)obj + 0x1C, 1, 0, 0);

        obj_i[5] = 0x388A48;
        obj_i[10] = 0x37E6A8;
        obj_i[9] = (int)desobj;
        obj_i[10] = 0x388568;
        ((char *)obj)[0x2C] = 1;
        ((char *)obj)[0x2D] = 0;
        obj_i[12] = 0;
        obj_i[13] = 0;
        obj_i[14] = (int)desobj | 1;
        obj_i[15] = 3;
        obj_i[16] = 2;
        obj_i[17] = 0;

        gcDesiredObject_ctor_cBase((char *)obj + 0x48, (cBase *)obj);

        obj_i[19] = 0x389508;
        obj_i[21] = 7;
        obj_i[22] = 0;
        obj_i[23] = (int)obj | 1;
        obj_i[24] = 0;
        obj_i[1] = (int)gcDoEntityFindAttachedvirtualtable;
        obj_i[25] = 0;

        result = obj;
    }
    return (cBase *)result;
}

// ── ~gcDoEntityFindAttached @ 0x002b0db8 ───────────────────────────────
gcDoEntityFindAttached::~gcDoEntityFindAttached(void) {
    *(void **)((char *)this + 4) = gcDoEntityFindAttachedvirtualtable;
}
