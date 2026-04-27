// gcAnimationEvent — animation event derived from gcEvent.
// Functions matched here:
//   gcAnimationEvent::AssignCopy(const cBase *)         @ 0x0023f0c4
//   gcAnimationEvent::New(cMemPool *, cBase *) static   @ 0x0023f108
//   gcAnimationEvent::GetType(void) const               @ 0x0023f1ac

inline void *operator new(unsigned int, void *p) { return p; }

class cBase;
class cMemPool;
class cType;

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

class gcAnimationEvent {
public:
    cBase *m_parent;                  // 0x00
    void *m_vtable;                   // 0x04
    gcAnimationEventName mName;       // 0x08
    gcEvent mEvent;                   // 0x0C..0x28

    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
};

gcAnimationEvent *dcast(const cBase *);
void gcEvent_ctor(gcEvent *self, cBase *parent, const char *name);

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
