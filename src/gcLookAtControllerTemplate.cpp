// gcLookAtControllerTemplate — gcAll_psp.obj
//
// Functions:
//   gcLookAtControllerTemplate::Write(cFile &) const                 @ 0x00146CD8 ( 88B)
//   gcLookAtControllerTemplate::AssignCopy(const cBase *)            @ 0x002C68B4 ( 96B)
//   gcLookAtControllerTemplate::~gcLookAtControllerTemplate(void)    @ 0x00146E3C (124B)
//
// Inheritance: cBase -> gcPartialEntityControllerTemplate -> gcLookAtControllerTemplate.
// Layout: gcPartialEntityControllerTemplate base (mOwner@0, mClassDesc@4,
// mHandle1@8, mIndex@C, mHandle2@10) plus a signed char field at 0x14.

class cBase;
class cFile;
class cMemPool;

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

extern char gcLookAtControllerTemplatevirtualtable[];

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
