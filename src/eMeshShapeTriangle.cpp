class cFile;

class cReadBlock {
public:
    int _data[5];
};

class cWriteBlock {
public:
    int _data[2];
    void Write(unsigned short);
    void Write(unsigned char);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

struct eMeshShapeTriangle {
    unsigned short mVerts[3];   // 0  (6 bytes)
    unsigned short mF6;         // 6
    unsigned short mF8;         // 8
    void Read(cReadBlock &);
};

#pragma control sched=1
// eMeshShapeTriangle::Read(cReadBlock &) — 0x0004e8e8
void eMeshShapeTriangle::Read(cReadBlock &rb) {
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this, 6);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 6, 2);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 8, 2);
}
#pragma control sched=2
