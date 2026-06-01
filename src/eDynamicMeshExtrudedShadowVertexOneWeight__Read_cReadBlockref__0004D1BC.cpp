// eDynamicMeshExtrudedShadowVertexOneWeight::Read(cReadBlock &) — 0x0004d1bc
//
// ODR-WARNING: split-TU local redeclarations. The class layout and helper
// signatures are declared locally so this flat method view does not clash with
// the inheritance/forward views used by other TUs. No new declarations are
// added to include/*.h.
//
// Structure mirrors the matched sibling eDynamicMeshNode::Read (0x4cd10) and
// eMeshShapeNode::Read (0x4ea6c): the incoming cReadBlock& holds the
// cFileHandle* in its first slot (lw a0,0(s0); lw a0,0(a0)). Six
// cFileSystem::Read calls copy bytes into this+0 (4), this+4 (4), this+8 (4),
// this+0xC (1), this+0xD (1), this+0xE (2). Arg order is (h=handle, dest, size).
//
// sched=1 prologue/epilogue (eAll transition zone): save s0/s1/ra, s0=rb&,
// s1=this, the reads, restore, jr.

class cFileHandle;
class cReadBlock;

extern "C" {
    void cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(
        cFileHandle *, void *, unsigned int);
}

class eDynamicMeshExtrudedShadowVertexOneWeight {
public:
    void Read(cReadBlock &rb);
};

#pragma control sched=1

void eDynamicMeshExtrudedShadowVertexOneWeight::Read(cReadBlock &rb) {
    void *rbp = (void *)&rb;
    void *self = (void *)this;
    __asm__ volatile("" : "+r"(rbp), "+r"(self) :: "memory");

    cFileHandle *h = *(cFileHandle **)*(void **)rbp;
    cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(
        h, (char *)self + 0, 4);

    h = *(cFileHandle **)*(void **)rbp;
    cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(
        h, (char *)self + 4, 4);

    h = *(cFileHandle **)*(void **)rbp;
    cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(
        h, (char *)self + 8, 4);

    h = *(cFileHandle **)*(void **)rbp;
    cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(
        h, (char *)self + 0xC, 1);

    h = *(cFileHandle **)*(void **)rbp;
    cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(
        h, (char *)self + 0xD, 1);

    h = *(cFileHandle **)*(void **)rbp;
    cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(
        h, (char *)self + 0xE, 2);
}

#pragma control sched=2
