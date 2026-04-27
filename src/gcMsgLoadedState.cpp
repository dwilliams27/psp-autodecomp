// gcMsgLoadedState — gcAll_psp.obj
//   0x00264710 GetType(void) const                (76B)   ✓ matched
//   0x0026475c New(nwMsgBuffer &) static          (116B)  ✓ matched
//   0x00124348 CurrentState(bool) static          (232B)  ✗ NRVO blocked by SNC
//
// Layout: 0x28 bytes total.  vtable @0x00, bool @0x04, mFlags @0x08,
// cGUIDT<gcMap> @0x0C, cGUIDT<gcRegion>[2] @0x14, bool @0x24.
//
// CurrentState returns gcMsgLoadedState by value (sret in $a0).  SNC at -O2
// declines to NRVO 40-byte locals (verified empirically: even
// `T r; r.x = ...; return r;` allocates a stack temp and memcpys to $a0 at
// return), so the body still mismatches.  See session notes.

class nwMsg;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int  mOffset;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *,
                                     nwMsg *(*)(nwMsgBuffer &));
};

extern "C" void *__vec_new(void *array, int count, int size, void (*ctor)(void *));

struct gcMapInfo {
    char  _pad00[0x20];
    int   mIdA;          // 0x20  cGUIDT<gcMap> hi
    int   mIdB;          // 0x24  cGUIDT<gcMap> lo
    char  _pad28[0x140 - 0x28];
    int   mFlags;        // 0x140
    int   mLoadedCount;  // 0x144
};

struct cGUIDT_gcRegion { int a; int b; };
struct cGUIDT_gcMap    { int a; int b; };

class gcMap {
public:
    static bool IsMapLoaded(bool);
    void GetLoadingRegions(cGUIDT_gcRegion *) const;
};

class gcMsgLoadedState {
public:
    void *           mVTable;        // 0x00
    bool             mLoaded;        // 0x04
    char             _pad05[3];
    int              mFlags;         // 0x08
    cGUIDT_gcMap     mMapId;         // 0x0C
    cGUIDT_gcRegion  mRegions[2];    // 0x14
    bool             mHasLoading;    // 0x24
    char             _pad25[3];

    gcMsgLoadedState() {}
    gcMsgLoadedState(bool b) {
        mVTable     = (void *)0x389030;
        mLoaded     = b;
        mFlags      = 0;
        mMapId.a    = 0;
        mMapId.b    = 0;
        __vec_new(&mRegions[0], 2, 8, (void (*)(void *))0x245578);
        mHasLoading = false;
    }

    static nwMsg *New(nwMsgBuffer &buf);
    static gcMsgLoadedState CurrentState(bool b);
    nwMsgType *GetType() const;
};

inline void *operator new(unsigned int, void *p) { return p; }

// -----------------------------------------------------------------------------
// 0x0026475c — gcMsgLoadedState::New(nwMsgBuffer &)   ✓ matched
// -----------------------------------------------------------------------------

nwMsg *gcMsgLoadedState::New(nwMsgBuffer &buf) {
    int cursor = buf.mOffset + 0x28;
    buf.mOffset = cursor;
    gcMsgLoadedState *obj = (gcMsgLoadedState *)((char *)&buf + cursor);
    nwMsg *result = 0;
    if (obj != 0) {
        new (obj) gcMsgLoadedState(false);
        result = (nwMsg *)obj;
    }
    return result;
}

// -----------------------------------------------------------------------------
// 0x00264710 — gcMsgLoadedState::GetType   ✓ matched
// -----------------------------------------------------------------------------

static nwMsgType *sLoadedStateType;

nwMsgType *gcMsgLoadedState::GetType() const {
    if (!sLoadedStateType) {
        sLoadedStateType = nwMsgType::InitializeType(
            0xD, 0, "gcMsgLoadedState", &gcMsgLoadedState::New);
    }
    return sLoadedStateType;
}

// -----------------------------------------------------------------------------
// 0x00124348 — gcMsgLoadedState::CurrentState(bool)   ✗ unmatched
// -----------------------------------------------------------------------------

gcMsgLoadedState gcMsgLoadedState::CurrentState(bool b) {
    gcMsgLoadedState r;
    r.mVTable     = (void *)0x389030;
    r.mLoaded     = b;
    r.mFlags      = 0;
    r.mMapId.a    = 0;
    r.mMapId.b    = 0;
    __vec_new(&r.mRegions[0], 2, 8, (void (*)(void *))0x245578);
    r.mHasLoading = false;
    if (gcMap::IsMapLoaded(false)) {
        gcMapInfo *map = *(gcMapInfo **)0x37D7FC;
        r.mFlags   = map->mLoadedCount;
        r.mMapId.a = map->mIdA;
        r.mMapId.b = map->mIdB;
        if ((map->mFlags & 1) != 0) {
            ((gcMap *)map)->GetLoadingRegions(&r.mRegions[0]);
        }
        r.mHasLoading = (map->mFlags & 1) != 0;
    }
    return r;
}
