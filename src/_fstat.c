__asm__(
    ".globl _fstat\n"
    "_fstat:\n"
    "li $v1, 0x2000\n"
    "move $v0, $zero\n"
    "jr $ra\n"
    "sw $v1, 4($a1)\n"
);
