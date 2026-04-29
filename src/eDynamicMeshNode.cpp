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
    wb.Write(mField0);
    wb.Write(3, mVec40);
    wb.Write(3, mVec10);
    wb.Write(3, mVec20);
    wb.Write(3, mVec30);
    mName.Write(wb);
}

#pragma control sched=2
