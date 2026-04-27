#include "cBase.h"

class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

struct DeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

struct AllocRecord {
    short offset;
    short pad;
    void *(*fn)(void *, int, int, int, int);
};

class cMemPool_shim {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

// Inheritance chain (single-inheritance ABI: derived-this == base-this).
class ePhysicsControllerTemplate {
public:
    ePhysicsControllerTemplate(cBase *);
    ~ePhysicsControllerTemplate();
};

class eSimulatedControllerTemplate : public ePhysicsControllerTemplate {
public:
    char _pad[0x8];
    eSimulatedControllerTemplate(cBase *);
    ~eSimulatedControllerTemplate();
    void Write(cFile &) const;
};

extern char eRigidBodyControllerTemplateclassdesc[];

class eRigidBodyControllerTemplate : public eSimulatedControllerTemplate {
public:
    char _data[0x10];

    eRigidBodyControllerTemplate(cBase *);
    ~eRigidBodyControllerTemplate();
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

// gc* sibling classes (already-matched ctors)
extern char gcPCBipedControllerTemplateclassdesc[];
extern char gcGeomTrailControllerclassdesc[];
extern char gcShadowControllerclassdesc[];

extern "C" void gcBipedControllerTemplate_gcBipedControllerTemplate(void *, cBase *);
extern "C" void gcSubGeomController_gcSubGeomController(void *, cBase *);

class gcPCBipedControllerTemplate {
public:
    gcPCBipedControllerTemplate(cBase *);
};

class gcGeomTrailController {
public:
    gcGeomTrailController(cBase *);
};

class gcShadowController {
public:
    gcShadowController(cBase *);
};

// ── eRigidBodyControllerTemplate::eRigidBodyControllerTemplate(cBase *) @ 0x00077288 ──
eRigidBodyControllerTemplate::eRigidBodyControllerTemplate(cBase *b)
    : eSimulatedControllerTemplate(b)
{
    ((void **)this)[1] = eRigidBodyControllerTemplateclassdesc;
}

// ── eRigidBodyControllerTemplate::Write(cFile &) const @ 0x000770f8 ──
#pragma control sched=1
void eRigidBodyControllerTemplate::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const eSimulatedControllerTemplate *)this)->Write(file);
    wb.End();
}
#pragma control sched=2

// ── eRigidBodyControllerTemplate::~eRigidBodyControllerTemplate(void) @ 0x000772bc ──
//
// Canonical C++ destructor. SNC's ABI auto-generates the (this != 0) guard,
// the chain to ~eSimulatedControllerTemplate, and the deleting-tail dispatch
// through `operator delete` (defined in-class above).
#pragma control sched=1
eRigidBodyControllerTemplate::~eRigidBodyControllerTemplate() {
    ((void **)this)[1] = eRigidBodyControllerTemplateclassdesc;
}
#pragma control sched=2

// ── eRigidBodyControllerTemplate::New(cMemPool *, cBase *) static @ 0x0020fca8 ──
extern "C" void eRigidBodyControllerTemplate__eRigidBodyControllerTemplate_cBaseptr(void *self, cBase *parent);

#pragma control sched=1
cBase *eRigidBodyControllerTemplate::New(cMemPool *pool, cBase *parent) {
    eRigidBodyControllerTemplate *result = 0;
    __asm__ volatile("" ::: "memory");
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRecord *rec = (AllocRecord *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    __asm__ volatile("" ::: "memory");
    eRigidBodyControllerTemplate *obj = (eRigidBodyControllerTemplate *)rec->fn(base, 0x2C, 4, 0, 0);
    if (obj != 0) {
        eRigidBodyControllerTemplate__eRigidBodyControllerTemplate_cBaseptr(obj, parent);
        result = obj;
    }
    return (cBase *)result;
}
#pragma control sched=2

// ── gc* sibling ctors (already matched) ──
gcPCBipedControllerTemplate::gcPCBipedControllerTemplate(cBase *b) {
    gcBipedControllerTemplate_gcBipedControllerTemplate(this, b);
    ((void **)this)[1] = gcPCBipedControllerTemplateclassdesc;
}

gcGeomTrailController::gcGeomTrailController(cBase *b) {
    gcSubGeomController_gcSubGeomController(this, b);
    ((void **)this)[1] = gcGeomTrailControllerclassdesc;
}

gcShadowController::gcShadowController(cBase *b) {
    gcSubGeomController_gcSubGeomController(this, b);
    ((void **)this)[1] = gcShadowControllerclassdesc;
}

// ── __malloc_unlock @ 0x001b6ab4 ──
extern "C" int sceKernelCpuResumeIntr(int);

extern int g_malloc_lock_count;   // at 0x35238
extern int g_malloc_intr_state;   // at 0x3523C

extern "C" void __malloc_unlock(void) {
    int n = g_malloc_lock_count - 1;
    g_malloc_lock_count = n;
    if (n == 0) {
        sceKernelCpuResumeIntr(g_malloc_intr_state);
    }
}
