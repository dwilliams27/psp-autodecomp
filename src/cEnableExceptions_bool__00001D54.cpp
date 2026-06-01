// cEnableExceptions(bool) @ 0x00001d54, cAll_psp.obj
// symbol: __0FRcEnableExceptionsb

unsigned int cGetFPUControl(void);
unsigned int cSetFPUControl(unsigned int);

void cEnableExceptions(bool enable) {
    unsigned int ctrl = cGetFPUControl();
    if (enable) {
        cSetFPUControl(ctrl | 0x700);
    } else {
        cSetFPUControl(ctrl & ~0x700);
    }
}
