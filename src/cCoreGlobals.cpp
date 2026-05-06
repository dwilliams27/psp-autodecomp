int cGetBuildId(void) {
    return *(int *)0x385EC;
}

int cGetBuildDateTime(void) {
    return *(int *)0x37BC00;
}

int cGetBuildInfo(void) {
    return *(int *)0x37C0A0 + 13;
}

void cRestartApp(bool restart) {
    *(unsigned char *)0x37BC0C = restart;
}

void cRestartedApp(bool restarted) {
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

void cSetAppQuickStarted(bool quickStarted) {
    *(unsigned char *)0x37BC0E = quickStarted;
}

void cSetCommandLine(const char *cmd) {
    *(const char **)0x37BC08 = cmd;
}

int cGetConfigTerritory(void) {
    return 0;
}

void cQuitApp(void) {
    *(unsigned char *)0x37BC18 = 1;
}

void cBuildPath(const char *) {
}

int cGetCurrentPlatform(void) {
    return 4;
}

int cGetFPUControl(void) {
    int result;
    __asm__ volatile("cfc1 %0, $31" : "=r"(result));
    return result;
}

struct mem_block_t {
    mem_block_t *next;
    int size;
};

struct mem_pool_t {
    char _pad[12];
    int used;
};

extern mem_block_t *g_free_block_head;
extern mem_pool_t *g_block_pool;

void free_in_mem_block(void *) {
    mem_block_t *blk = g_free_block_head;
    mem_pool_t *pool = g_block_pool;
    g_free_block_head = blk->next;
    pool->used = pool->used - blk->size - 16;
}

class eCollisionInfo;
class eContactCollector;
class mSphere;

class eStaticModel {
public:
    void GetEmbedContacts(const eCollisionInfo &, int, const mSphere *,
                          eContactCollector *) const;
};

void eStaticModel::GetEmbedContacts(const eCollisionInfo &info, int idx,
                                    const mSphere *sphere,
                                    eContactCollector *collector) const {
    char *shape = ((char **)&info)[1];
    int *entry = (int *)(((char **)shape)[1] + 0xB0);
    ((void (*)(char *, int, const mSphere *, const eCollisionInfo &,
               eContactCollector *))entry[1])
        (shape + *(short *)entry, idx, sphere, info, collector);
}
