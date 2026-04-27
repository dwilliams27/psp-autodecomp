// gcLobbyConfigStrings — gcAll_psp.obj
//   Class hierarchy: cBase -> gcStringValue -> gcStringLValue -> gcLobbyConfigStrings
//   Total size 0xC. mText at offset 8.

class cBase {
public:
    int _b0;
    int _b4;
};

class cFile;
class cMemPool;

template <class T> T *dcast(const cBase *);
void cStrCopy(wchar_t *, const char *, int);

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

extern char gcLobbyConfigStringsvirtualtable[];

class gcStringValue : public cBase {
public:
    void Write(cFile &) const;
};

class gcStringLValue : public gcStringValue {
public:
    void Write(cFile &) const;
};

class gcLobbyConfigStrings : public gcStringLValue {
public:
    int mText;  // 0x8

    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void Get(wchar_t *, int) const;
    static gcLobbyConfigStrings *New(cMemPool *, cBase *);
};

class nwNetwork {
public:
    static void *GetLobby(void);
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

// ── gcLobbyConfigStrings::New(cMemPool *, cBase *) static  @ 0x002803e4, 136B ──
struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

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
