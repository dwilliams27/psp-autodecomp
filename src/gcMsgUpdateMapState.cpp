// gcMsgUpdateMapState / gcDoConsoleOp / gcDoEntityApplyRigidBodyImpulse
// New() methods from gcAll_psp.obj.

class nwMsg;
class cBase;
class cMemPool;

struct nwMsgBuffer {
    char _pad[0x4B0];
    int mOffset;
};

inline void *operator new(unsigned int, void *p) { return p; }

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcMsgUpdateMapState {
public:
    static nwMsg *New(nwMsgBuffer &);
};

struct gcMsgUpdateMapState_layout {
    void *vtable;
    char mB;
    char _pad[3];
    int mSubA;
    int mSubB;
};

class gcDoConsoleOp {
public:
    gcDoConsoleOp(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

class gcDoEntityApplyRigidBodyImpulse {
public:
    gcDoEntityApplyRigidBodyImpulse(cBase *);
    static cBase *New(cMemPool *, cBase *);
};

// -----------------------------------------------------------------------------
// 0x00284f44 — gcMsgUpdateMapState::New(nwMsgBuffer &)
// -----------------------------------------------------------------------------

nwMsg *gcMsgUpdateMapState::New(nwMsgBuffer &buf) {
    int cursor = buf.mOffset + 0x10;
    buf.mOffset = cursor;
    gcMsgUpdateMapState_layout *obj =
        (gcMsgUpdateMapState_layout *)((char *)&buf + cursor);
    nwMsg *result = 0;
    if (obj != 0) {
        volatile int zb[4];
        zb[0] = 0;
        zb[1] = 0;
        int v0 = zb[0];
        int v1 = zb[1];
        zb[2] = v0;
        __asm__ volatile(
            "lui $a1, 0x39\n\t"
            "sw %1, 12($sp)\n\t"
            "addiu $a1, $a1, -21824\n\t"
            "ori $a3, $0, 0\n\t"
            "sw $a1, 0(%0)\n\t"
            "sb $a3, 4(%0)"
            :
            : "r"(obj), "r"(v1)
            : "$a1", "$a3", "memory"
        );
        int v2 = zb[2];
        int *p = (int *)((char *)obj + 8);
        int v3 = zb[3];
        p[0] = v2;
        p[1] = v3;
        result = (nwMsg *)obj;
    }
    return result;
}

// -----------------------------------------------------------------------------
// 0x0029cbe8 — gcDoConsoleOp::New(cMemPool *, cBase *) static
// -----------------------------------------------------------------------------

cBase *gcDoConsoleOp::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoConsoleOp *result = 0;
    gcDoConsoleOp *obj = (gcDoConsoleOp *)rec->fn(base, 0x24, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoConsoleOp(parent);
        result = obj;
    }
    return (cBase *)result;
}

// -----------------------------------------------------------------------------
// 0x002a1eb0 — gcDoEntityApplyRigidBodyImpulse::New(cMemPool *, cBase *) static
// -----------------------------------------------------------------------------

cBase *gcDoEntityApplyRigidBodyImpulse::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcDoEntityApplyRigidBodyImpulse *result = 0;
    gcDoEntityApplyRigidBodyImpulse *obj =
        (gcDoEntityApplyRigidBodyImpulse *)rec->fn(base, 0x7C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcDoEntityApplyRigidBodyImpulse(parent);
        result = obj;
    }
    return (cBase *)result;
}
