// ODR-WARNING: this TU intentionally redeclares eShape with only the
// members needed for Read(cFile &, cMemPool *). Including eShape.h would
// require adding a shared declaration and can perturb matched sibling TUs.

class cFile;
class cMemPool;

class cReadBlock {
public:
    int _data[5];
    cReadBlock(cFile &, unsigned int, bool);
    ~cReadBlock(void);
};

class cName {
public:
    void Read(cReadBlock &);
};

class cFileSystem {
public:
    static void Read(void *, void *, unsigned int);
};

void cFile_SetCurrentPos(void *, unsigned int);

class eShape {
public:
    char _pad[0x80];

    int Read(cFile &, cMemPool *);
};

// eShape::Read(cFile &, cMemPool *) - 0x0002b820
int eShape::Read(cFile &file, cMemPool *) {
    int result;
    __asm__ volatile("ori %0, $0, 1" : "=r"(result));
    cReadBlock rb(file, 1, true);
    if ((unsigned int)rb._data[3] != 1) {
        cFile_SetCurrentPos(*(void **)&rb._data[0], rb._data[1]);
        return 0;
    }
    ((cName *)((char *)this + 0x54))->Read(rb);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x6C, 4);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x70, 4);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x40, 0xC);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x10, 0xC);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x20, 0xC);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x30, 0xC);
    return result;
}
