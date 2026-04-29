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
class cType;

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *, cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
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
    static bool IsManagedTypeExternalStatic();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType() const;
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
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

extern cType *D_000385DC;
extern cType *D_00040C94;
extern cType *D_000998D4;

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

// ── gcCinematicGroup::New(cMemPool *, cBase *) static @ 0x00237C64 ──
cBase *gcCinematicGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcCinematicGroup *result = 0;
    gcCinematicGroup *obj = (gcCinematicGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = gcCinematicGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcCinematicGroup::GetType(void) const @ 0x00237D20 ──
const cType *gcCinematicGroup::GetType() const {
    if (D_000998D4 == 0) {
        if (D_00040C94 == 0) {
            if (D_000385DC == 0) {
                D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                   (const char *)0x36D89C,
                                                   1, 0, 0, 0, 0, 0);
            }
            D_00040C94 = cType::InitializeType(0, 0, 4, D_000385DC, 0, 0, 0, 0);
        }
        D_000998D4 = cType::InitializeType(
            0, 0, 0x168, D_00040C94,
            (cBase *(*)(cMemPool *, cBase *))&gcCinematicGroup::New,
            0, 0, 8);
    }
    return D_000998D4;
}

// ── gcCinematicGroup::~gcCinematicGroup(void) @ 0x00237E18 ──
gcCinematicGroup::~gcCinematicGroup() {
    *(void **)((char *)this + 4) = gcCinematicGroupvirtualtable;
}
