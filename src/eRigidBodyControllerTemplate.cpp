#include "cBase.h"

// Parent constructors (extern "C" to bypass C++ name mangling complexity;
// the relocation is masked by compare_func.py).
extern "C" void eSimulatedControllerTemplate_eSimulatedControllerTemplate(void *, cBase *);
extern "C" void gcBipedControllerTemplate_gcBipedControllerTemplate(void *, cBase *);
extern "C" void gcSubGeomController_gcSubGeomController(void *, cBase *);

// classdesc symbols (relocations masked at compare time).
extern char eRigidBodyControllerTemplateclassdesc[];
extern char gcPCBipedControllerTemplateclassdesc[];
extern char gcGeomTrailControllerclassdesc[];
extern char gcShadowControllerclassdesc[];

class eRigidBodyControllerTemplate {
public:
    eRigidBodyControllerTemplate(cBase *);
};

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
eRigidBodyControllerTemplate::eRigidBodyControllerTemplate(cBase *b) {
    eSimulatedControllerTemplate_eSimulatedControllerTemplate(this, b);
    ((void **)this)[1] = eRigidBodyControllerTemplateclassdesc;
}

// ── gcPCBipedControllerTemplate::gcPCBipedControllerTemplate(cBase *) @ 0x00152588 ──
gcPCBipedControllerTemplate::gcPCBipedControllerTemplate(cBase *b) {
    gcBipedControllerTemplate_gcBipedControllerTemplate(this, b);
    ((void **)this)[1] = gcPCBipedControllerTemplateclassdesc;
}

// ── gcGeomTrailController::gcGeomTrailController(cBase *) @ 0x0015487c ──
gcGeomTrailController::gcGeomTrailController(cBase *b) {
    gcSubGeomController_gcSubGeomController(this, b);
    ((void **)this)[1] = gcGeomTrailControllerclassdesc;
}

// ── gcShadowController::gcShadowController(cBase *) @ 0x00157b10 ──
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
