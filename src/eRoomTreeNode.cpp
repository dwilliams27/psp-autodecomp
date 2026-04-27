// eRoomTreeNode — small POD-style room tree node serialized via cWriteBlock /
// cReadBlock. Layout: two u16 IDs at offsets 0,2 and two 12-byte float triples
// at offsets 0x10 and 0x20 (likely AABB min/max or two RGB-ish vectors).
//
// Functions matched here:
//   eRoomTreeNode::Write(cWriteBlock &) const   @ 0x0003c7fc  100B
//   eRoomTreeNode::Read(cReadBlock &)           @ 0x0003c860  124B

class cFile;

class cWriteBlock {
public:
    int _data[2];
    void Write(unsigned short);
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

struct eRoomTreeNode {
    unsigned short mField0;     // 0x00
    unsigned short mField2;     // 0x02
    char           _pad[0x0C];  // 0x04..0x0F
    float          mFloats0[3]; // 0x10
    char           _pad2[0x04]; // 0x1C..0x1F
    float          mFloats1[3]; // 0x20

    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

// ── eRoomTreeNode::Write ── @ 0x0003c7fc
void eRoomTreeNode::Write(cWriteBlock &wb) const {
    wb.Write(mField0);
    wb.Write(mField2);
    wb.Write(3, mFloats0);
    wb.Write(3, mFloats1);
}

// ── eRoomTreeNode::Read ── @ 0x0003c860
void eRoomTreeNode::Read(cReadBlock &rb) {
    cFileSystem::Read(*(void **)rb._data[0], (char *)this, 2);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 2, 2);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x10, 0xC);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 0x20, 0xC);
}
