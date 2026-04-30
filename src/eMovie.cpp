#pragma control sched=1

#include "eMovie.h"
#include "eNavMesh.h"
#include "eDynamicFluid.h"
#include "eUser.h"

class cBase;
class cFile;
class cFilePlatform {
public:
    ~cFilePlatform(void);
};

extern eMovie *D_0037D2E0;
extern int D_0037D2F0;
extern char D_0037D2E4[];
extern char D_0037D2E8[];
extern double D_0036CD30;
extern int D_0036C7FC;
extern char eBipedControllerTemplateclassdesc[];

extern "C" {
    void eMoviePlatform___ct_eMoviePlatform_void(void *self);
    void eMoviePlatform__Initialize_voidstatic(void);
    void eMoviePlatform__Close_bool(void *self, bool force);
    void eMoviePlatform__Update_cTimeValueptr(void *self, cTimeValue *tv);
    unsigned long long sceKernelGetSystemTimeWide(void);
    int sceKernelSysClock2USecWide(unsigned long long clk, int *sec, int *usec);
    void free(void *);
}

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class ePhysicsControllerTemplate {
public:
    ePhysicsControllerTemplate(cBase *);
};

class ePhysicsController {
public:
    void Write(cFile &) const;
};

class eBipedControllerTemplate : public ePhysicsControllerTemplate {
public:
    eBipedControllerTemplate(cBase *);
};

class eBipedController : public ePhysicsController {
public:
    void Write(cFile &) const;
};

struct eMovieDeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

inline void eMovie::operator delete(void *p) {
    cMemPool *pool = cMemPool::GetPoolFromPtr(p);
    if (pool != 0) {
        void *block = *(void **)((char *)pool + 0x24);
        __asm__ volatile("" ::: "memory");
        eMovieDeleteRecord *rec =
            (eMovieDeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
        rec->fn((char *)block + rec->offset, p);
    } else {
        free(p);
    }
}

eMovie::eMovie(void) {
    __asm__ volatile("" ::: "memory");
    ((int *)this)[0] = 0;
    ((int *)this)[1] = 0;
    eMoviePlatform___ct_eMoviePlatform_void((char *)this + 8);
    D_0037D2E0 = this;
}

void eMovie::Initialize(void) {
    eMoviePlatform__Initialize_voidstatic();
}

void eMovie::Close(void) {
    __asm__ volatile("" ::: "memory");
    eMoviePlatform__Close_bool((char *)this + 8, true);
}

void eMovie::Update(cTimeValue *tv) {
    eMoviePlatform__Update_cTimeValueptr((char *)this + 8, tv);
}

void eMovie::OnDraw(void) {
    void (*callback)(void *) = *(void (**)(void *))D_0037D2E4;
    if (callback != 0) {
        callback(*(void **)D_0037D2E8);
    }
}

void eMovie::Pause(void) {
    eMovie *inst = D_0037D2E0;
    if (inst != 0) {
        int c = D_0037D2F0 + 1;
        D_0037D2F0 = c;
        __asm__ volatile("" ::: "memory");
        if (c == 1) {
            inst->PlatformPause();
        }
    }
}

void eMovie::Resume(void) {
    if (D_0037D2E0 != 0) {
        int c = D_0037D2F0 - 1;
        D_0037D2F0 = c;
        if (c == 0) {
            D_0037D2E0->ResetTime();
            D_0037D2E0->PlatformResume();
        }
    }
}

void eMovie::PlatformResume(void) {
}

void eMovie::ResetTime(void) {
    unsigned int now = (unsigned int)sceKernelGetSystemTimeWide();
    __asm__ volatile("" ::: "memory");
    if (*(int *)this == 0) {
        *(int *)this = now;
    }
    int sec, usec;
    __asm__ volatile("" ::: "memory");
    sceKernelSysClock2USecWide((unsigned long long)(now - *(unsigned int *)this), &sec, &usec);
    int frame = (int)((float)((double)sec + (double)usec * D_0036CD30) * (float)D_0036C7FC);
    int pad_local;
    int *pPad = &pad_local;
    int *pTrap = &frame;
    __asm__ volatile("" ::: "memory");
    int *pField = (int *)((char *)this + 4);
    *pField = frame;
    (void)pTrap;
    (void)pPad;
}

void eNavMesh::Reset(cMemPool *, bool) {
}

void eDynamicFluid::Cull(unsigned int, const eCamera &, const mFrustum &, eCameraBins *, unsigned int, int, const cHandleT<eMaterial> *, float) const {
}

void eDynamicFluid::Draw(const eDrawInfo &) const {
}

bool eUser::PlatformSignIn(void) {
    return true;
}

// --- eMovie::~eMovie(void) @ 0x00056d78 ---
eMovie::~eMovie() {
    Close();
    D_0037D2E0 = 0;
    D_0037D2F0 = 0;
    void *platformBase = (char *)this + 8;
    if (platformBase != 0) {
        ((cFilePlatform *)((char *)this + 0x98))->~cFilePlatform();
    }
}

// --- eBipedControllerTemplate::eBipedControllerTemplate(cBase *) @ 0x000629a8 ---
eBipedControllerTemplate::eBipedControllerTemplate(cBase *b) : ePhysicsControllerTemplate(b) {
    *(void **)((char *)this + 4) = eBipedControllerTemplateclassdesc;
    *(float *)((char *)this + 0x2C) = 50.0f;
    __asm__ volatile("" ::: "memory");
    *(float *)((char *)this + 0x30) = 1.0f;
}

// --- eBipedController::Write(cFile &) const @ 0x0006487c ---
void eBipedController::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    ePhysicsController::Write(file);
    wb.End();
}
