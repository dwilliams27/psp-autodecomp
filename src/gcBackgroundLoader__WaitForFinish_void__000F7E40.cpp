// ODR-WARNING: split-TU local redeclaration of gcBackgroundLoader for isolated
// matching of gcBackgroundLoader::WaitForFinish. Do not link with
// src/gcBackgroundLoader.cpp.
//
// gcBackgroundLoader::WaitForFinish(void) @ 0x000f7e40, gcAll_psp.obj
// Poll loop modeled on matched sibling cThread::WaitForFinish (0x00001e44):
// while the loader is still running, pump the OS, then briefly bump our thread
// to priority 0x6f and rotate the ready queue so the loader thread runs, then
// restore our previous priority. After the loop, hand off to the active
// gcMap's foreground wait if one is registered.

extern "C" {
    int sceKernelChangeThreadPriority(int thid, int priority);
    int sceKernelRotateThreadReadyQueue(int priority);
    int sceKernelGetThreadId(void);
    int sceKernelGetThreadCurrentPriority(void);
}

extern int cUpdateOS(void);

class gcMap {
public:
    void WaitForBackgroundLoad(void);
};

class gcBackgroundLoader {
public:
    int m_field0;
    unsigned int m_flags;

    void WaitForFinish(void);
    bool IsRunning(void) const { return (m_flags & 0x1000) != 0; }
};

void gcBackgroundLoader::WaitForFinish(void) {
    int slot[3];
    __asm__ volatile("" ::: "memory");
    while (IsRunning()) {
        cUpdateOS();
        slot[1] = (0x6F);
        slot[2] = sceKernelGetThreadCurrentPriority();
        slot[0] = sceKernelGetThreadId();
        __asm__ volatile("" ::: "memory");
        sceKernelChangeThreadPriority(slot[0], 0x6F);
        sceKernelRotateThreadReadyQueue(slot[1]);
        sceKernelChangeThreadPriority(slot[0], slot[2]);
    }

    gcMap *map = *(gcMap **)0x37D7FC;
    if (map != 0) {
        map->WaitForBackgroundLoad();
    }
}
