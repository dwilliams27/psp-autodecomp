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

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct AllocEntry {
    short offset;
    short pad;
    int (*fn)(void *, int, int, int, int);
};

extern "C" void cFile_SetCurrentPos(void *file, unsigned int pos);

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

// Function 6: 0x000eaf50, 120 bytes — Read
int gcCinematicInstance::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    __asm__ volatile("" ::: "memory");
    cReadBlock rb(file, 1, true);
    if (rb._data[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    return result;
}

// Function 7: 0x000ebdcc, 124 bytes — UpdateDialogs
void gcCinematicInstance::UpdateDialogs(cTimeValue tv) {
    gcUIDialog *dlg0 = NULL;
    gcDialogHolder *dh0 = mpDialogHolder0;
    if (dh0 != NULL) {
        dlg0 = dh0->mpDialog;
        if (dlg0 != NULL) {
            dlg0->Update(tv);
        }
    }
    gcDialogHolder *dh1 = mpDialogHolder1;
    if (dh1 != NULL) {
        gcUIDialog *dlg1 = dh1->mpDialog;
        if (dlg1 != NULL && dlg1 != dlg0) {
            dlg1->Update(tv);
        }
    }
}

// Function 8: 0x00243df8, 124 bytes — New (static)
void gcCinematicInstance_gcCinematicInstance(gcCinematicInstance *, cBase *);

gcCinematicInstance *gcCinematicInstance::New(cMemPool *pool, cBase *parent) {
    gcCinematicInstance *result = 0;
    void *block = ((void **)pool)[9];
    char *allocTable = ((PoolBlock *)block)->allocTable;
    AllocEntry *entry = (AllocEntry *)(allocTable + 0x28);
    short off = entry->offset;
    int (*fn)(void *, int, int, int, int) = entry->fn;
    void *base = (char *)block + off;
    gcCinematicInstance *obj = (gcCinematicInstance *)fn(base, 0x58, 4, 0, 0);
    if (obj != 0) {
        gcCinematicInstance_gcCinematicInstance(obj, parent);
        result = obj;
    }
    return result;
}
