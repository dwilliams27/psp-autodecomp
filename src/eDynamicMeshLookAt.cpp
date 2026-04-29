// eDynamicMeshLookAt — name + one int + seven 3-float vectors.
//
// Functions matched here:
//   eDynamicMeshLookAt::Write(cWriteBlock &) const  @ 0x0004ce94  172B
//   eDynamicMeshLookAt::Read(cReadBlock &)          @ 0x0004cf40  208B

class cWriteBlock {
public:
    void Write(int);
    void Write(int, const float *);
};

class cReadBlock {
public:
    int _data[5];
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class cName {
public:
    char mName[0x14];
    short mField14;
    unsigned short mField16;

    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

class eDynamicMeshLookAt {
public:
    cName mName;          // 0x00
    int mField18;         // 0x18
    char _pad1C[0x4];     // 0x1C
    float mVec20[3];      // 0x20
    char _pad2C[0x4];     // 0x2C
    float mVec30[3];      // 0x30
    char _pad3C[0x4];     // 0x3C
    float mVec40[3];      // 0x40
    char _pad4C[0x4];     // 0x4C
    float mVec50[3];      // 0x50
    char _pad5C[0x4];     // 0x5C
    float mVec60[3];      // 0x60
    char _pad6C[0x4];     // 0x6C
    float mVec70[3];      // 0x70
    char _pad7C[0x4];     // 0x7C
    float mVec80[3];      // 0x80

    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

#pragma control sched=1

void eDynamicMeshLookAt::Write(cWriteBlock &wb) const {
    mName.Write(wb);
    wb.Write(mField18);
    wb.Write(3, mVec50);
    wb.Write(3, mVec20);
    wb.Write(3, mVec30);
    wb.Write(3, mVec40);
    wb.Write(3, mVec60);
    wb.Write(3, mVec70);
    wb.Write(3, mVec80);
}

void eDynamicMeshLookAt::Read(cReadBlock &rb) {
    mName.Read(rb);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], &mField18, 4);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], mVec50, 0xC);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], mVec20, 0xC);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], mVec30, 0xC);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], mVec40, 0xC);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], mVec60, 0xC);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], mVec70, 0xC);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], mVec80, 0xC);
}

#pragma control sched=2
