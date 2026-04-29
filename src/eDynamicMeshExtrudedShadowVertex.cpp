// eDynamicMeshExtrudedShadowVertex.cpp -- decompiled from eAll_psp.obj

class cWriteBlock {
public:
    void Write(float);
    void Write(unsigned char);
};

class eDynamicMeshExtrudedShadowVertex {
public:
    float mPos[3];
    unsigned char mA;
    unsigned char mB;
    unsigned char mC;
    unsigned char mD;

    void Write(cWriteBlock &) const;
};

#pragma control sched=1
void eDynamicMeshExtrudedShadowVertex::Write(cWriteBlock &wb) const {
    cWriteBlock *out = &wb;
    const eDynamicMeshExtrudedShadowVertex *self = this;
    __asm__ volatile("" : "+r"(out), "+r"(self));
    out->Write(self->mPos[0]);
    out->Write(self->mPos[1]);
    out->Write(self->mPos[2]);
    out->Write(self->mA);
    out->Write(self->mB);
    out->Write(self->mC);
    out->Write(self->mD);
}
#pragma control sched=2
