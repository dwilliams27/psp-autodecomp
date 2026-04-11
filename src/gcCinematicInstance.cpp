#include "gcCinematic.h"
#include "gcStreamedCinematic.h"
#include "gcUIDialog.h"

#define NULL 0

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

extern char *D_0037D7C8;
extern gcStreamedCinematic *D_0037D7D4[2];

// Function 1: 0x000ebdb4, 24 bytes
void gcCinematicInstance::Chain(cHandleT<gcCinematic> cinematic, cHandle handle, float time) {
    mChainCinematic = cinematic;
    mChainHandle = handle;
    mChainTime = time;
}

// Function 2: 0x000eaf28, 40 bytes
void gcCinematicInstance::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.End();
}

// Function 3: 0x000eb7dc, 92 bytes
void gcCinematicInstance::FreeDynamicInstances(void) {
    char *inst = D_0037D7C8;
    if (inst != NULL) {
        do {
            int *vt = (int *)(((char **)inst)[1] + 0x50);
            short thunk = *(short *)vt;
            ((void (*)(char *, int))vt[1])(inst + thunk, 3);
            inst = D_0037D7C8;
        } while (inst != NULL);
    }
}

// Function 4: 0x000ebee0, 104 bytes
void gcCinematicInstance::CloseDialogs(void) {
    gcDialogHolder *dh0 = mpDialogHolder0;
    if (dh0 != NULL) {
        gcUIDialog *dlg = dh0->mpDialog;
        if (dlg != NULL) {
            dlg->Close();
        }
        mpDialogHolder0 = NULL;
    }
    gcDialogHolder *dh1 = mpDialogHolder1;
    if (dh1 != NULL) {
        gcUIDialog *dlg = dh1->mpDialog;
        if (dlg != NULL) {
            dlg->Close();
        }
        mpDialogHolder1 = NULL;
    }
}

// Function 5: 0x000ec660, 108 bytes
void gcCinematicInstance::HandleStreamedCinematicDelete(void) {
    int i = 0;
    do {
        gcStreamedCinematic *sc = D_0037D7D4[i];
        if (sc != NULL) {
            if (sc->mpCinematicInstance == this) {
                sc->mpCinematicInstance = NULL;
                sc->NeedsDelete();
            }
        }
        i++;
    } while (i < 2);
}
