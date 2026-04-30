// gcViewport — viewport class. Instances live in a contiguous global array
// based at *(0x37D840); each is 0x1390 bytes wide. GetName formats the
// instance's 1-based index into the supplied buffer.

class cBase;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};
class cType;
class gcUI {
public:
    ~gcUI();
    void Reset(void);
};

struct ViewportPoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct ViewportAllocEntry {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class gcCamera {
public:
    gcCamera(void);
    gcCamera &operator=(const gcCamera &);
};

class gcViewport {
public:
    typedef int gcConfig;

    gcViewport(cBase *);
    ~gcViewport();
    void GetName(char *) const;
    void AssignCopy(const cBase *);
    static void SetConfiguration(gcConfig);
    static void SetFullscreen(bool);
    static cBase *New(cMemPool *, cBase *);
    static int PausesGame(void);
    const cType *GetType(void) const;
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        ViewportAllocEntry *rec =
            (ViewportAllocEntry *)(((ViewportPoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void *(*fn)(void *, int, int, int, int) = rec->fn;
        ((void (*)(void *, void *))fn)(base, p);
    }
};

template <class T> T *dcast(const cBase *);

extern "C" int cStrFormat(char *, const char *, ...);
extern "C" void gcCamera__gcCamera_void(void *self);
extern "C" void gcViewport__gcViewport_cBaseptr(void *self, cBase *parent);

extern cType *D_000385DC;
extern cType *D_0009A2E8;

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct GlobalViewportPauseData {
    char pad0[0x0B];
    unsigned char unk0B;
    char pad1[0x23];
    unsigned char unk2F;
    char pad2[0xA8];
    int unkD8;
};

struct ViewportPauseData {
    char pad0[0x11EF];
    unsigned char unk11EF;
    char pad1[0x23];
    unsigned char unk1213;
    char pad2[0xA8];
    int unk12BC;
};

// ── gcViewport::GetName(char *) const @ 0x002492E8 ──
void gcViewport::GetName(char *buf) const {
    cStrFormat(buf, (const char *)0x36DBCC,
               ((int)((const char *)this - *(char **)0x37D840) / 0x1390) + 1);
}

// ── gcViewport::AssignCopy(const cBase *) @ 0x0024917c ──
void gcViewport::AssignCopy(const cBase *base) {
    gcViewport *other = dcast<gcViewport>(base);
    const gcCamera &src = *(const gcCamera *)(other + 0x10);
    *(gcCamera *)((char *)this + 0x10) = src;
    *(unsigned char *)((char *)this + 0x1380) = *(const unsigned char *)((char *)other + 0x1380);
    *(unsigned char *)((char *)this + 0x1381) = *(const unsigned char *)((char *)other + 0x1381);
}

// ── gcViewport::New(cMemPool *, cBase *) static @ 0x002491cc ──
cBase *gcViewport::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcViewport *result = 0;
    gcViewport *obj = (gcViewport *)rec->fn(base, 0x1390, 0x10, 0, 0);
    if (obj != 0) {
        gcViewport__gcViewport_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcViewport::gcViewport(cBase *) @ 0x000FDB7C ──
gcViewport::gcViewport(cBase *parent) {
    *(cBase **)this = parent;
    *(void **)((char *)this + 4) = (void *)0x387E28;
    gcCamera__gcCamera_void((char *)this + 0x10);
    *(unsigned char *)((char *)this + 0x1380) = 0;
    *(unsigned char *)((char *)this + 0x1381) = 1;
    *(gcViewport **)((char *)this + 0x11F4) = this;
}

// ── gcViewport::~gcViewport(void) @ 0x000CCD94 ──
gcViewport::~gcViewport() {
    *(void **)((char *)this + 4) = (void *)0x387E28;
    if ((char *)this + 0x10 != 0) {
        ((gcUI *)((char *)this + 0x11F4))->~gcUI();
    }
    *(void **)((char *)this + 4) = (void *)0x37E6A8;
}

// ── gcViewport::SetFullscreen(bool) static @ 0x000FDC34 ──
void gcViewport::SetFullscreen(bool fullscreen) {
    unsigned char value = fullscreen;
    int *base = (int *)0x380000;
    if (value != 0) {
        int *src = (int *)0x380000;
        int config = src[-0x9EE];
        base[-0x9ED] = config;
        gcViewport::SetConfiguration(0);
        ((gcUI *)0x99928)->Reset();
    } else {
        gcViewport::SetConfiguration(base[-0x9ED]);
    }
    *(unsigned char *)0x37D844 = value;
}

// ── gcViewport::PausesGame(void) static @ 0x000FE08C ──
int gcViewport::PausesGame(void) {
    int pauses;
    int active;
    int i;
    ViewportPauseData *viewport;
    ViewportPauseData *current;

    pauses = 0;
    if (((GlobalViewportPauseData *)0x99928)->unkD8 > 0) {
        goto global_true;
    }
    if (((GlobalViewportPauseData *)0x99928)->unk0B != 0) {
        goto global_true;
    }
    if (((GlobalViewportPauseData *)0x99928)->unk2F == 0) {
        goto global_done;
    }
global_true:
    pauses = 1;
global_done:
    if ((unsigned char)pauses != 0) {
        return 1;
    }
    i = 0;
    viewport = (ViewportPauseData *)(*(char **)0x37D840 + 0x10);
    do {
        current = viewport;
        active = 0;
        if (current->unk12BC > 0) {
            goto active_true;
        }
        if (current->unk11EF != 0) {
            goto active_true;
        }
        if (current->unk1213 == 0) {
            goto active_done;
        }
active_true:
        active = 1;
active_done:
        i += 1;
        if ((unsigned char)active != 0) {
            return 1;
        }
        viewport = (ViewportPauseData *)((char *)viewport + 0x1390);
    } while (i < 5);
    return 0;
}

// ── gcViewport::GetType(void) const @ 0x00249248 ──
const cType *gcViewport::GetType(void) const {
    if (D_0009A2E8 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_0009A2E8 = cType::InitializeType(0, 0, 0xEE, D_000385DC,
                                           &gcViewport::New, 0, 0, 0);
    }
    return D_0009A2E8;
}
