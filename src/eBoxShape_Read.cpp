// ODR-WARNING: this TU intentionally redeclares eBoxShape with only the
// members needed for Read(cFile &, cMemPool *). Including eBoxShape.h would
// require adding a shared declaration and can perturb matched sibling TUs.

class cFile;
class cMemPool;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cFileSystem {
public:
    static void Read(void *, void *, unsigned int);
};

void cFile_SetCurrentPos(void *, unsigned int);
int eShape_Read(void *, cFile &, cMemPool *);
void eShape_ComputeMinAxis(void *);

class eBoxShape {
public:
    char _pad[0x80];
    float mHalfExtents[4];

    int Read(cFile &, cMemPool *);
};

#pragma control sched=1

// eBoxShape::Read(cFile &, cMemPool *) - 0x0006d3c4
int eBoxShape::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && eShape_Read(this, file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    void *h = *(void **)rb._data[0];
    __asm__ volatile("" : "+r"(h));
    cFileSystem::Read(h, mHalfExtents, 12);
    __asm__ volatile(
        "lv.q C120, 0x80(%0)\n"
        "vdot.t S100, C120, C120\n"
        "vsqrt.s S100, S100\n"
        "mfv $a0, S100\n"
        "mtc1 $a0, $f12\n"
        "swc1 $f12, 0x74(%0)\n"
        :: "r"(this) : "$a0", "$f12", "memory"
    );
    eShape_ComputeMinAxis(this);
    return result;
}

#pragma control sched=2
