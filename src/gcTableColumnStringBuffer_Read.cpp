// ODR-WARNING: split-TU local redeclarations for adding Read methods without
// changing the matched gcTableColumnStringBuffer translation unit.

class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cReadBlock;

class cArrayBase_cArray_wchar_t {
public:
    wchar_t **mData;
    bool SetSize(int, const cArrayBase_cArray_wchar_t &);
};

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
};

class gcTableColumn {
public:
    int Read(cFile &, cMemPool *);
};

class gcAction {
public:
    int Read(cFile &, cMemPool *);
};

class cName {
public:
    void Read(cReadBlock &);
};

class gcDesiredValue {
public:
    void Read(cReadBlock &);
};

template <class T> class cArray {
public:
    void Read(cReadBlock &);
};

struct PoolBlock {
    char pad[0x1C];
    char *allocTable;
};

struct ReadRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

struct DeleteRec {
    short offset;
    short pad;
    void (*fn)(void *, void *, int, short);
};

class gcTableColumnStringBuffer {
public:
    int Read(cFile &, cMemPool *);
};

class gcDoEntityApplyRigidBodyImpulse {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

// 0x0012b324
int gcTableColumnStringBuffer::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    cArrayBase_cArray_wchar_t old;
    int count;

    __0oKcReadBlockctR6FcFileUib(rb, file, 3, true);
    if (rb[3] != 3 || ((gcTableColumn *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    __asm__ volatile("" ::: "memory");
    void *maxLen = (char *)this + 0x08;
    cFileSystem::Read(*(cFileHandle **)rb[0], maxLen, 4);
    void *countPtr = &count;
    cFileSystem::Read(*(cFileHandle **)rb[0], countPtr, 4);

    int size = count;
    __asm__ volatile("" : "+r"(size));
    ((volatile cArrayBase_cArray_wchar_t *)&old)->mData = 0;
    void *arrayBase = (char *)this + 0x0C;
    ((volatile cArrayBase_cArray_wchar_t *)&old)->mData = 0;
    ((cArrayBase_cArray_wchar_t *)arrayBase)->SetSize(size, old);

    wchar_t **data = old.mData;
    int oldCount = 0;
    if (data != 0) {
        oldCount = ((int *)data)[-1] & 0x3fffffff;
    }

    int index = 0;
    int inRange = index < oldCount;
    if (inRange != 0) {
        do {
            index++;
            inRange = index < oldCount;
        } while (inRange != 0);
    }

    if (data != 0) {
        data -= 1;
        if (data != 0) {
            cMemPool *oldPool = cMemPool::GetPoolFromPtr(data);
            char *block = ((char **)oldPool)[9];
            DeleteRec *rec = (DeleteRec *)(((PoolBlock *)block)->allocTable + 0x30);
            short off = rec->offset;
            void (*fn)(void *, void *, int, short) = rec->fn;
            fn(block + off, data, (int)fn, off);
            old.mData = 0;
        } else {
            old.mData = 0;
        }
    }

    cArray<wchar_t> *values = *(cArray<wchar_t> **)((char *)this + 0x0C);
    int total = count;
    int row = 0;
    if (row < total) {
        register int cursor __asm__("$18") = 0;
        __asm__ volatile("" : "+r"(cursor));
        cursor += (int)values;
        do {
            ((cArray<wchar_t> *)cursor)->Read(*(cReadBlock *)rb);
            row++;
            cursor += 4;
        } while (row < total);
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}

// 0x002a2140
int gcDoEntityApplyRigidBodyImpulse::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];

    __0oKcReadBlockctR6FcFileUib(rb, file, 5, true);
    if ((unsigned int)rb[3] >= 6 || (unsigned int)rb[3] < 4 ||
        ((gcAction *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x10, 4);
    ((cName *)((char *)this + 0x14))->Read(*(cReadBlock *)rb);

    char *typeInfo = *(char **)((char *)this + 0x30);
    void *base = (char *)this + 0x2C;
    ReadRec *rec = (ReadRec *)(typeInfo + 0x30);
    short off = rec->offset;
    cFile *f = *(cFile **)&rb[0];
    rec->fn((char *)base + off, f, cMemPool::GetPoolFromPtr(base));

    ((gcDesiredValue *)((char *)this + 0x58))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x5C))->Read(*(cReadBlock *)rb);
    ((gcDesiredValue *)((char *)this + 0x60))->Read(*(cReadBlock *)rb);
    if ((unsigned int)rb[3] >= 5) {
        ((gcDesiredValue *)((char *)this + 0x64))->Read(*(cReadBlock *)rb);
        ((gcDesiredValue *)((char *)this + 0x68))->Read(*(cReadBlock *)rb);
        ((gcDesiredValue *)((char *)this + 0x6C))->Read(*(cReadBlock *)rb);
        ((gcDesiredValue *)((char *)this + 0x70))->Read(*(cReadBlock *)rb);
        ((gcDesiredValue *)((char *)this + 0x74))->Read(*(cReadBlock *)rb);
        ((gcDesiredValue *)((char *)this + 0x78))->Read(*(cReadBlock *)rb);
    }

    __0oKcReadBlockdtv(rb, 2);
    return result;
}
