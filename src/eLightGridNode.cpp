// eLightGridNode — light grid node with packed 32-bit field at offset 0,
// signed short at offset 2 (overlapping upper half of mPacked) used in the
// non-mode-3 (compact) representation.
//
//   mPacked layout:
//     bits 0..1   : mode (0,1,2 = packed-with-short; 3 = full 24-bit value)
//     packed mode: bits 2..15 = 14-bit aux value, bits 16..31 = signed short
//                  (read separately via *(short *)((char*)this + 2))
//     full mode  : bits 2..7  = 6-bit aux value, bits 8..31 = 24-bit value
//
// Functions:
//   eLightGridNode::Write(cWriteBlock &) const  @ 0x00054984  156B
//   eLightGridNode::Read (cReadBlock  &)        @ 0x00054a20  292B

class cFile;

class cWriteBlock {
public:
    int _data[2];
    void Write(int);
    void Write(unsigned int);
};

class cReadBlock {
public:
    int _data[5];
};

class cFileSystem {
public:
    static void Read(void *handle, void *buf, unsigned int size);
};

struct eLightGridNode {
    union {
        int mPacked;        // 0x00
        struct {
            unsigned short mLow;   // 0x00
            short          mShort; // 0x02 (overlaps upper half of mPacked)
        };
    };

    void Write(cWriteBlock &) const;
    void Read(cReadBlock &);
};

#pragma control sched=1

// ── eLightGridNode::Write @ 0x00054984 ──
void eLightGridNode::Write(cWriteBlock &wb) const {
    __asm__ volatile("" : : "r"(&wb));
    wb.Write((unsigned int)(mPacked & 3));
    if ((mPacked & 3) != 3) {
        wb.Write((unsigned int)((unsigned int)(mPacked & 0xFFFC) >> 2));
        wb.Write((int)mShort);
    } else {
        const eLightGridNode *self = this;
        __asm__ volatile("" : "+r"(self));
        wb.Write((unsigned int)((unsigned int)(self->mPacked & 0xFC) >> 2));
        wb.Write((unsigned int)((unsigned int)(self->mPacked & ~0xFF) >> 8));
    }
}

// ── eLightGridNode::Read @ 0x00054a20 ──
void eLightGridNode::Read(cReadBlock &rb) {
    int local;
    void *h;
    __asm__ volatile("" : : "r"(this));
    h = *(void **)rb._data[0];
    __asm__ volatile("" : "+r"(h));
    cFileSystem::Read(h, &local, 4);
    int p = mPacked & ~3;
    int q = local & 3;
    mPacked = p | q;
    if ((mPacked & 3) != 3) {
        h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &local, 4);
        int p2 = mPacked & 0xFFFF0003;
        int q2 = (local & 0x3FFF) << 2;
        mPacked = p2 | q2;
        h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &local, 4);
        mShort = (short)local;
    } else {
        h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &local, 4);
        int p3 = mPacked & 0xFFFFFF03;
        int q3 = (local & 0x3F) << 2;
        mPacked = p3 | q3;
        h = *(void **)rb._data[0];
        __asm__ volatile("" : "+r"(h));
        cFileSystem::Read(h, &local, 4);
        int p4 = mPacked & 0xFF;
        int q4 = (local & 0xFFFFFF) << 8;
        mPacked = p4 | q4;
    }
}
