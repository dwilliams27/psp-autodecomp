// 0x000f6f54 (240B) — gcMap::MemCardReplicateStreamedCinematicLoad(gcReplicationVisitor &)
// Obj: gcAll_psp.obj
//
// Split-TU: gcMap is declared locally here (the (gcReplicationVisitor&,int)
// overload is not present in include/gcMap.h, and adding method decls to the
// shared header is banned). The single-arg method is the visible symbol.

class gcReplicationVisitor;

class gcStreamedCinematic {
public:
    void RemoveFromWorld(void);
    static void Delete(void *);
};

class gcGame {
public:
    void ResetLoadingCinematicPools(void);
};

class gcMap {
public:
    void MemCardReplicateStreamedCinematicLoad(gcReplicationVisitor &);
    void MemCardReplicateStreamedCinematicLoad(gcReplicationVisitor &, int);
    void WaitForBackgroundLoad(void);
};

namespace cMemPoolNS {
    const void *GetPoolFromPtr(const void *);
}

extern int gcStreamedCinematic_currentIndex;             // 0x37D7D0
extern gcStreamedCinematic *gcStreamedCinematic_table[]; // 0x37D7D4
extern gcGame *gGameInstance;                            // 0x37D854

struct VCall3 {
    short offset;
    short pad;
    void (*fn)(void *, const void *, int);
};

void gcMap::MemCardReplicateStreamedCinematicLoad(gcReplicationVisitor &visitor) {
    int mode = *(int *)&visitor;
    if (((mode & 2) != 0) & 0xFF) {
        WaitForBackgroundLoad();
        gcStreamedCinematic *cin = gcStreamedCinematic_table[gcStreamedCinematic_currentIndex];
        if (cin != 0) {
            cin->RemoveFromWorld();
            void *root = *(void **)((char *)this + 4);
            VCall3 *entry = (VCall3 *)((char *)root + 0x58);
            short off = entry->offset;
            void *base = (char *)this + off;
            const void *pool = cMemPoolNS::GetPoolFromPtr(
                gcStreamedCinematic_table[gcStreamedCinematic_currentIndex]);
            entry->fn(base, pool, 0);
            gcStreamedCinematic::Delete(0);
        }
        gGameInstance->ResetLoadingCinematicPools();
    }
    MemCardReplicateStreamedCinematicLoad(visitor, 0);
    MemCardReplicateStreamedCinematicLoad(visitor, 1);
}
