// gcDoEntityFindAttached.cpp — decompiled from gcAll_psp.obj
//
//   0x002b0b00  Write(cFile &) const                  — 88B
//   0x002b0d50  GetText(char *) const                 — 104B
//   0x002b0db8  ~gcDoEntityFindAttached(void)         — 124B
//
// Derives from gcDoEntityFindAttachedBase. Vtable at offset 0x4. Same
// pattern as gcDoEntityForEachAttached / gcValEntityPrimaryController.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
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

class gcDoEntityFindAttachedBase {
public:
    char _padToC[0xC];
    int mFieldC;    // 0x0C — accessed by derived GetText
    char _padCto64[0x54];
    void Write(cFile &) const;
    void GetText(char *, char *, bool) const;
    ~gcDoEntityFindAttachedBase(void);
};

class gcDesiredEntity {
public:
    gcDesiredEntity &operator=(const gcDesiredEntity &);
};

class gcDesiredEntityTemplate {
public:
    gcDesiredEntityTemplate &operator=(const gcDesiredEntityTemplate &);
};

class gcEnumeration;
class gcEnumerationEntry;

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cSubHandleT {
public:
    int mIndex;
};

template <class T, class U>
class cHandlePairT {
public:
    cHandleT<T> mHandle;
    U mSubHandle;
};

template <class T>
class cArrayBase {
public:
    void *mData;
    cArrayBase &operator=(const cArrayBase &);
};

class gcDoEntityFindAttached : public gcDoEntityFindAttachedBase {
public:
    int mField64;   // 0x64
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    void GetText(char *) const;
    ~gcDoEntityFindAttached(void);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

void cStrAppend(char *, const char *, ...);

extern char gcDoEntityFindAttachedvirtualtable[];
extern const char gcDoEntityFindAttached_text_fmt[];
extern const char gcDoEntityFindAttached_text_arg[];
extern const char gcDoEntityFindAttached_text_label[];

template <class T> T *dcast(const cBase *);

// ── AssignCopy @ 0x002b07b8 ────────────────────────────────────────────
void gcDoEntityFindAttached::AssignCopy(const cBase *src) {
    gcDoEntityFindAttached *other = dcast<gcDoEntityFindAttached>(src);
    int flags = *(int *)((char *)this + 8) & ~3;
    *(int *)((char *)this + 8) = flags;
    *(int *)((char *)this + 8) = flags | (*(int *)((char *)other + 8) & 3);
    *(int *)((char *)this + 0x0C) = *(int *)((char *)other + 0x0C);
    ((gcDesiredEntity *)((char *)this + 0x10))->operator=(
        *(const gcDesiredEntity *)((char *)other + 0x10));
    *(int *)((char *)this + 0x3C) = *(int *)((char *)other + 0x3C);
    *(int *)((char *)this + 0x40) = *(int *)((char *)other + 0x40);
    *(int *)((char *)this + 0x44) = *(int *)((char *)other + 0x44);
    ((gcDesiredEntityTemplate *)((char *)this + 0x48))->operator=(
        *(const gcDesiredEntityTemplate *)((char *)other + 0x48));
    ((cArrayBase<cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > > *)((char *)this + 0x60))->operator=(
        *(const cArrayBase<cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > > *)((char *)other + 0x60));
    mField64 = other->mField64;
}

// ── Write @ 0x002b0b00 ─────────────────────────────────────────────────
void gcDoEntityFindAttached::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    gcDoEntityFindAttachedBase::Write(file);
    wb.Write(mField64);
    wb.End();
}

// ── GetText @ 0x002b0d50 ───────────────────────────────────────────────
void gcDoEntityFindAttached::GetText(char *buf) const {
    cStrAppend(buf,
               gcDoEntityFindAttached_text_fmt,
               gcDoEntityFindAttached_text_arg);
    gcDoEntityFindAttachedBase::GetText(buf,
                                        (char *)gcDoEntityFindAttached_text_label,
                                        mFieldC == 1);
}

// ── ~gcDoEntityFindAttached @ 0x002b0db8 ───────────────────────────────
gcDoEntityFindAttached::~gcDoEntityFindAttached(void) {
    *(void **)((char *)this + 4) = gcDoEntityFindAttachedvirtualtable;
}
