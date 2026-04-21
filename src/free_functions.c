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

struct __thrown_obj_t {
    int _0;
    int type;
    int _8;
    unsigned char flag;
    char _padC[3];
    int data;
};
extern struct __thrown_obj_t *volatile g_thrown_obj;
void __type_of_thrown_object(int *out_type, unsigned char *out_flag, int *out_data) {
    *out_type = g_thrown_obj->type;
    *out_flag = g_thrown_obj->flag;
    *out_data = g_thrown_obj->data;
}

struct gcDoBreak_root_t {
    char _0[16];
    int *unk10;
};
extern struct gcDoBreak_root_t *g_gcDoBreak_root;
float gcDoBreak_Evaluate(const void *self) {
    int *p = 0;
    struct gcDoBreak_root_t *r = g_gcDoBreak_root;
    if (r != 0) {
        p = r->unk10;
    }
    *p |= 2;
    return 1.0f;
}

struct gcDoEval_t {
    char _0[16];
    int unk10;
    int _14;
    int unk18;
};
int gcDoEvaluation_GetBranch(const struct gcDoEval_t *self, int idx) {
    int v = 0;
    if (idx == 0) return self->unk10;
    if (idx == 1) v = self->unk18;
    return v;
}

int gcDoSwitch_GetBranch(const struct gcDoEval_t *self, int idx) {
    int v = 0;
    if (idx == 0) return self->unk10;
    if (idx == 1) v = self->unk18;
    return v;
}

typedef void (*stop_fn_t)(void);
extern stop_fn_t _stop_hook_ptr;
int _stop(void) {
    if (&_stop_hook_ptr != 0) {
        _stop_hook_ptr();
    }
    return 0;
}

struct mem_block_t {
    struct mem_block_t *next;
    int size;
};
struct mem_pool_t {
    char _pad[12];
    int used;
};
extern struct mem_block_t *g_free_block_head;
extern struct mem_pool_t *g_block_pool;
void free_in_mem_block(void *p) {
    struct mem_block_t *blk = g_free_block_head;
    struct mem_pool_t *pool = g_block_pool;
    g_free_block_head = blk->next;
    pool->used = pool->used - blk->size - 16;
}

void *gcMsgCinematicEnded_New(void *buf) {
    int cursor = *(int *)((char *)buf + 1200) + 4;
    *(int *)((char *)buf + 1200) = cursor;
    void *obj = (char *)buf + cursor;
    void *result = 0;
    if (obj != 0) {
        *(int *)obj = 0x388FF0;
        result = obj;
    }
    return result;
}

void *gcMsgRequestLoadedState_New(void *buf) {
    int cursor = *(int *)((char *)buf + 1200) + 4;
    *(int *)((char *)buf + 1200) = cursor;
    void *obj = (char *)buf + cursor;
    void *result = 0;
    if (obj != 0) {
        *(int *)obj = 0x389050;
        result = obj;
    }
    return result;
}

void *gcMsgCheckSynchronization_New(void *buf) {
    int cursor = *(int *)((char *)buf + 1200) + 4;
    *(int *)((char *)buf + 1200) = cursor;
    void *obj = (char *)buf + cursor;
    void *result = 0;
    if (obj != 0) {
        *(int *)obj = 0x38AD00;
        result = obj;
    }
    return result;
}

unsigned char ePlatformInput_AnyButtonPressed(unsigned char *state) {
    int i = 4;
    state += 0x60;
    do {
        if (state[4] > 0) {
            return 0xFF;
        }
        i++;
        state += 24;
    } while (i < 41);
    return 0;
}

void *memset(void *dst, int val, unsigned int n) {
    unsigned char *p = (unsigned char *)dst;
    while (n-- != 0) {
        *p++ = (unsigned char)val;
    }
    return dst;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

struct _Bigint {
    struct _Bigint *_next;
    int _k;
};

struct _Balloc_reent {
    char _pad[0x4C];
    struct _Bigint **_freelist;
};

void _Bfree(struct _Balloc_reent *r, struct _Bigint *b) {
    if (b != 0) {
        b->_next = r->_freelist[b->_k];
        r->_freelist[b->_k] = b;
    }
}

int gcTableColumnShort_Compare(const void *self, int i, int j) {
    short *arr = *(short **)((const char *)self + 8);
    int result = -1;
    if (arr[i] >= arr[j]) {
        result = arr[j] < arr[i];
    }
    return result;
}

/* eTexture::GetFullTexCoords lives in src/eTexture_GetFullTexCoords.cpp
   (SNC C does not accept V4SF aggregate initializers; C++ is required). */
