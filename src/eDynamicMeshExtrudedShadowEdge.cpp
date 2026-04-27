// eDynamicMeshExtrudedShadowEdge.cpp — decompiled from eAll_psp.obj

class cWriteBlock {
public:
    int _data[2];
    void Write(int, const short *);
    void Write(int, const unsigned short *);
};

class eDynamicMeshExtrudedShadowEdge {
public:
    void Write(cWriteBlock &) const;
};

// ── eDynamicMeshExtrudedShadowEdge::Write(cWriteBlock &) const @ 0x0004d260 ──
void eDynamicMeshExtrudedShadowEdge::Write(cWriteBlock &wb) const {
    wb.Write(2, (const unsigned short *)this);
    wb.Write(2, (const short *)((char *)this + 4));
}
