// gcTriggerGroup: Write/Read serialization + destructor + AssignCopy.
//
// Same shape as gcEntityTemplateGroup / gcUIDialogGroup / gcEntityGroup
// (parent cGroup; cBase header 8 bytes; vtable at offset 4). Patterns
// mirror cFactory::Write / eWeatherEffect::Read / eSphereShape::~eSphereShape
// — SNC's ABI auto-generates the dtor's (this!=0) guard, the chain call to
// ~cGroup, and the deleting-tail dispatch through operator delete.

class cBase;
class cFile;
class cMemPool;

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
    char _pad[8];
    cGroup(cBase *);
    ~cGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcTriggerGroup : public cGroup {
public:
    unsigned char mFlag;     // 0x08
    char _pad1[3];
    int mField;              // 0x0C

    gcTriggerGroup(cBase *);
    ~gcTriggerGroup();
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
};

class gcCinematicGroup {
public:
    char _pad0[8];
    unsigned char mFlag;
    char _pad1[3];
    int mField;
    void AssignCopy(const cBase *);
};

extern char gcTriggerGroupvirtualtable[];

void gcTriggerGroup::AssignCopy(const cBase *base) {
    gcTriggerGroup *src = dcast<gcTriggerGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

void gcCinematicGroup::AssignCopy(const cBase *base) {
    gcCinematicGroup *src = dcast<gcCinematicGroup>(base);
    mFlag = src->mFlag;
    mField = src->mField;
}

// ── gcTriggerGroup::Write(cFile &) const @ 0x000CFE00 ──
void gcTriggerGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── gcTriggerGroup::Read(cFile &, cMemPool *) @ 0x000CFE4C ──
// Unmatched: 20-byte prologue scheduling diff. The expected output
// interleaves register saves (sw s0/s1/s2/s3/ra) with arg-setup moves and
// places `li s3,1` BEFORE the jal to cReadBlock. Our SNC issues the saves
// in a contiguous block and defers `li s3,1` to AFTER the jal (since s3 is
// callee-saved and unused until the success epilogue). Tested: original
// asm-first ordering (34B diff), permuter for 5min/5369 candidates
// (improved from 34→20 but plateaued), `__asm__ volatile("":"+r"):::"memory"`
// anchor (back to 34B), pragma sched=0 (127B diff), pragma sched=1 (31B
// diff), register asm("$s3")=1 (size 176B — constant-folded). Same
// pattern affects every gcXxxGroup::Read in the codebase (gcEntityGroup,
// gcEntityTemplateGroup, gcConstantGroup, etc — none matched).
int gcTriggerGroup::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->cGroup::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcTriggerGroup::~gcTriggerGroup(void) @ 0x00237210 ──
gcTriggerGroup::~gcTriggerGroup() {
    *(void **)((char *)this + 4) = gcTriggerGroupvirtualtable;
}

extern "C" {

long strtol(const char *s, char **endptr, int base);
struct lconv *_localeconv_r(void *r);
void *memset(void *dst, int val, unsigned int n);

extern void *_impure_ptr;

int atoi(const char *s) {
    return (int)strtol(s, 0, 10);
}

struct lconv *localeconv(void) {
    return _localeconv_r(_impure_ptr);
}

void *__memzero(void *dst, unsigned int n) {
    return memset(dst, 0, n);
}

}
