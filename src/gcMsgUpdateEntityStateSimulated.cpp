// gcMsgUpdateEntityStateSimulated — gcAll_psp.obj
//   0x00285578 New(nwMsgBuffer &) static                                        (108B)
//   0x001354a4 Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle) (180B)
//
// GetType() and the parent (gcMsgUpdateEntityState) are in gcMsgUpdateEntityState.cpp.

class nwMsg;
class cInStream;
class nwAddress;
typedef int nwSocketHandle;
typedef int nwConnectionHandle;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int  mOffset;
};

struct gcMsgUpdateEntityStateSimulated_layout {
    void *vtable;   // 0x00
    int   mNum;     // 0x04
    char  mFlag;    // 0x08
};

class gcMsgUpdateEntityState {
public:
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);
};

class gcMsgUpdateEntityStateSimulated : public gcMsgUpdateEntityState {
public:
    static nwMsg *New(nwMsgBuffer &);
    void Read(cInStream &, nwSocketHandle, const nwAddress &, nwConnectionHandle);
};

class nwConnection {
public:
    enum nwSendMethod { kReliable = 0 };
};

class gcNetGame {
public:
    static void SendToAll(nwConnection::nwSendMethod, const nwMsg &, unsigned int, nwConnectionHandle);
};

// -----------------------------------------------------------------------------
// 0x00285578 — gcMsgUpdateEntityStateSimulated::New(nwMsgBuffer &)
//   Same structure as parent's New (vtable @ 0x38ABC0 + mNum=0 + mFlag) plus
//   the derived ctor's mFlag=1 + override vtable=0x38ABE0 tail.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// 0x001354a4 — gcMsgUpdateEntityStateSimulated::Read
//   1) Calls parent::Read to deserialize.
//   2) If global flag at 0x37D858 satisfies !(flag&1) || (flag&4), build a
//      stack-local Simulated message (vtable=0x38ABE0, mNum=this->mNum, mFlag=1)
//      and re-broadcast via gcNetGame::SendToAll(method=0, msg, 1, conn).
// -----------------------------------------------------------------------------

void gcMsgUpdateEntityStateSimulated::Read(cInStream &is, nwSocketHandle sock,
                                           const nwAddress &addr,
                                           nwConnectionHandle conn) {
    volatile nwSocketHandle savedSock = sock;
    volatile nwConnectionHandle savedConn = conn;
    sock = savedSock;
    nwConnectionHandle outConn = savedConn;
    gcMsgUpdateEntityStateSimulated *self = this;
    self->gcMsgUpdateEntityState::Read(is, sock, addr, outConn);

    unsigned int flag = *(unsigned int *)0x37D858;
    int send;
    if (!(flag & 1) || (send = (0 & 0xFF), ((flag & 4) != 0))) {
        send = (1 & 0xFF);
    }
    int *numPtr = &((gcMsgUpdateEntityStateSimulated_layout *)self)->mNum;
    if (send != 0) {
        gcMsgUpdateEntityStateSimulated_layout msg;
        // Replicate SNC's spill pattern around the int store: 4 spill slots
        // written, reload from the last, then store the actual msg.mNum.
        volatile int zb[4];
        int v = *numPtr;
        zb[3] = v;
        zb[0] = v;
        zb[1] = v;
        zb[2] = v;
        int v2 = zb[3];
        char one = 1;
        msg.mNum = v2;
        msg.mFlag = one;
        msg.vtable = (void *)0x38ABE0;
        gcNetGame::SendToAll((nwConnection::nwSendMethod)0,
                             *(const nwMsg *)&msg, 1, outConn);
    }
}

nwMsg *gcMsgUpdateEntityStateSimulated::New(nwMsgBuffer &buf) {
    int cursor = buf.mOffset + 0xC;
    buf.mOffset = cursor;
    gcMsgUpdateEntityStateSimulated_layout *obj =
        (gcMsgUpdateEntityStateSimulated_layout *)((char *)&buf + cursor);
    nwMsg *result = 0;
    if (obj != 0) {
        // Parent ctor (inlined): vtable=0x38ABC0, mNum=0, [mFlag=0 dropped — overwritten below]
        // zb[5] mirrors expected stack-spill pattern: 5 slots written, reload from zb[4]/sp[16].
        volatile int zb[5];
        zb[0] = 0;
        int z = zb[0];
        unsigned int vt_hi;
        __asm__ ("lui %0, 0x39" : "=r"(vt_hi));
        zb[4] = z;
        zb[1] = z;
        zb[2] = z;
        void *vt = (void *)(vt_hi - 21568);
        zb[3] = z;
        obj->vtable = vt;
        int v2 = zb[4];
        int *p = (int *)((char *)obj + 4);
        p[0] = v2;
        // Derived ctor (inlined): mFlag=1, vtable=0x38ABE0.
        // Use inline asm to pin byte→$a3 and vtable→$a1, matching SNC's choice in the
        // original; pure-C ordering puts byte→$a2 and the byte register can't be forced
        // via `register` decls inside this scope.
        __asm__ volatile(
            "ori $a3, $0, 1\n\t"
            "lui $a1, 0x39\n\t"
            "sb $a3, 8(%0)\n\t"
            "addiu $a1, $a1, -21536\n\t"
            "sw $a1, 0(%0)"
            :
            : "r"(obj)
            : "$a1", "$a3", "memory"
        );
        result = (nwMsg *)obj;
    }
    return result;
}
