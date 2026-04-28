// Batch of small methods assigned to this session.

class cBase;
class cFile;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class nwNetwork {
public:
    static void *GetLobby(void);
};

template <class T> T *dcast(const cBase *);

void cStrAppend(char *, const char *, ...);

extern const char gcValLobbyOptions_fmt[];  // @ 0x36F50C
extern const char gcValGetText_text[];      // @ 0x36DAF0
extern char cBaseclassdesc[];               // @ 0x37E6A8

struct DispatchEntry {
    short offset;
    short pad;
    int (*fn)(void *);
};

// ─────────────────────────────────────────────────────────────────────────
// Support classes for gcValLobbyOptions Read/Write/New
// ─────────────────────────────────────────────────────────────────────────

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

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

extern char gcLValuevirtualtable[];
extern char gcValLobbyOptionsvirtualtable[];

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcLValue {
public:
    cBase *mParent;
    void *mVtable;
    gcLValue(cBase *parent);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcValLobbyOptions : public gcLValue {
public:
    int mField8;

    gcValLobbyOptions(cBase *parent);
    ~gcValLobbyOptions();
    void GetText(char *) const;
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    float Evaluate(void) const;
    static cBase *New(cMemPool *, cBase *);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        AllocEntry *rec = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void *(*fn)(void *, int, int, int, int) = rec->fn;
        ((void (*)(void *, void *))fn)(base, p);
    }
};

inline gcLValue::gcLValue(cBase *parent) {
    mVtable = gcLValuevirtualtable;
    mParent = parent;
}

inline gcValLobbyOptions::gcValLobbyOptions(cBase *parent) : gcLValue(parent) {
    mVtable = gcValLobbyOptionsvirtualtable;
    mField8 = 0;
}

inline void *operator new(unsigned, void *p) { return p; }

// ── gcValLobbyOptions::GetText(char *) const @ 0x0034ae98 ──
void gcValLobbyOptions::GetText(char *buf) const {
    cStrAppend(buf, gcValLobbyOptions_fmt, gcValGetText_text);
}

// ── gcValLobbyOptions::New(cMemPool *, cBase *) static @ 0x0034a9f4 ──
cBase *gcValLobbyOptions::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcValLobbyOptions *result = 0;
    gcValLobbyOptions *obj = (gcValLobbyOptions *)e->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValLobbyOptions(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcValLobbyOptions::Write(cFile &) const @ 0x0034abd0 ──
void gcValLobbyOptions::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue::Write(file);
    wb.Write(mField8);
    wb.End();
}

// ── gcValLobbyOptions::Read(cFile &, cMemPool *) @ 0x0034ac28 ──
int gcValLobbyOptions::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && ((gcLValue *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    {
        char *bufp = (char *)this + 8;
        void *h = *(void **)rb._data[0];
        cFileSystem::Read(h, bufp, 4);
    }
    return result;
}

// ── gcValLobbyOptions::Evaluate(void) const @ 0x0034acf8 ──
float gcValLobbyOptions::Evaluate(void) const {
    void *lobby = nwNetwork::GetLobby();
    DispatchEntry *e;
    short off;
    int (*fn)(void *);
    int field8;
    int status;

    if (lobby == 0) goto retZeroLobby;
    field8 = ((const int *)this)[2];
    if (field8 > 0) goto entryOne;
    if (field8 >= 0) goto entryZero;
retZeroNeg:
    return 0.0f;
retZeroLobby:
    return 0.0f;
entryOne:
    if (field8 >= 2) goto retZeroNeg;
    e = (DispatchEntry *)(*(char **)lobby + 0x4A0);
    off = e->offset;
    fn = e->fn;
    status = fn((char *)lobby + off);
    return (float)status;
entryZero:
    e = (DispatchEntry *)(*(char **)lobby + 0x498);
    off = e->offset;
    fn = e->fn;
    status = fn((char *)lobby + off);
    return (float)status;
}

// ── gcValLobbyOptions::~gcValLobbyOptions(void) @ 0x0034aec8 ──
gcValLobbyOptions::~gcValLobbyOptions() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

// gcUIMarqueeControl::OnTextChanged(void) @ 0x0013c314
class gcUITextControl {
public:
    void OnTextChanged(void);
};

class gcUIMarqueeControl : public gcUITextControl {
public:
    char _pad0[0x118];
    struct MarqueeState {
        int f118;
        int f11C;
        float f120;
    } mState;
    void OnTextChanged(void);
};

void gcUIMarqueeControl::OnTextChanged(void) {
    gcUITextControl::OnTextChanged();
    mState.f120 = 0.0f;
    MarqueeState *state = &mState;
    state->f118 = 0;
}

// eAnimationGroup::AssignCopy(const cBase *) @ 0x001dc89c
class eAnimationGroup {
public:
    char _pad0[8];
    unsigned char f8;
    char _pad9[3];
    int fC;
    void AssignCopy(const cBase *);
};

void eAnimationGroup::AssignCopy(const cBase *base) {
    eAnimationGroup *other = dcast<eAnimationGroup>(base);
    f8 = other->f8;
    fC = other->fC;
}

// gcEntityCustomAttackGroup::AssignCopy(const cBase *) @ 0x002380fc
class gcEntityCustomAttackGroup {
public:
    char _pad0[8];
    unsigned char f8;
    char _pad9[3];
    int fC;
    void AssignCopy(const cBase *);
};

void gcEntityCustomAttackGroup::AssignCopy(const cBase *base) {
    gcEntityCustomAttackGroup *other = dcast<gcEntityCustomAttackGroup>(base);
    f8 = other->f8;
    fC = other->fC;
}

// gcValCameraIsValid::AssignCopy(const cBase *) @ 0x00322fb4
class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValCameraIsValid {
public:
    char _pad0[0x10];
    int f10;
    gcDesiredEntity f14;
    void AssignCopy(const cBase *);
};

void gcValCameraIsValid::AssignCopy(const cBase *base) {
    gcValCameraIsValid *other = dcast<gcValCameraIsValid>(base);
    f10 = other->f10;
    f14 = other->f14;
}
