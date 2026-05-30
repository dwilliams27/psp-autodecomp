class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned char);
};

class eConvexHullShape {
public:
    struct ePolyFace {
        unsigned char a;
        unsigned char b;
        unsigned char c;
        unsigned char d;
        void Write(cWriteBlock &) const;
    };
};

#pragma control sched=1
// eConvexHullShape::ePolyFace::Write(cWriteBlock &) const — 0x00072950
void eConvexHullShape::ePolyFace::Write(cWriteBlock &wb) const {
    const ePolyFace *p = this;
    __asm__ volatile("" ::: "memory");
    wb.Write(p->a);
    wb.Write(p->b);
    wb.Write(p->c);
    wb.Write(p->d);
}
#pragma control sched=2
