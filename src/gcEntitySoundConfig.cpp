// gcEntitySoundConfig — sound config entity (44-byte struct).
//
// Functions in this file:
//   gcEntitySoundConfig::Write(cFile &) const           @ 0x00125cd8  100B
//   gcEntitySoundConfig::gcEntitySoundConfig(cBase *)   @ 0x00125e24   52B
//   gcEntitySoundConfig::AssignCopy(const cBase *)      @ 0x00267980  136B  (not matched)
//   gcEntitySoundConfig::New(cMemPool *, cBase *)       @ 0x00267a08  124B
//   gcEntitySoundConfig::GetType(void) const            @ 0x00267a84  160B
//   gcEntitySoundConfig::GetName(char *) const          @ 0x00267b24  172B
//   gcEntitySoundConfig::~gcEntitySoundConfig(void)     @ 0x00267ee8  100B

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cFile;
class cMemPool;
class cType;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void Write(unsigned short);
    void End(void);
};

class cHandle {
public:
    int m_val;
    void Write(cWriteBlock &) const;
};

class cName {
public:
    int _data[6];
    void Write(cWriteBlock &) const;
};

void cStrCopy(char *, const char *);

extern char gcEntityGeomConfigvirtualtable[];
extern char cBaseclassdesc[];

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

void gcEntitySoundConfig_ctor(void *, cBase *);

class gcEntitySoundConfigPoolNS {
public:
    static gcEntitySoundConfigPoolNS *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcEntitySoundConfig {
public:
    cBase *mOwner;
    void *mClassDesc;
    cHandle mHandle;
    cName mName;
    cHandle mHandle2;
    unsigned short mField;

    gcEntitySoundConfig(cBase *);
    ~gcEntitySoundConfig();
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    void GetName(char *) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        gcEntitySoundConfigPoolNS *pool =
            gcEntitySoundConfigPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec =
            (DeleteRecord *)(((PoolBlock *)block)->allocTable + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

gcEntitySoundConfig *dcast(const cBase *);

// ── Constructor ── @ 0x00125e24, 52B
gcEntitySoundConfig::gcEntitySoundConfig(cBase *parent) {
    ((cBase **)this)[0] = parent;
    ((int *)this)[1] = 0x389388;
    *(int *)((char *)this + 0x08) = 0;
    *(short *)((char *)this + 0x20) = 0;
    *(short *)((char *)this + 0x22) = 0;
    *(char *)((char *)this + 0x0C) = 0;
    *(int *)((char *)this + 0x24) = 0;
    *(short *)((char *)this + 0x28) = 1;
}

// ── Write ── @ 0x00125cd8, 100B
void gcEntitySoundConfig::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((cHandle *)((char *)this + 0x08))->Write(wb);
    ((cName *)((char *)this + 0x0C))->Write(wb);
    wb.Write(*(int *)((char *)this + 0x24));
    wb.Write(*(unsigned short *)((char *)this + 0x28));
    wb.End();
}

// ── AssignCopy ── @ 0x00267980, 136B
void gcEntitySoundConfig::AssignCopy(const cBase *base) {
    const gcEntitySoundConfig *src = dcast(base);
    mHandle = src->mHandle;
    mName = src->mName;
    mHandle2 = src->mHandle2;
    mField = src->mField;
}

// ── New ── @ 0x00267a08, 124B
cBase *gcEntitySoundConfig::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcEntitySoundConfig *result = 0;
    gcEntitySoundConfig *obj =
        (gcEntitySoundConfig *)entry->fn(base, 0x2C, 4, 0, 0);
    if (obj != 0) {
        gcEntitySoundConfig_ctor(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── GetType ── @ 0x00267a84, 160B
static cType *type_base;
static cType *type_gcEntitySoundConfig;

const cType *gcEntitySoundConfig::GetType(void) const {
    if (!type_gcEntitySoundConfig) {
        if (!type_base) {
            type_base = cType::InitializeType((const char *)0x36D894,
                                              (const char *)0x36D89C,
                                              1, 0, 0, 0, 0, 0);
        }
        type_gcEntitySoundConfig =
            cType::InitializeType(0, 0, 0xA1, type_base,
                                  &gcEntitySoundConfig::New,
                                  0, 0, 0);
    }
    return type_gcEntitySoundConfig;
}

// ── GetName ── @ 0x00267b24, 172B
//
// Looks the handle at offset 8 up in the global handle table at 0x38890.
// The string-selection cascade picks one of three name fallbacks based on
// whether the handle resolves and whether the resolved entry has a non-empty
// short at offset 0x1C (likely a flag indicating "use embedded name").
void gcEntitySoundConfig::GetName(char *buf) const {
    int field = *(int *)((char *)this + 8);
    void *found;
    if (field == 0) {
        found = 0;
    } else {
        found = 0;
        void *cand = ((void **)0x38890)[field & 0xFFFF];
        if (cand != 0 && *(int *)((char *)cand + 0x30) == field) {
            found = cand;
        }
    }
    const char *str;
    if (found != 0) {
        if ((*(short *)((char *)found + 0x1C) == 0) & 0xff) {
            str = (const char *)0x36DAB8;
        } else {
            str = (const char *)found + 8;
        }
    } else if (field != 0) {
        str = (const char *)0x36DAC4;
    } else {
        str = (const char *)0x36DACC;
    }
    cStrCopy(buf, str);
}

// ── Destructor ── @ 0x00267ee8, 100B
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard, the
// absence of a parent-destructor chain (cBase has none), and the deleting-
// tail dispatch through operator delete on (flag & 1). Body just resets the
// classdesc pointer at offset 4 to the parent (cBase) classdesc.
gcEntitySoundConfig::~gcEntitySoundConfig() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}
