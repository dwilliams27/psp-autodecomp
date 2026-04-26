// -----------------------------------------------------------------------------
// gcValLobbyConnectionStatus::AssignCopy(const cBase *)  @ 0x003478f8
//   (gcAll_psp.obj, 48B)
// -----------------------------------------------------------------------------

class cBase;

class gcValLobbyConnectionStatus {
public:
    int pad0;
    int pad4;
    int f8;
    void AssignCopy(const cBase *);
};

gcValLobbyConnectionStatus *dcast(const cBase *);

void gcValLobbyConnectionStatus::AssignCopy(const cBase *base) {
    gcValLobbyConnectionStatus *other = dcast(base);
    this->f8 = other->f8;
}
