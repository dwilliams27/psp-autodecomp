// Batch of unrelated functions grouped by the orchestrator into one TU.
// Sections are independent.

// ─────────────────────────────────────────────────────────────────────────
// Function 1: cLanguageSubscriber::GetIndex(void *) const  @ 0x001c75d0
// Function 2: cLanguageSubscriber::Detach(void)            @ 0x001c759c
// Function 3: cLanguageSubscriber::Write(cFile &) const    @ 0x001c7464
// Function 4: cLanguageSubscriber::AssignCopy(const cBase *) @ 0x001c72a0
// Function 5: cLanguageSubscriber::New(cMemPool *, cBase *) static @ 0x001c7300
// Function 6: cLanguageSubscriber::~cLanguageSubscriber()   @ 0x001c7608
// Function 7: cLanguageSubscriber::Read(cFile &, cMemPool *)@ 0x001c74b0
// Function 8: cLanguageSubscriber::GetType() const          @ 0x001c7388
// ─────────────────────────────────────────────────────────────────────────

class cBase;
class cFile;
class cMemPool;
class cType;

void cFile_SetCurrentPos(void *, unsigned int);

struct cLS_PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct cLS_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cLS_cMemPoolNS {
public:
    static cLS_cMemPoolNS *GetPoolFromPtr(const void *);
};

extern char cBaseclassdesc[];                            // @ 0x37E6A8

class cListSubscriber {
public:
    cBase *mOwner;            // 0x00
    void *mTypeInfo;          // 0x04
    void *mNext;              // 0x08
    void *mPrev;              // 0x0C
    unsigned char mAttached;  // 0x10
    int mFirst;               // 0x14
    int mLast;                // 0x18
    int mCount;               // 0x1C
    int mNumVisible;          // 0x20

    cListSubscriber(cBase *);
    void Detach(void);
    int  Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class cLanguage {
public:
    enum gcSubscription {};
    static int GetLanguageIndex(void *);
    static void Unsubscribe(gcSubscription, cListSubscriber *);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cLanguageSubscriber : public cListSubscriber {
public:
    int GetIndex(void *) const;
    void Detach(void);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    ~cLanguageSubscriber();
    int  Read(cFile &, cMemPool *);
    const cType *GetType() const;

    // Inline so SNC inlines this into the deleting-destructor variant.
    static void operator delete(void *p) {
        cLS_cMemPoolNS *pool = cLS_cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        cLS_DeleteRecord *rec =
            (cLS_DeleteRecord *)(((cLS_PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

cLanguageSubscriber *dcast(const cBase *);

extern char cLanguageSubscribervirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern "C" void cListSubscriber_cListSubscriber(void *, cBase *);

int cLanguageSubscriber::GetIndex(void *p) const {
    return cLanguage::GetLanguageIndex(p);
}

void cLanguageSubscriber::Detach(void) {
    cLanguage::Unsubscribe((cLanguage::gcSubscription)0, this);
    cListSubscriber::Detach();
}

// ── cLanguageSubscriber::Write(cFile &) const  @ 0x001c7464, 76B ──
void cLanguageSubscriber::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cListSubscriber *)this)->Write(file);
    wb.End();
}

// ── cLanguageSubscriber::AssignCopy(const cBase *)  @ 0x001c72a0, 96B ──
void cLanguageSubscriber::AssignCopy(const cBase *base) {
    cLanguageSubscriber *other = dcast(base);
    mNext = other->mNext;
    mPrev = other->mPrev;
    mAttached = other->mAttached;
    mFirst = other->mFirst;
    mLast = other->mLast;
    mCount = other->mCount;
    mNumVisible = other->mNumVisible;
}

// ── cLanguageSubscriber::New(cMemPool *, cBase *) static  @ 0x001c7300, 136B ──
cBase *cLanguageSubscriber::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    cLanguageSubscriber *result = 0;
    cLanguageSubscriber *obj = (cLanguageSubscriber *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        cListSubscriber_cListSubscriber(obj, parent);
        *(void **)((char *)obj + 4) = cLanguageSubscribervirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ─────────────────────────────────────────────────────────────────────────
// Function 3: eFontGroup::IsManagedTypeExternal(void) const @ 0x001dd40c
// ─────────────────────────────────────────────────────────────────────────

class eFontGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

bool eFontGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ─────────────────────────────────────────────────────────────────────────
// Function 4: gcConstantGroup::IsManagedTypeExternal(void) const @ 0x002365ec
// ─────────────────────────────────────────────────────────────────────────

class gcConstantGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

bool gcConstantGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ─────────────────────────────────────────────────────────────────────────
// Function 5: gcExternalCinematicGroup::IsManagedTypeExternal(void) const @ 0x00238a04
// ─────────────────────────────────────────────────────────────────────────

class gcExternalCinematicGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

bool gcExternalCinematicGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

// ─────────────────────────────────────────────────────────────────────────
// cLanguageSubscriber::~cLanguageSubscriber(void)  @ 0x001c7608, 100B
// ─────────────────────────────────────────────────────────────────────────
cLanguageSubscriber::~cLanguageSubscriber() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

// ─────────────────────────────────────────────────────────────────────────
// cLanguageSubscriber::Read(cFile &, cMemPool *)  @ 0x001c74b0, 188B
// ─────────────────────────────────────────────────────────────────────────
int cLanguageSubscriber::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] != 1) goto fail;
    if (this->cListSubscriber::Read(file, pool)) goto success;
fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ─────────────────────────────────────────────────────────────────────────
// cLanguageSubscriber::GetType(void) const  @ 0x001c7388, 220B
// ─────────────────────────────────────────────────────────────────────────
extern const char cLanguageSubscriber_cBase_name[];   // @ 0x36C830 ("cBase")
extern const char cLanguageSubscriber_cBase_desc[];   // @ 0x36C838 ("Base")

static cType *type_cBase;
static cType *type_cListSubscriber;
static cType *type_cLanguageSubscriber;

const cType *cLanguageSubscriber::GetType() const {
    if (!type_cLanguageSubscriber) {
        if (!type_cListSubscriber) {
            if (!type_cBase) {
                type_cBase = cType::InitializeType(
                    cLanguageSubscriber_cBase_name, cLanguageSubscriber_cBase_desc,
                    1, 0, 0, 0, 0, 0);
            }
            type_cListSubscriber = cType::InitializeType(
                0, 0, 0x187, type_cBase, 0, 0, 0, 0);
        }
        type_cLanguageSubscriber = cType::InitializeType(
            0, 0, 0xEF, type_cListSubscriber,
            (cBase *(*)(cMemPool *, cBase *))&cLanguageSubscriber::New,
            0, 0, 0);
    }
    return type_cLanguageSubscriber;
}
