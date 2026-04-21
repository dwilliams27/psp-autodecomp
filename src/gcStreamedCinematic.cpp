// gcStreamedCinematic — streaming cinematic playback state

#define NULL 0

class cFile;
class cMemPool;
class cBase;

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void End(void);
};

class cFactory {
public:
    void Write(cFile &) const;
    void WriteGroups(cWriteBlock &) const;
};

class gcEntity {
public:
    static void DestroyAll(const cMemPool *);
};

namespace cMemPoolNS {
    const cMemPool *GetPoolFromPtr(const void *);
}

class gcStreamedCinematic {
public:
    void Write(cFile &) const;
    void RemoveFromWorld(void);
    static unsigned char IsFullscreenInProgress(void);
    static unsigned char HasFullscreenStarted(void);
    static unsigned char IsFullscreenPlaying(void);
};

// ──────────────────────────────────────────────────────
// 0x000ed660 (36B) — RemoveFromWorld
// ──────────────────────────────────────────────────────
void gcStreamedCinematic::RemoveFromWorld(void) {
    gcEntity::DestroyAll(cMemPoolNS::GetPoolFromPtr(this));
}

// ──────────────────────────────────────────────────────
// 0x000ed254 (72B) — IsFullscreenInProgress (static)
// ──────────────────────────────────────────────────────
unsigned char gcStreamedCinematic::IsFullscreenInProgress(void) {
    int *g = *(int **)0x37D7FC;
    int result = 0;
    if (g != NULL) {
        int inner = 0;
        int f = g[0x140 / 4];
        if (f & 0x4000) {
            if (f & 0x2000) {
                inner = 1;
            }
        }
        if ((unsigned char)inner) {
            result = 1;
        }
    }
    return (unsigned char)result;
}

// ──────────────────────────────────────────────────────
// 0x000ed29c (72B) — HasFullscreenStarted (static)
// ──────────────────────────────────────────────────────
unsigned char gcStreamedCinematic::HasFullscreenStarted(void) {
    int *g = *(int **)0x37D7FC;
    int result = 0;
    if (g != NULL) {
        int inner = 0;
        int f = g[0x140 / 4];
        if (f & 0x4000) {
            if (f & 0x8000) {
                inner = 1;
            }
        }
        if ((unsigned char)inner) {
            result = 1;
        }
    }
    return (unsigned char)result;
}

// ──────────────────────────────────────────────────────
// 0x000ed1fc (88B) — IsFullscreenPlaying (static)
// ──────────────────────────────────────────────────────
unsigned char gcStreamedCinematic::IsFullscreenPlaying(void) {
    int *g = *(int **)0x37D7FC;
    int result = 0;
    if (g != NULL) {
        int inner = 0;
        int f = g[0x140 / 4];
        if (((f & 0x4000) && (f & 0x1000)) || (f & 0x200000)) {
            inner = 1;
        }
        if ((unsigned char)inner) {
            result = 1;
        }
    }
    return (unsigned char)result;
}

// ──────────────────────────────────────────────────────
// 0x000ecfd4 (120B) — Write(cFile &) const
// ──────────────────────────────────────────────────────
struct gcStreamedCinematicVTEntry {
    short this_adjust;
    short pad;
    void (*fn)(void *, cFile *);
};

void gcStreamedCinematic::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const cFactory *)this)->Write(file);
    ((const cFactory *)this)->WriteGroups(wb);
    gcStreamedCinematicVTEntry *entry =
        (gcStreamedCinematicVTEntry *)(*(char **)((char *)this + 0x50) + 0x28);
    char *base = (char *)this + 0x4C;
    ((void (*)(void *, cFile *))entry->fn)(base + entry->this_adjust, &file);
    wb.End();
}
