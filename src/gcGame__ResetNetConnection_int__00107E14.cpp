// gcGame::ResetNetConnection(int) @ 0x00107e14
// Symbol: __0fGgcGameSResetNetConnectioni
// Split-TU: local class declarations to avoid touching shared headers.

class gcReplicationVisitor {
public:
    int mMode;                // 0x00
    void *mOutStream;         // 0x04
    void *mInStream;          // 0x08
    int mStreamPos;           // 0x0C
    int mNetConnection;       // 0x10
    int mReadActive;          // 0x14
    bool mWriteEnabled;       // 0x18
    bool mByteSwap;           // 0x19

    void SetNetConnection(int);
};

struct VisitorDispatchEntry {
    short offset;
    short _pad;
    void (*fn)(void *, gcReplicationVisitor *);
};

class gcMap {
public:
    void ResetNetConnection(int);
};

class gcPlayer {
public:
    static void ResetNetConnection(int);
};

class gcGame {
public:
    void ResetNetConnection(int);
};

extern gcMap *gMapSingleton; // global at 0x37D7FC

void gcGame::ResetNetConnection(int conn) {
    gcReplicationVisitor visitor;
    visitor.mMode = 0x401;
    visitor.mOutStream = 0;
    visitor.mInStream = 0;
    visitor.mStreamPos = -1;
    visitor.mNetConnection = -1;
    visitor.mReadActive = 0;
    visitor.mWriteEnabled = true;
    visitor.SetNetConnection(conn);

    if (this != 0) {
        char *type = *(char **)((char *)this + 4);
        VisitorDispatchEntry *entry = (VisitorDispatchEntry *)(type + 0xC0);
        entry->fn((char *)this + entry->offset, &visitor);
    }
    gcPlayer::ResetNetConnection(conn);
    if (gMapSingleton != 0) {
        gMapSingleton->ResetNetConnection(conn);
    }
}
