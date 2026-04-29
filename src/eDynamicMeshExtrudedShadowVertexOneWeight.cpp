// eDynamicMeshExtrudedShadowVertexOneWeight.cpp — decompiled from eAll_psp.obj

class cWriteBlock {
public:
    int _data[2];
    void Write(float);
    void Write(unsigned char);
    void Write(unsigned short);
};

struct eDynamicMeshExtrudedShadowVertexOneWeight {
    float mPos[3];
    unsigned char mWeight0;
    unsigned char mWeight1;
    unsigned short mIndex;

    void Write(cWriteBlock &) const;
};

#pragma control sched=1

// ── eDynamicMeshExtrudedShadowVertexOneWeight::Write(cWriteBlock &) const @ 0x0004d148 ──
void eDynamicMeshExtrudedShadowVertexOneWeight::Write(cWriteBlock &wb) const {
    cWriteBlock *out = &wb;
    const eDynamicMeshExtrudedShadowVertexOneWeight *self = this;
    __asm__ volatile("" : "+r"(out), "+r"(self));
    out->Write(self->mPos[0]);
    out->Write(self->mPos[1]);
    out->Write(self->mPos[2]);
    out->Write(self->mWeight0);
    out->Write(self->mWeight1);
    out->Write(self->mIndex);
}

#pragma control sched=2
