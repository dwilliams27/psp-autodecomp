// eDynamicMeshExtrudedShadowEdge.cpp — decompiled from eAll_psp.obj

class cReadBlock {
public:
    int _data[5];
};

class cWriteBlock {
public:
    int _data[2];
    void Write(int, const short *);
    void Write(int, const unsigned short *);
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

class eDynamicMeshExtrudedShadowEdge {
public:
    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

// ── eDynamicMeshExtrudedShadowEdge::Write(cWriteBlock &) const @ 0x0004d260 ──
void eDynamicMeshExtrudedShadowEdge::Write(cWriteBlock &wb) const {
    wb.Write(2, (const unsigned short *)this);
    wb.Write(2, (const short *)((char *)this + 4));
}

// ── eDynamicMeshExtrudedShadowEdge::Read(cReadBlock &) @ 0x0004d2ac ──
void eDynamicMeshExtrudedShadowEdge::Read(cReadBlock &rb) {
    cFileSystem::Read(*(void **)rb._data[0], this, 4);
    cFileSystem::Read(*(void **)rb._data[0], (char *)this + 4, 4);
}
