// gcCreatureController — gcAll_psp.obj.
//
// Functions:
//   gcCreatureController::Write(cFile &) const         @ 0x0013ca68  (76B)
//   gcCreatureController::Read(cFile &, cMemPool *)    @ 0x0013cab4 (188B)
//   gcCreatureController::~gcCreatureController(void)  @ 0x0029429c (124B)
//
// gcCreatureController inherits from gcEntityController.

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

class gcEntityController {
public:
    gcEntityController(cBase *);
    ~gcEntityController();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcCreatureController : public gcEntityController {
public:
    gcCreatureController(cBase *);
    ~gcCreatureController();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        void (*fn)(void *, void *) = rec->fn;
        char *base = block + off;
        fn(base, p);
    }
};

extern char gcCreatureControllervirtualtable[];

// ── gcCreatureController::Write(cFile &) const @ 0x0013ca68 ──
void gcCreatureController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcEntityController *)this)->Write(file);
    wb.End();
}

// ── gcCreatureController::Read(cFile &, cMemPool *) @ 0x0013cab4 ──
int gcCreatureController::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcEntityController::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcCreatureController::~gcCreatureController(void) @ 0x0029429c ──
gcCreatureController::~gcCreatureController() {
    *(void **)((char *)this + 4) = gcCreatureControllervirtualtable;
}
