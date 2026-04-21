#include "gcMsgCinematicEnded.h"
#include "gcMsgRequestLoadedState.h"
#include "gcEntityTemplate.h"
#include "gcValNumber.h"
#include "gcUIWidgetList.h"

// -----------------------------------------------------------------------------
// Forward declarations for newly added functions below (gcAll_psp.obj,
// nwAll_psp.obj, cAll_psp.obj).  These are kept local to the translation unit
// because the associated headers either do not yet exist or their layouts
// conflict with the `typedef int nwConnectionHandle;` used above.
// -----------------------------------------------------------------------------

class cInStream;
class nwMsg;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int mOffset;
};

class nwMsgType {
public:
    static nwMsgType *InitializeType(unsigned char, unsigned char, const char *,
                                     nwMsg *(*)(nwMsgBuffer &));
};

class gcGame_Stub {
public:
    void ResetNetConnection(int);
};

extern gcGame_Stub *gpGame;

class nwNetwork {
public:
    static void GetLastErrorString(char *, int);
    static void GetEULA(char *, int);
};

class cNamed {
public:
    void GetName(char *) const;
};

// 2-arg and 3-arg overloads of cStrCopy (cAll_psp.obj @0x1428 / @0x1588)
void cStrCopy(char *, const char *);
void cStrCopy(char *, const char *, int);

// -----------------------------------------------------------------------------
// Existing stubs (previously matched)
// -----------------------------------------------------------------------------

void gcMsgCinematicEnded::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
}

void gcMsgRequestLoadedState::Write(cOutStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) const {
}

void gcEntityTemplate::CalcInstanceSize(void) {
}

float gcValNumber::Evaluate(void) const {
    return mValue;
}

void gcUIWidgetList::FillCell(gcUICell, int) {
}

// -----------------------------------------------------------------------------
// gcMsgCinematicEnded::Read  @ 0x00123ff4  (gcAll_psp.obj, 36B)
//   void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle)
// Calls gcGame::ResetNetConnection(int) on gpGame with the zero-extended
// nwConnectionHandle argument (5th arg, arrives in $t0 on PSP EABI).
// -----------------------------------------------------------------------------

void gcMsgCinematicEnded::Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle conn) {
    gpGame->ResetNetConnection((unsigned char)conn);
}

// -----------------------------------------------------------------------------
// gcMsgCinematicEnded::GetType  @ 0x002645dc  (gcAll_psp.obj, 76B)
//   Lazily creates the nwMsgType singleton for gcMsgCinematicEnded.
// -----------------------------------------------------------------------------

static nwMsgType *sCinematicEndedType;

nwMsgType *gcMsgCinematicEnded::GetType() const {
    if (!sCinematicEndedType) {
        sCinematicEndedType = nwMsgType::InitializeType(
            0x1E, 0, "gcMsgCinematicEnded", &gcMsgCinematicEnded::New);
    }
    return sCinematicEndedType;
}

// -----------------------------------------------------------------------------
// nwNetwork::GetLastErrorString  @ 0x001a5868  (nwAll_psp.obj, 36B)
// nwNetwork::GetEULA             @ 0x001a588c  (nwAll_psp.obj, 36B)
//   Both forward to cStrCopy(dest, <fixed rodata string>, size).  The
//   underlying string address is the same (0x371198 — an empty-string sentinel
//   in the read-only data segment).
// -----------------------------------------------------------------------------

void nwNetwork::GetLastErrorString(char *dest, int size) {
    cStrCopy(dest, (const char *)0x371198, size);
}

void nwNetwork::GetEULA(char *dest, int size) {
    cStrCopy(dest, (const char *)0x371198, size);
}

// -----------------------------------------------------------------------------
// cNamed::GetName  @ 0x001c6ed4  (cAll_psp.obj, 36B)
//   Copies the embedded name string (stored inline at this+8) into the
//   caller-supplied buffer via the 2-argument cStrCopy.
// -----------------------------------------------------------------------------

void cNamed::GetName(char *dest) const {
    cStrCopy(dest, (const char *)this + 8);
}
