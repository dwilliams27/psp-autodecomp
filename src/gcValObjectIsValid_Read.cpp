// ODR-WARNING: split-TU local redeclaration for adding Read without
// changing the matched gcValObjectIsValid translation unit.

class cBase;
class cFile;
class cMemPool;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

class cReadBlock {
public:
    void ReadBase(cMemPool *, cBase *, cBase *&);
};

class gcValue {
public:
    int Read(cFile &, cMemPool *);
};

class gcValObjectIsValid : public gcValue {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

// 0x00356848 - gcValObjectIsValid::Read(cFile &, cMemPool *)
int gcValObjectIsValid::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 1, true);

    if ((unsigned int)rb[3] != 1 || gcValue::Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    int val = *(int *)((char *)this + 8);
    int tag = val & 1;

    int isTagged = 0;
    if (tag != 0) {
        isTagged = 1;
    }

    cBase *out;
    if (isTagged != 0) {
        out = 0;
    } else {
        out = (cBase *)val;
    }
    cBase *outRef = out;

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

    ((cReadBlock *)rb)->ReadBase(cMemPool::GetPoolFromPtr((char *)this + 8),
                                 (cBase *)base, outRef);

    int newVal = (int)outRef;
    if (newVal == 0) goto make_new_value;
    goto store_value;

make_new_value:
    __asm__ volatile("" ::: "memory");
    newVal = base | 1;

store_value:
    __asm__ volatile("" ::: "memory");
    *(int *)((char *)this + 8) = newVal;
    __0oKcReadBlockdtv(rb, 2);
    return result;
}
