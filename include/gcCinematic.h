#ifndef GCCINEMATIC_H
#define GCCINEMATIC_H

#include "gcReplicationVisitor.h"

class cFile;
class cMemPool;
class gcStreamedCinematic;
class gcUIDialog;

template <class T>
class cHandleT {
public:
    int mIndex;
};

class cHandle {
public:
    int mIndex;
};

class cBase;

class gcCinematic {
public:
    gcCinematic(cBase *);
    void Reset(cMemPool *, bool);
    void AssignCopy(const cBase *);
    void Write(cFile &) const;
    static cBase *New(cMemPool *, cBase *);
    static void FreeDynamicInstance(class gcCinematicInstance *);
};

struct gcDialogHolder {
    char pad[0x20];
    gcUIDialog *mpDialog;
};

class gcCinematicInstance {
public:
    char pad_00[0x30];
    gcDialogHolder *mpDialogHolder0;    // 0x30
    gcDialogHolder *mpDialogHolder1;    // 0x34
    char pad_38[0x04];
    cHandleT<gcCinematic> mChainCinematic; // 0x3C
    cHandle mChainHandle;               // 0x40
    float mChainTime;                   // 0x44

    void Reset(cMemPool *, bool);
    static void UpdateProfile(void);
    void Chain(cHandleT<gcCinematic>, cHandle, float);
    void Write(cFile &) const;
    static void FreeDynamicInstances(void);
    void CloseDialogs(void);
    void HandleStreamedCinematicDelete(void);
};

#endif
