// ODR-WARNING: split TU for ePath::Read. Keep local declarations minimal so
// this Read match does not perturb the existing ePath.cpp translation unit.

class cBase;
class cFile;
class cFileHandle;
class cMemPool;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(cFileHandle *, void *, unsigned int);
};

class cObject {
public:
    int Read(cFile &, cMemPool *);
};

struct ePathPoint {
    char _pad[0x20];

    void Read(cReadBlock &);
};

template <class T>
class cArrayBase {
public:
    T *mData;
    bool SetSize(int, const cArrayBase<T> &);
};

extern "C" void cFile_SetCurrentPos(void *, unsigned int);
extern "C" void __0oKcReadBlockctR6FcFileUib(void *, cFile &, unsigned int, bool);
extern "C" void __0oKcReadBlockdtv(void *, int);
extern "C" ePathPoint *ePathPoint_ctor(ePathPoint *) asm("__0oKePathPointctv");
extern "C" bool ePathPointArray_SetSize(cArrayBase<ePathPoint> *, int,
                                         const ePathPoint *)
    asm("__0fKcArrayBase76KePathPoint_HSetSizeiRC9BA_b");

struct VReadSlot {
    short offset;
    short pad;
    void (*fn)(void *, cMemPool *, int);
};

struct ReadLocals {
    int rb[5];
    int pad[3];
    ePathPoint defaultPoint;
    int count;
};

class ePath : public cObject {
public:
    int Read(cFile &, cMemPool *);
};

#pragma control sched=1
int ePath::Read(cFile &file, cMemPool *pool) {
    int result = 1;
    ReadLocals locals;

    __0oKcReadBlockctR6FcFileUib(locals.rb, file, 1, true);
    if ((unsigned int)locals.rb[3] != 1 ||
        ((cObject *)this)->Read(file, pool) == 0) {
        cFile_SetCurrentPos(*(void **)&locals.rb[0], locals.rb[1]);
        __0oKcReadBlockdtv(locals.rb, 2);
        return 0;
    }

    {
        void *h = *(void **)&locals.rb[0];
        h = *(void **)h;
        __asm__ volatile("" : "+r"(h) :: "memory");
        cFileSystem::Read((cFileHandle *)h, &locals.count, 4);
    }
    ePathPointArray_SetSize((cArrayBase<ePathPoint> *)((char *)this + 0x44),
                            locals.count,
                            ePathPoint_ctor(&locals.defaultPoint));

    register int limit __asm__("$21") = locals.count;
    register int i __asm__("$20") = 0;
    register ePathPoint *points __asm__("$19") =
        *(ePathPoint **)((char *)this + 0x44);
    if (i < limit) goto read_points;
    goto done_points;

read_points:
    {
        register int offset __asm__("$4") = 0;
        __asm__ volatile("ori %0, $0, 0" : "=r"(offset));
        points = (ePathPoint *)((char *)points + offset);
        do {
            points->Read(*(cReadBlock *)locals.rb);
            i++;
            points = (ePathPoint *)((char *)points + 0x20);
        } while (i < limit);
    }

done_points:
    {
        void *h = *(void **)&locals.rb[0];
        h = *(void **)h;
        __asm__ volatile("" : "+r"(h) :: "memory");
        cFileSystem::Read((cFileHandle *)h, (char *)this + 0x48, 4);
    }

    char *vt = *(char **)((char *)this + 4);
    register VReadSlot *slot __asm__("$5") = (VReadSlot *)(vt + 0x38);
    register int target __asm__("$4") = slot->offset;
    target = (int)this + target;
    register void (*fn)(void *, cMemPool *, int) __asm__("$7") = slot->fn;
    fn((void *)target, pool, 0);

    __0oKcReadBlockdtv(locals.rb, 2);
    return result;
}
#pragma control sched=2
