unsigned int cSetFPUControl(unsigned int ctrl) {
    unsigned int old;
    __asm__ volatile("cfc1 %0, $31" : "=r"(old));
    __asm__ volatile("ctc1 $0, $31");
    __asm__ volatile("nop");
    __asm__ volatile("ctc1 %0, $31" : : "r"(ctrl));
    return old;
}
