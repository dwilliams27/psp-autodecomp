// eHeightmapData and small stubs for gcGameSettings / gcEntityController.

// -------- eHeightmapData (eAll_psp.obj) --------
class eHeightmapData {
public:
    void *mA;
    void *mB;
    void *mC;
    void *mD;
    void *mE;
    eHeightmapData();
};

eHeightmapData::eHeightmapData() {
    mA = 0;
    mB = 0;
    mC = 0;
    mD = 0;
    mE = 0;
}

// -------- gcGameSettings (gcAll_psp.obj) --------
class gcGameSettings {
public:
    char pad[0x30];
    int mFlags;
    static gcGameSettings *Get(void);
    void OnMapEnded(void);
};

gcGameSettings *gcGameSettings::Get(void) {
    gcGameSettings *g = *(gcGameSettings **)0x37D854;
    return g ? (gcGameSettings *)((char *)g + 0x1700) : 0;
}

void gcGameSettings::OnMapEnded(void) {
    int x = mFlags;
    x &= ~1;
    x &= ~2;
    mFlags = x;
}

// -------- gcEntityController::ClearCurrentNavMeshPosition (gcAll_psp.obj) --------
namespace {
struct EC_Nav {
    char pad[0x4C];
    int mFlags;
};
struct EC_Container {
    char pad[0x1F8];
    EC_Nav *mNav;
};
struct EC_Entity {
    EC_Container *mContainer;
};
}

class gcEntityControllerNM {
public:
    void ClearCurrentNavMeshPosition(void);
};

void gcEntityControllerNM::ClearCurrentNavMeshPosition(void) {
    EC_Entity *e = (EC_Entity *)this;
    e->mContainer->mNav->mFlags &= ~8;
}
