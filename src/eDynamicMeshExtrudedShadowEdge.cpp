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

#pragma control sched=1

// ── eDynamicMeshExtrudedShadowEdge::Write(cWriteBlock &) const @ 0x0004d260 ──
void eDynamicMeshExtrudedShadowEdge::Write(cWriteBlock &wb) const {
    wb.Write(2, (const unsigned short *)this);
    wb.Write(2, (const short *)((char *)this + 4));
}

// ── eDynamicMeshExtrudedShadowEdge::Read(cReadBlock &) @ 0x0004d2ac ──
void eDynamicMeshExtrudedShadowEdge::Read(cReadBlock &rb) {
    cReadBlock *in = &rb;
    eDynamicMeshExtrudedShadowEdge *self = this;
    __asm__ volatile("" : "+r"(in), "+r"(self));
    void *handle = *(void **)in->_data[0];
    cFileSystem::Read(handle, self, 4);
    handle = *(void **)in->_data[0];
    cFileSystem::Read(handle, (char *)self + 4, 4);
}

#pragma control sched=2
