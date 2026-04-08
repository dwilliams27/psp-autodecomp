int cGetBuildId(void) {
    return *(int *)0x385EC;
}

int cGetBuildDateTime(void) {
    return *(int *)0x37BC00;
}

unsigned char cIsAppRestarting(void) {
    return *(unsigned char *)0x37BC0C;
}

unsigned char cIsAppRestarted(void) {
    return *(unsigned char *)0x37BC0D;
}

unsigned char cIsAppQuickStarted(void) {
    return *(unsigned char *)0x37BC0E;
}

void cBuildPath(const char *path) {
}

int cGetCurrentPlatform(void) {
    return 4;
}

void cDynamicMemAllocator_LogAllocations(const void *self) {
}

int cFileSystem_Create(const char *name) {
    return 0;
}

int cFileSystem_Write(void *handle, const void *data, unsigned int size) {
    return 0;
}

void _fini(void) {
}

void cBase_SetDirty(void *self) {
}

int cBase_IsEditable(const void *self) {
    return 0;
}

void cBase_Reset(void *self, void *pool, int reset) {
}

void cBase_GetName(const void *self, char *name) {
    *name = 0;
}

void cSetCommandLine(const char *cmd) {
    *(const char **)0x37BC08 = cmd;
}

int cGetFPUControl(void) {
    int result;
    __asm__ volatile("cfc1 %0, $31" : "=r"(result));
    return result;
}
