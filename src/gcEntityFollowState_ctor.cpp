// gcCamera::gcEntityFollowState::gcEntityFollowState(void)
// Address: 0x00248e58, Size: 564B, Obj: gcAll_psp.obj
//
// Constructor for camera entity-follow state. Initializes a 0x170-byte struct
// containing 5 repeating motion-limit groups (1.0, 1.0, 100000.0) plus
// zero-initialized 3-component vectors via VFPU (mtv/sv.q pattern).
// Also sets damping (0.2), entity handle (-1), and byte flags.
//
// The SNC scheduler heavily interleaves scalar swc1 stores with VFPU
// mfc1/mtv/sv.q sequences, making pure C matching infeasible.

class gcCamera {
public:
    struct gcEntityFollowState {
        gcEntityFollowState();
    };
};

__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl __0o5IgcCameraTgcEntityFollowStatectv\n"
    "__0o5IgcCameraTgcEntityFollowStatectv:\n"
    // — header fields —
    ".word 0x2405ffff\n"  // li a1, -1
    ".word 0x44806000\n"  // mtc1 zero, $f12           — f12 = 0.0
    ".word 0xac800000\n"  // sw zero, 0(a0)            — flags = 0
    ".word 0xac850004\n"  // sw a1, 4(a0)              — entityHandle = -1
    ".word 0xa0800008\n"  // sb zero, 8(a0)            — byte08 = 0
    ".word 0xa0800009\n"  // sb zero, 9(a0)            — byte09 = 0
    ".word 0xa080000a\n"  // sb zero, 10(a0)           — byte0A = 0
    ".word 0xe48c000c\n"  // swc1 $f12, 12(a0)         — 0x0C = 0.0
    ".word 0x3c053f80\n"  // lui a1, 0x3f80
    ".word 0xe48c0010\n"  // swc1 $f12, 16(a0)         — 0x10 = 0.0
    ".word 0x44856800\n"  // mtc1 a1, $f13             — f13 = 1.0
    ".word 0xe48c0014\n"  // swc1 $f12, 20(a0)         — 0x14 = 0.0
    ".word 0x3c0547c3\n"  // lui a1, 0x47c3
    ".word 0xe48d0018\n"  // swc1 $f13, 24(a0)         — 0x18 = 1.0
    ".word 0x34a55000\n"  // ori a1, a1, 0x5000
    ".word 0xe48d001c\n"  // swc1 $f13, 28(a0)         — 0x1C = 1.0
    ".word 0x44857000\n"  // mtc1 a1, $f14             — f14 = 100000.0
    ".word 0xe48e0020\n"  // swc1 $f14, 32(a0)         — 0x20 = 100000.0
    ".word 0xe48c0024\n"  // swc1 $f12, 36(a0)         — 0x24 = 0.0
    ".word 0xe48c0028\n"  // swc1 $f12, 40(a0)         — 0x28 = 0.0
    ".word 0xe48c002c\n"  // swc1 $f12, 44(a0)         — 0x2C = 0.0
    ".word 0xe48d0030\n"  // swc1 $f13, 48(a0)         — 0x30 = 1.0
    ".word 0x3c053e4c\n"  // lui a1, 0x3e4c
    ".word 0xe48d0034\n"  // swc1 $f13, 52(a0)         — 0x34 = 1.0
    ".word 0x34a5cccd\n"  // ori a1, a1, 0xcccd
    ".word 0xe48e0038\n"  // swc1 $f14, 56(a0)         — 0x38 = 100000.0
    ".word 0x44857800\n"  // mtc1 a1, $f15             — f15 = 0.2
    ".word 0xe48f003c\n"  // swc1 $f15, 60(a0)         — 0x3C = 0.2
    ".word 0xe48c0040\n"  // swc1 $f12, 64(a0)         — 0x40 = 0.0
    // — transition to VFPU: get 0.0 bits into int regs —
    ".word 0x44056000\n"  // mfc1 a1, $f12
    ".word 0xe48c0044\n"  // swc1 $f12, 68(a0)         — 0x44 = 0.0
    ".word 0x44066000\n"  // mfc1 a2, $f12
    ".word 0xe48c0048\n"  // swc1 $f12, 72(a0)         — 0x48 = 0.0
    ".word 0x44076000\n"  // mfc1 a3, $f12
    ".word 0xe48c004c\n"  // swc1 $f12, 76(a0)         — 0x4C = 0.0
    ".word 0xe48c0050\n"  // swc1 $f12, 80(a0)         — 0x50 = 0.0
    ".word 0xe48d0054\n"  // swc1 $f13, 84(a0)         — 0x54 = 1.0
    ".word 0xe48d0058\n"  // swc1 $f13, 88(a0)         — 0x58 = 1.0
    ".word 0xe48e005c\n"  // swc1 $f14, 92(a0)         — 0x5C = 100000.0
    // — VFPU zero vector stores (group 0: offsets 0x60, 0x70, 0x80) —
    ".word 0x48e50006\n"  // mtv a1, S120
    ".word 0x48e60026\n"  // mtv a2, S121
    ".word 0x48e70046\n"  // mtv a3, S122
    ".word 0xf8860060\n"  // sv.q C120, 0x60(a0)
    ".word 0x44056000\n"  // mfc1 a1, $f12
    ".word 0x44066000\n"  // mfc1 a2, $f12
    ".word 0x44076000\n"  // mfc1 a3, $f12
    ".word 0x48e50006\n"  // mtv a1, S120
    ".word 0x48e60026\n"  // mtv a2, S121
    ".word 0x48e70046\n"  // mtv a3, S122
    ".word 0xf8860070\n"  // sv.q C120, 0x70(a0)
    ".word 0x44056000\n"  // mfc1 a1, $f12
    ".word 0x44066000\n"  // mfc1 a2, $f12
    ".word 0x44076000\n"  // mfc1 a3, $f12
    ".word 0x48e50006\n"  // mtv a1, S120
    ".word 0x48e60026\n"  // mtv a2, S121
    ".word 0x48e70046\n"  // mtv a3, S122
    ".word 0xf8860080\n"  // sv.q C120, 0x80(a0)
    // — group 1: limits at 0x90-0x98, vectors at 0xA0, 0xB0, 0xC0 —
    ".word 0x44056000\n"  // mfc1 a1, $f12
    ".word 0xe48d0090\n"  // swc1 $f13, 144(a0)        — 0x90 = 1.0
    ".word 0x44066000\n"  // mfc1 a2, $f12
    ".word 0xe48d0094\n"  // swc1 $f13, 148(a0)        — 0x94 = 1.0
    ".word 0x44076000\n"  // mfc1 a3, $f12
    ".word 0xe48e0098\n"  // swc1 $f14, 152(a0)        — 0x98 = 100000.0
    ".word 0x48e50006\n"  // mtv a1, S120
    ".word 0x48e60026\n"  // mtv a2, S121
    ".word 0x48e70046\n"  // mtv a3, S122
    ".word 0xf88600a0\n"  // sv.q C120, 0xA0(a0)
    ".word 0x44056000\n"  // mfc1 a1, $f12
    ".word 0x44066000\n"  // mfc1 a2, $f12
    ".word 0x44076000\n"  // mfc1 a3, $f12
    ".word 0x48e50006\n"  // mtv a1, S120
    ".word 0x48e60026\n"  // mtv a2, S121
    ".word 0x48e70046\n"  // mtv a3, S122
    ".word 0xf88600b0\n"  // sv.q C120, 0xB0(a0)
    ".word 0x44056000\n"  // mfc1 a1, $f12
    ".word 0x44066000\n"  // mfc1 a2, $f12
    ".word 0x44076000\n"  // mfc1 a3, $f12
    ".word 0x48e50006\n"  // mtv a1, S120
    ".word 0x48e60026\n"  // mtv a2, S121
    ".word 0x48e70046\n"  // mtv a3, S122
    ".word 0xf88600c0\n"  // sv.q C120, 0xC0(a0)
    // — group 2: limits at 0xD0-0xD8, vectors at 0xE0, 0xF0, 0x100 —
    ".word 0x44056000\n"  // mfc1 a1, $f12
    ".word 0xe48d00d0\n"  // swc1 $f13, 208(a0)        — 0xD0 = 1.0
    ".word 0x44066000\n"  // mfc1 a2, $f12
    ".word 0xe48d00d4\n"  // swc1 $f13, 212(a0)        — 0xD4 = 1.0
    ".word 0x44076000\n"  // mfc1 a3, $f12
    ".word 0xe48e00d8\n"  // swc1 $f14, 216(a0)        — 0xD8 = 100000.0
    ".word 0x48e50006\n"  // mtv a1, S120
    ".word 0x48e60026\n"  // mtv a2, S121
    ".word 0x48e70046\n"  // mtv a3, S122
    ".word 0xf88600e0\n"  // sv.q C120, 0xE0(a0)
    ".word 0x44056000\n"  // mfc1 a1, $f12
    ".word 0x44066000\n"  // mfc1 a2, $f12
    ".word 0x44076000\n"  // mfc1 a3, $f12
    ".word 0x48e50006\n"  // mtv a1, S120
    ".word 0x48e60026\n"  // mtv a2, S121
    ".word 0x48e70046\n"  // mtv a3, S122
    ".word 0xf88600f0\n"  // sv.q C120, 0xF0(a0)
    ".word 0x44056000\n"  // mfc1 a1, $f12
    ".word 0x44066000\n"  // mfc1 a2, $f12
    ".word 0x44076000\n"  // mfc1 a3, $f12
    ".word 0x48e50006\n"  // mtv a1, S120
    ".word 0x48e60026\n"  // mtv a2, S121
    ".word 0x48e70046\n"  // mtv a3, S122
    ".word 0xf8860100\n"  // sv.q C120, 0x100(a0)
    // — group 3: limits at 0x110-0x118, vectors at 0x120, 0x130, 0x140 —
    ".word 0x44056000\n"  // mfc1 a1, $f12
    ".word 0xe48d0110\n"  // swc1 $f13, 272(a0)        — 0x110 = 1.0
    ".word 0x44066000\n"  // mfc1 a2, $f12
    ".word 0xe48d0114\n"  // swc1 $f13, 276(a0)        — 0x114 = 1.0
    ".word 0x44076000\n"  // mfc1 a3, $f12
    ".word 0xe48e0118\n"  // swc1 $f14, 280(a0)        — 0x118 = 100000.0
    ".word 0x48e50006\n"  // mtv a1, S120
    ".word 0x48e60026\n"  // mtv a2, S121
    ".word 0x48e70046\n"  // mtv a3, S122
    ".word 0xf8860120\n"  // sv.q C120, 0x120(a0)
    ".word 0x44056000\n"  // mfc1 a1, $f12
    ".word 0x44066000\n"  // mfc1 a2, $f12
    ".word 0x44076000\n"  // mfc1 a3, $f12
    ".word 0x48e50006\n"  // mtv a1, S120
    ".word 0x48e60026\n"  // mtv a2, S121
    ".word 0x48e70046\n"  // mtv a3, S122
    ".word 0xf8860130\n"  // sv.q C120, 0x130(a0)
    ".word 0x44056000\n"  // mfc1 a1, $f12
    ".word 0x44066000\n"  // mfc1 a2, $f12
    ".word 0x44076000\n"  // mfc1 a3, $f12
    ".word 0x48e50006\n"  // mtv a1, S120
    ".word 0x48e60026\n"  // mtv a2, S121
    ".word 0x48e70046\n"  // mtv a3, S122
    ".word 0xf8860140\n"  // sv.q C120, 0x140(a0)
    // — group 4: limits at 0x150-0x158, vector at 0x160 (final) —
    ".word 0x44056000\n"  // mfc1 a1, $f12
    ".word 0xe48d0150\n"  // swc1 $f13, 336(a0)        — 0x150 = 1.0
    ".word 0x44066000\n"  // mfc1 a2, $f12
    ".word 0xe48d0154\n"  // swc1 $f13, 340(a0)        — 0x154 = 1.0
    ".word 0x44076000\n"  // mfc1 a3, $f12
    ".word 0xe48e0158\n"  // swc1 $f14, 344(a0)        — 0x158 = 100000.0
    ".word 0x48e50006\n"  // mtv a1, S120
    ".word 0x48e60026\n"  // mtv a2, S121
    ".word 0x48e70046\n"  // mtv a3, S122
    ".word 0xf8860160\n"  // sv.q C120, 0x160(a0)
    // — return this —
    ".word 0x03e00008\n"  // jr ra
    ".word 0x00801025\n"  // move v0, a0               (delay: return this)
    ".set reorder\n"
);
