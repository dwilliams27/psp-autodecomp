// Batch of unrelated functions grouped by the orchestrator into one TU.
// Sections are independent.

// ─────────────────────────────────────────────────────────────────────────
// Function 1: cLanguageSubscriber::GetIndex(void *) const  @ 0x001c75d0
// Function 2: cLanguageSubscriber::Detach(void)            @ 0x001c759c
// Function 3: cLanguageSubscriber::Write(cFile &) const    @ 0x001c7464
// Function 4: cLanguageSubscriber::AssignCopy(const cBase *) @ 0x001c72a0
// Function 5: cLanguageSubscriber::New(cMemPool *, cBase *) static @ 0x001c7300
// ─────────────────────────────────────────────────────────────────────────

class cBase;
class cFile;
class cMemPool;

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
    void Write(cFile &) const;
};

class cLanguage {
public:
    enum gcSubscription {};
    static int GetLanguageIndex(void *);
    static void Unsubscribe(gcSubscription, cListSubscriber *);
};

class cLanguageSubscriber : public cListSubscriber {
public:
    int GetIndex(void *) const;
    void Detach(void);
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
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
