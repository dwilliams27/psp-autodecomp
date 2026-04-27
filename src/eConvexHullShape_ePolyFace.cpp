// eConvexHullShape::ePolyFace::Write(cWriteBlock &) const @ 0x00072950
// eConvexHullShape::ePolyFace::Read(cReadBlock &)         @ 0x000729ac
// In sched=1 zone within eAll_psp.obj unity build.

class cFile;

class cReadBlock {
public:
    int _data[5];
};

class cWriteBlock {
public:
    int _data[2];
    void Write(unsigned char);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class eConvexHullShape {
public:
    class ePolyFace {
    public:
        unsigned char b0;
        unsigned char b1;
        unsigned char b2;
        unsigned char b3;
        void Write(cWriteBlock &) const;
        void Read(cReadBlock &);
    };
};

#pragma control sched=1
void eConvexHullShape::ePolyFace::Write(cWriteBlock &wb) const {
    wb.Write(b0);
    wb.Write(b1);
    wb.Write(b2);
    wb.Write(b3);
}

void eConvexHullShape::ePolyFace::Read(cReadBlock &rb) {
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0, 1);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 1, 1);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 2, 1);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 3, 1);
}
#pragma control sched=2
