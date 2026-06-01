// eMeshShapeNode::Read(cReadBlock &) — 0x0004ea6c
//
// ODR-WARNING: split-TU local redeclarations. The class layout and helper
// signature are declared locally so this flat method view does not clash with
// the inheritance/forward views used by other TUs. No new declarations are
// added to include/*.h.
//
// Structure: the incoming cReadBlock& holds the cFileHandle* in its first slot
// (lw a0,0(s0); lw a0,0(a0)). Read two 2-byte words off the file into this+0
// and this+2 via cFileSystem::Read. The arg order is (this=dest, rb&).
//
// Layout matches the classic sched=1 prologue/epilogue (eAll transition zone):
// save s0/s1/ra, move s0=rb&, s1=this, two reads, restore, jr.

class cFileHandle;
class cReadBlock;

extern "C" {
    void cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(
        cFileHandle *, void *, unsigned int);
}

class eMeshShapeNode {
public:
    short mFields[2];
    void Read(cReadBlock &rb);
};

#pragma control sched=1

void eMeshShapeNode::Read(cReadBlock &rb) {
    void *rbp = (void *)&rb;
    void *self = (void *)this;
    __asm__ volatile("" : "+r"(rbp), "+r"(self) :: "memory");

    cFileHandle *h = *(cFileHandle **)*(void **)rbp;
    cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(
        h, (char *)self + 0, 2);

    h = *(cFileHandle **)*(void **)rbp;
    cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(
        h, (char *)self + 2, 2);
}

#pragma control sched=2
