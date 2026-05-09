// ODR-WARNING: split-TU method addition for gcValObjectCompare::Read.
// Keep this local redeclaration minimal so matched siblings in
// gcValObjectCompare.cpp do not drift from original TU allocator context.

class cBase;
class cFile;
class cMemPool;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cFile {
public:
    void SetCurrentPos(unsigned int);
};

class cReadBlock {
public:
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int,
                                             bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

class gcValue {
public:
    int Read(cFile &, cMemPool *);
};

class gcValObjectCompare : public gcValue {
public:
    int Read(cFile &, cMemPool *);
};

int gcValObjectCompare::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);
    if (rb[3] != 3 || gcValue::Read(file, pool) == 0) {
        ((cFile *)rb[0])->SetCurrentPos(rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    int sp14;
    int value = *(int *)((char *)this + 0x08);
    int tag = value & 1;
    int flag = 0;
    if (tag != 0) {
        flag = 1;
    }

    int outValue;
    if (flag != 0) {
        outValue = 0;
        goto out_done;
    }
    outValue = value;
out_done:
    sp14 = outValue;

    int flag2 = 0;
    if (tag != 0) {
        flag2 = 1;
    }

    int base;
    if (flag2 != 0) {
        base = value & ~1;
    } else {
        base = *(int *)value;
    }

    cMemPool *childPool = cMemPool::GetPoolFromPtr((char *)this + 0x08);
    ((cReadBlock *)rb)->ReadBase(childPool, (cBase *)base, *(cBase **)&sp14);

    int value2 = *(int *)((char *)this + 0x0C);
    int tag2 = value2 & 1;
    int newValue;
    if (sp14 == 0) {
        newValue = base | 1;
    } else {
        newValue = sp14;
    }
    *(int *)((char *)this + 0x08) = newValue;

    int sp18;
    int flag3 = 0;
    if (tag2 != 0) {
        flag3 = 1;
    }

    int outValue2;
    if (flag3 != 0) {
        outValue2 = 0;
        goto second_out_done;
    }
    outValue2 = value2;
second_out_done:
    sp18 = outValue2;

    int flag4 = 0;
    if (tag2 != 0) {
        flag4 = 1;
    }

    int base2;
    if (flag4 != 0) {
        base2 = value2 & ~1;
    } else {
        base2 = *(int *)value2;
    }

    cMemPool *childPool2 = cMemPool::GetPoolFromPtr((char *)this + 0x0C);
    ((cReadBlock *)rb)->ReadBase(childPool2, (cBase *)base2,
                                 *(cBase **)&sp18);

    int newValue2;
    if (sp18 == 0) {
        newValue2 = base2 | 1;
    } else {
        newValue2 = sp18;
    }
    *(int *)((char *)this + 0x0C) = newValue2;

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
