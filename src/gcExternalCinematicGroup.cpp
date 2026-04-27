// Multi-class wrapper file for small AssignCopy / GetText / ctor methods,
// plus gcExternalCinematicGroup serialization + destructor.
// Functions:
//   gcExternalCinematicGroup::Write(cFile &) const         @ 0x000d37a8
//   gcExternalCinematicGroup::Read(cFile &, cMemPool *)    @ 0x000d37f4
//   gcExternalCinematicGroup::~gcExternalCinematicGroup    @ 0x00238a20
//   gcExternalCinematicGroup::AssignCopy(const cBase *)    @ 0x00238834
//   gcValCameraIsValid::GetText(char *) const              @ 0x00323334
//   gcValConstant::AssignCopy(const cBase *)               @ 0x0032535c
//   gcValVariable::AssignCopy(const cBase *)               @ 0x0036a9a8
//   eFrameBufferTexture::eFrameBufferTexture(cBase *)      @ 0x00081718

class cBase;
class cFile;
class cMemPool;

void cStrAppend(char *, const char *, ...);

extern const char gcValCameraIsValid_text[];   // @ 0x36F18C

// ─────────────────────────────────────────────────────────────────────────
// gcExternalCinematicGroup: parent cGroup; cBase header 8 bytes; vtable at
// offset 4. Same shape as gcCinematicGroup (see src/gcCinematicGroup.cpp).
// ─────────────────────────────────────────────────────────────────────────

template <class T> T *dcast(const cBase *);

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

void cFile_SetCurrentPos(void *, unsigned int);

class cGroup {
public:
    cGroup(cBase *);
    char _pad[8];
    ~cGroup();
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcExternalCinematicGroup : public cGroup {
public:
    unsigned char f8;       // 0x8 (immediately after cGroup base)
    char _pad9[3];
    int fC;                 // 0xC
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    ~gcExternalCinematicGroup();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

extern char gcExternalCinematicGroupvirtualtable[];

// ── gcExternalCinematicGroup::AssignCopy(const cBase *) @ 0x00238834 ──
void gcExternalCinematicGroup::AssignCopy(const cBase *base) {
    gcExternalCinematicGroup *other = dcast<gcExternalCinematicGroup>(base);
    this->f8 = other->f8;
    this->fC = other->fC;
}

// ── gcExternalCinematicGroup::Write(cFile &) const @ 0x000d37a8 ──
void gcExternalCinematicGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── gcExternalCinematicGroup::Read(cFile &, cMemPool *) @ 0x000d37f4 ──
int gcExternalCinematicGroup::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] != 1) goto fail;
    if (!this->cGroup::Read(file, pool)) goto fail;
    return result;
fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
}

// ── gcExternalCinematicGroup::~gcExternalCinematicGroup(void) @ 0x00238a20 ──
// Canonical C++ destructor; SNC ABI auto-generates the (this != 0) guard,
// the chain call to ~cGroup() with flags=0, and the deleting-tail dispatch
// through operator delete.
gcExternalCinematicGroup::~gcExternalCinematicGroup() {
    *(void **)((char *)this + 4) = gcExternalCinematicGroupvirtualtable;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValCameraIsValid::GetText(char *) const @ 0x00323334
// ─────────────────────────────────────────────────────────────────────────

class gcDesiredCamera {
public:
    void GetText(char *) const;
};

class gcValCameraIsValid {
public:
    char _pad0[8];
    gcDesiredCamera f8;
    void GetText(char *) const;
};

void gcValCameraIsValid::GetText(char *buf) const {
    this->f8.GetText(buf);
    cStrAppend(buf, gcValCameraIsValid_text);
}

// ─────────────────────────────────────────────────────────────────────────
// gcValConstant::AssignCopy(const cBase *) @ 0x0032535c
// ─────────────────────────────────────────────────────────────────────────

class gcValConstant {
public:
    char _pad0[8];
    int f8;
    void AssignCopy(const cBase *);
};

void gcValConstant::AssignCopy(const cBase *base) {
    gcValConstant *other = dcast<gcValConstant>(base);
    int *src = (int *)((char *)other + 8);
    int *dst = (int *)((char *)this + 8);
    *dst = *src;
}

// ─────────────────────────────────────────────────────────────────────────
// gcValVariable::AssignCopy(const cBase *) @ 0x0036a9a8
// ─────────────────────────────────────────────────────────────────────────

class gcValVariable {
public:
    char _pad0[8];
    int f8;
    void AssignCopy(const cBase *);
};

void gcValVariable::AssignCopy(const cBase *base) {
    gcValVariable *other = dcast<gcValVariable>(base);
    int *src = (int *)((char *)other + 8);
    int *dst = (int *)((char *)this + 8);
    *dst = *src;
}

// ─────────────────────────────────────────────────────────────────────────
// eFrameBufferTexture::eFrameBufferTexture(cBase *) @ 0x00081718
// ─────────────────────────────────────────────────────────────────────────

class eVirtualTexture {
public:
    eVirtualTexture(cBase *);
};

class eFrameBufferTexture : public eVirtualTexture {
public:
    eFrameBufferTexture(cBase *);
};

extern char eFrameBufferTextureclassdesc[];

eFrameBufferTexture::eFrameBufferTexture(cBase *parent) : eVirtualTexture(parent) {
    ((void **)this)[1] = eFrameBufferTextureclassdesc;
    __asm__ volatile("" ::: "memory");
    ((unsigned char *)this)[0x50] = 1;
}
