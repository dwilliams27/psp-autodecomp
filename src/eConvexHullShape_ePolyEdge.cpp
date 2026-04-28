// eConvexHullShape::ePolyEdge::Read(cReadBlock &) @ 0x00072a78
// In sched=1 zone within eAll_psp.obj unity build.

class cFile;

class cReadBlock {
public:
    int _data[5];
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class eConvexHullShape {
public:
    class ePolyEdge {
    public:
        unsigned char b0;
        unsigned char b1;
        unsigned char b2;
        void Read(cReadBlock &);
    };
};

#pragma control sched=1
void eConvexHullShape::ePolyEdge::Read(cReadBlock &rb) {
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0, 1);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 1, 1);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 2, 1);
}
#pragma control sched=2
