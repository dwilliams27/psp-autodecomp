// gcAnimationEvent — animation event derived from gcEvent.
// Functions matched here:
//   gcAnimationEvent::AssignCopy(const cBase *)         @ 0x0023f0c4
//   gcAnimationEvent::New(cMemPool *, cBase *) static   @ 0x0023f108
//   gcAnimationEvent::GetType(void) const               @ 0x0023f1ac

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cMemPool;
class cFile;
class cType;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcExpressionList {
public:
    gcExpressionList &operator=(const gcExpressionList &other);
};

class gcEvent {
public:
    char _pad0[8];
    gcExpressionList mExprList;     // +0x08
    char _pad1[4];
    int mField10;                   // +0x10

    gcEvent &operator=(const gcEvent &other);
};

class gcAnimationEventName {
public:
    int mIndex;
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class gcAnimationEvent {
public:
    cBase *m_parent;                  // 0x00
    void *m_vtable;                   // 0x04
    gcAnimationEventName mName;       // 0x08
    gcEvent mEvent;                   // 0x0C..0x28

    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    void Write(cFile &) const;
    void GetName(char *) const;
    ~gcAnimationEvent(void);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

gcAnimationEvent *dcast(const cBase *);
void gcEvent_ctor(gcEvent *self, cBase *parent, const char *name);
extern "C" void gcEvent___dtor_gcEvent_void(void *, int);
void cStrCopy(char *, const char *);
void cStrCopy(char *, const char *, int);
extern "C" void *memcpy(void *, const void *, unsigned int);

extern char gcAnimationEvent_cBase_vtable[];   // 0x37E6A8
extern char gcAnimationEventvirtualtable[];    // 0x387278

extern const char gcAnimationEvent_event_name[]; // 0x36DB28

extern cType *D_000385DC;
extern cType *D_00099AB4;

extern const char gcAnimationEvent_base_name[]; // 0x36D894
extern const char gcAnimationEvent_base_desc[]; // 0x36D89C

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

struct EventWriteSlot {
    short offset;
    short pad;
    void (*fn)(void *, cFile &);
};

struct EventNameSlot {
    short offset;
    short pad;
    void (*fn)(void *, char *);
};

// ============================================================
// 0x0023f0c4 — AssignCopy(const cBase *)
// ============================================================
void gcAnimationEvent::AssignCopy(const cBase *base) {
    gcAnimationEvent *src = dcast(base);
    this->mName = src->mName;
    __asm__ volatile("" ::: "memory");
    this->mEvent = src->mEvent;
}

// ============================================================
// 0x0023f108 — New(cMemPool *, cBase *) static
// ============================================================
cBase *gcAnimationEvent::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    gcAnimationEvent *result = 0;
    gcAnimationEvent *obj = (gcAnimationEvent *)entry->fn(base, 0x28, 4, 0, 0);
    if (obj != 0) {
        ((void **)obj)[1] = gcAnimationEvent_cBase_vtable;
        ((void **)obj)[0] = parent;
        ((void **)obj)[1] = gcAnimationEventvirtualtable;
        ((int *)obj)[2] = 0;
        gcEvent_ctor((gcEvent *)((char *)obj + 0x0C), (cBase *)obj, gcAnimationEvent_event_name);
        result = obj;
    }
    return (cBase *)result;
}

// ============================================================
// 0x0023f1ac — GetType(void) const
// ============================================================
const cType *gcAnimationEvent::GetType(void) const {
    if (D_00099AB4 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType(gcAnimationEvent_base_name,
                                               gcAnimationEvent_base_desc,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00099AB4 = cType::InitializeType(0, 0, 0x179, D_000385DC,
                                           &gcAnimationEvent::New, 0, 0, 0);
    }
    return D_00099AB4;
}

// ============================================================
// 0x000e8e2c — Write(cFile &) const
// ============================================================
void gcAnimationEvent::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.Write(mName.mIndex);
    void *event = (char *)this + 0x0C;
    void *vtable = *(void **)((char *)this + 0x10);
    EventWriteSlot *slot = (EventWriteSlot *)((char *)vtable + 0x28);
    short offset = slot->offset;
    void (*fn)(void *, cFile &) = slot->fn;
    fn((char *)event + offset, *(cFile *)wb._data[0]);
    wb.End();
}

// ============================================================
// 0x000e8f6c — GetName(char *) const
// ============================================================
void gcAnimationEvent::GetName(char *dst) const {
    if (*(void **)((char *)this + 0x14) != 0) {
        char buf[0x1000];
        memcpy(buf, (const void *)0x36D944, 1);
        void *expr = *(void **)((char *)this + 0x14);
        EventNameSlot *slot = (EventNameSlot *)((char *)*(void **)((char *)expr + 4) + 0xD0);
        short offset = slot->offset;
        void (*fn)(void *, char *) = slot->fn;
        fn((char *)expr + offset, buf);
        cStrCopy(dst, buf, 0x50);
    } else {
        cStrCopy(dst, (const char *)0x36DB38);
    }
}

// ============================================================
// 0x0023f460 — ~gcAnimationEvent(void)
// ============================================================
gcAnimationEvent::~gcAnimationEvent(void) {
    *(void **)((char *)this + 4) = gcAnimationEventvirtualtable;
    gcEvent___dtor_gcEvent_void((char *)this + 0x0C, 2);
    *(void **)((char *)this + 4) = gcAnimationEvent_cBase_vtable;
}
