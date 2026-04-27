// gcStringTableGroup: Write/Read serialization + destructor.
//
// Parent class is cGroup (same shape as gcVariableGroup, eSkinGroup, etc.).
// Patterns mirror cFactory::Write / eWeatherEffect::Read / eSphereShape::~eSphereShape
// — SNC's ABI auto-generates the dtor's (this!=0) guard, the chain call to
// ~cGroup, and the deleting-tail dispatch through operator delete.

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
    int base;

    cGroup(cBase *);
    ~cGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcStringTableGroup : public cGroup {
public:
    gcStringTableGroup(cBase *);
    ~gcStringTableGroup();
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

extern char gcStringTableGroupvirtualtable[];

// ── gcStringTableGroup::Write(cFile &) const @ 0x000CE6F0 ──
void gcStringTableGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── gcStringTableGroup::Read(cFile &, cMemPool *) @ 0x000CE73C ──
int gcStringTableGroup::Read(cFile &file, cMemPool *pool) {
    int ok;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(ok));
    if ((unsigned int)rb._data[3] != 1 || !this->cGroup::Read(file, pool)) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    return ok;
}

// ── gcStringTableGroup::~gcStringTableGroup(void) @ 0x00236870 ──
gcStringTableGroup::~gcStringTableGroup() {
    *(void **)((char *)this + 4) = gcStringTableGroupvirtualtable;
}
