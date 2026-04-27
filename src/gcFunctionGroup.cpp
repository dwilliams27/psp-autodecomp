// gcFunctionGroup — Write, Read, ~gcFunctionGroup
//
// Modeled directly on the matched gcStaticInstanceGroup family in
// src/gcStaticInstanceGroup.cpp (same .obj, same shape, same calls).

class cBase;
class cFile;
class cMemPool;

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cGroup {
public:
    cBase *m_parent;        // 0x00
    void *m_vtbl;           // 0x04
    unsigned char mFlag;    // 0x08
    char _pad[3];
    int mField;             // 0x0C
    cGroup(cBase *);
    ~cGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcFunctionGroup : public cGroup {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    ~gcFunctionGroup();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        fn(block + off, p);
    }
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

void cFile_SetCurrentPos_Group(void *, unsigned int);

extern char gcFunctionGroupvirtualtable[];

// ── gcFunctionGroup::Write(cFile &) const @ 0x000cd5a4 ──
// Same shape as cFactory::Write — chain to cGroup::Write under a cWriteBlock.
void gcFunctionGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->cGroup::Write(file);
    wb.End();
}

// ── gcFunctionGroup::Read(cFile &, cMemPool *) @ 0x000cd5f0 ──
// Same shape as eWeatherEffect::Read / gcStaticInstanceGroup::Read.
#pragma control sched=1
int gcFunctionGroup::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->cGroup::Read(file, pool)) goto success;
    cFile_SetCurrentPos_Group(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}
#pragma control sched=2

// ── gcFunctionGroup::~gcFunctionGroup(void) @ 0x00236138 ──
// Canonical C++ destructor; SNC ABI auto-generates the (this != 0) guard,
// the chain call to ~cGroup(), and the deleting-tail `if (flags & 1)`
// dispatch via operator delete.
gcFunctionGroup::~gcFunctionGroup() {
    *(void **)((char *)this + 4) = gcFunctionGroupvirtualtable;
}
