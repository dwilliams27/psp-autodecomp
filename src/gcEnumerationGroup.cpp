// gcEnumerationGroup: Write/Read serialization + destructor.
//
// Same shape as gcEntityTemplateGroup / gcUIDialogGroup / gcEntityGroup
// (parent cGroup; cBase header 8 bytes; vtable at offset 4). Patterns
// mirror cFactory::Write / eWeatherEffect::Read / eSphereShape::~eSphereShape
// — SNC's ABI auto-generates the dtor's (this!=0) guard, the chain call
// to ~cGroup, and the deleting-tail dispatch through operator delete.

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
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcEnumerationGroup : public cGroup {
public:
    gcEnumerationGroup(cBase *);
    static bool IsManagedTypeExternalStatic();
    static cBase *New(cMemPool *, cBase *);
    const cType *GetType() const;
    ~gcEnumerationGroup();
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

extern char gcEnumerationGroupvirtualtable[];
extern char cGroupvirtualtable[];
extern char cBasevirtualtable[];

// ── gcEnumerationGroup::New(cMemPool *, cBase *) static @ 0x002372C4 ──
cBase *gcEnumerationGroup::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    AllocEntry *e = (AllocEntry *)((char *)((void **)block)[7] + 0x28);
    short off = e->offset;
    void *base = (char *)block + off;
    gcEnumerationGroup *result = 0;
    gcEnumerationGroup *obj =
        (gcEnumerationGroup *)e->fn(base, 0x10, 4, 0, 0);
    if (obj != 0) {
        unsigned char flag = 0;
        if (IsManagedTypeExternalStatic() == 0) flag = 1;
        flag = (unsigned char)(flag & 0xff);
        ((void **)obj)[1] = cBasevirtualtable;
        ((cBase **)obj)[0] = parent;
        ((void **)obj)[1] = cGroupvirtualtable;
        ((unsigned char *)obj)[8] = flag;
        ((int *)obj)[3] = 0;
        ((void **)obj)[1] = gcEnumerationGroupvirtualtable;
        result = obj;
    }
    return (cBase *)result;
}

// ── gcEnumerationGroup::Write(cFile &) const @ 0x000D03C4 ──
void gcEnumerationGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    cGroup::Write(file);
    wb.End();
}

// ── gcEnumerationGroup::Read(cFile &, cMemPool *) @ 0x000D0410 ──
int gcEnumerationGroup::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->cGroup::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcEnumerationGroup::~gcEnumerationGroup(void) @ 0x00237478 ──
gcEnumerationGroup::~gcEnumerationGroup() {
    *(void **)((char *)this + 4) = gcEnumerationGroupvirtualtable;
}
