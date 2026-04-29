// gcLobbyConfigStrings — gcAll_psp.obj
//   Class hierarchy: cBase -> gcStringValue -> gcStringLValue -> gcLobbyConfigStrings
//   Total size 0xC. mText at offset 8.

class cBase {
public:
    int _b0;
    int _b4;
};

class cFile;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

template <class T> T *dcast(const cBase *);
void cStrCopy(wchar_t *, const char *, int);
void cStrCopy(char *, const wchar_t *, int);

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

extern char gcLobbyConfigStringsvirtualtable[];
extern char cBaseclassdesc[];                               // @ 0x37E6A8
extern cType *D_000385DC;
extern cType *D_0009F454;
extern cType *D_0009F458;
extern cType *D_0009F4EC;

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct DispatchEntry {
    short offset;
    short pad;
    void (*fn)(void *, void *, int);
};

class gcStringValue : public cBase {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcStringLValue : public gcStringValue {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class nwNetwork {
public:
    static void *GetLobby(void);
};

class gcLobbyConfigStrings : public gcStringLValue {
public:
    int mText;  // 0x8

    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void Get(wchar_t *, int) const;
    void Set(const wchar_t *) const;
    const cType *GetType(void) const;
    static gcLobbyConfigStrings *New(cMemPool *, cBase *);

    ~gcLobbyConfigStrings(void);

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteEntry *rec = (DeleteEntry *)(((PoolBlock *)block)->allocTable + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

class gcTableTemplateGroup {
public:
    static bool IsManagedTypeExternalStatic();
    bool IsManagedTypeExternal() const;
};

class gcDesiredTableTemplate;
class gcDesiredTableTemplateHelper;
class gcTableTemplate;
class gcDesiredTrigger;
class gcDesiredTriggerHelper;
class gcTrigger;
class gcDesiredPortal;
class gcDesiredPortalHelper;
class ePortal;

template <class T, class H, class O>
class gcDesiredObjectT {
public:
    H *Get(bool) const;
    cBase *GetObject(bool) const;
};

// ── gcLobbyConfigStrings::AssignCopy(const cBase *)  @ 0x002804dc ──
void gcLobbyConfigStrings::AssignCopy(const cBase *base) {
    gcLobbyConfigStrings *other = dcast<gcLobbyConfigStrings>(base);
    mText = other->mText;
}

// ── gcLobbyConfigStrings::Write(cFile &) const  @ 0x00280584, 88B ──
void gcLobbyConfigStrings::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcStringLValue::Write(file);
    wb.Write(mText);
    wb.End();
}

// ── gcLobbyConfigStrings::Read(cFile &, cMemPool *)  @ 0x002805dc, 208B ──
int gcLobbyConfigStrings::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcStringLValue::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    {
        void *h = *(void **)rb._data[0];
        cFileSystem::Read(h, (char *)this + 8, 4);
    }
    return result;
}

// ── gcLobbyConfigStrings::Get(wchar_t *, int) const  @ 0x002806ac, 124B ──
struct gcLobbyTypeNameEntry {
    short offset;
    short pad;
    char *(*fn)(void *);
};

void gcLobbyConfigStrings::Get(wchar_t *buf, int size) const {
    void *lobby = nwNetwork::GetLobby();
    if (lobby != 0 && this->mText == 0) {
        gcLobbyTypeNameEntry *e = (gcLobbyTypeNameEntry *)(*(char **)lobby + 0x28);
        char *name = e->fn((char *)lobby + e->offset);
        cStrCopy(buf, name, size);
    }
}

// ── gcLobbyConfigStrings::Set(const wchar_t *) const  @ 0x00280728, 128B ──
void gcLobbyConfigStrings::Set(const wchar_t *src) const {
    char buf[256];
    void *lobby = nwNetwork::GetLobby();
    if (lobby != 0) {
        buf[0] = 0;
        cStrCopy(buf, src, 0xff);
        if (this->mText == 0) {
            DispatchEntry *e = (DispatchEntry *)(*(char **)lobby + 0x20);
            e->fn((char *)lobby + e->offset, buf, e->offset);
        }
    }
}

const cType *gcLobbyConfigStrings::GetType(void) const {
    if (D_0009F4EC == 0) {
        if (D_0009F458 == 0) {
            if (D_0009F454 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_0009F454 = cType::InitializeType(0, 0, 0x170, D_000385DC,
                                                   0, 0, 0, 0);
            }
            D_0009F458 = cType::InitializeType(0, 0, 0x171, D_0009F454,
                                               0, 0, 0, 0);
        }
        D_0009F4EC = cType::InitializeType(
            0, 0, 0x172, D_0009F458,
            (cBase *(*)(cMemPool *, cBase *))&gcLobbyConfigStrings::New,
            0, 0, 0);
    }
    return D_0009F4EC;
}

// ── gcLobbyConfigStrings::~gcLobbyConfigStrings(void)  @ 0x002807d8, 100B ──
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard, the
// (flag & 1) deleting-tail dispatch through inline operator delete, and the
// classdesc reset. Body just reverts the classdesc pointer at offset 4 to
// cBase (no intermediate dtors in the chain).
gcLobbyConfigStrings::~gcLobbyConfigStrings(void) {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

// ── gcLobbyConfigStrings::New(cMemPool *, cBase *) static  @ 0x002803e4, 136B ──
gcLobbyConfigStrings *gcLobbyConfigStrings::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    void *base = (char *)block + entry->offset;
    gcLobbyConfigStrings *result = 0;
    int *p = (int *)entry->fn(base, 0xC, 4, 0, 0);
    if (p != 0) {
        *(int *)((char *)p + 4) = 0x37E6A8;
        *(cBase **)((char *)p + 0) = parent;
        *(int *)((char *)p + 4) = (int)gcLobbyConfigStringsvirtualtable;
        *(int *)((char *)p + 8) = 0;
        result = (gcLobbyConfigStrings *)p;
    }
    return result;
}

// ── gcTableTemplateGroup::IsManagedTypeExternal(void) const ──
bool gcTableTemplateGroup::IsManagedTypeExternal() const {
    return IsManagedTypeExternalStatic();
}

typedef gcDesiredObjectT<gcDesiredTableTemplate,
                         gcDesiredTableTemplateHelper,
                         gcTableTemplate> gcDesiredTableTemplateObject;
typedef gcDesiredObjectT<gcDesiredTrigger,
                         gcDesiredTriggerHelper,
                         gcTrigger> gcDesiredTriggerObject;
typedef gcDesiredObjectT<gcDesiredPortal,
                         gcDesiredPortalHelper,
                         ePortal> gcDesiredPortalObject;

extern "C" cBase *
__0fQgcDesiredObjectT76WgcDesiredTableTemplate6cgcDesiredTableTemplateHelper6PgcTableTemplate_JGetObjectb_P6FcBaseK(
    gcDesiredTableTemplateObject *self, bool b) {
    return (cBase *)self->Get(b);
}

extern "C" cBase *
__0fQgcDesiredObjectT76QgcDesiredTrigger6WgcDesiredTriggerHelper6JgcTrigger_JGetObjectb_P6FcBaseK(
    gcDesiredTriggerObject *self, bool b) {
    return (cBase *)self->Get(b);
}

extern "C" cBase *
__0fQgcDesiredObjectT76PgcDesiredPortal6VgcDesiredPortalHelper6HePortal_JGetObjectb_P6FcBaseK(
    gcDesiredPortalObject *self, bool b) {
    return (cBase *)self->Get(b);
}
