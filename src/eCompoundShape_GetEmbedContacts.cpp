// eCompoundShape::GetEmbedContacts(int, const mSphere *, const eCollisionInfo &, eContactCollector *) const
// Address: 0x0007340c, Size: 400B, Obj: eAll_psp.obj
// Dense VFPU: transforms spheres to sub-shape local space, transforms
// collector's basis matrix, calls virtual method, restores matrix.

__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl __0fOeCompoundShapeQGetEmbedContactsiPC6HmSphereRC6OeCollisionInfoP6ReContactCollectorK\n"
    "__0fOeCompoundShapeQGetEmbedContactsiPC6HmSphereRC6OeCollisionInfoP6ReContactCollectorK:\n"
    ".word 0x27bdff30\n"  // addiu   sp, sp, -208
    ".word 0xafb000c0\n"  // sw      s0, 192(sp)
    ".word 0xafb100c4\n"  // sw      s1, 196(sp)
    ".word 0xafbf00c8\n"  // sw      ra, 200(sp)
    ".word 0x01008025\n"  // move    s0, t0
    ".word 0x34110000\n"  // li      s1, 0
    ".word 0xda060050\n"  // lv.q    C120, 0x50(s0)
    ".word 0xfba60030\n"  // sv.q    C120, 0x30(sp)
    ".word 0xda060020\n"  // lv.q    C120, 0x20(s0)
    ".word 0xfba60000\n"  // sv.q    C120, 0x00(sp)
    ".word 0xda060030\n"  // lv.q    C120, 0x30(s0)
    ".word 0xfba60010\n"  // sv.q    C120, 0x10(sp)
    ".word 0xda060040\n"  // lv.q    C120, 0x40(s0)
    ".word 0xfba60020\n"  // sv.q    C120, 0x20(sp)
    ".word 0x34080000\n"  // li      t0, 0
    ".word 0x0105582a\n"  // slt     t3, t0, a1
    ".word 0x94e90008\n"  // lhu     t1, 8(a3)
    ".word 0x1160001a\n"  // beqz    t3, past_loop
    ".word 0x8c8a0080\n"  // lw      t2, 128(a0)
    ".word 0x00c05825\n"  // move    t3, a2
    ".word 0x34020000\n"  // li      v0, 0
    ".word 0x27a60040\n"  // addiu   a2, sp, 64
    ".word 0x01625821\n"  // addu    t3, t3, v0
    "loop_body:\n"
    ".word 0xc56c000c\n"  // lwc1    f12, 12(t3)
    ".word 0x00094880\n"  // sll     t1, t1, 2
    ".word 0x01494821\n"  // addu    t1, t2, t1
    ".word 0x8d290000\n"  // lw      t1, 0(t1)
    ".word 0x25290010\n"  // addiu   t1, t1, 16
    ".word 0xd9660000\n"  // lv.q    C120, 0(t3)
    ".word 0xd9200000\n"  // lv.q    C000, 0(t1)
    ".word 0xd9210010\n"  // lv.q    C010, 0x10(t1)
    ".word 0xd9220020\n"  // lv.q    C020, 0x20(t1)
    ".word 0xd9230030\n"  // lv.q    C030, 0x30(t1)
    ".word 0x60838606\n"  // vsub.t  C120, C120, C030
    ".word 0xf1068007\n"  // vtfm3.t C130, M000, C120
    ".word 0xf8c70000\n"  // sv.q    C130, 0(a2)
    ".word 0xe4cc000c\n"  // swc1    f12, 12(a2)
    ".word 0x25080001\n"  // addiu   t0, t0, 1
    ".word 0x24c60010\n"  // addiu   a2, a2, 16
    ".word 0x256b0010\n"  // addiu   t3, t3, 16
    ".word 0x0105102a\n"  // slt     v0, t0, a1
    ".word 0x94e90008\n"  // lhu     t1, 8(a3)
    ".word 0x1440ffec\n"  // bnez    v0, loop_body
    ".word 0x8c8a0080\n"  // lw      t2, 128(a0)
    "past_loop:\n"
    ".word 0x00093080\n"  // sll     a2, t1, 2
    ".word 0x01463021\n"  // addu    a2, t2, a2
    ".word 0x8cc60000\n"  // lw      a2, 0(a2)
    ".word 0x24c60010\n"  // addiu   a2, a2, 16
    ".word 0xd8c60000\n"  // lv.q    C120, 0(a2)
    ".word 0xd8c70010\n"  // lv.q    C130, 0x10(a2)
    ".word 0xd8c80020\n"  // lv.q    C200, 0x20(a2)
    ".word 0xd8c90030\n"  // lv.q    C210, 0x30(a2)
    ".word 0xdba00000\n"  // lv.q    C000, 0x00(sp)
    ".word 0xdba10010\n"  // lv.q    C010, 0x10(sp)
    ".word 0xdba20020\n"  // lv.q    C020, 0x20(sp)
    ".word 0xdba30030\n"  // lv.q    C030, 0x30(sp)
    ".word 0xf106a00a\n"  // vtfm3.t C220, E000, C120
    ".word 0xf107a006\n"  // vtfm3.t C120, E000, C130
    ".word 0xf108a007\n"  // vtfm3.t C130, E000, C200
    ".word 0xf189a008\n"  // vhtfm4.q C200, E000, C210
    ".word 0xfbaa0080\n"  // sv.q    C220, 0x80(sp)
    ".word 0xfba60090\n"  // sv.q    C120, 0x90(sp)
    ".word 0xfba700a0\n"  // sv.q    C130, 0xA0(sp)
    ".word 0xfba800b0\n"  // sv.q    C200, 0xB0(sp)
    ".word 0xfa080050\n"  // sv.q    C200, 0x50(s0)
    ".word 0xdba60080\n"  // lv.q    C120, 0x80(sp)
    ".word 0xfa060020\n"  // sv.q    C120, 0x20(s0)
    ".word 0xdba60090\n"  // lv.q    C120, 0x90(sp)
    ".word 0xfa060030\n"  // sv.q    C120, 0x30(s0)
    ".word 0xdba600a0\n"  // lv.q    C120, 0xA0(sp)
    ".word 0xfa060040\n"  // sv.q    C120, 0x40(s0)
    ".word 0x94e60008\n"  // lhu     a2, 8(a3)
    ".word 0x8c840080\n"  // lw      a0, 128(a0)
    ".word 0x00063080\n"  // sll     a2, a2, 2
    ".word 0x00862021\n"  // addu    a0, a0, a2
    ".word 0x8c840000\n"  // lw      a0, 0(a0)
    ".word 0x8c860004\n"  // lw      a2, 4(a0)
    ".word 0x24c800b0\n"  // addiu   t0, a2, 176
    ".word 0x85060000\n"  // lh      a2, 0(t0)
    ".word 0x00862021\n"  // addu    a0, a0, a2
    ".word 0x27a60040\n"  // addiu   a2, sp, 64
    ".word 0x8d090004\n"  // lw      t1, 4(t0)
    ".word 0x0120f809\n"  // jalr    t1
    ".word 0x02004025\n"  // move    t0, s0
    ".word 0xdba60030\n"  // lv.q    C120, 0x30(sp)
    ".word 0x54400001\n"  // bnezl   v0, 1f
    ".word 0x34110001\n"  // li      s1, 1
    "1:\n"
    ".word 0xfa060050\n"  // sv.q    C120, 0x50(s0)
    ".word 0xdba60000\n"  // lv.q    C120, 0x00(sp)
    ".word 0xfa060020\n"  // sv.q    C120, 0x20(s0)
    ".word 0xdba60010\n"  // lv.q    C120, 0x10(sp)
    ".word 0xfa060030\n"  // sv.q    C120, 0x30(s0)
    ".word 0xdba60020\n"  // lv.q    C120, 0x20(sp)
    ".word 0xfa060040\n"  // sv.q    C120, 0x40(s0)
    ".word 0x02201025\n"  // move    v0, s1
    ".word 0x8fb000c0\n"  // lw      s0, 192(sp)
    ".word 0x8fb100c4\n"  // lw      s1, 196(sp)
    ".word 0x8fbf00c8\n"  // lw      ra, 200(sp)
    ".word 0x03e00008\n"  // jr      ra
    ".word 0x27bd00d0\n"  // addiu   sp, sp, 208
    ".set reorder\n"
);
