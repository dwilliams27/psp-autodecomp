// ODR-WARNING: this TU intentionally redeclares eCapsuleShape with only the
// members needed for Read(cFile &, cMemPool *). Including eCapsuleShape.h
// would require adding a shared declaration and can perturb matched sibling TUs.

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

class eCapsuleShape {
public:
    char _pad[0x80];
    float radius;
    float halfHeight;

    int Read(cFile &, cMemPool *);
};

#pragma control sched=1

// eCapsuleShape::Read(cFile &, cMemPool *) - 0x00069fdc
int eCapsuleShape::Read(cFile &file, cMemPool *pool) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] == 1 && eShape_Read(this, file, pool)) goto success;
    cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
    return 0;
success:
    void *h = *(void **)rb._data[0];
    __asm__ volatile("" : "+r"(h));
    cFileSystem::Read(h, &radius, 4);
    h = *(void **)rb._data[0];
    __asm__ volatile("" : "+r"(h));
    cFileSystem::Read(h, &halfHeight, 4);
    *(float *)((char *)this + 0x74) = radius + halfHeight;
    eShape_ComputeMinAxis(this);
    return result;
}

#pragma control sched=2
