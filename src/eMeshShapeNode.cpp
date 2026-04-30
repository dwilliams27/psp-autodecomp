// eMeshShapeNode.cpp - decompiled from eAll_psp.obj

#pragma control sched=1

class cWriteBlock {
public:
    void Write(unsigned short);
};

class eMeshShapeNode {
public:
    void Write(cWriteBlock &) const;
};

void eMeshShapeNode::Write(cWriteBlock &wb) const {
    register cWriteBlock *block __asm__("s0") = &wb;
    register const eMeshShapeNode *node __asm__("s1") = this;
    __asm__ volatile("" : "+r"(block), "+r"(node));

    block->Write(*(unsigned short *)node);
    block->Write(*(unsigned short *)((char *)node + 2));
}

#pragma control sched=2
