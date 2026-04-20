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

float fabsf(float x) {
    union { float f; int i; } u;
    u.f = x;
    u.i &= 0x7FFFFFFF;
    return u.f;
}

void *cMemPoolPlatform_cMemPoolPlatform(int *self) {
    self[0] = 0;
    self[1] = -1;
    self[2] = 0;
    return self;
}

void *eDynamicVertChunk_eDynamicVertChunk(int *self) {
    self[0] = 0;
    self[1] = 0;
    self[2] = 0;
    self[3] = 0;
    return self;
}

void *eViewport_eViewport(int *self, int a, int b, int c, int d) {
    self[0] = a;
    self[1] = b;
    self[2] = c;
    self[3] = d;
    return self;
}

int eCompoundShape_GetNumSubShapes(const int *self) {
    int count = 0;
    const int *arr;
    __asm__ volatile("" ::: "memory");
    arr = (const int *)self[32];
    if (arr != 0) {
        count = arr[-1];
    }
    return count;
}

int strlen(const char *s) {
    const char *start = s;
    while (*s != 0) s++;
    return s - start;
}

int eDynamicAABBTreeNode_NeedsSplit(const void *self) {
    if (*((const unsigned char *)self + 0x4E)) {
        return 0;
    }
    return *((const unsigned short *)((const char *)self + 0x4C)) >= 4;
}

void eDynamicAABBTreeNode_UnsplitNode(int *self) {
    int i = 0;
    int *p = self;
    do {
        p[15] = 0;
        i++;
        p++;
    } while (i < 3);
    *((char *)self + 0x4E) = 0;
}

int gcBoolSet_GetSize(const void *self) {
    int *p = *(int **)((const char *)self + 12);
    if (p == 0) return 0;
    return p[-1] & 0x3FFFFFFF;
}

int gcFloatSet_GetSize(const void *self) {
    int *p = *(int **)((const char *)self + 12);
    if (p == 0) return 0;
    return p[-1] & 0x3FFFFFFF;
}

float copysignf(float x, float y) {
    union { float f; int i; } ux, uy;
    ux.f = x;
    uy.f = y;
    ux.i = (ux.i & 0x7FFFFFFF) | (uy.i & 0x80000000);
    return ux.f;
}

void *eCameraEffectMgrEntry_eCameraEffectMgrEntry(int *self) {
    *(unsigned char *)self = 0;
    *((float *)self + 1) = 1.0f;
    *((float *)self + 2) = 0.0f;
    self[3] = 0;
    self[4] = 0;
    return self;
}

void *mWaveGen_mWaveGen(int *self) {
    self[4] = 0;
    ((float *)self)[0] = 0.0f;
    ((float *)self)[1] = 1.0f;
    ((float *)self)[2] = 0.0f;
    ((float *)self)[3] = 1.0f;
    return self;
}

extern int cLanguage__s_pInstance;
unsigned char cLanguage_IsTextLanguageSupported(int lang) {
    int v = cLanguage__s_pInstance;
    int found = 0;
    if (v != 0 && *(unsigned char *)(lang + v + 520) != 0) {
        found = 1;
    }
    return (unsigned char)found;
}

typedef struct {
    int eAudioFader_unk0;
    float target;
    int eAudioFader_unk8;
    int eAudioFader_unkC;
} eAudioFader_t;

typedef struct {
    int eAudioGroup_g0;
    int eAudioGroup_g4;
    eAudioFader_t faders[3];
} eAudioGroup_t;

extern eAudioGroup_t eAudio__s_groups[];
float eAudio_GetGroupVolume(int group, int fader) {
    eAudioGroup_t *gp = &eAudio__s_groups[group];
    eAudioFader_t *f = &gp->faders[fader];
    return f->target;
}
