// eLightGridSample::Read(cReadBlock &) — 0x00054bb8
//
// ODR-WARNING: split-TU local redeclarations. The class layout and helper
// signatures are declared locally so this flat method view does not clash
// with the views used by other TUs. No new declarations are added to
// include/*.h.
//
// Structure (mirror of sibling eLightGridSample::Write @ 0x54b44 inverted):
// the incoming cReadBlock& holds the cFileHandle* in its first slot
// (lw a0,0(s0); lw a0,0(a0)). First cFileSystem::Read copies 6 bytes into
// this+0 (mValues[0..2]). Then three 2-byte reads into a 16-bit stack temp,
// each packed into mValues[3] (offset 6) as a 5/6/5 bitfield via lh/and/or/sh.

class cFileHandle;

extern "C" {
    void cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(
        cFileHandle *, void *, unsigned int);
}

class cReadBlock;

struct eLightGridSample {
    short mValues[4];
    void Read(cReadBlock &rb);
};

#pragma control sched=1

void eLightGridSample::Read(cReadBlock &rb) {
    cReadBlock *rbp = &rb;
    eLightGridSample *self = this;
    __asm__ volatile("" : "+r"(rbp), "+r"(self) :: "memory");

    unsigned short tmp;

    cFileHandle *h = *(cFileHandle **)*(void **)rbp;
    cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(h, self->mValues, 6);

    h = *(cFileHandle **)*(void **)rbp;
    cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(h, &tmp, 2);
    self->mValues[3] = (short)((self->mValues[3] & ~0x1F) | (tmp & 0x1F));

    h = *(cFileHandle **)*(void **)rbp;
    cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(h, &tmp, 2);
    self->mValues[3] = (short)((self->mValues[3] & ~0x7E0) | ((tmp & 0x3F) << 5));

    h = *(cFileHandle **)*(void **)rbp;
    cFileSystem__Read_cFileHandleptr_voidptr_unsignedintstatic(h, &tmp, 2);
    self->mValues[3] = (short)((self->mValues[3] & ~0xF800) | ((tmp & 0x1F) << 11));
}

#pragma control sched=2
