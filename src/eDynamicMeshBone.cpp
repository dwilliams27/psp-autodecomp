// eDynamicMeshBone — bone with four 3-float vectors at offsets 0x00, 0x10,
// 0x20, 0x30 (likely a 4x3 transform: 3 axis vectors + translation, or
// origin + axes). Serialized via cWriteBlock / cReadBlock.
//
// Functions matched here:
//   eDynamicMeshBone::Write(cWriteBlock &) const  @ 0x0004cdac  108B
//   eDynamicMeshBone::Read(cReadBlock &)          @ 0x0004ce18  124B

class cFile;

class cWriteBlock {
public:
    int _data[2];
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

struct eDynamicMeshBone {
    float mVec0[3];   // 0x00
    char  _pad0[0x4]; // 0x0C
    float mVec1[3];   // 0x10
    char  _pad1[0x4]; // 0x1C
    float mVec2[3];   // 0x20
    char  _pad2[0x4]; // 0x2C
    float mVec3[3];   // 0x30

    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

#pragma control sched=1

// ── eDynamicMeshBone::Write ── @ 0x0004cdac
void eDynamicMeshBone::Write(cWriteBlock &wb) const {
    wb.Write(3, mVec3);
    wb.Write(3, mVec0);
    wb.Write(3, mVec1);
    wb.Write(3, mVec2);
}

// ── eDynamicMeshBone::Read ── @ 0x0004ce18
void eDynamicMeshBone::Read(cReadBlock &rb) {
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x30, 0xC);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this,        0xC);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x10, 0xC);
    __asm__ volatile("" ::: "memory");
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x20, 0xC);
}

#pragma control sched=2
