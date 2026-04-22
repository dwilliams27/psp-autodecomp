#include "gcPlayer.h"

void cStrCopy(char *, const char *);

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

extern char *gcViewport_s_viewports;
extern int gcPlayer_s_nDefaultController;
extern unsigned short gcPlayer_s_nLastBoundController;  // 0x37D2FA
extern char *gcPlayer_s_pPlayers;                        // 0x37D87C, stride 68
extern int gcPlayer_s_nLocalControllerId[8];             // 0x37D884
extern float gcPlayer_s_fLocalControllerStrength[8];     // 0x37D8A4
extern char gcPlayervirtualtable[];                      // vtable at 0x37E6A8

void *cMemPool_GetPoolFromPtr(void *);

struct gcPlayerDeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

// -----------------------------------------------------------------------------
// gcPlayer::GetCamera(void) const
// -----------------------------------------------------------------------------
void *gcPlayer::GetCamera(void) const {
    int idx = *(int *)((char *)this + 48);
    int result = 0;
    if (idx >= 0) {
        char *base = gcViewport_s_viewports;
        int entry = 0;
        if (base != 0) {
            entry = (int)base + idx * 5008;
        }
        result = entry + 0x10;
    }
    return (void *)result;
}

// -----------------------------------------------------------------------------
// gcPlayer::GetName(char *) const
// -----------------------------------------------------------------------------
void gcPlayer::GetName(char *dest) const {
    cStrCopy(dest, (const char *)this + 12);
}

// -----------------------------------------------------------------------------
// gcPlayer::Write(cFile &) const
// -----------------------------------------------------------------------------
void gcPlayer::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.End();
}

// -----------------------------------------------------------------------------
// gcPlayer::~gcPlayer(void)
// -----------------------------------------------------------------------------
extern "C" {
void gcPlayer___dtor_gcPlayer_void(gcPlayer *self, int flags) {
    if (self != 0) {
        ((void **)self)[1] = gcPlayervirtualtable;
        if (flags & 1) {
            void *pool = cMemPool_GetPoolFromPtr(self);
            void *block = *(void **)((char *)pool + 0x24);
            gcPlayerDeleteRecord *rec = (gcPlayerDeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, self);
        }
    }
}
}

// -----------------------------------------------------------------------------
// gcPlayer::GetPlayerForCamera(const gcCamera *) static
// -----------------------------------------------------------------------------
gcPlayer *gcPlayer::GetPlayerForCamera(const gcCamera *cam) {
    gcPlayer *p = (gcPlayer *)gcPlayer_s_pPlayers;
    char *viewports = gcViewport_s_viewports;
    int i = 0;
    do {
        int vpIdx = *(int *)((char *)p + 48);
        if (vpIdx >= 0) {
            char *vp = 0;
            if (viewports != 0) {
                vp = viewports + vpIdx * 5008;
            }
            if ((const gcCamera *)(vp + 16) == cam) {
                return p;
            }
        }
        i++;
        p = (gcPlayer *)((char *)p + 68);
    } while (i < 8);
    return 0;
}

// -----------------------------------------------------------------------------
// gcPlayer::BindDefaultController(void) static
// -----------------------------------------------------------------------------
int gcPlayer::BindDefaultController(void) {
    int id = GetStartingController();
    if (id < 0) return -1;
    int s = id & 0xffff;
    gcPlayer_s_nDefaultController = id;
    s = s & 0xffff;
    gcPlayer_s_nLastBoundController = (unsigned short)s;
    return id;
}

// -----------------------------------------------------------------------------
// gcPlayer::AssignNew(void) static
// -----------------------------------------------------------------------------
int gcPlayer::AssignNew(void) {
    int id = BindLocalController();
    if (id < 0) return -1;
    AssignLocalController(id, true);
    return id;
}

// -----------------------------------------------------------------------------
// gcPlayer::ResetController(int) static
// -----------------------------------------------------------------------------
void gcPlayer::ResetController(int id) {
    int *p = (int *)(gcPlayer_s_pPlayers + id * 68);
    p[8] = -1;
    gcPlayer_s_nLocalControllerId[id] = -1;
    gcPlayer_s_fLocalControllerStrength[id] = 0.0f;
}

// -----------------------------------------------------------------------------
// gcPlayer::ResetOtherControllers(void) static
// -----------------------------------------------------------------------------
void gcPlayer::ResetOtherControllers(void) {
    for (int i = 1; i < 8; i++) {
        ResetController(i);
    }
}

// -----------------------------------------------------------------------------
// gcPlayer::ResetControllers(void) static
// -----------------------------------------------------------------------------
void gcPlayer::ResetControllers(void) {
    for (int i = 0; i < 8; i++) {
        ResetController(i);
    }
    gcPlayer_s_nDefaultController = -1;
}
