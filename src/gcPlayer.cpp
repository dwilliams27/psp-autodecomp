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
