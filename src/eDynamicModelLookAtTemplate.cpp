// Multi-class wrapper file for small leaf GetName/GetText methods.
// Functions:
//   eDynamicModelLookAtTemplate::eDynamicModelLookAtTemplate(cBase *)  @ 0x0004b3a4
//   eDynamicModelLookAtTemplate::~eDynamicModelLookAtTemplate(void)    @ 0x0004b3e8
//   eDynamicModelLookAtTemplate::GetName(char *) const           @ 0x001f0fdc
//   gcStateHandlerBase::GetName(char *) const                    @ 0x002571a8
//   gcEvent::GetName(char *) const                               @ 0x0023a7e0
//   gcLobbyConfigStrings::GetName(char *) const                  @ 0x002807a8
//   gcValLobbyConnectionStatus::GetText(char *) const            @ 0x00347c9c

class cBase;
class cFile;
class cMemPool;
class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int, const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

void cStrCopy(char *, const char *);
void cStrAppend(char *, const char *, ...);

extern "C" void cHandlePairT_GetName(void *, char *, int, int);

extern const char gcLobbyConfigStrings_fmt[];               // @ 0x36E068
extern const char gcValLobbyConnectionStatus_fmt[];         // @ 0x36F4AC
extern const char gcValGetText_text[];                      // @ 0x36DAF0

extern char eDynamicModelLookAtTemplatevirtualtable[];      // @ 0x3813A8
extern char cBaseclassdesc[];                               // @ 0x37E6A8
extern cType *D_000385DC;
extern cType *D_000469E4;

// Pool-block layout used by the deleting-destructor tail.
struct eDynamicModelLookAtTemplate_PoolBlock {
    char  pad[0x1C];
    char *allocTable;
};

struct eDynamicModelLookAtTemplate_DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

// AllocRec used by ::New through allocTable+0x28 (size, alignment, ...).
struct eDynamicModelLookAtTemplate_AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(float);
    void End(void);
};

// Constructor invoked from ::New via safe-name wrapper. The actual symbol is
// the canonical C++ ctor defined below; the relocation is target-name-agnostic
// for byte-level compare.
extern "C" void eDynamicModelLookAtTemplate_eDynamicModelLookAtTemplate(void *self, cBase *parent);

class eDynamicModelLookAtTemplate_cMemPoolNS {
public:
    static eDynamicModelLookAtTemplate_cMemPoolNS *GetPoolFromPtr(const void *);
};

// ──────────────────────────────────────────────────────────────────────────
// eDynamicModelLookAtTemplate::eDynamicModelLookAtTemplate(cBase *)  @ 0x0004b3a4
// eDynamicModelLookAtTemplate::~eDynamicModelLookAtTemplate(void)    @ 0x0004b3e8
// eDynamicModelLookAtTemplate::GetName(char *) const  @ 0x001f0fdc
// ──────────────────────────────────────────────────────────────────────────

class eDynamicModelLookAtTemplate {
public:
    eDynamicModelLookAtTemplate(cBase *);
    ~eDynamicModelLookAtTemplate();
    void GetName(char *) const;
    void Write(cFile &) const;
    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);

    // Inline so SNC inlines it into the deleting-destructor variant.
    static void operator delete(void *p) {
        eDynamicModelLookAtTemplate_cMemPoolNS *pool =
            eDynamicModelLookAtTemplate_cMemPoolNS::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        eDynamicModelLookAtTemplate_DeleteRecord *rec =
            (eDynamicModelLookAtTemplate_DeleteRecord *)
            (((eDynamicModelLookAtTemplate_PoolBlock *)block)->allocTable + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

eDynamicModelLookAtTemplate::eDynamicModelLookAtTemplate(cBase *parent) {
    *(cBase **)((char *)this + 0) = parent;
    *(void **)((char *)this + 4) = eDynamicModelLookAtTemplatevirtualtable;
    __asm__ volatile("" ::: "memory");
    *(char *)((char *)this + 8) = 1;
    *(float *)((char *)this + 0xC) = 0.0f;
    *(float *)((char *)this + 0x10) = 0.0f;
    *(float *)((char *)this + 0x14) = 0.0f;
    *(float *)((char *)this + 0x18) = 0.0f;
    *(float *)((char *)this + 0x1C) = 0.0f;
    *(short *)((char *)this + 0x34) = 0;
    *(short *)((char *)this + 0x36) = 0;
    *(char *)((char *)this + 0x20) = 0;
}

void eDynamicModelLookAtTemplate::GetName(char *buf) const {
    cStrCopy(buf, (const char *)((char *)this + 32));
}

// ── ~eDynamicModelLookAtTemplate ── @ 0x0004b3e8, 100B
//
// Canonical C++ destructor. SNC's ABI auto-emits the (this != 0) guard, the
// absence of a parent-destructor chain (cBase has none), and the deleting-
// tail dispatch through operator delete on (flag & 1). Body just resets the
// classdesc pointer at offset 4 to the parent (cBase) classdesc.
eDynamicModelLookAtTemplate::~eDynamicModelLookAtTemplate() {
    *(void **)((char *)this + 4) = cBaseclassdesc;
}

// ── eDynamicModelLookAtTemplate::Write(cFile &) const  @ 0x0004b220, 124B ──
//
// Type-2 cWriteBlock: one bool flag (offset 8) followed by five floats
// (offsets 0xC..0x1C), then End().
void eDynamicModelLookAtTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    wb.Write(*(const bool *)((const char *)this + 8));
    wb.Write(*(const float *)((const char *)this + 0xC));
    wb.Write(*(const float *)((const char *)this + 0x10));
    wb.Write(*(const float *)((const char *)this + 0x14));
    wb.Write(*(const float *)((const char *)this + 0x18));
    wb.Write(*(const float *)((const char *)this + 0x1C));
    wb.End();
}

// ── eDynamicModelLookAtTemplate::New(cMemPool *, cBase *) static  @ 0x001f0ec4, 124B ──
//
// Pool-driven allocation through allocTable+0x28: size 0x38, alignment 4.
// On success, runs the constructor and returns the new object cast to cBase*.
// `result = 0` first + memory barrier locks `li s2,0` at the prologue position
// (matched-exemplar pattern from eDynamicFluidTemplate::New).
#pragma control sched=1
cBase *eDynamicModelLookAtTemplate::New(cMemPool *pool, cBase *parent) {
    eDynamicModelLookAtTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    eDynamicModelLookAtTemplate_AllocRec *rec =
        (eDynamicModelLookAtTemplate_AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    eDynamicModelLookAtTemplate *obj =
        (eDynamicModelLookAtTemplate *)rec->fn(base, 0x38, 4, 0, 0);
    if (obj != 0) {
        eDynamicModelLookAtTemplate_eDynamicModelLookAtTemplate(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2

// ── eDynamicModelLookAtTemplate::GetType(void) const  @ 0x001f0f40 ──
#pragma control sched=1
const cType *eDynamicModelLookAtTemplate::GetType(void) const {
    if (D_000469E4 == 0) {
        if (D_000385DC == 0) {
            const char *name = (const char *)0x36CD74;
            const char *desc = (const char *)0x36CD7C;
            __asm__ volatile("" : "+r"(name), "+r"(desc));
            D_000385DC = cType::InitializeType(name, desc, 1, 0, 0, 0, 0, 0);
        }
        __asm__ volatile("" ::: "memory");
        const cType *base = D_000385DC;
        __asm__ volatile("" : "+r"(base));
        cBase *(*factory)(cMemPool *, cBase *) = &eDynamicModelLookAtTemplate::New;
        __asm__ volatile("" : "+r"(factory));
        D_000469E4 = cType::InitializeType(0, 0, 0x48, base, factory, 0, 0, 0);
    }
    return D_000469E4;
}
#pragma control sched=2

// ──────────────────────────────────────────────────────────────────────────
// gcStateHandlerBase::GetName(char *) const  @ 0x002571a8
// ──────────────────────────────────────────────────────────────────────────

class gcStateHandlerBase {
public:
    void GetName(char *) const;
};

void gcStateHandlerBase::GetName(char *buf) const {
    cHandlePairT_GetName((char *)this + 8, buf, 0, 1);
}

// ──────────────────────────────────────────────────────────────────────────
// gcEvent::GetName(char *) const  @ 0x0023a7e0
// ──────────────────────────────────────────────────────────────────────────

class gcEvent {
public:
    void GetName(char *) const;
};

struct gcEvent_data {
    char _pad[16];
    const char *mName;
};

void gcEvent::GetName(char *buf) const {
    *buf = 0;
    if (((const gcEvent_data *)this)->mName != 0) {
        cStrCopy(buf, ((const gcEvent_data *)this)->mName);
    }
}

// ──────────────────────────────────────────────────────────────────────────
// gcLobbyConfigStrings::GetName(char *) const  @ 0x002807a8
// ──────────────────────────────────────────────────────────────────────────

class gcLobbyConfigStrings {
public:
    void GetName(char *) const;
};

void gcLobbyConfigStrings::GetName(char *buf) const {
    cStrAppend(buf, gcLobbyConfigStrings_fmt, gcValGetText_text);
}

// ──────────────────────────────────────────────────────────────────────────
// gcValLobbyConnectionStatus::GetText(char *) const  @ 0x00347c9c
// ──────────────────────────────────────────────────────────────────────────

class gcValLobbyConnectionStatus {
public:
    void GetText(char *) const;
};

void gcValLobbyConnectionStatus::GetText(char *buf) const {
    cStrAppend(buf, gcValLobbyConnectionStatus_fmt, gcValGetText_text);
}
