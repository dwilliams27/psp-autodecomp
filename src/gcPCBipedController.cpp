// gcPCBipedController — gcAll_psp.obj.
//
// Functions:
//   gcPCBipedController::Write(cFile &) const               @ 0x00152754  (76B)
//   gcPCBipedController::New(cMemPool *, cBase *) static    @ 0x0031a4c8 (124B)
//   gcPCBipedController::~gcPCBipedController(void)         @ 0x0031a6dc (124B)
//
// gcPCBipedController inherits from gcBipedController which inherits from
// gcCreatureController -> gcEntityController.

class cBase;
class cFile;
class cMemPool;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class gcEntityController {
public:
    gcEntityController(cBase *);
    ~gcEntityController();
};

class gcCreatureController : public gcEntityController {
public:
    gcCreatureController(cBase *);
    ~gcCreatureController();
};

class gcBipedController : public gcCreatureController {
public:
    gcBipedController(cBase *);
    ~gcBipedController();
    void Write(cFile &) const;
};

class gcPCBipedController : public gcBipedController {
public:
    gcPCBipedController(cBase *);
    ~gcPCBipedController();
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
};

extern "C" {
    void gcPCBipedController__gcPCBipedController_cBaseptr(void *self, cBase *parent);
    void gcEntityController___dtor_gcEntityController_void(void *self, int flags);
    void *cMemPool_GetPoolFromPtr(const void *);
}

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern char gcPCBipedControllervirtualtable[];

// ── gcPCBipedController::Write(cFile &) const @ 0x00152754 ──
void gcPCBipedController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcBipedController *)this)->Write(file);
    wb.End();
}

// ── gcPCBipedController::New(cMemPool *, cBase *) static @ 0x0031a4c8 ──
cBase *gcPCBipedController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcPCBipedController *result = 0;
    gcPCBipedController *obj = (gcPCBipedController *)rec->fn(base, 0xD0, 0x10, 0, 0);
    if (obj != 0) {
        gcPCBipedController__gcPCBipedController_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcPCBipedController::~gcPCBipedController(void) @ 0x0031a6dc ──
extern "C" {

void gcPCBipedController___dtor_gcPCBipedController_void(void *self, int flags) {
    if (self != 0) {
        *(void **)((char *)self + 4) = gcPCBipedControllervirtualtable;
        gcEntityController___dtor_gcEntityController_void(self, 0);
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            char *allocTable = *(char **)((char *)block + 0x1C);
            DeleteRecord *rec = (DeleteRecord *)(allocTable + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}

}
