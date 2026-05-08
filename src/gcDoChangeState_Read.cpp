// ODR-WARNING: this TU intentionally redeclares gcDoChangeState with a
// minimal member list because include/gcDoChangeState.h does not yet declare
// Read. Keep this split from src/gcDoChangeState.cpp so existing matches in
// the canonical TU are not perturbed.

class cBase;
class cFile;
class cFileHandle;
class cMemPool;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class cHandle {
public:
    void Read(cReadBlock &, cMemPool *);
};

class cBaseArray {
public:
    void Read(cReadBlock &);
    void SetSize(int);
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
};

class gcAction {
public:
    int Read(cFile &, cMemPool *);
};

class gcDoChangeState {
public:
    int Read(cFile &, cMemPool *);
};

struct HandleEntry {
    char pad[0x30];
    int handle;
};

extern HandleEntry *D_00048890[];
extern "C" void cFile_SetCurrentPos(void *, unsigned int);

int gcDoChangeState::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 4, true);

    if ((unsigned int)rb._data[3] != 4) goto fail;
    if (((gcAction *)this)->Read(file, pool) == 0) goto fail;

    cHandle *handle = (cHandle *)((char *)this + 0x0C);
    *(int *)((char *)this + 0x0C) = 0;
    handle->Read(rb, cMemPool::GetPoolFromPtr(handle));
    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x10, 4);

    int value = *(int *)((char *)this + 0x0C);
    register HandleEntry *entry __asm__("$6");
    if (value == 0) {
        entry = 0;
    } else {
        HandleEntry *candidate = D_00048890[value & 0xFFFF];
        entry = 0;
        if (candidate != 0) {
            if (candidate->handle == value) {
                entry = candidate;
            }
        }
    }
    if (entry != 0) {
        register int never __asm__("$4");
        __asm__ volatile("andi %0, $0, 0xff" : "=r"(never));
        if (never != 0) {
            *(int *)((char *)this + 0x10) =
                (*(int *)((char *)this + 0x10) & 0x8000FFFF) | 0x10000;
        }
    }
    __asm__ volatile("" : "+r"(entry));
    ((gcDesiredValue *)((char *)this + 0x14))->Read(rb);
    ((cBaseArray *)((char *)this + 0x18))->Read(rb);

    int val = *(int *)((char *)this + 0x20);
    int tag = val & 1;
    int isTagged = 0;
    if (tag != 0) {
        isTagged = 1;
    }

    cBase *slot;
    if (isTagged != 0) {
        slot = 0;
    } else {
        slot = (cBase *)val;
    }
    cBase *out = slot;

    int isTaggedAgain = 0;
    if (tag != 0) {
        isTaggedAgain = 1;
    }

    int base;
    if (isTaggedAgain != 0) {
        base = val & ~1;
    } else {
        base = *(int *)val;
    }

    rb.ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x20), (cBase *)base,
                out);

    int newVal = (int)out;
    if (newVal != 0) goto have_new_value;
    goto make_new_value;

make_new_value:
    newVal = base | 1;

have_new_value:
    void *readFile = *(void **)rb._data[0];
    *(int *)((char *)this + 0x20) = newVal;

    char flag;
    cFileSystem::Read(*(cFileHandle **)readFile, &flag, 1);
    *(unsigned char *)((char *)this + 0x24) = flag != 0;

    ((cBaseArray *)((char *)this + 0x18))->SetSize(4);
    return result;

fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
}
