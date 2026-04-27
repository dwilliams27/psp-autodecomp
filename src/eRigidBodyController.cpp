// eRigidBodyController — engine sched=1 zone (eAll_psp.obj 0x06e000-0x0bab28).
//
// The constructor (0x000768c8) is matched in src/eInput.cpp using locally
// redeclared classes; this file holds the remaining methods (Write, dtor,
// New). Class is laid out with a tighter eSimulatedController padding than
// include/eSimulatedController.h, sized so eRigidBodyController itself is
// 0x160 bytes (matches the size literal passed by ::New).

class cBase;
class cMemPool;
class cFile;

#pragma control sched=1

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cMemPool_shim {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocRec {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class eSimulatedController {
public:
    char _pad[0x154];
    eSimulatedController(cBase *);
    ~eSimulatedController();
    void Write(cFile &) const;
};

class eRigidBodyController : public eSimulatedController {
public:
    char _extra[0xC];   // brings size to 0x160
    eRigidBodyController(cBase *);
    ~eRigidBodyController();
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool_shim::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

// ── eRigidBodyController::Write(cFile &) const @ 0x000767c0 ──
void eRigidBodyController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ((const eSimulatedController *)this)->Write(file);
    wb.End();
}

// ── eRigidBodyController::~eRigidBodyController(void) @ 0x000768fc ──
eRigidBodyController::~eRigidBodyController() {
    *(void **)((char *)this + 4) = (void *)0x384078;
}

// ── eRigidBodyController::New(cMemPool *, cBase *) static @ 0x0020f850 ──
extern "C" {
    void eRigidBodyController__eRigidBodyController_cBaseptr(void *self, cBase *parent);
}

cBase *eRigidBodyController::New(cMemPool *pool, cBase *parent) {
    eRigidBodyController *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    __asm__ volatile("" ::: "memory");
    eRigidBodyController *obj = (eRigidBodyController *)rec->fn((char *)block + rec->offset, 0x160, 0x10, 0, 0);
    if (obj != 0) {
        eRigidBodyController__eRigidBodyController_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
