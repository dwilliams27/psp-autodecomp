// eKeyframedController::~eKeyframedController(void) — 0x00074ef4
//
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain call to ~ePhysicsController() with flags=0, and the deleting-
// tail dispatch through `operator delete` (which itself does pool/block
// lookup and calls the slot fn). Same shape as eSphereShape::~eSphereShape
// (matched in src/eSphereShape_dtor.cpp), but with ePhysicsController as
// the base class and a 0x10 base layout (no free() fallback in operator
// delete since the original binary lacks the null-pool branch here).
//
// Local class redeclaration is used so eKeyframedController can declare
// inheritance from ePhysicsController; include/eKeyframedController.h
// declares the class without a base because most of its methods don't
// need the chain-call wiring, and adding inheritance there would risk
// disturbing already-matched siblings.

class cBase;

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

#pragma control sched=1

class ePhysicsController {
public:
    char _pad[0x10];
    ePhysicsController(cBase *);
    ~ePhysicsController();
};

class eKeyframedController : public ePhysicsController {
public:
    int mField10;
    int mField14;
    ~eKeyframedController();
    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        char *block = ((char **)pool)[9];
        DeleteRecord *rec = (DeleteRecord *)(((char **)block)[7] + 0x30);
        short off = rec->offset;
        __asm__ volatile("" ::: "memory");
        char *base = block + off;
        void (*fn)(void *, void *) = rec->fn;
        fn(base, p);
    }
};

extern char eKeyframedControllervirtualtable[];

eKeyframedController::~eKeyframedController() {
    *(void **)((char *)this + 4) = eKeyframedControllervirtualtable;
}
