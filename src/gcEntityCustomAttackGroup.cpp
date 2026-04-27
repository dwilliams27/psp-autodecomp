class cBase;
class cFile;
class cMemPool;

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

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

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
    int _pad[2];
    ~cGroup();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcEntityCustomAttackGroup : public cGroup {
public:
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    ~gcEntityCustomAttackGroup();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        rec->fn(block + rec->offset, p);
    }
};

extern char gcEntityCustomAttackGroupvirtualtable[];

// gcEntityCustomAttackGroup::Write(cFile &) const @ 0x000d265c
void gcEntityCustomAttackGroup::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    this->cGroup::Write(file);
    wb.End();
}

// gcEntityCustomAttackGroup::Read(cFile &, cMemPool *) @ 0x000d26a8
int gcEntityCustomAttackGroup::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->cGroup::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// gcEntityCustomAttackGroup::~gcEntityCustomAttackGroup(void) @ 0x002382e8
gcEntityCustomAttackGroup::~gcEntityCustomAttackGroup() {
    *(void **)((char *)this + 4) = gcEntityCustomAttackGroupvirtualtable;
}
