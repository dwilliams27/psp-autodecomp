int cGetBuildId(void) {
    return *(int *)0x385EC;
}

int cGetBuildDateTime(void) {
    return *(int *)0x37BC00;
}

int cGetBuildInfo(void) {
    return *(int *)0x37C0A0 + 13;
}

void cRestartApp(_Bool restart) {
    *(unsigned char *)0x37BC0C = restart;
}

void cRestartedApp(_Bool restarted) {
    *(unsigned char *)0x37BC0D = restarted;
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

void cSetAppQuickStarted(_Bool quickStarted) {
    *(unsigned char *)0x37BC0E = quickStarted;
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

void cQuitApp(void) {
    *(unsigned char *)0x37BC18 = 1;
}

void cSetCommandLine(const char *cmd) {
    *(const char **)0x37BC08 = cmd;
}

int cGetFPUControl(void) {
    int result;
    __asm__ volatile("cfc1 %0, $31" : "=r"(result));
    return result;
}
