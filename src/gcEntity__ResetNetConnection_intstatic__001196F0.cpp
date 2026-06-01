// gcEntity::ResetNetConnection(int) static @ 0x001196f0
// Symbol: __0fIgcEntitySResetNetConnectioniT
//
// Static method: initializes a stack gcReplicationVisitor (same init block as
// gcGame::ResetNetConnection @ 0x00107e14), forwards the connection id via
// gcReplicationVisitor::SetNetConnection, then walks the global circular list
// of live entities (head at *0x37D86C, next at entity+0x228). For each
// non-null node it dispatches through a per-type record at entity+4: at
// record+0xA0 a signed short gives the sub-object byte offset, record+0xA4
// holds the handler fn, called as fn(entity + offset, &visitor).
//
// Split-TU: gcEntity / gcReplicationVisitor declared locally so SNC mangles
// ResetNetConnection natively as __0fIgcEntitySResetNetConnectioniT.

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

struct EntityTypeRec {
    char pad[0xA0];
    short offset;   // +0xA0
    short _pad;
    void (*fn)(void *, gcReplicationVisitor *);  // +0xA4
};

struct gcEntityNode {
    char pad0[0x04];             // +0x00
    EntityTypeRec *rec;          // +0x04
    char pad1[0x220];
    gcEntityNode *next;          // +0x228
};

class gcEntity {
public:
    static void ResetNetConnection(int);
};

void gcEntity::ResetNetConnection(int conn) {
    gcReplicationVisitor visitor;
    visitor.mMode = 0x401;
    visitor.mOutStream = 0;
    visitor.mInStream = 0;
    visitor.mStreamPos = -1;
    visitor.mNetConnection = -1;
    visitor.mReadActive = 0;
    visitor.mWriteEnabled = true;
    visitor.SetNetConnection(conn);

    gcEntityNode **head = (gcEntityNode **)0x380000;
    gcEntityNode *node = head[-2533];   // *0x37D86C
    if (node != 0) {
        __asm__ volatile("" ::: "memory");
        do {
            if (node != 0) {
                EntityTypeRec *rec = node->rec;
                EntityTypeRec *slot = (EntityTypeRec *)((char *)rec + 0xA0);
                short off = *(short *)slot;
                void (*fn)(void *, gcReplicationVisitor *) =
                    *(void (**)(void *, gcReplicationVisitor *))((char *)slot + 4);
                fn((char *)node + off, &visitor);
            }
            node = node->next;
        } while (node != head[-2533]);
    }
}
