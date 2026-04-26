// Batch of gcSaveGameSubscriber methods grouped into one TU.
// Mirrors the structure of gcProfileSubscriber.cpp.

class cBase;
class cFile;
class cMemPool;

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

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

extern char gcSaveGameSubscribervirtualtable[];   // 0x37E6A8
void *cMemPool_GetPoolFromPtr(void *);

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

class gcSaveGameSubscriber {
public:
    void Attach(void);
    void Write(cFile &) const;
    int GetItem(int) const;
    void AssignCopy(const cBase *);
};

// ── gcSaveGameSubscriber::Attach(void) @ 0x00288604 ──
void gcSaveGameSubscriber::Attach(void) {
    ((cListSubscriber *)this)->Attach();
    gcGameSettings::Get()->Subscribe(
        (gcGameSettings::gcSubscription)0, (cListSubscriber *)this);
}

// ── gcSaveGameSubscriber::Write(cFile &) const @ 0x002884FC ──
void gcSaveGameSubscriber::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const cListSubscriber *)this)->Write(file);
    wb.End();
}

// ── gcSaveGameSubscriber::GetItem(int) const @ 0x002886AC ──
int gcSaveGameSubscriber::GetItem(int idx) const {
    void *p = gcGameSettings::Get();
    int result = 0;
    if (idx >= 0) {
        p = *(void **)((char *)p + 0x5C);
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

// ── gcSaveGameSubscriber::AssignCopy(const cBase *) @ 0x00288338 ──
void gcSaveGameSubscriber::AssignCopy(const cBase *base) {
    gcSaveGameSubscriber *other = dcast<gcSaveGameSubscriber>(base);
    *(int *)((char *)this + 8) = *(int *)((char *)other + 8);
    *(int *)((char *)this + 12) = *(int *)((char *)other + 12);
    *(unsigned char *)((char *)this + 16) = *(unsigned char *)((char *)other + 16);
    *(int *)((char *)this + 20) = *(int *)((char *)other + 20);
    *(int *)((char *)this + 24) = *(int *)((char *)other + 24);
    *(int *)((char *)this + 28) = *(int *)((char *)other + 28);
    *(int *)((char *)this + 32) = *(int *)((char *)other + 32);
}

// ── gcSaveGameSubscriber::~gcSaveGameSubscriber(void) @ 0x00288708 ──
extern "C" void gcSaveGameSubscriber___dtor_gcSaveGameSubscriber_void(void *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = gcSaveGameSubscribervirtualtable;
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
