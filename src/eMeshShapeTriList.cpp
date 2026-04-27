class cFile;

class cReadBlock {
public:
    int _data[5];
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(unsigned char);
    void Write(unsigned short);
    void End(void);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

struct eMeshShapeTriList {
    unsigned char mFlags;       // 0
    unsigned char mPad;         // 1
    unsigned short mNumVerts;   // 2
    unsigned short mNumTris;    // 4
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

#pragma control sched=1
// eMeshShapeTriList::Write(cWriteBlock &) const — 0x0004e950
void eMeshShapeTriList::Write(cWriteBlock &wb) const {
    wb.Write(mFlags);
    wb.Write(mNumVerts);
    wb.Write(mPad);
    wb.Write(mNumTris);
}

// eMeshShapeTriList::Read(cReadBlock &) — 0x0004e9ac
void eMeshShapeTriList::Read(cReadBlock &rb) {
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this, 1);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 2, 2);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 1, 1);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 4, 2);
}
#pragma control sched=2
