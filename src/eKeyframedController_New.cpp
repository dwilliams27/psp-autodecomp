// eKeyframedController::New(cMemPool *, cBase *) static — 0x0020de5c
//
// Standard pool-allocator + placement-construct factory. Same template as
// eConfigBase::New (matched 124B exemplar) and gcDoChangeState::New: walk
// pool[9] -> block, allocTable+0x28 -> AllocEntry, call entry->fn(base,
// sizeof, align, 0, 0); on success chain to the cBaseptr ctor, return the
// new object cast to cBase *. Object size is 0x18 (16B base
// ePhysicsController + mField10 + mField14).
//
// The constructor is referenced via free-function alias because the
// ctor's mangled symbol isn't yet emitted by any matched source file in
// this TU; the relocation gets masked during compare. sched=1 because the
// surrounding eAll_psp unity-build region (incl. AssignCopy at 0x20de14)
// uses it.

class cBase;
class cMemPool;

class eKeyframedController {
public:
    static cBase *New(cMemPool *, cBase *);
};

void eKeyframedController_eKeyframedController(eKeyframedController *, cBase *);

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

#pragma control sched=1
cBase *eKeyframedController::New(cMemPool *pool, cBase *parent) {
    eKeyframedController *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eKeyframedController *obj = (eKeyframedController *)entry->fn(base, 0x18, 4, 0, 0);
    if (obj != 0) {
        eKeyframedController_eKeyframedController(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
