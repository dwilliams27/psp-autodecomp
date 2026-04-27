// gcCinematicGroup: Write/Read serialization + destructor.
//
// Same shape as gcEnumerationGroup / gcEntityTemplateGroup / gcUIDialogGroup
// (parent cGroup; cBase header 8 bytes; vtable at offset 4). Patterns mirror
// cFactory::Write / eWeatherEffect::Read / eSphereShape::~eSphereShape — SNC's
// ABI auto-generates the dtor's (this!=0) guard, the chain call to ~cGroup,
// and the deleting-tail dispatch through operator delete.

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
    char _pad[8];
    cGroup(cBase *);
    ~cGroup();
    int Read(cFile &, cMemPool *);
    void Write(cFile &) const;
};

class gcCinematicGroup : public cGroup {
public:
    gcCinematicGroup(cBase *);
    ~gcCinematicGroup();
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

extern char gcCinematicGroupvirtualtable[];

// ── gcCinematicGroup::Write(cFile &) const @ 0x000D1AD4 ──
void gcCinematicGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── gcCinematicGroup::Read(cFile &, cMemPool *) @ 0x000D1B20 ──
int gcCinematicGroup::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->cGroup::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcCinematicGroup::~gcCinematicGroup(void) @ 0x00237E18 ──
gcCinematicGroup::~gcCinematicGroup() {
    *(void **)((char *)this + 4) = gcCinematicGroupvirtualtable;
}
