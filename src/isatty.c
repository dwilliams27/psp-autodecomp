__asm__(".text\n"
".globl isatty\n"
"isatty:\n"
"jr $ra\n"
"addiu $v0, $0, 1\n");
