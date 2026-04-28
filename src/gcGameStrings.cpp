// Days of Thunder decompilation: gcGameStrings methods

class cBase;
class cFile;
class cMemPool;

class cBase {
public:
    void *_b0;
    void *_b4;
};

void cFile_SetCurrentPos(void *, unsigned int);

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

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class gcStringValue : public cBase {
public:
    void Write(cFile &) const;
    int  Read(cFile &, cMemPool *);
};

extern char cBaseclassdesc[];                         // @ 0x37E6A8
extern char gcGameStringsvirtualtable[];              // @ 0x000C58 (PRX, relocated)

extern const char gcGameStrings_fmt[];
extern const char gcGameStrings_name[];

// Pool-block layout used by the deleting-destructor tail and New.
struct gcGS_PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct gcGS_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct gcGS_AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcGS_cMemPoolNS {
public:
    static gcGS_cMemPoolNS *GetPoolFromPtr(const void *);
};

class gcGameStrings : public gcStringValue {
public:
    int mValue;  // offset 8

    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void GetName(char *) const;
    void VisitReferences(unsigned int, cBase *, void (*)(cBase *, unsigned int, void *), void *, unsigned int);
    int  Read(cFile &, cMemPool *);
    static cBase *New(cMemPool *, cBase *);
    ~gcGameStrings();

    // Inline so SNC inlines this into the deleting-destructor variant.
    static void operator delete(void *p) {
        gcGS_cMemPoolNS *pool = gcGS_cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        gcGS_DeleteRecord *rec =
            (gcGS_DeleteRecord *)(((gcGS_PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

gcGameStrings *dcast(const cBase *);
void cStrAppend(char *, const char *, ...);

// ============================================================
// gcGameStrings::AssignCopy(const cBase *)  @ 0x0027da78
// ============================================================
void gcGameStrings::AssignCopy(const cBase *base) {
    gcGameStrings *src = dcast(base);
    this->mValue = src->mValue;
}

// ============================================================
// gcGameStrings::Write(cFile &) const  @ 0x0027dc0c
// ============================================================
void gcGameStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->gcStringValue::Write(file);
    wb.Write(this->mValue);
    wb.End();
}

// ============================================================
// gcGameStrings::GetName(char *) const  @ 0x0027ddb4
// ============================================================
void gcGameStrings::GetName(char *buf) const {
    cStrAppend(buf, gcGameStrings_fmt, gcGameStrings_name);
}

// ============================================================
// gcGameStrings::VisitReferences  @ 0x0027dde4
// ============================================================
void gcGameStrings::VisitReferences(unsigned int flags, cBase *ctx,
                                    void (*cb)(cBase *, unsigned int, void *),
                                    void *user, unsigned int mask) {
    if (cb != 0) {
        cb(ctx, (unsigned int)(void *)this, user);
    }
}

// ============================================================
// gcGameStrings::~gcGameStrings(void)  @ 0x0027de20, 100B
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0)
// guard and the deleting-tail dispatch through operator delete on
// (flag & 1). Body resets the classdesc pointer at offset 4 to the
// parent (cBase) classdesc.
// ============================================================
gcGameStrings::~gcGameStrings() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

// ============================================================
// gcGameStrings::New(cMemPool *, cBase *) static  @ 0x0027daa8, 136B
// ============================================================
cBase *gcGameStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((gcGS_PoolBlock *)block)->allocTable;
    gcGS_AllocEntry *entry = (gcGS_AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcGameStrings *result = 0;
    gcGameStrings *obj = (gcGameStrings *)entry->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        // cBase::cBase(parent) inlined: writes classdesc then parent.
        *(void **)((char *)obj + 4) = cBaseclassdesc;
        *(cBase **)obj = parent;
        // Derived vtable + mValue init.
        *(void **)((char *)obj + 4) = gcGameStringsvirtualtable;
        obj->mValue = 0;
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// gcGameStrings::Read(cFile &, cMemPool *)  @ 0x0027dc64, 208B
// ============================================================
int gcGameStrings::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcStringValue::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 8, 4);
    return result;
}
