// src/eSpriteFilter.cpp
// Functions:
//   0x0008bdcc  eSpriteFilter::Write(cFile &) const            eAll_psp.obj  (sched=1 zone)
//   0x0021cd0c  eSpriteFilter::AssignCopy(const cBase *)        eAll_psp.obj
//
// eSpriteFilter derives from eTextureFilter. Layout (relative to this):
//   +0x00 vtable / cBase header
//   +0x08 int (eTextureFilter base field)
//   +0x0C int (eTextureFilter base field)
//   +0x10 cHandle  (texture / image handle)
//   +0x14 bool
//   +0x18 unsigned int
//   +0x1C bool

class cBase;
class cFile;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(unsigned int);
    void End(void);
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

class eTextureFilter {
public:
    void Write(cFile &) const;
};

class eSpriteFilter : public eTextureFilter {
public:
    cBase   *mOwner;          // +0x00
    void    *mClassDesc;      // +0x04
    int      mField8;         // +0x08
    int      mFieldC;         // +0x0C
    cHandle  mField10;        // +0x10
    bool     mField14;        // +0x14
    unsigned int mField18;    // +0x18
    bool     mField1C;        // +0x1C

    void AssignCopy(const cBase *);
    void Write(cFile &) const;
};

template <class T> T *dcast(const cBase *);

#pragma control sched=1
// ── 0x0008bdcc — Write(cFile &) const, 124B ──
void eSpriteFilter::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    eTextureFilter::Write(file);
    mField10.Write(wb);
    wb.Write(mField14);
    wb.Write(mField1C);
    wb.Write(mField18);
    wb.End();
}

// ── 0x0021cd0c — AssignCopy(const cBase *), 120B ──
void eSpriteFilter::AssignCopy(const cBase *base) {
    eSpriteFilter *other = dcast<eSpriteFilter>(base);
    *(cHandle *)((char *)this + 8) = *(const cHandle *)((char *)other + 8);
    __asm__ volatile("" ::: "memory");
    *(cHandle *)((char *)this + 12) = *(const cHandle *)((char *)other + 12);
    __asm__ volatile("" ::: "memory");
    *(cHandle *)((char *)this + 16) = *(const cHandle *)((char *)other + 16);
    __asm__ volatile("" ::: "memory");
    *(unsigned char *)((char *)this + 20) = *(const unsigned char *)((char *)other + 20);
    *(cHandle *)((char *)this + 24) = *(const cHandle *)((char *)other + 24);
    __asm__ volatile("" ::: "memory");
    *(unsigned char *)((char *)this + 28) = *(const unsigned char *)((char *)other + 28);
}
#pragma control sched=2
