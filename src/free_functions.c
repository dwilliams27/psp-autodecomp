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

extern int *__exception_ptr;
void __exception_caught(void) {
    __exception_ptr = *__exception_ptr;
}

void gcAction_SetNext(int *self, int next) {
    self[2] = next | (self[2] & 3);
}

extern const int cType__vtable[];
void *cType_cType(void *self) {
    *(const int **)((char *)self + 44) = cType__vtable;
    return self;
}

int eSoundData_GetNumExternalDependencies(const void *self) {
    return ((_Bool)(*(const short *)((const char *)self + 28) == 0)) == 0;
}

void *eInput_eButtonState_eButtonState(int *self) {
    self[3] = 0;
    self[4] = 0;
    self[5] = 0;
    return self;
}
