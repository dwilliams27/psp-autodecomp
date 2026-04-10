#include "eRenderTarget.h"
#include "eVideo.h"

extern char eIVideoRegisterInitvirtualtable[];
extern char eRenderTargetVideoRegisterInitvirtualtable[];
extern char eRenderTargetVideoRegisterInitName[];

class eIVideoRegisterInit;

class eRenderTargetVideoRegisterInit {
public:
    int pad0;
    int pad4;
    void *vtable;

    eRenderTargetVideoRegisterInit(void);
    void Initialize(void);
    void Uninitialize(void);
    const char *GetName(void);
    void Flip(void);
};

eRenderTargetVideoRegisterInit::eRenderTargetVideoRegisterInit(void) {
    asm volatile("");
    *(void **)((char *)this + 8) = eIVideoRegisterInitvirtualtable;
    eVideo::RegisterInit((eIVideoRegisterInit *)this);
    *(void **)((char *)this + 8) = eRenderTargetVideoRegisterInitvirtualtable;
}

void eRenderTargetVideoRegisterInit::Initialize(void) {
    eRenderTarget::Initialize();
}

void eRenderTargetVideoRegisterInit::Uninitialize(void) {
    eRenderTarget::Uninitialize();
}

const char *eRenderTargetVideoRegisterInit::GetName(void) {
    return eRenderTargetVideoRegisterInitName;
}

void eRenderTargetVideoRegisterInit::Flip(void) {
    eRenderTarget::Flip();
}
