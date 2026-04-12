// eCompoundShape::GetAABB(mBox *, const mOCS &) const
// Address: 0x0007359c, Size: 336B, Obj: eAll_psp.obj
// Dense VFPU: initializes box to empty (FLT_MAX/-FLT_MAX via VFPU broadcast),
// iterates sub-shapes, transforms each sub-shape's OCS by parent OCS using
// vtfm3/vhtfm4 matrix multiply, calls virtual GetAABB, extends result box.
// Calls: mBox::Extend(const mBox &) at 0x1a0a74

__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl __0fOeCompoundShapeHGetAABBP6EmBoxRC6EmOCSK\n"
    "__0fOeCompoundShapeHGetAABBP6EmBoxRC6EmOCSK:\n"
    // Prologue: save s0-s5, ra; alloc 128B stack
    ".word 0x27bdff80\n"  // addiu   sp, sp, -128
    ".word 0xafb00060\n"  // sw      s0, 96(sp)
    ".word 0xafb10064\n"  // sw      s1, 100(sp)
    ".word 0xafb20068\n"  // sw      s2, 104(sp)
    ".word 0xafb3006c\n"  // sw      s3, 108(sp)
    ".word 0xafb40070\n"  // sw      s4, 112(sp)
    ".word 0xafb50074\n"  // sw      s5, 116(sp)
    ".word 0xafbf0078\n"  // sw      ra, 120(sp)
    // s2 = this
    ".word 0x00809025\n"  // move    s2, a0
    // Initialize box->min = {FLT_MAX, FLT_MAX, FLT_MAX} via VFPU broadcast
    ".word 0x3c047f7f\n"  // lui     a0, 0x7f7f
    ".word 0x3484ffff\n"  // ori     a0, a0, 0xffff       ; a0 = 0x7f7fffff (FLT_MAX)
    ".word 0x44846000\n"  // mtc1    a0, $f12
    ".word 0x44046000\n"  // mfc1    a0, $f12
    ".word 0x44076000\n"  // mfc1    a3, $f12
    ".word 0x44086000\n"  // mfc1    t0, $f12
    ".word 0x48e40006\n"  // mtv     $a0, S120
    ".word 0x48e70026\n"  // mtv     $a3, S121
    ".word 0x48e80046\n"  // mtv     $t0, S122
    ".word 0xf8a60000\n"  // sv.q    C120, 0x0($a1)       ; box->min = FLT_MAX
    // Initialize box->max = {-FLT_MAX, -FLT_MAX, -FLT_MAX}
    ".word 0x3c04ff7f\n"  // lui     a0, 0xff7f
    ".word 0x3484ffff\n"  // ori     a0, a0, 0xffff       ; a0 = 0xff7fffff (-FLT_MAX)
    ".word 0x44846800\n"  // mtc1    a0, $f13
    ".word 0x44046800\n"  // mfc1    a0, $f13
    ".word 0x44076800\n"  // mfc1    a3, $f13
    ".word 0x44086800\n"  // mfc1    t0, $f13
    ".word 0x48e40006\n"  // mtv     $a0, S120
    ".word 0x48e70026\n"  // mtv     $a3, S121
    ".word 0x48e80046\n"  // mtv     $t0, S122
    ".word 0xf8a60010\n"  // sv.q    C120, 0x10($a1)      ; box->max = -FLT_MAX
    // Loop setup
    ".word 0x34150000\n"  // li      s5, 0                 ; i = 0
    ".word 0x27b30040\n"  // addiu   s3, sp, 64            ; tempBox on stack
    ".word 0x8e440080\n"  // lw      a0, 128(s2)           ; a0 = this->mSubShapes
    ".word 0x34140000\n"  // li      s4, 0                 ; byte offset = 0
    ".word 0x00a08825\n"  // move    s1, a1                ; s1 = box
    ".word 0x00c08025\n"  // move    s0, a2                ; s0 = &ocs
    // Loop head: get array count via bnezl pattern
    "loop_head:\n"
    ".word 0x34050000\n"  // li      a1, 0
    ".word 0x54800001\n"  // bnezl   a0, 1f                ; if (mSubShapes != NULL)
    ".word 0x8c85fffc\n"  // lw      a1, -4(a0)            ;   a1 = array count
    "1:\n"
    ".word 0x02a5282a\n"  // slt     a1, s5, a1            ; a1 = (i < count)
    ".word 0x10a00023\n"  // beqz    a1, loop_end
    ".word 0x00942021\n"  // addu    a0, a0, s4            ; delay: a0 = &mSubShapes[i]
    // Loop body: transform sub-shape OCS and call virtual GetAABB
    ".word 0x8c850000\n"  // lw      a1, 0(a0)             ; a1 = subShape
    ".word 0x24a50010\n"  // addiu   a1, a1, 16            ; a1 = &subShape->localOCS
    // Load sub-shape's local transform (4 quadwords)
    ".word 0xd8a60000\n"  // lv.q    C120, 0x0($a1)
    ".word 0xd8a70010\n"  // lv.q    C130, 0x10($a1)
    ".word 0xd8a80020\n"  // lv.q    C200, 0x20($a1)
    ".word 0xd8a90030\n"  // lv.q    C210, 0x30($a1)
    // Load parent OCS (4 quadwords from s0)
    ".word 0xda000000\n"  // lv.q    C000, 0x0($s0)
    ".word 0xda010010\n"  // lv.q    C010, 0x10($s0)
    ".word 0xda020020\n"  // lv.q    C020, 0x20($s0)
    ".word 0xda030030\n"  // lv.q    C030, 0x30($s0)
    // Matrix multiply: combined = parent * subLocal
    ".word 0xf106a00a\n"  // vtfm3.t C220, E000, C120     ; row0
    ".word 0xf107a006\n"  // vtfm3.t C120, E000, C130     ; row1
    ".word 0xf108a007\n"  // vtfm3.t C130, E000, C200     ; row2
    ".word 0xf189a008\n"  // vhtfm4.q C200, E000, C210    ; position (homogeneous)
    // Store combined OCS to stack (sp+0..sp+0x3f)
    ".word 0xfbaa0000\n"  // sv.q    C220, 0x0($sp)
    ".word 0xfba60010\n"  // sv.q    C120, 0x10($sp)
    ".word 0xfba70020\n"  // sv.q    C130, 0x20($sp)
    ".word 0xfba80030\n"  // sv.q    C200, 0x30($sp)
    // Virtual dispatch: subShape->GetAABB(&tempBox, combinedOCS)
    ".word 0x8c840000\n"  // lw      a0, 0(a0)             ; re-read subShape ptr
    ".word 0x8c850004\n"  // lw      a1, 4(a0)             ; a1 = vtable
    ".word 0x24a500c8\n"  // addiu   a1, a1, 200           ; a1 = &vtable[GetAABB]
    ".word 0x84a60000\n"  // lh      a2, 0(a1)             ; a2 = this-adjust
    ".word 0x00862021\n"  // addu    a0, a0, a2            ; adjust this ptr
    ".word 0x8ca70004\n"  // lw      a3, 4(a1)             ; a3 = function ptr
    ".word 0x02602825\n"  // move    a1, s3                ; a1 = &tempBox
    ".word 0x00e0f809\n"  // jalr    a3                     ; call GetAABB
    ".word 0x03a03025\n"  // move    a2, sp                ; delay: a2 = &combinedOCS
    // box->Extend(tempBox)
    ".word 0x02202025\n"  // move    a0, s1                ; a0 = box
    ".word 0x0c06829d\n"  // jal     mBox::Extend          ; 0x1a0a74
    ".word 0x02602825\n"  // move    a1, s3                ; delay: a1 = &tempBox
    // Loop increment
    ".word 0x26b50001\n"  // addiu   s5, s5, 1             ; i++
    ".word 0x26940004\n"  // addiu   s4, s4, 4             ; byteOff += 4
    ".word 0x1000ffd9\n"  // b       loop_head
    ".word 0x8e440080\n"  // lw      a0, 128(s2)           ; delay: reload mSubShapes
    // Epilogue
    "loop_end:\n"
    ".word 0x8fb00060\n"  // lw      s0, 96(sp)
    ".word 0x8fb10064\n"  // lw      s1, 100(sp)
    ".word 0x8fb20068\n"  // lw      s2, 104(sp)
    ".word 0x8fb3006c\n"  // lw      s3, 108(sp)
    ".word 0x8fb40070\n"  // lw      s4, 112(sp)
    ".word 0x8fb50074\n"  // lw      s5, 116(sp)
    ".word 0x8fbf0078\n"  // lw      ra, 120(sp)
    ".word 0x03e00008\n"  // jr      ra
    ".word 0x27bd0080\n"  // addiu   sp, sp, 128           ; delay: dealloc stack
    ".set reorder\n"
);

// eCompoundShape::GetSweptContacts(int, const mSphere *, const mCollideInfo *,
//     const eCollisionInfo &, eContactCollector *) const
// Address: 0x000731e4, Size: 552B, Obj: eAll_psp.obj
// Dense VFPU: saves/restores collector basis matrix (mOCS at s0+0x20..0x50),
// transforms sphere positions and collide info to sub-shape local space via
// vsub.t + vtfm3.t, transforms basis via vtfm3/vhtfm4, calls virtual
// GetSweptContacts on the indexed sub-shape, restores original basis.

__asm__(
    ".set noreorder\n"
    ".text\n"
    ".globl __0fOeCompoundShapeQGetSweptContactsiPC6HmSpherePC6MmCollideInfoRC6OeCollisionInfoP6ReContactCollectorK\n"
    "__0fOeCompoundShapeQGetSweptContactsiPC6HmSpherePC6MmCollideInfoRC6OeCollisionInfoP6ReContactCollectorK:\n"
    // Prologue: save s0, s1, ra; alloc 464B stack
    ".word 0x27bdfe30\n"  // addiu   sp, sp, -464
    ".word 0xafb001c0\n"  // sw      s0, 448(sp)
    ".word 0xafb101c4\n"  // sw      s1, 452(sp)
    ".word 0xafbf01c8\n"  // sw      ra, 456(sp)
    // s0 = eContactCollector* (from t1), s1 = result
    ".word 0x01208025\n"  // move    s0, t1
    ".word 0x34110000\n"  // li      s1, 0
    // Save collector's basis matrix to stack (sp+0..sp+0x3f)
    ".word 0xda060050\n"  // lv.q    C120, 0x50($s0)       ; collector->position
    ".word 0xfba60030\n"  // sv.q    C120, 0x30($sp)
    ".word 0xda060020\n"  // lv.q    C120, 0x20($s0)       ; collector->row0
    ".word 0xfba60000\n"  // sv.q    C120, 0x0($sp)
    ".word 0xda060030\n"  // lv.q    C120, 0x30($s0)       ; collector->row1
    ".word 0xfba60010\n"  // sv.q    C120, 0x10($sp)
    ".word 0xda060040\n"  // lv.q    C120, 0x40($s0)       ; collector->row2
    ".word 0xfba60020\n"  // sv.q    C120, 0x20($sp)
    // Loop setup
    ".word 0x34090000\n"  // li      t1, 0                  ; loop counter
    ".word 0x0125102a\n"  // slt     v0, t1, a1             ; v0 = (0 < numSpheres)
    ".word 0x950a0008\n"  // lhu     t2, 8(t0)              ; t2 = subShapeIndex
    ".word 0x1040003f\n"  // beqz    v0, past_loop
    ".word 0x8c8b0080\n"  // lw      t3, 128(a0)            ; delay: t3 = mSubShapes
    // Rearrange args for loop: v1 = collideInfo*, a3 = spheres*, v0 = localCollide*, a2 = localSpheres*
    ".word 0x00e01825\n"  // move    v1, a3                 ; v1 = collideInfo base
    ".word 0x00c03825\n"  // move    a3, a2                 ; a3 = spheres base
    ".word 0x34020000\n"  // li      v0, 0
    ".word 0x27a60140\n"  // addiu   a2, sp, 320            ; a2 = localSpheres on stack
    ".word 0x00e23821\n"  // addu    a3, a3, v0
    ".word 0x340c0000\n"  // li      t4, 0
    ".word 0x27a20040\n"  // addiu   v0, sp, 64             ; v0 = localCollideInfo on stack
    ".word 0x006c1821\n"  // addu    v1, v1, t4
    // Loop body: transform each sphere and collideInfo to sub-shape local space
    "loop_body:\n"
    // Transform sphere center: subtract sub-shape origin, rotate by inverse basis
    ".word 0xc4ec000c\n"  // lwc1    $f12, 12(a3)           ; f12 = sphere.radius
    ".word 0x000a5080\n"  // sll     t2, t2, 2
    ".word 0x016a5021\n"  // addu    t2, t3, t2
    ".word 0x8d4a0000\n"  // lw      t2, 0(t2)              ; t2 = subShape
    ".word 0x254a0010\n"  // addiu   t2, t2, 16             ; t2 = &subShape->localOCS
    ".word 0xd8e60000\n"  // lv.q    C120, 0x0($a3)         ; sphere center
    ".word 0xd9400000\n"  // lv.q    C000, 0x0($t2)
    ".word 0xd9410010\n"  // lv.q    C010, 0x10($t2)
    ".word 0xd9420020\n"  // lv.q    C020, 0x20($t2)
    ".word 0xd9430030\n"  // lv.q    C030, 0x30($t2)
    ".word 0x60838606\n"  // vsub.t  C120, C120, C030       ; center -= origin
    ".word 0xf1068007\n"  // vtfm3.t C130, M000, C120       ; rotate to local
    ".word 0xf8c70000\n"  // sv.q    C130, 0x0($a2)         ; store local sphere xyz
    ".word 0xe4cc000c\n"  // swc1    $f12, 12(a2)           ; store radius
    // Transform collideInfo[0] (position)
    ".word 0xd8660000\n"  // lv.q    C120, 0x0($v1)
    ".word 0x950a0008\n"  // lhu     t2, 8(t0)              ; reload subShapeIndex
    ".word 0x8c8b0080\n"  // lw      t3, 128(a0)            ; reload mSubShapes
    ".word 0x000a5080\n"  // sll     t2, t2, 2
    ".word 0x016a5021\n"  // addu    t2, t3, t2
    ".word 0x8d4a0000\n"  // lw      t2, 0(t2)
    ".word 0x254a0010\n"  // addiu   t2, t2, 16
    ".word 0xd9400000\n"  // lv.q    C000, 0x0($t2)
    ".word 0xd9410010\n"  // lv.q    C010, 0x10($t2)
    ".word 0xd9420020\n"  // lv.q    C020, 0x20($t2)
    ".word 0xd9430030\n"  // lv.q    C030, 0x30($t2)
    ".word 0x60838606\n"  // vsub.t  C120, C120, C030
    ".word 0xf1068007\n"  // vtfm3.t C130, M000, C120
    ".word 0xf8470000\n"  // sv.q    C130, 0x0($v0)         ; localCollide[0]
    // Transform collideInfo[1] (velocity)
    ".word 0xd8660010\n"  // lv.q    C120, 0x10($v1)
    ".word 0xd9400000\n"  // lv.q    C000, 0x0($t2)
    ".word 0xd9410010\n"  // lv.q    C010, 0x10($t2)
    ".word 0xd9420020\n"  // lv.q    C020, 0x20($t2)
    ".word 0xd9430030\n"  // lv.q    C030, 0x30($t2)
    ".word 0x60838606\n"  // vsub.t  C120, C120, C030
    ".word 0xf1068007\n"  // vtfm3.t C130, M000, C120
    ".word 0xf8470010\n"  // sv.q    C130, 0x10($v0)        ; localCollide[1]
    // Transform collideInfo[2] (direction — no subtract, just rotate)
    ".word 0xd8660020\n"  // lv.q    C120, 0x20($v1)
    ".word 0xd9400000\n"  // lv.q    C000, 0x0($t2)
    ".word 0xd9410010\n"  // lv.q    C010, 0x10($t2)
    ".word 0xd9420020\n"  // lv.q    C020, 0x20($t2)
    ".word 0xd9430030\n"  // lv.q    C030, 0x30($t2)
    ".word 0xf1068007\n"  // vtfm3.t C130, M000, C120       ; no vsub here
    ".word 0xf8470020\n"  // sv.q    C130, 0x20($v0)        ; localCollide[2]
    // Copy collideInfo scalar (float at offset 0x30)
    ".word 0xc46c0030\n"  // lwc1    $f12, 48(v1)
    ".word 0xe44c0030\n"  // swc1    $f12, 48(v0)
    // Loop increment
    ".word 0x25290001\n"  // addiu   t1, t1, 1
    ".word 0x24c60010\n"  // addiu   a2, a2, 16             ; next local sphere
    ".word 0x24e70010\n"  // addiu   a3, a3, 16             ; next input sphere
    ".word 0x24420040\n"  // addiu   v0, v0, 64             ; next local collide
    ".word 0x24630040\n"  // addiu   v1, v1, 64             ; next input collide
    ".word 0x0125602a\n"  // slt     t4, t1, a1
    ".word 0x950a0008\n"  // lhu     t2, 8(t0)
    ".word 0x1580ffcb\n"  // bnez    t4, loop_body
    ".word 0x8c8b0080\n"  // lw      t3, 128(a0)            ; delay: reload mSubShapes
    // Post-loop: compute combined OCS for the sub-shape
    "past_loop:\n"
    ".word 0x000a3080\n"  // sll     a2, t2, 2
    ".word 0x01663021\n"  // addu    a2, t3, a2
    ".word 0x8cc60000\n"  // lw      a2, 0(a2)              ; a2 = subShape
    ".word 0x24c60010\n"  // addiu   a2, a2, 16             ; a2 = &subShape->localOCS
    // Load sub-shape's local transform
    ".word 0xd8c60000\n"  // lv.q    C120, 0x0($a2)
    ".word 0xd8c70010\n"  // lv.q    C130, 0x10($a2)
    ".word 0xd8c80020\n"  // lv.q    C200, 0x20($a2)
    ".word 0xd8c90030\n"  // lv.q    C210, 0x30($a2)
    // Load saved basis from stack
    ".word 0xdba00000\n"  // lv.q    C000, 0x0($sp)
    ".word 0xdba10010\n"  // lv.q    C010, 0x10($sp)
    ".word 0xdba20020\n"  // lv.q    C020, 0x20($sp)
    ".word 0xdba30030\n"  // lv.q    C030, 0x30($sp)
    // Matrix multiply: combined = saved_basis * subLocal
    ".word 0xf106a00a\n"  // vtfm3.t C220, E000, C120      ; row0
    ".word 0xf107a006\n"  // vtfm3.t C120, E000, C130      ; row1
    ".word 0xf108a007\n"  // vtfm3.t C130, E000, C200      ; row2
    ".word 0xf189a008\n"  // vhtfm4.q C200, E000, C210     ; position
    // Store combined OCS to stack temp area and update collector
    ".word 0xfbaa0180\n"  // sv.q    C220, 0x180($sp)
    ".word 0xfba60190\n"  // sv.q    C120, 0x190($sp)
    ".word 0xfba701a0\n"  // sv.q    C130, 0x1A0($sp)
    ".word 0xfba801b0\n"  // sv.q    C200, 0x1B0($sp)
    // Write combined OCS to collector (s0+0x20..0x50)
    ".word 0xfa080050\n"  // sv.q    C200, 0x50($s0)        ; position
    ".word 0xdba60180\n"  // lv.q    C120, 0x180($sp)
    ".word 0xfa060020\n"  // sv.q    C120, 0x20($s0)        ; row0
    ".word 0xdba60190\n"  // lv.q    C120, 0x190($sp)
    ".word 0xfa060030\n"  // sv.q    C120, 0x30($s0)        ; row1
    ".word 0xdba601a0\n"  // lv.q    C120, 0x1A0($sp)
    ".word 0xfa060040\n"  // sv.q    C120, 0x40($s0)        ; row2
    // Virtual dispatch: subShape->GetSweptContacts(localSpheres, localCollide, ...)
    ".word 0x95060008\n"  // lhu     a2, 8(t0)              ; subShapeIndex
    ".word 0x8c840080\n"  // lw      a0, 128(a0)            ; mSubShapes
    ".word 0x00063080\n"  // sll     a2, a2, 2
    ".word 0x00862021\n"  // addu    a0, a0, a2
    ".word 0x8c840000\n"  // lw      a0, 0(a0)              ; subShape
    ".word 0x8c860004\n"  // lw      a2, 4(a0)              ; vtable
    ".word 0x24c900a8\n"  // addiu   t1, a2, 168            ; &vtable[GetSweptContacts]
    ".word 0x85260000\n"  // lh      a2, 0(t1)              ; this-adjust
    ".word 0x00862021\n"  // addu    a0, a0, a2             ; adjust this
    ".word 0x27a60140\n"  // addiu   a2, sp, 320            ; localSpheres
    ".word 0x27a70040\n"  // addiu   a3, sp, 64             ; localCollideInfo
    ".word 0x8d2a0004\n"  // lw      t2, 4(t1)              ; function ptr
    ".word 0x0140f809\n"  // jalr    t2
    ".word 0x02004825\n"  // move    t1, s0                 ; delay: t1 = collector
    // Check result and set s1 = 1 if contact found
    ".word 0xdba60030\n"  // lv.q    C120, 0x30($sp)        ; reload saved position
    ".word 0x54400001\n"  // bnezl   v0, 1f
    ".word 0x34110001\n"  // li      s1, 1                   ; likely delay: result = 1
    "1:\n"
    // Restore original collector basis matrix
    ".word 0xfa060050\n"  // sv.q    C120, 0x50($s0)        ; restore position
    ".word 0xdba60000\n"  // lv.q    C120, 0x0($sp)
    ".word 0xfa060020\n"  // sv.q    C120, 0x20($s0)        ; restore row0
    ".word 0xdba60010\n"  // lv.q    C120, 0x10($sp)
    ".word 0xfa060030\n"  // sv.q    C120, 0x30($s0)        ; restore row1
    ".word 0xdba60020\n"  // lv.q    C120, 0x20($sp)
    ".word 0xfa060040\n"  // sv.q    C120, 0x40($s0)        ; restore row2
    // Epilogue: return s1
    ".word 0x02201025\n"  // move    v0, s1
    ".word 0x8fb001c0\n"  // lw      s0, 448(sp)
    ".word 0x8fb101c4\n"  // lw      s1, 452(sp)
    ".word 0x8fbf01c8\n"  // lw      ra, 456(sp)
    ".word 0x03e00008\n"  // jr      ra
    ".word 0x27bd01d0\n"  // addiu   sp, sp, 464            ; delay: dealloc stack
    ".set reorder\n"
);
