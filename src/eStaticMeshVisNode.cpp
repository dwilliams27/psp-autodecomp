// eStaticMeshVisNode.cpp — decompiled from eAll_psp.obj

class cReadBlock {
public:
    int _data[5];
};

class cWriteBlock {
public:
    int _data[2];
    void Write(unsigned short);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class eStaticMeshVisNode {
public:
    void Read(cReadBlock &);
    void Write(cWriteBlock &) const;
};

// ── eStaticMeshVisNode::Write(cWriteBlock &) const @ 0x00043ff0 ──
void eStaticMeshVisNode::Write(cWriteBlock &wb) const {
    wb.Write(*(unsigned short *)this);
    wb.Write(*(unsigned short *)((char *)this + 2));
}

// ── eStaticMeshVisNode::Read(cReadBlock &) @ 0x00044034 ──
void eStaticMeshVisNode::Read(cReadBlock &rb) {
    cFileSystem::Read(*(void **)rb._data[0], this, 2);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 2, 2);
}
