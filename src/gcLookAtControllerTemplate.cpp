// gcLookAtControllerTemplate — gcAll_psp.obj
//
// Functions:
//   gcLookAtControllerTemplate::Write(cFile &) const                 @ 0x00146CD8 ( 88B)
//   gcLookAtControllerTemplate::AssignCopy(const cBase *)            @ 0x002C68B4 ( 96B)
//   gcLookAtControllerTemplate::~gcLookAtControllerTemplate(void)    @ 0x00146E3C (124B)
//   gcLookAtControllerTemplate::GetInstanceType(void) const          @ 0x00146EB8 (228B)
//
// Also instantiates two gcDesiredObjectT<T1,T2,T3>::~gcDesiredObjectT
// destructors that the orchestrator routed into this file:
//   gcDesiredObjectT<gcDesiredCustomAnimation, ...>::~gcDesiredObjectT @ 0x0026566C
//   gcDesiredObjectT<gcDesiredCustomAttack,    ...>::~gcDesiredObjectT @ 0x00266D30
//
// Inheritance: cBase -> gcPartialEntityControllerTemplate -> gcLookAtControllerTemplate.
// Layout: gcPartialEntityControllerTemplate base (mOwner@0, mClassDesc@4,
// mHandle1@8, mIndex@C, mHandle2@10) plus a signed char field at 0x14.

class cBase;
class cFile;
class cMemPool;
class cType;

inline void *operator new(unsigned int, void *p) { return p; }

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
    void Write(char);
    void End(void);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

extern char gcLookAtControllerTemplatevirtualtable[];
extern cType *D_000385DC;
extern cType *D_0009F5DC;
extern cType *D_0009F644;
extern cType *D_0009F5E0;
extern cType *D_0009F648;

class gcPartialEntityController {
public:
    static cBase *New(cMemPool *, cBase *);
};

class gcLookAtController {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cHandle {
public:
    int mIndex;
    cHandle &operator=(const cHandle &rhs) { mIndex = rhs.mIndex; return *this; }
};

class gcPartialEntityControllerTemplate {
public:
    cBase *mOwner;          // 0x00
    void *mClassDesc;       // 0x04
    cHandle mHandle1;       // 0x08
    int mIndex;             // 0x0C
    cHandle mHandle2;       // 0x10

    gcPartialEntityControllerTemplate(cBase *);
    ~gcPartialEntityControllerTemplate();
    void Write(cFile &) const;
};

class gcLookAtControllerTemplate : public gcPartialEntityControllerTemplate {
public:
    signed char mField14;   // 0x14

    gcLookAtControllerTemplate(cBase *);
    ~gcLookAtControllerTemplate();
    void Write(cFile &) const;
    void AssignCopy(const cBase *);
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType(void) const;
    const cType *GetInstanceType(void) const;

    // Inlined into the deleting-destructor tail. No null-pool guard
    // matches the 0x00146e7c-0x00146ea0 pool-dispatch sequence.
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

template <class T> T *dcast(const cBase *);

// ── gcLookAtControllerTemplate::Write(cFile &) const @ 0x00146CD8 ──
void gcLookAtControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcPartialEntityControllerTemplate::Write(file);
    wb.Write(mField14);
    wb.End();
}

// ── gcLookAtControllerTemplate::AssignCopy(const cBase *) @ 0x002C68B4 ──
// SNC emits a `addiu / lw / addiu / sw` quartet per int field rather than
// folding the immediate into the load/store. The pattern reproduces when
// the source uses explicit pointer arithmetic with separate src/dst pointer
// expressions per field.
void gcLookAtControllerTemplate::AssignCopy(const cBase *base) {
    gcLookAtControllerTemplate *other = dcast<gcLookAtControllerTemplate>(base);
    {
        cHandle *p = &this->mHandle1;
        const cHandle *q = &other->mHandle1;
        *p = *q;
    }
    __asm__ volatile("" ::: "memory");
    const int *qi = &other->mIndex;
    int *pi = &this->mIndex;
    const cHandle *qh = &other->mHandle2;
    *pi = *qi;
    cHandle *ph = &this->mHandle2;
    *ph = *qh;
    mField14 = other->mField14;
}

// ── gcLookAtControllerTemplate::~gcLookAtControllerTemplate(void) @ 0x00146E3C ──
// SNC ABI auto-generates the (this != 0) entry guard, the parent-chain
// ~gcPartialEntityControllerTemplate(0) call, and the deleting tail
// `if (flags & 1) operator delete(this)` epilogue around this body.
gcLookAtControllerTemplate::~gcLookAtControllerTemplate() {
    *(void **)((char *)this + 4) = gcLookAtControllerTemplatevirtualtable;
}

// ── gcLookAtControllerTemplate::New(cMemPool *, cBase *) static @ 0x002C6914 ──
cBase *gcLookAtControllerTemplate::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcLookAtControllerTemplate *result = 0;
    gcLookAtControllerTemplate *obj = (gcLookAtControllerTemplate *)rec->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcLookAtControllerTemplate(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcLookAtControllerTemplate::GetType(void) const @ 0x002C6990 ──
const cType *gcLookAtControllerTemplate::GetType(void) const {
    if (D_0009F644 == 0) {
        if (D_0009F5DC == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F5DC = cType::InitializeType(0, 0, 0x104, D_000385DC,
                                               0, 0, 0, 0);
        }
        D_0009F644 = cType::InitializeType(0, 0, 0x10D, D_0009F5DC,
                                           &gcLookAtControllerTemplate::New,
                                           0, 0, 0);
    }
    return D_0009F644;
}

// ── gcLookAtControllerTemplate::GetInstanceType(void) const @ 0x00146EB8 ──
// Mirrors gcPartialBodyControllerTemplate::GetInstanceType pattern.
// Three-level type registration: cBase root → gcPartialEntityController (0x105)
// → gcLookAtController (0x10E).
const cType *gcLookAtControllerTemplate::GetInstanceType(void) const {
    if (D_0009F648 == 0) {
        if (D_0009F5E0 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_0009F5E0 = cType::InitializeType(0, 0, 0x105, D_000385DC,
                                               &gcPartialEntityController::New,
                                               0, 0, 0);
        }
        D_0009F648 = cType::InitializeType(0, 0, 0x10E, D_0009F5E0,
                                           &gcLookAtController::New,
                                           0, 0, 0);
    }
    return D_0009F648;
}

// =====================================================================
// gcDesiredObjectT<T1,T2,T3>::~gcDesiredObjectT — template instantiations
// =====================================================================
// NOT MATCHED. See logs/session_results/7429592b.json notes.
//
// The original SNC binary contains these as 240-byte deleting-destructor
// thunks (`__0oQ...dtv`) emitted in `.gnu.linkonce.t.<symbol>` sections
// with 8 trailing bytes (`b $- ; nop`) padding the symbol to 0xF0. The
// canonical C++ template-method form below compiles to the right body
// (~232B) but SNC's section selection for template instantiations means
// the trailing-pad/`.size` extension trick that works for the non-template
// case (see src/gcDesiredObject.cpp, src/gcDesiredPath.cpp) cannot extend
// a linkonce section. The byte content of the body itself matches; only
// the 8-byte tail is missing. Left as canonical so the source teaches the
// correct C++ abstraction, even at the cost of a non-match.
//
// Mangled names (from extern/extracted_symbols/Game-dvd-pal.sym):
//   __0oQgcDesiredObjectT76YgcDesiredCustomAnimation6egcDesiredCustomAnimationHelper6XgcEntityCustomAnimation_dtv
//   __0oQgcDesiredObjectT76VgcDesiredCustomAttack6bgcDesiredCustomAttackHelper6UgcEntityCustomAttack_dtv

class gcDesiredCustomAnimation;
class gcDesiredCustomAnimationHelper;
class gcEntityCustomAnimation;
class gcDesiredCustomAttack;
class gcDesiredCustomAttackHelper;
class gcEntityCustomAttack;

extern "C" void free(void *);

extern char gcDesiredObjectvirtualtable[];
extern char cBaseclassdesc[];

struct DtorDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

template <class T1, class T2, class T3>
class gcDesiredObjectT {
public:
    void *_owner;
    void *_vtable;
    int _slot;
    ~gcDesiredObjectT();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            DtorDeleteRecord *rec =
                (DtorDeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(block + off, p);
        } else {
            free(p);
        }
    }
};

template <class T1, class T2, class T3>
gcDesiredObjectT<T1, T2, T3>::~gcDesiredObjectT() {
    *(char **)((char *)this + 4) = gcDesiredObjectvirtualtable;
    char *slot = (char *)this + 0x08;
    if (slot != 0) {
        int keep = 1;
        int val = *(int *)((char *)this + 0x08);
        if (val & 1) {
            keep = 0;
        }
        if (keep != 0 && val != 0) {
            char *obj = (char *)val;
            char *type = ((char **)obj)[1];
            DtorDeleteRecord *rec = (DtorDeleteRecord *)(type + 0x50);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(obj + off, (void *)3);
            *(int *)((char *)this + 0x08) = 0;
        }
    }
    *(char **)((char *)this + 4) = cBaseclassdesc;
}

template class gcDesiredObjectT<gcDesiredCustomAnimation,
                                gcDesiredCustomAnimationHelper,
                                gcEntityCustomAnimation>;
template class gcDesiredObjectT<gcDesiredCustomAttack,
                                gcDesiredCustomAttackHelper,
                                gcEntityCustomAttack>;
