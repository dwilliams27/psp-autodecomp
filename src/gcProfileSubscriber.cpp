// Batch of unrelated functions grouped by the orchestrator into one TU.
// Sections are independent.

class cBase;
class cFile;
class cMemPool;
class cType;

template <class T> T *dcast(const cBase *);

class cListSubscriber {
public:
    void Attach(void);
    void Detach(void);
    void Write(cFile &) const;
};

class gcGameSettings {
public:
    enum gcSubscription {};
    static gcGameSettings *Get(void);
    void Subscribe(gcSubscription, cListSubscriber *);
    void Unsubscribe(gcSubscription, cListSubscriber *);
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
    void End(void);
};

extern char gcProfileSubscribervirtualtable[];   // 0x37E6A8
extern char gcSaveGameSubscribervirtualtable[];
void *cMemPool_GetPoolFromPtr(void *);

extern const char gcProfileSubscriber_cBase_name[];
extern const char gcProfileSubscriber_cBase_desc[];

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

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

// ── gcProfileSubscriber::Attach(void) @ 0x00287B78 ──
class gcProfileSubscriber {
public:
    void Attach(void);
    void Detach(void);
    void Write(cFile &) const;
    int GetIndex(void *) const;
    int GetItem(int) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType() const;
};

static cType *type_cBase;
static cType *type_cListSubscriber;
static cType *type_gcProfileSubscriber;

// ── gcProfileSubscriber::New(cMemPool *, cBase *) static @ 0x0028790C ──
cBase *gcProfileSubscriber::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcProfileSubscriber *result = 0;
    gcProfileSubscriber *obj = (gcProfileSubscriber *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        cListSubscriber_cListSubscriber(obj, parent);
        *(void **)((char *)obj + 4) = gcProfileSubscribervirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcProfileSubscriber::GetType(void) const @ 0x00287994 ──
const cType *gcProfileSubscriber::GetType() const {
    if (!type_gcProfileSubscriber) {
        if (!type_cListSubscriber) {
            if (!type_cBase) {
                type_cBase = cType::InitializeType(
                    gcProfileSubscriber_cBase_name, gcProfileSubscriber_cBase_desc,
                    1, 0, 0, 0, 0, 0);
            }
            type_cListSubscriber = cType::InitializeType(
                0, 0, 0x187, type_cBase, 0, 0, 0, 0);
        }
        type_gcProfileSubscriber = cType::InitializeType(
            0, 0, 0x1EF, type_cListSubscriber,
            (cBase *(*)(cMemPool *, cBase *))&gcProfileSubscriber::New,
            0, 0, 0);
    }
    return type_gcProfileSubscriber;
}

void gcProfileSubscriber::Attach(void) {
    ((cListSubscriber *)this)->Attach();
    gcGameSettings::Get()->Subscribe(
        (gcGameSettings::gcSubscription)1, (cListSubscriber *)this);
}

// ── gcProfileSubscriber::Detach(void) @ 0x00287BB4 ──
void gcProfileSubscriber::Detach(void) {
    gcGameSettings::Get()->Unsubscribe(
        (gcGameSettings::gcSubscription)1, (cListSubscriber *)this);
    ((cListSubscriber *)this)->Detach();
}

// ── gcProfileSubscriber::Write(cFile &) const @ 0x00287A70 ──
void gcProfileSubscriber::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cListSubscriber *)this)->Write(file);
    wb.End();
}

// ── gcProfileSubscriber::GetItem(int) const @ 0x00287C20 ──
int gcProfileSubscriber::GetItem(int idx) const {
    void *p = gcGameSettings::Get();
    int result = 0;
    if (idx >= 0) {
        p = *(void **)((char *)p + 0x40);
        int count = 0;
        if (p != 0) {
            count = ((int *)p)[-1] & 0x3FFFFFFF;
        }
        if (idx < count) {
            result = idx + 1;
        }
    }
    return result;
}

// ── gcProfileSubscriber::AssignCopy(const cBase *) @ 0x002878AC ──
void gcProfileSubscriber::AssignCopy(const cBase *base) {
    gcProfileSubscriber *other = dcast<gcProfileSubscriber>(base);
    *(int *)((char *)this + 8) = *(int *)((char *)other + 8);
    *(int *)((char *)this + 12) = *(int *)((char *)other + 12);
    *(unsigned char *)((char *)this + 16) = *(unsigned char *)((char *)other + 16);
    *(int *)((char *)this + 20) = *(int *)((char *)other + 20);
    *(int *)((char *)this + 24) = *(int *)((char *)other + 24);
    *(int *)((char *)this + 28) = *(int *)((char *)other + 28);
    *(int *)((char *)this + 32) = *(int *)((char *)other + 32);
}

// ── gcProfileSubscriber::~gcProfileSubscriber(void) @ 0x00287C7C ──
extern "C" void gcProfileSubscriber___dtor_gcProfileSubscriber_void(void *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = gcProfileSubscribervirtualtable;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            char *allocTable = *(char **)((char *)block + 0x1C);
            DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

// ── gcSaveGameSubscriber::Detach(void) @ 0x00288640 ──
class gcSaveGameSubscriber {
public:
    static cBase *New(cMemPool *, cBase *);
    void Detach(void);
    int GetIndex(void *) const;
};

// ── gcSaveGameSubscriber::New(cMemPool *, cBase *) static @ 0x00288398 ──
cBase *gcSaveGameSubscriber::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcSaveGameSubscriber *result = 0;
    gcSaveGameSubscriber *obj = (gcSaveGameSubscriber *)entry->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        cListSubscriber_cListSubscriber(obj, parent);
        *(void **)((char *)obj + 4) = gcSaveGameSubscribervirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

void gcSaveGameSubscriber::Detach(void) {
    gcGameSettings::Get()->Unsubscribe(
        (gcGameSettings::gcSubscription)0, (cListSubscriber *)this);
    ((cListSubscriber *)this)->Detach();
}

// ── gcValEntityVelocity::AssignCopy(const cBase *) @ 0x00341018 ──
class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcValEntityVelocity {
public:
    void AssignCopy(const cBase *);
};

void gcValEntityVelocity::AssignCopy(const cBase *base) {
    gcValEntityVelocity *other = dcast<gcValEntityVelocity>(base);
    *(int *)((char *)this + 8) = *(int *)((char *)other + 8);
    ((gcDesiredEntity *)((char *)this + 12))->operator=(
        *(gcDesiredEntity *)((char *)other + 12));
}

// ── eRoomSet::RemoveFromWorld(eWorld *) @ 0x000400E4 ──
class eRoomSet;
class eWorld;

class eRoomAABBTree {
public:
    void RemoveFromWorld(eWorld *);
};

class eWorld {
public:
    void RemoveRoomSet(eRoomSet *);
};

class eRoomSet {
public:
    void RemoveFromWorld(eWorld *);
};

void eRoomSet::RemoveFromWorld(eWorld *world) {
    ((eRoomAABBTree *)((char *)this + 0x44))->RemoveFromWorld(world);
    world->RemoveRoomSet(this);
}

// ── ePath::ePath(cBase *) @ 0x0005B5A0 ──
class cObject {
public:
    cObject(cBase *);
};

class ePath : public cObject {
public:
    ePath(cBase *);
};

ePath::ePath(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x381F70;
    *(int *)((char *)this + 0x44) = 0;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x48) = 0.0f;
    __asm__ volatile("" : : "r"(this) : "memory");
}

// ── gcProfileSubscriber::GetIndex(void *) const @ 0x00287BF0 ──
int gcProfileSubscriber::GetIndex(void *p) const {
    gcGameSettings::Get();
    int v = -1;
    if (p) v = (int)p - 1;
    return v;
}

// ── gcSaveGameSubscriber::GetIndex(void *) const @ 0x0028867C ──
int gcSaveGameSubscriber::GetIndex(void *p) const {
    gcGameSettings::Get();
    int v = -1;
    if (p) v = (int)p - 1;
    return v;
}

// ── gcValLobbySessionStatus::AssignCopy(const cBase *) @ 0x0034E4CC ──
class gcValLobbySessionStatus {
public:
    void AssignCopy(const cBase *);
};

void gcValLobbySessionStatus::AssignCopy(const cBase *base) {
    gcValLobbySessionStatus *other = dcast<gcValLobbySessionStatus>(base);
    *(int *)((char *)this + 8) = *(int *)((char *)other + 8);
}

// ── gcVariableGroup::AssignCopy(const cBase *) @ 0x002361B4 ──
class gcVariableGroup {
public:
    void AssignCopy(const cBase *);
};

void gcVariableGroup::AssignCopy(const cBase *base) {
    gcVariableGroup *other = dcast<gcVariableGroup>(base);
    *(unsigned char *)((char *)this + 8) = *(unsigned char *)((char *)other + 8);
    *(int *)((char *)this + 12) = *(int *)((char *)other + 12);
}

// ── gcDesiredObjectT<gcDesiredEntity, gcDesiredEntityHelper, gcEntity>::GetObject(bool) const @ 0x0025FA84 ──
class gcDesiredEntity;
class gcDesiredEntityHelper;
class gcEntity;

template <class T, class H, class O>
class gcDesiredObjectT {
public:
    H *Get(bool) const;
    cBase *GetObject(bool) const;
};

template <class T, class H, class O>
cBase *gcDesiredObjectT<T, H, O>::GetObject(bool b) const {
    return (cBase *)Get(b);
}

template class gcDesiredObjectT<gcDesiredEntity, gcDesiredEntityHelper, gcEntity>;
