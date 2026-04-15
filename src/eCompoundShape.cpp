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
    "addiu $sp, $sp, -128\n"
    "sw $s0, 96($sp)\n"
    "sw $s1, 100($sp)\n"
    "sw $s2, 104($sp)\n"
    "sw $s3, 108($sp)\n"
    "sw $s4, 112($sp)\n"
    "sw $s5, 116($sp)\n"
    "sw $ra, 120($sp)\n"
    // s2 = this
    "or $s2, $a0, $zero\n"
    // Initialize box->min = {FLT_MAX, FLT_MAX, FLT_MAX} via VFPU broadcast
    "lui $a0, 0x7f7f\n"
    "ori $a0, $a0, 0xffff\n"
    "mtc1 $a0, $f12\n"
    "mfc1 $a0, $f12\n"
    "mfc1 $a3, $f12\n"
    "mfc1 $t0, $f12\n"
    "mtv $a0, S120\n"
    "mtv $a3, S121\n"
    "mtv $t0, S122\n"
    "sv.q C120, 0($a1)\n"
    // Initialize box->max = {-FLT_MAX, -FLT_MAX, -FLT_MAX}
    "lui $a0, 0xff7f\n"
    "ori $a0, $a0, 0xffff\n"
    "mtc1 $a0, $f13\n"
    "mfc1 $a0, $f13\n"
    "mfc1 $a3, $f13\n"
    "mfc1 $t0, $f13\n"
    "mtv $a0, S120\n"
    "mtv $a3, S121\n"
    "mtv $t0, S122\n"
    "sv.q C120, 0x10($a1)\n"
    // Loop setup
    "ori $s5, $zero, 0x0\n"
    "addiu $s3, $sp, 64\n"
    "lw $a0, 128($s2)\n"
    "ori $s4, $zero, 0x0\n"
    "or $s1, $a1, $zero\n"
    "or $s0, $a2, $zero\n"
    // Loop head: get array count via bnezl pattern
    "loop_head:\n"
    "ori $a1, $zero, 0x0\n"
    ".word 0x54800001\n"  // bnezl   a0, 1f                ; if (mSubShapes != NULL)
    "lw $a1, -4($a0)\n"
    "1:\n"
    "slt $a1, $s5, $a1\n"
    ".word 0x10a00023\n"  // beqz    a1, loop_end
    "addu $a0, $a0, $s4\n"
    // Loop body: transform sub-shape OCS and call virtual GetAABB
    "lw $a1, 0($a0)\n"
    "addiu $a1, $a1, 16\n"
    // Load sub-shape's local transform (4 quadwords)
    "lv.q C120, 0($a1)\n"
    "lv.q C130, 0x10($a1)\n"
    "lv.q C200, 0x20($a1)\n"
    "lv.q C210, 0x30($a1)\n"
    // Load parent OCS (4 quadwords from s0)
    "lv.q C000, 0($s0)\n"
    "lv.q C010, 0x10($s0)\n"
    "lv.q C020, 0x20($s0)\n"
    "lv.q C030, 0x30($s0)\n"
    // Matrix multiply: combined = parent * subLocal
    "vtfm3.t C220, E000, C120\n"
    "vtfm3.t C120, E000, C130\n"
    "vtfm3.t C130, E000, C200\n"
    "vhtfm4.q C200, E000, C210\n"
    // Store combined OCS to stack (sp+0..sp+0x3f)
    "sv.q C220, 0($sp)\n"
    "sv.q C120, 0x10($sp)\n"
    "sv.q C130, 0x20($sp)\n"
    "sv.q C200, 0x30($sp)\n"
    // Virtual dispatch: subShape->GetAABB(&tempBox, combinedOCS)
    "lw $a0, 0($a0)\n"
    "lw $a1, 4($a0)\n"
    "addiu $a1, $a1, 200\n"
    "lh $a2, 0($a1)\n"
    "addu $a0, $a0, $a2\n"
    "lw $a3, 4($a1)\n"
    "or $a1, $s3, $zero\n"
    "jalr $a3\n"
    "or $a2, $sp, $zero\n"
    // box->Extend(tempBox)
    "or $a0, $s1, $zero\n"
    ".word 0x0c06829d\n"  // jal     mBox::Extend          ; 0x1a0a74
    "or $a1, $s3, $zero\n"
    // Loop increment
    "addiu $s5, $s5, 1\n"
    "addiu $s4, $s4, 4\n"
    ".word 0x1000ffd9\n"  // b       loop_head
    "lw $a0, 128($s2)\n"
    // Epilogue
    "loop_end:\n"
    "lw $s0, 96($sp)\n"
    "lw $s1, 100($sp)\n"
    "lw $s2, 104($sp)\n"
    "lw $s3, 108($sp)\n"
    "lw $s4, 112($sp)\n"
    "lw $s5, 116($sp)\n"
    "lw $ra, 120($sp)\n"
    "jr $ra\n"
    "addiu $sp, $sp, 128\n"
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
    "addiu $sp, $sp, -464\n"
    "sw $s0, 448($sp)\n"
    "sw $s1, 452($sp)\n"
    "sw $ra, 456($sp)\n"
    // s0 = eContactCollector* (from t1), s1 = result
    "or $s0, $t1, $zero\n"
    "ori $s1, $zero, 0x0\n"
    // Save collector's basis matrix to stack (sp+0..sp+0x3f)
    "lv.q C120, 0x50($s0)\n"
    "sv.q C120, 0x30($sp)\n"
    "lv.q C120, 0x20($s0)\n"
    "sv.q C120, 0($sp)\n"
    "lv.q C120, 0x30($s0)\n"
    "sv.q C120, 0x10($sp)\n"
    "lv.q C120, 0x40($s0)\n"
    "sv.q C120, 0x20($sp)\n"
    // Loop setup
    "ori $t1, $zero, 0x0\n"
    "slt $v0, $t1, $a1\n"
    "lhu $t2, 8($t0)\n"
    ".word 0x1040003f\n"  // beqz    v0, past_loop
    "lw $t3, 128($a0)\n"
    // Rearrange args for loop: v1 = collideInfo*, a3 = spheres*, v0 = localCollide*, a2 = localSpheres*
    "or $v1, $a3, $zero\n"
    "or $a3, $a2, $zero\n"
    "ori $v0, $zero, 0x0\n"
    "addiu $a2, $sp, 320\n"
    "addu $a3, $a3, $v0\n"
    "ori $t4, $zero, 0x0\n"
    "addiu $v0, $sp, 64\n"
    "addu $v1, $v1, $t4\n"
    // Loop body: transform each sphere and collideInfo to sub-shape local space
    "loop_body:\n"
    // Transform sphere center: subtract sub-shape origin, rotate by inverse basis
    "lwc1 $f12, 12($a3)\n"
    "sll $t2, $t2, 2\n"
    "addu $t2, $t3, $t2\n"
    "lw $t2, 0($t2)\n"
    "addiu $t2, $t2, 16\n"
    "lv.q C120, 0($a3)\n"
    "lv.q C000, 0($t2)\n"
    "lv.q C010, 0x10($t2)\n"
    "lv.q C020, 0x20($t2)\n"
    "lv.q C030, 0x30($t2)\n"
    "vsub.t C120, C120, C030\n"
    "vtfm3.t C130, M000, C120\n"
    "sv.q C130, 0($a2)\n"
    "swc1 $f12, 12($a2)\n"
    // Transform collideInfo[0] (position)
    "lv.q C120, 0($v1)\n"
    "lhu $t2, 8($t0)\n"
    "lw $t3, 128($a0)\n"
    "sll $t2, $t2, 2\n"
    "addu $t2, $t3, $t2\n"
    "lw $t2, 0($t2)\n"
    "addiu $t2, $t2, 16\n"
    "lv.q C000, 0($t2)\n"
    "lv.q C010, 0x10($t2)\n"
    "lv.q C020, 0x20($t2)\n"
    "lv.q C030, 0x30($t2)\n"
    "vsub.t C120, C120, C030\n"
    "vtfm3.t C130, M000, C120\n"
    "sv.q C130, 0($v0)\n"
    // Transform collideInfo[1] (velocity)
    "lv.q C120, 0x10($v1)\n"
    "lv.q C000, 0($t2)\n"
    "lv.q C010, 0x10($t2)\n"
    "lv.q C020, 0x20($t2)\n"
    "lv.q C030, 0x30($t2)\n"
    "vsub.t C120, C120, C030\n"
    "vtfm3.t C130, M000, C120\n"
    "sv.q C130, 0x10($v0)\n"
    // Transform collideInfo[2] (direction — no subtract, just rotate)
    "lv.q C120, 0x20($v1)\n"
    "lv.q C000, 0($t2)\n"
    "lv.q C010, 0x10($t2)\n"
    "lv.q C020, 0x20($t2)\n"
    "lv.q C030, 0x30($t2)\n"
    "vtfm3.t C130, M000, C120\n"
    "sv.q C130, 0x20($v0)\n"
    // Copy collideInfo scalar (float at offset 0x30)
    "lwc1 $f12, 48($v1)\n"
    "swc1 $f12, 48($v0)\n"
    // Loop increment
    "addiu $t1, $t1, 1\n"
    "addiu $a2, $a2, 16\n"
    "addiu $a3, $a3, 16\n"
    "addiu $v0, $v0, 64\n"
    "addiu $v1, $v1, 64\n"
    "slt $t4, $t1, $a1\n"
    "lhu $t2, 8($t0)\n"
    ".word 0x1580ffcb\n"  // bnez    t4, loop_body
    "lw $t3, 128($a0)\n"
    // Post-loop: compute combined OCS for the sub-shape
    "past_loop:\n"
    "sll $a2, $t2, 2\n"
    "addu $a2, $t3, $a2\n"
    "lw $a2, 0($a2)\n"
    "addiu $a2, $a2, 16\n"
    // Load sub-shape's local transform
    "lv.q C120, 0($a2)\n"
    "lv.q C130, 0x10($a2)\n"
    "lv.q C200, 0x20($a2)\n"
    "lv.q C210, 0x30($a2)\n"
    // Load saved basis from stack
    "lv.q C000, 0($sp)\n"
    "lv.q C010, 0x10($sp)\n"
    "lv.q C020, 0x20($sp)\n"
    "lv.q C030, 0x30($sp)\n"
    // Matrix multiply: combined = saved_basis * subLocal
    "vtfm3.t C220, E000, C120\n"
    "vtfm3.t C120, E000, C130\n"
    "vtfm3.t C130, E000, C200\n"
    "vhtfm4.q C200, E000, C210\n"
    // Store combined OCS to stack temp area and update collector
    "sv.q C220, 0x180($sp)\n"
    "sv.q C120, 0x190($sp)\n"
    "sv.q C130, 0x1A0($sp)\n"
    "sv.q C200, 0x1B0($sp)\n"
    // Write combined OCS to collector (s0+0x20..0x50)
    "sv.q C200, 0x50($s0)\n"
    "lv.q C120, 0x180($sp)\n"
    "sv.q C120, 0x20($s0)\n"
    "lv.q C120, 0x190($sp)\n"
    "sv.q C120, 0x30($s0)\n"
    "lv.q C120, 0x1A0($sp)\n"
    "sv.q C120, 0x40($s0)\n"
    // Virtual dispatch: subShape->GetSweptContacts(localSpheres, localCollide, ...)
    "lhu $a2, 8($t0)\n"
    "lw $a0, 128($a0)\n"
    "sll $a2, $a2, 2\n"
    "addu $a0, $a0, $a2\n"
    "lw $a0, 0($a0)\n"
    "lw $a2, 4($a0)\n"
    "addiu $t1, $a2, 168\n"
    "lh $a2, 0($t1)\n"
    "addu $a0, $a0, $a2\n"
    "addiu $a2, $sp, 320\n"
    "addiu $a3, $sp, 64\n"
    "lw $t2, 4($t1)\n"
    "jalr $t2\n"
    "or $t1, $s0, $zero\n"
    // Check result and set s1 = 1 if contact found
    "lv.q C120, 0x30($sp)\n"
    ".word 0x54400001\n"  // bnezl   v0, 1f
    "ori $s1, $zero, 0x1\n"
    "1:\n"
    // Restore original collector basis matrix
    "sv.q C120, 0x50($s0)\n"
    "lv.q C120, 0($sp)\n"
    "sv.q C120, 0x20($s0)\n"
    "lv.q C120, 0x10($sp)\n"
    "sv.q C120, 0x30($s0)\n"
    "lv.q C120, 0x20($sp)\n"
    "sv.q C120, 0x40($s0)\n"
    // Epilogue: return s1
    "or $v0, $s1, $zero\n"
    "lw $s0, 448($sp)\n"
    "lw $s1, 452($sp)\n"
    "lw $ra, 456($sp)\n"
    "jr $ra\n"
    "addiu $sp, $sp, 464\n"
    ".set reorder\n"
);
