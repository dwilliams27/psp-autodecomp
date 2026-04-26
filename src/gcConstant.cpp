// Decompiled functions:
//   0x00124f1c gcConstant::gcConstant(cBase *)
//   0x001304e0 gcVariable::gcVariable(cBase *)
//   0x00146e00 gcLookAtControllerTemplate::gcLookAtControllerTemplate(cBase *)
//   0x001a6604 nwConnection::SetError(nwConnection::nwConnectionError)
//   0x001b6a78 __malloc_lock

#include "cBase.h"

// Parent constructors. Symbols renamed via extern "C" because compare_func.py
// masks the call's relocation bytes — only the call shape needs to match.
extern "C" void cObject_cObject(void *self, cBase *parent);
extern "C" void gcPartialEntityControllerTemplate_gcPartialEntityControllerTemplate(
    void *self, cBase *parent);

// classdesc symbols (relocations masked at compare time).
extern char gcConstantclassdesc[];
extern char gcVariableclassdesc[];
extern char gcLookAtControllerTemplateclassdesc[];

class gcConstant {
public:
    gcConstant(cBase *);
};

class gcVariable {
public:
    gcVariable(cBase *);
};

class gcLookAtControllerTemplate {
public:
    gcLookAtControllerTemplate(cBase *);
};

// ── gcConstant::gcConstant(cBase *) @ 0x00124f1c ──
gcConstant::gcConstant(cBase *b) {
    cObject_cObject(this, b);
    ((void **)this)[1] = gcConstantclassdesc;
    *(float *)((char *)this + 68) = 0.0f;
}

// ── gcVariable::gcVariable(cBase *) @ 0x001304e0 ──
gcVariable::gcVariable(cBase *b) {
    cObject_cObject(this, b);
    ((void **)this)[1] = gcVariableclassdesc;
    *(float *)((char *)this + 68) = 0.0f;
}

// ── gcLookAtControllerTemplate::gcLookAtControllerTemplate(cBase *) @ 0x00146e00 ──
gcLookAtControllerTemplate::gcLookAtControllerTemplate(cBase *b) {
    gcPartialEntityControllerTemplate_gcPartialEntityControllerTemplate(this, b);
    ((void **)this)[1] = gcLookAtControllerTemplateclassdesc;
    *((signed char *)this + 20) = (signed char)-1;
}

// ── nwConnection::SetError @ 0x001a6604 ──
class nwConnection {
public:
    enum nwConnectionError {
        ERROR_NONE = 0
    };

    char _pad00[0x1C];
    int mField1C;       // 0x1C — error code
    int mField20;       // 0x20 — flag/state cleared after FlushAll
    void SetError(nwConnectionError);
    void FlushAll(void);
};

void nwConnection::SetError(nwConnectionError err) {
    if (mField1C == 0) {
        mField1C = (int)err;
        FlushAll();
        mField20 = 0;
    }
}

// ── __malloc_lock @ 0x001b6a78 ──
extern "C" int sceKernelCpuSuspendIntr(void);

extern int g_malloc_lock_count;   // at 0x35238
extern int g_malloc_intr_state;   // at 0x3523C

extern "C" void __malloc_lock(void) {
    int state = sceKernelCpuSuspendIntr();
    int n = g_malloc_lock_count;
    if (n == 0) {
        g_malloc_intr_state = state;
    }
    g_malloc_lock_count = n + 1;
}
