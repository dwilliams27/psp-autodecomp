// ODR-WARNING: split translation unit with local declarations for gcDoFunction::Read.
// Keep this file self-contained to avoid changing shared gcDoFunction headers and
// perturbing matched sibling methods.
class cBase;
class cFile;
class cFileHandle;
class cMemPool;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cReadBlock {
public:
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

class gcAction {
public:
    int Read(cFile &, cMemPool *);
};

class gcDoFunction : public gcAction {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" {
void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
void __0oKcReadBlockdtv(void *, int);
}

void cFile_SetCurrentPos(void *, unsigned int);

int gcDoFunction::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 2, true);
    if (rb[3] != 2 || ((gcAction *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    *(int *)((char *)this + 0x0C) = 0;
    char *handle = (char *)this + 0x0C;
    ((cHandle *)handle)->Read(*(cReadBlock *)rb, cMemPool::GetPoolFromPtr(handle));

    cBaseArray *children = (cBaseArray *)((char *)this + 0x10);
    children->Read(*(cReadBlock *)rb);

    int value = *(int *)((char *)this + 0x18);
    int tag = value & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    int outValue;
    if (flag == 0) goto not_tagged_out;
    outValue = 0;
    goto got_out;
not_tagged_out:
    outValue = value;
got_out:
    cBase *out = (cBase *)outValue;

    int tagged = 0;
    if (tag != 0) {
        tagged = 1;
    }

    cBase *base;
    if (tagged != 0) {
        base = (cBase *)(value & ~1);
    } else {
        base = *(cBase **)value;
    }

    ((cReadBlock *)rb)->ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x18), base, out);

    int newValue;
    if ((int)out == 0) {
        newValue = ((int)base) | 1;
    } else {
        newValue = (int)out;
    }
    *(int *)((char *)this + 0x18) = newValue;

    children->SetSize(4);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}
