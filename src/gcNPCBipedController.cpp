// gcNPCBipedController — gcAll_psp.obj.
//
// Functions:
//   gcNPCBipedController::Write(cFile &) const            @ 0x00154d70  (76B)
//   gcNPCBipedController::New(cMemPool *, cBase *) static @ 0x0031d25c (124B)
//   gcNPCBipedController::~gcNPCBipedController(void)     @ 0x0031d470 (124B)
//
// gcNPCBipedController inherits directly from gcEntityController (the
// destructor chain at 0x31d49c jumps to ~gcEntityController, not
// ~gcBipedController). Write() forwards to gcBipedController::Write —
// gcBipedController is a sibling class, not an ancestor.

class cBase;
class cFile;
class cMemPool;

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
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

class gcEntityController {
public:
    gcEntityController(cBase *);
    ~gcEntityController();
};

// Sibling class — not an ancestor. Used only to call gcBipedController::Write
// on `this` via a C-style cast. The jal target is a relocation, masked by
// asm-differ during object-file compare.
class gcBipedController {
public:
    void Write(cFile &) const;
};

class gcNPCBipedController : public gcEntityController {
public:
    gcNPCBipedController(cBase *);
    ~gcNPCBipedController();
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
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

extern char gcNPCBipedControllervirtualtable[];

extern "C" void gcNPCBipedController__gcNPCBipedController_cBaseptr(void *self, cBase *parent);

// ── gcNPCBipedController::Write(cFile &) const @ 0x00154d70 ──
void gcNPCBipedController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const gcBipedController *)this)->Write(file);
    wb.End();
}

// ── gcNPCBipedController::New(cMemPool *, cBase *) static @ 0x0031d25c ──
cBase *gcNPCBipedController::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcNPCBipedController *result = 0;
    gcNPCBipedController *obj = (gcNPCBipedController *)rec->fn(base, 0xC0, 0x10, 0, 0);
    if (obj != 0) {
        gcNPCBipedController__gcNPCBipedController_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcNPCBipedController::~gcNPCBipedController(void) @ 0x0031d470 ──
gcNPCBipedController::~gcNPCBipedController() {
    *(void **)((char *)this + 4) = gcNPCBipedControllervirtualtable;
}
