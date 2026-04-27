// gcMsgRequestLoadedState / gcMsgLoadRegions — gcAll_psp.obj
//
//   0x002647d0  gcMsgRequestLoadedState::GetType(void) const          (76B)
//   0x001249b8  gcMsgRequestLoadedState::Read(cInStream &, ...)       (184B)
//   0x0026484c  gcMsgLoadRegions::GetType(void) const                 (76B)
//
// GetType(): same lazy nwMsgType-singleton template as gcMsgPlayCinematic /
// gcMsgLoadedState / gcMsgCinematicEnded.  Only the type id, the static
// stringtab pointer, the static singleton slot, and the &::New function
// pointer differ.
//
// Read(): if a remote map-load request can be satisfied, reply with the
// current loaded-state.  Conditions:
//   * not currently in a fullscreen cinematic,
//   * the connection from the incoming nwConnectionHandle is alive,
//   * the global map is loaded (gcMap::IsMapLoaded(false)),
//   * the global map's load-counter at +0x144 is non-zero.
// When all four are met, build the reply via gcMsgLoadedState::CurrentState
// (passing whether a flag bit is set in the global word at 0x37D858) and
// dispatch it on the connection with nwSendMethod=2.

#include "gcMsgRequestLoadedState.h"

class cInStream;
class cOutStream;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int  mOffset;
};

class nwMsg {
public:
    void *_vt;  // 0x00
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *,
                                     nwMsg *(*)(nwMsgBuffer &));
};

// gcMsgLoadedState — only the layout/size matters here (it is returned by
// value).  Single inheritance from nwMsg lets the compiler bind a
// gcMsgLoadedState rvalue directly to `const nwMsg &` without copying.
// Copy ctor / assignment are made non-copyable (declared but never defined)
// so SNC must elide the copy from CurrentState's sret slot to the local.
class gcMsgLoadedState : public nwMsg {
private:
    gcMsgLoadedState(const gcMsgLoadedState &);
    gcMsgLoadedState &operator=(const gcMsgLoadedState &);
public:
    gcMsgLoadedState() {}
    char _pad[0x24];   // 0x28 total - 4 byte vtable
    static gcMsgLoadedState CurrentState(bool);
};

class gcStreamedCinematic {
public:
    static bool IsFullscreenPlaying();
};

class gcMap {
public:
    static bool IsMapLoaded(bool);
};

class nwConnection;

class nwSocket {
public:
    static nwConnection *GetConnection(nwConnectionHandle);
};

class nwConnection {
public:
    enum nwSendMethod { };
    void Send(nwSendMethod, const nwMsg &, unsigned int, cOutStream *);
};

// Layout of the global gcMap singleton (only the field we touch).
struct gcMapInfo_layout {
    char _pad[0x144];
    int  mLoadedCount;
};

// gcMsgLoadRegions — sibling message class with the same GetType template.
class gcMsgLoadRegions {
public:
    static nwMsg *New(nwMsgBuffer &);
    nwMsgType *GetType() const;
};

// -----------------------------------------------------------------------------
// 0x002647d0 — gcMsgRequestLoadedState::GetType
// -----------------------------------------------------------------------------

static nwMsgType *sRequestLoadedStateType;

nwMsgType *gcMsgRequestLoadedState::GetType() const {
    if (!sRequestLoadedStateType) {
        sRequestLoadedStateType = nwMsgType::InitializeType(
            0x1F, 0, "gcMsgRequestLoadedState",
            &gcMsgRequestLoadedState::New);
    }
    return sRequestLoadedStateType;
}

// -----------------------------------------------------------------------------
// 0x0026484c — gcMsgLoadRegions::GetType
// -----------------------------------------------------------------------------

static nwMsgType *sLoadRegionsType;

nwMsgType *gcMsgLoadRegions::GetType() const {
    if (!sLoadRegionsType) {
        sLoadRegionsType = nwMsgType::InitializeType(
            0x1B, 0, "gcMsgLoadRegions",
            &gcMsgLoadRegions::New);
    }
    return sLoadRegionsType;
}

// -----------------------------------------------------------------------------
// 0x001249b8 — gcMsgRequestLoadedState::Read
// -----------------------------------------------------------------------------

void gcMsgRequestLoadedState::Read(cInStream &, nwSocketHandle,
                                   const nwAddress &,
                                   nwConnectionHandle conn) {
    if (gcStreamedCinematic::IsFullscreenPlaying()) {
        return;
    }
    if (!nwSocket::GetConnection(conn)) {
        return;
    }
    if (!gcMap::IsMapLoaded(false)) {
        return;
    }
    gcMapInfo_layout *map = *(gcMapInfo_layout **)0x37D7FC;
    if (!map->mLoadedCount) {
        return;
    }
    nwSocket::GetConnection(conn)->Send(
        (nwConnection::nwSendMethod)2,
        gcMsgLoadedState::CurrentState(
            (unsigned char)((*(unsigned int *)0x37D858 & 2) != 0) != 0),
        0, 0);
}
