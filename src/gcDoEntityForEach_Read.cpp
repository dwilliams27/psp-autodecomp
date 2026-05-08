// ODR-WARNING: split-TU local redeclaration for adding Read without
// changing the matched gcDoEntityForEach translation unit.

class cBase;
class cFile;
class cFileHandle;
class cMemPool;
class cReadBlock;
class gcEnumeration;
class gcEnumerationEntry;

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

class gcAction {
public:
    int Read(cFile &, cMemPool *);
};

class gcExpressionList {
public:
    void Read(cReadBlock &);
};

template <class T> class cSubHandleT;
template <class A, class B> class cHandlePairT;
template <class T> class cArray {
public:
    void Read(cReadBlock &);
};

struct ReadRec {
    short offset;
    short pad;
    void (*fn)(void *, cFile *, cMemPool *);
};

class gcDoEntityForEach {
public:
    int Read(cFile &, cMemPool *);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);

// 0x002b2cac - gcDoEntityForEach::Read(cFile &, cMemPool *)
int gcDoEntityForEach::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    int rb[5];
    __0oKcReadBlockctR6FcFileUib(rb, file, 4, true);

    if ((unsigned int)rb[3] != 4 ||
        ((gcAction *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&rb[0], rb[1]);
        __0oKcReadBlockdtv(rb, 2);
        return 0;
    }

    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x0C, 4);
    cFileSystem::Read(*(cFileHandle **)rb[0], (char *)this + 0x10, 4);

    char *desired = (char *)this + 0x14;
    char *type = *(char **)((char *)this + 0x18);
    ReadRec *rec = (ReadRec *)(type + 0x30);
    cFile *f = *(cFile **)&rb[0];
    rec->fn(desired + rec->offset, f, cMemPool::GetPoolFromPtr(desired));

    ((gcExpressionList *)((char *)this + 0x2C))->Read(*(cReadBlock *)rb);
    ((cArray<cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > > *)((char *)this + 0x34))
        ->Read(*(cReadBlock *)rb);
    __0oKcReadBlockdtv(rb, 2);
    return result;
}
