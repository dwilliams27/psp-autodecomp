// ODR-WARNING: this TU intentionally redeclares gcDoSetValue with a minimal
// member list because include/gcDoReturn.h does not yet declare Read. Keep this
// split from src/gcDoSetValue.cpp so existing matches in the canonical TU are
// not perturbed.

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

class gcAction {
public:
    int Read(cFile &, cMemPool *);
};

class gcDoSetValue {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);

// -- gcDoSetValue::Read(cFile &, cMemPool *) @ 0x0014f48c --
int gcDoSetValue::Read(cFile &file, cMemPool *pool) {
    register int result __asm__("$19");
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 2, true);

    if ((unsigned int)rb._data[3] != 2) goto fail;
    if (((gcAction *)this)->Read(file, pool) == 0) goto fail;

    cFileSystem::Read(*(cFileHandle **)rb._data[0], (char *)this + 0x0C, 4);

    int first = *(int *)((char *)this + 0x10);
    int firstTag = first & 1;
    int firstRef = 0;
    if (firstTag != 0) goto first_tagged;
    goto first_join;
fail:
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
first_tagged:
    firstRef = 1;
first_join:
    int firstRefValue;
    if (firstRef == 0) {
        firstRefValue = first;
    } else {
        firstRefValue = 0;
    }
    cBase *firstSlot = (cBase *)firstRefValue;

    int firstFlag = 0;
    if (firstTag != 0) {
        firstFlag = 1;
    }
    int firstBase;
    if (firstFlag != 0) {
        firstBase = first & ~1;
    } else {
        firstBase = *(int *)first;
    }

    rb.ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x10),
                (cBase *)firstBase, firstSlot);

    int second = *(int *)((char *)this + 0x14);
    int firstOut = (int)firstSlot;
    int secondTag = second & 1;
    if (firstOut == 0) {
        firstOut = firstBase | 1;
    }
    *(int *)((char *)this + 0x10) = firstOut;

    int secondRef = 0;
    if (secondTag != 0) {
        secondRef = 1;
    }
    int secondRefValue;
    if (secondRef == 0) {
        secondRefValue = second;
    } else {
        secondRefValue = 0;
    }
    cBase *secondSlot = (cBase *)secondRefValue;

    int secondFlag = 0;
    if (secondTag != 0) {
        secondFlag = 1;
    }
    int secondBase;
    if (secondFlag != 0) {
        secondBase = second & ~1;
    } else {
        secondBase = *(int *)second;
    }

    rb.ReadBase(cMemPool::GetPoolFromPtr((char *)this + 0x14),
                (cBase *)secondBase, secondSlot);

    int secondOut = (int)secondSlot;
    if (secondOut == 0) {
        secondOut = secondBase | 1;
    }
    *(int *)((char *)this + 0x14) = secondOut;
    return result;
}
