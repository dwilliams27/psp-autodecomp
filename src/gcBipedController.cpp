// gcBipedController — gcAll_psp.obj.
//
// Functions:
//   gcBipedController::Write(cFile &) const          @ 0x0014111c  (76B)
//   gcBipedController::Read(cFile &, cMemPool *)     @ 0x00141168 (188B)
//   gcBipedController::OnDeactivated(void)           @ 0x00142154  (48B)
//   gcBipedController::~gcBipedController(void)      @ 0x002a7b84 (124B)
//
// gcBipedController inherits from gcCreatureController, which inherits
// from gcEntityController.

class cBase;
class cFile;
class cMemPool;
class cType;

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
    void OnDeactivated(void);
    void SetPhysicsController(const cType *);
};

class gcCreatureController : public gcEntityController {
public:
    gcCreatureController(cBase *);
    ~gcCreatureController();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
};

class gcBipedController : public gcCreatureController {
public:
    gcBipedController(cBase *);
    ~gcBipedController();
    void Write(cFile &) const;
    int Read(cFile &, cMemPool *);
    void OnDeactivated(void);
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

extern char gcBipedControllervirtualtable[];

// ── gcBipedController::Write(cFile &) const @ 0x0014111c ──
void gcBipedController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcCreatureController *)this)->Write(file);
    wb.End();
}

// ── gcBipedController::Read(cFile &, cMemPool *) @ 0x00141168 ──
int gcBipedController::Read(cFile &file, cMemPool *pool) {
    int result;
    cReadBlock rb(file, 1, true);
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    if ((unsigned int)rb._data[3] == 1 && this->gcCreatureController::Read(file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    return result;
}

// ── gcBipedController::OnDeactivated(void) @ 0x00142154 ──
void gcBipedController::OnDeactivated(void) {
    gcEntityController::OnDeactivated();
    gcEntityController::SetPhysicsController(0);
}

// ── gcBipedController::~gcBipedController(void) @ 0x002a7b84 ──
gcBipedController::~gcBipedController() {
    *(void **)((char *)this + 4) = gcBipedControllervirtualtable;
}
