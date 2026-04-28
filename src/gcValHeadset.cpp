// Multi-class wrapper file for small leaf-of-leaf methods.
// Functions:
//   gcValHeadset::GetText(char *) const                  @ 0x003472c8
//   gcValHeadset::Write(cFile &) const                   @ 0x003470a4
//   gcValHeadset::New(cMemPool *, cBase *) static        @ 0x00346ec8
//   gcValHeadset::Evaluate(void) const                   @ 0x003471cc
//   eSound::IsPlaying(void) const                        @ 0x00021460
//   gcValLobbyInfo::GetText(char *) const                @ 0x00349db8
//   gcValLobbyOptions::AssignCopy(const cBase *)         @ 0x0034a9c4
//   gcValLobbySessionStatus::GetText(char *) const       @ 0x0034e830

class cBase;
class cFile;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

void cStrAppend(char *, const char *, ...);

extern const char gcValHeadset_fmt[];               // @ 0x36F494
extern const char gcValLobbyInfo_fmt[];             // @ 0x36F4EC
extern const char gcValLobbySessionStatus_fmt[];    // @ 0x36F544
extern const char gcValGetText_text[];              // @ 0x36DAF0

extern char gcLValuevirtualtable[];
extern char gcValHeadsetvirtualtable[];
extern char cBaseclassdesc[];                       // @ 0x37E6A8

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

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class nwHeadset {
public:
    static int IsHeadsetAvailable(void);
    static int GetChannel(void);
    static int IsVoiceAllowed(void);
};

class gcLValue {
public:
    cBase *mParent;       // 0x0
    void *mVtable;        // 0x4
    gcLValue(cBase *parent);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcValHeadset : public gcLValue {
public:
    int mField8;          // 0x8
    gcValHeadset(cBase *parent);
    ~gcValHeadset();
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

inline gcValHeadset::gcValHeadset(cBase *parent) : gcLValue(parent) {
    mVtable = gcValHeadsetvirtualtable;
    mField8 = 0;
}

inline void *operator new(unsigned, void *p) { return p; }

// ─────────────────────────────────────────────────────────────────────────
// gcValHeadset::GetText(char *) const @ 0x003472c8
// ─────────────────────────────────────────────────────────────────────────

void gcValHeadset::GetText(char *buf) const {
    cStrAppend(buf, gcValHeadset_fmt, gcValGetText_text);
}

// ─────────────────────────────────────────────────────────────────────────
// gcValHeadset::Write(cFile &) const @ 0x003470a4
// ─────────────────────────────────────────────────────────────────────────

void gcValHeadset::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcLValue::Write(file);
    wb.Write(mField8);
    wb.End();
}

// ─────────────────────────────────────────────────────────────────────────
// gcValHeadset::New(cMemPool *, cBase *) static @ 0x00346ec8
// ─────────────────────────────────────────────────────────────────────────

cBase *gcValHeadset::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)(((PoolBlock *)block)->allocTable + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcValHeadset *result = 0;
    gcValHeadset *obj = (gcValHeadset *)e->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcValHeadset(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValHeadset::Evaluate(void) const @ 0x003471cc
// ─────────────────────────────────────────────────────────────────────────

float gcValHeadset::Evaluate(void) const {
    switch (mField8) {
        case 0: return (float)nwHeadset::IsHeadsetAvailable();
        case 1: return (float)nwHeadset::GetChannel();
        case 2: return (float)nwHeadset::IsVoiceAllowed();
    }
    return 0.0f;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValHeadset::Read(cFile &, cMemPool *) @ 0x003470fc
// ─────────────────────────────────────────────────────────────────────────

int gcValHeadset::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && ((gcLValue *)this)->Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    {
        void *h = *(void **)rb._data[0];
        cFileSystem::Read(h, (char *)this + 8, 4);
    }
    return result;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValHeadset::~gcValHeadset(void) @ 0x003472f8
// ─────────────────────────────────────────────────────────────────────────

gcValHeadset::~gcValHeadset() {
    mVtable = cBaseclassdesc;
}

// ─────────────────────────────────────────────────────────────────────────
// eSound::IsPlaying(void) const @ 0x00021460
// ─────────────────────────────────────────────────────────────────────────

class eAudio {
public:
    static int IsPlayingSound(int);
};

class eSound {
public:
    short pad0[24];     // 0x00..0x2F
    short channel;      // 0x30
    bool IsPlaying(void) const;
};

bool eSound::IsPlaying(void) const {
    return (this->channel >= 0) && (eAudio::IsPlayingSound(this->channel) != 0);
}

// ─────────────────────────────────────────────────────────────────────────
// gcValLobbyInfo::GetText(char *) const @ 0x00349db8
// ─────────────────────────────────────────────────────────────────────────

class gcValLobbyInfo {
public:
    void GetText(char *) const;
};

void gcValLobbyInfo::GetText(char *buf) const {
    cStrAppend(buf, gcValLobbyInfo_fmt, gcValGetText_text);
}

// ─────────────────────────────────────────────────────────────────────────
// gcValLobbyOptions::AssignCopy(const cBase *) @ 0x0034a9c4
// ─────────────────────────────────────────────────────────────────────────

class gcValLobbyOptions {
public:
    int pad0;
    int pad4;
    int f8;
    void AssignCopy(const cBase *);
};

gcValLobbyOptions *dcast(const cBase *);

void gcValLobbyOptions::AssignCopy(const cBase *base) {
    gcValLobbyOptions *other = dcast(base);
    this->f8 = other->f8;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValLobbySessionStatus::GetText(char *) const @ 0x0034e830
// ─────────────────────────────────────────────────────────────────────────

class gcValLobbySessionStatus {
public:
    void GetText(char *) const;
};

void gcValLobbySessionStatus::GetText(char *buf) const {
    cStrAppend(buf, gcValLobbySessionStatus_fmt, gcValGetText_text);
}
