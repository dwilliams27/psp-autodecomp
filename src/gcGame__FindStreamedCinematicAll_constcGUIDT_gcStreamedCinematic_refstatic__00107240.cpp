// gcGame::FindStreamedCinematicAll(const cGUIDT<gcStreamedCinematic> &) static
// Address: 0x00107240, Size: 268B  Obj: gcAll_psp.obj
// Split-TU: class declared locally; callees declared locally (relocations masked).

template <class T> class cGUIDT;
class gcStreamedCinematic;
template <class T> class cBaseArrayT;
class gcStreamedCinematicConfigGroup;
class cBase;

class gcGame {
public:
    static void *FindStreamedCinematic(
        const cGUIDT<gcStreamedCinematic> &,
        const cBaseArrayT<gcStreamedCinematicConfigGroup *> *,
        int *,
        const cBaseArrayT<gcStreamedCinematicConfigGroup *> *);
    static void *FindStreamedCinematicAll(const cGUIDT<gcStreamedCinematic> &);
};

class gcRegion;
class gcMap {
public:
    gcRegion *GetLoadedRegion(int) const;
};

// Globals.
extern char *gConfigInstance asm("D_0037D854");   // 0x0037D854
extern gcMap *g_gcMap        asm("D_0037D7FC");    // 0x0037D7FC

void *gcGame::FindStreamedCinematicAll(const cGUIDT<gcStreamedCinematic> &guid) {
    char *cfg = gConfigInstance;
    if (cfg == 0) {
        return 0;
    }

    void *r = FindStreamedCinematic(
        guid,
        (const cBaseArrayT<gcStreamedCinematicConfigGroup *> *)(cfg + 0x17A0),
        0, 0);
    if (r != 0) {
        return r;
    }

    gcMap *map = g_gcMap;
    if (map == 0) {
        return 0;
    }

    void *r2 = FindStreamedCinematic(
        guid,
        (const cBaseArrayT<gcStreamedCinematicConfigGroup *> *)((char *)map + 0x5C),
        0, 0);
    if (r2 != 0) {
        return r2;
    }

    for (int i = 0; i < 2; i++) {
        gcRegion *region = map->GetLoadedRegion(i);
        if (region != 0) {
            void *r3 = FindStreamedCinematic(
                guid,
                (const cBaseArrayT<gcStreamedCinematicConfigGroup *> *)((char *)region + 0x5C),
                0, 0);
            if (r3 != 0) {
                return r3;
            }
        }
    }
    return 0;
}
