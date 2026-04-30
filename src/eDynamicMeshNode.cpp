// eDynamicMeshNode.cpp - eAll_psp.obj

class cWriteBlock {
public:
    void Write(int, const float *);
    void Write(int);
};

class cName {
public:
    void Write(cWriteBlock &) const;
};

class eDynamicMeshNode {
public:
    int mField0;          // 0x00
    char _pad04[0x0C];
    float mVec10[4];      // 0x10
    float mVec20[4];      // 0x20
    float mVec30[4];      // 0x30
    float mVec40[4];      // 0x40
    cName mName;          // 0x50

    void Write(cWriteBlock &) const;
};

#pragma control sched=1

// 0x0004cc8c - eDynamicMeshNode::Write(cWriteBlock &) const
void eDynamicMeshNode::Write(cWriteBlock &wb) const {
    cWriteBlock *out = &wb;
    const eDynamicMeshNode *self = this;
    __asm__ volatile("" : "+r"(out), "+r"(self));
    out->Write(self->mField0);
    out->Write(3, self->mVec40);
    out->Write(3, self->mVec10);
    out->Write(3, self->mVec20);
    out->Write(3, self->mVec30);
    self->mName.Write(*out);
}

#pragma control sched=2
