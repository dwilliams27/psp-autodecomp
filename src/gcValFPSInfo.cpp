// Days of Thunder decompilation: gcValFPSInfo
//   gcValFPSInfo::VisitReferences(...)                 — matched (60B)
//   gcValFPSInfo::AssignCopy(const cBase *)            — matched (48B)
//   gcValFPSInfo::GetText(char *) const                — matched (40B)
//   gcValFPSInfo::Evaluate(void) const                 — in-progress (108B)
//   gcValFPSInfo::~gcValFPSInfo(void)                  @ 0x003445f4 (100B)
//   gcValFPSInfo::New(cMemPool *, cBase *) static      @ 0x00344388 (132B)
//
// Self-contained TU (no gcValFPSInfo.h include) following the project's
// established single-TU pattern for classes whose deleting-destructor needs
// an inline operator delete (see gcValNumber.cpp, eDynamicModelLookAtTemplate.cpp).

class cBase;
class cMemPool;

// ──────────────────────────────────────────────────────────────────────────
// Engine-side helper types used by ~gcValFPSInfo and gcValFPSInfo::New.
// ──────────────────────────────────────────────────────────────────────────

struct gcValFPSInfo_PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct gcValFPSInfo_AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcValFPSInfo_cMemPoolNS {
public:
    static gcValFPSInfo_cMemPoolNS *GetPoolFromPtr(const void *);
};

// ──────────────────────────────────────────────────────────────────────────
// gcValFPSInfo class definition (mirrors include/gcValFPSInfo.h, plus the
// destructor / New / operator delete needed for the deleting-dtor variant).
// ──────────────────────────────────────────────────────────────────────────

class gcValFPSInfo {
public:
    void  VisitReferences(unsigned int, cBase *,
                          void (*)(cBase *, unsigned int, void *),
                          void *, unsigned int);
    void  AssignCopy(const cBase *);
    void  GetText(char *) const;
    float Evaluate(void) const;
    ~gcValFPSInfo();

    static cBase *New(cMemPool *, cBase *);

    int   pad0;          // 0x0  cBase *parent
    int   pad4;          // 0x4  vtable / classdesc
    float f8;            // 0x8

    // Inlined into the deleting-destructor variant of ~gcValFPSInfo.
    static void operator delete(void *p) {
        gcValFPSInfo_cMemPoolNS *pool =
            gcValFPSInfo_cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        gcValFPSInfo_AllocEntry *rec = (gcValFPSInfo_AllocEntry *)
            (((gcValFPSInfo_PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        char *base = block + off;
        void *(*fn)(void *, int, int, int, int) = rec->fn;
        ((void (*)(void *, void *))fn)(base, p);
    }
};

// External symbols / helpers
void  cStrAppend(char *, const char *, ...);
gcValFPSInfo *dcast(const cBase *);

extern const char gcValFPSInfo_text[];
extern char gcValFPSInfovirtualtable[];
extern char cBaseclassdesc[];

extern void *g_fps_pInstance;
extern int   g_fps_x;
extern int   g_fps_y;
extern float g_fps_rate;

// ──────────────────────────────────────────────────────────────────────────
// gcValFPSInfo::VisitReferences(...)  — matched
// ──────────────────────────────────────────────────────────────────────────
void gcValFPSInfo::VisitReferences(unsigned int arg0, cBase *arg1,
    void (*arg2)(cBase *, unsigned int, void *),
    void *arg3, unsigned int arg4) {
    if (arg2 != 0) {
        arg2(arg1, (unsigned int)this, arg3);
    }
}

// ──────────────────────────────────────────────────────────────────────────
// gcValFPSInfo::AssignCopy(const cBase *)  — matched
// ──────────────────────────────────────────────────────────────────────────
void gcValFPSInfo::AssignCopy(const cBase *base) {
    gcValFPSInfo *other = dcast(base);
    this->f8 = other->f8;
}

// ──────────────────────────────────────────────────────────────────────────
// gcValFPSInfo::GetText(char *) const  — matched
// ──────────────────────────────────────────────────────────────────────────
void gcValFPSInfo::GetText(char *buf) const {
    cStrAppend(buf, gcValFPSInfo_text);
}

// ──────────────────────────────────────────────────────────────────────────
// gcValFPSInfo::Evaluate(void) const
// ──────────────────────────────────────────────────────────────────────────
float gcValFPSInfo::Evaluate(void) const {
    void *inst = g_fps_pInstance;
    void *obj = 0;
    if (inst != 0) {
        obj = ((void **)inst)[4];
    }
    if (obj != 0) {
        *(float *)((char *)obj + 0x18) = (float)g_fps_x + (float)g_fps_y;
        *(float *)((char *)obj + 0x1C) = (float)g_fps_x;
        *(float *)((char *)obj + 0x20) = (float)g_fps_y;
    }
    return g_fps_rate;
}

// ──────────────────────────────────────────────────────────────────────────
// gcValFPSInfo::~gcValFPSInfo(void)  @ 0x003445f4, 100B
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard, the
// absence of a parent-destructor chain (cBase has none), and the deleting-
// tail dispatch through the inline static operator delete on (flag & 1).
// The body just resets the classdesc pointer at offset 4 to the parent
// (cBase) classdesc.
// ──────────────────────────────────────────────────────────────────────────
gcValFPSInfo::~gcValFPSInfo() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

// ──────────────────────────────────────────────────────────────────────────
// gcValFPSInfo::New(cMemPool *, cBase *) static  @ 0x00344388, 132B
//
// Allocate a 12-byte block from pool slot 0x28 and inline-construct a
// gcValFPSInfo with the supplied parent. Construction order matches the cBase
// ctor preamble (vtable = cBaseclassdesc) followed by gcValFPSInfo's body
// (parent + own vtable), exactly the three writes the matched asm performs.
// ──────────────────────────────────────────────────────────────────────────
cBase *gcValFPSInfo::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    gcValFPSInfo_AllocEntry *e = (gcValFPSInfo_AllocEntry *)
        (((gcValFPSInfo_PoolBlock *)block)->allocTable + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcValFPSInfo *result = 0;
    gcValFPSInfo *obj = (gcValFPSInfo *)e->fn(base, 0xC, 4, 0, 0);
    if (obj != 0) {
        *(void **)((char *)obj + 4) = cBaseclassdesc;             // cBase ctor preamble
        *(cBase **)((char *)obj + 0) = parent;                    // gcValFPSInfo body: parent
        *(void **)((char *)obj + 4) = gcValFPSInfovirtualtable;   // gcValFPSInfo body: own vtable
        result = obj;
    }
    return (cBase *)result;
}
