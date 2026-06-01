// ePhysics::Initialize(void) static @ 0x000373e4 (eAll_psp.obj)
// Split-TU local declarations — do NOT add these to include/*.h.
//
// Allocates the global ePhysics singleton out of the game's physics memory
// pool and constructs it in place, then hands the pool to the collision
// constraint subsystem.
//
//   pool = *(cMemPool**)0x41050              (the physics cMemPool, here &g_pool)
//   alloc = pool->m_pAllocator               (cMemPool + 0x24)
//   size  = eConfigBase::s_pInstance->GetPhysicsMemPoolSize()
//   alloc->Allocate(size, 0)                 (first pool reserve, result unused)
//   buf  = alloc->vtbl[5](0xC50, 0x10, 0, 0) (virtual allocate of the ePhysics
//                                             block; SNC virtual dispatch through
//                                             vtable ptr at alloc+0x1C)
//   if (buf) new(buf) ePhysics;
//   ePhysics::s_pPhysics = buf;
//   eCollisionConstraint::Initialize(pool);
//   return true;

class eConfigBase {
public:
    int GetPhysicsMemPoolSize(void) const;
    static eConfigBase *s_pInstance;
};

// The pool's allocator: polymorphic. vtable pointer sits at offset 0x1C (after
// the leading data members), matching SNC's end-of-data vtable layout. The
// in-place allocate used for the singleton is virtual entry index 5
// (byte offset 0x28 in the vtable).
class iMemAllocator {
public:
    char _pad00[0x1C];
    // vtable ptr lives here at 0x1C
    void *AllocPoolMemory(unsigned int size, void *hint);
    virtual void *v0();
    virtual void *v1();
    virtual void *v2();
    virtual void *v3();
    virtual void *Allocate(unsigned int size, unsigned int align, int a, int b); // vtable slot 4 → byte offset 0x28
};

class cMemPool {
public:
    char _pad00[0x24];
    iMemAllocator *m_pAllocator; // 0x24
};

class ePhysics {
public:
    ePhysics(void);
    static int Initialize(void);
    static ePhysics *s_pPhysics;
};

class eCollisionConstraint {
public:
    static void Initialize(cMemPool *pool);
};

inline void *operator new(unsigned int, void *p) { return p; }

// Globals as raw addresses (compare_func masks the relocations).
#define g_pPool        (*(cMemPool **)0x41050)
#define g_pConfig      (*(eConfigBase **)0x37D0C8)
#define g_pPhysics     (*(ePhysics **)0x37D158)

int ePhysics::Initialize(void)
{
    cMemPool *pool = (cMemPool *)0x41050;
    iMemAllocator *alloc = pool->m_pAllocator;

    int size = g_pConfig->GetPhysicsMemPoolSize();
    alloc->AllocPoolMemory((unsigned int)size, 0);

    ePhysics *result = 0;
    iMemAllocator *alloc2 = pool->m_pAllocator;
    ePhysics *buf = (ePhysics *)alloc2->Allocate(0xC50, 0x10, 0, 0);
    if (buf != 0) {
        new (buf) ePhysics();
        result = buf;
    }
    g_pPhysics = result;

    eCollisionConstraint::Initialize(pool);
    return 1;
}
