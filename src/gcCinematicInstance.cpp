#include "gcCinematic.h"
#include "gcStreamedCinematic.h"
#include "gcUIDialog.h"

#define NULL 0

class cInStream {
public:
    void Read(int &, int, bool);
    void Read(float &, bool);
};

class cOutStream {
public:
    void Write(int, int, bool);
    void Write(float, bool);
};

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

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
extern "C" void __0fKcTimeValueEReadR6JcInStream(void *, cInStream &);
extern "C" void __0fKcTimeValueFWriteR6KcOutStreamK(const void *, cOutStream &);

template <class T> T *dcast(const cBase *);

extern char *D_0037D7C8;
extern gcStreamedCinematic *D_0037D7D4[2];
extern cType *D_000385DC;
extern cType *D_00099AD0;

// Function 1: 0x000ebdb4, 24 bytes
void gcCinematicInstance::Chain(cHandleT<gcCinematic> cinematic, cHandle handle, float time) {
    mChainCinematic = cinematic;
    mChainHandle = handle;
    mChainTime = time;
}

// Function 1a: 0x00243cf0, 264 bytes
void gcCinematicInstance::AssignCopy(const cBase *base) {
    gcCinematicInstance *other = dcast<gcCinematicInstance>(base);
    int *src8 = (int *)((char *)other + 8);
    int value8 = *src8;
    int *dst8 = (int *)((char *)this + 8);
    *dst8 = value8;
    *(float *)((char *)this + 0xC) = *(float *)((char *)other + 0xC);
    int *src10 = (int *)((char *)other + 0x10);
    int value10 = *src10;
    int *dst10 = (int *)((char *)this + 0x10);
    int *src14 = (int *)((char *)other + 0x14);
    int *dst14 = (int *)((char *)this + 0x14);
    *dst10 = value10;
    int value14 = *src14;
    *dst14 = value14;
    *(int *)((char *)this + 0x18) = *(int *)((char *)other + 0x18);
    *(int *)((char *)this + 0x1C) = *(int *)((char *)other + 0x1C);
    int *src20 = (int *)((char *)other + 0x20);
    int value20 = *src20;
    int *dst20 = (int *)((char *)this + 0x20);
    *dst20 = value20;
    *(unsigned char *)((char *)this + 0x24) =
        *(unsigned char *)((char *)other + 0x24);
    *(int *)((char *)this + 0x28) = *(int *)((char *)other + 0x28);
    *(int *)((char *)this + 0x2C) = *(int *)((char *)other + 0x2C);
    *(int *)((char *)this + 0x30) = *(int *)((char *)other + 0x30);
    *(int *)((char *)this + 0x34) = *(int *)((char *)other + 0x34);
    *(unsigned char *)((char *)this + 0x38) =
        *(unsigned char *)((char *)other + 0x38);
    *(unsigned char *)((char *)this + 0x39) =
        *(unsigned char *)((char *)other + 0x39);
    *(unsigned char *)((char *)this + 0x3A) =
        *(unsigned char *)((char *)other + 0x3A);
    int *src3C = (int *)((char *)other + 0x3C);
    int value3C = *src3C;
    int *dst3C = (int *)((char *)this + 0x3C);
    int *src40 = (int *)((char *)other + 0x40);
    int *dst40 = (int *)((char *)this + 0x40);
    *dst3C = value3C;
    int value40 = *src40;
    *dst40 = value40;
    *(float *)((char *)this + 0x44) = *(float *)((char *)other + 0x44);
    *(int *)((char *)this + 0x48) = *(int *)((char *)other + 0x48);
    *(int *)((char *)this + 0x4C) = *(int *)((char *)other + 0x4C);
    *(int *)((char *)this + 0x50) = *(int *)((char *)other + 0x50);
    *(int *)((char *)this + 0x54) = *(int *)((char *)other + 0x54);
}

// Function 2: 0x000eaf28, 40 bytes
void gcCinematicInstance::Write(cFile &file) const {
    cWriteBlock wb(file, 1);
    wb.End();
}

// Function 2a: 0x000ec27c, 152 bytes
void gcCinematicInstance::Write(cOutStream &stream) const {
    ((const cHandle *)((const char *)this + 8))->Write(stream);
    stream.Write(*(const float *)((const char *)this + 0xC), true);
    __0fKcTimeValueFWriteR6KcOutStreamK((const char *)this + 0x10, stream);
    __0fKcTimeValueFWriteR6KcOutStreamK((const char *)this + 0x14, stream);
    stream.Write(*(const int *)((const char *)this + 0x18), 0x20, true);
    ((const cHandle *)((const char *)this + 0x3C))->Write(stream);
    ((const cHandle *)((const char *)this + 0x40))->Write(stream);
    stream.Write(*(const float *)((const char *)this + 0x44), true);
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

// Function 6a: 0x000ec314, 180 bytes
void gcCinematicInstance::Read(cInStream &stream) {
    ((cHandle *)((char *)this + 8))->Read(stream);
    stream.Read(*(float *)((char *)this + 0xC), true);
    __0fKcTimeValueEReadR6JcInStream((char *)this + 0x10, stream);
    __0fKcTimeValueEReadR6JcInStream((char *)this + 0x14, stream);
    stream.Read(*(int *)((char *)this + 0x18), 0x20, true);
    ((cHandle *)((char *)this + 0x3C))->Read(stream);
    ((cHandle *)((char *)this + 0x40))->Read(stream);
    stream.Read(*(float *)((char *)this + 0x44), true);

    int index = *(int *)((char *)this + 0x18) - 1;
    if (index < 0) {
        index = 0;
    }
    PlayVoiceOver(index);
}

// Function 7: 0x000ebdcc, 124 bytes — UpdateDialogs
void gcCinematicInstance::UpdateDialogs(cTimeValue tv) {
    gcUIDialog *dlg0 = NULL;
    if (mpDialogHolder0 != NULL) {
        dlg0 = mpDialogHolder0->mpDialog;
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

// Function 9: 0x00243e74, 160 bytes — GetType
const cType *gcCinematicInstance::GetType(void) const {
    if (D_00099AD0 == 0) {
        if (D_000385DC == 0) {
            D_000385DC = cType::InitializeType((const char *)0x36D894,
                                               (const char *)0x36D89C,
                                               1, 0, 0, 0, 0, 0);
        }
        D_00099AD0 = cType::InitializeType(
            0, 0, 0x169, D_000385DC,
            (cBase *(*)(cMemPool *, cBase *))&gcCinematicInstance::New,
            0, 0, 0);
    }
    return D_00099AD0;
}
