// Five small 40-byte leaf functions grouped by the orchestrator.

// =============================================================================
// Function 1: cXML::cNode::cNode(cXML::cNodeType, const char *)
// Address: 0x0000dcf0, Size: 40B, Obj: cAll_psp.obj
// =============================================================================

class cXML {
public:
    enum cNodeType { _dummy_node_type = 0 };
    class cNode {
    public:
        int mType;        // +0x00
        const char *mName;// +0x04
        int mPad8;        // +0x08
        int mPadC;        // +0x0C
        int mPad10;       // +0x10
        void *mVtbl;      // +0x14
        cNode(cXML::cNodeType, const char *);
    };
};

extern "C" char cXML_cNode_vtable[];

cXML::cNode::cNode(cXML::cNodeType type, const char *name) {
    this->mVtbl  = cXML_cNode_vtable;
    this->mType  = (int)type;
    this->mName  = name;
    this->mPad8  = 0;
    this->mPadC  = 0;
    this->mPad10 = 0;
}

// =============================================================================
// Function 2: eDynamicModel::HasCollision(void) const
// Address: 0x000497a0, Size: 40B, Obj: eAll_psp.obj  (sched=2 per hint)
// NOTE: This file name does NOT match the eDynamicModel* Makefile override,
// so this function compiles at default sched=2.
// =============================================================================

class eDynamicModel {
public:
    char _pad[0x110];
    int  *mPhysics;  // +0x110 pointer to some object with flags at [-1]
    int HasCollision(void) const;
};

int eDynamicModel::HasCollision(void) const {
    int *p = this->mPhysics;
    int result = 0;
    if (p != 0) {
        result = (p[-1] & 0x3FFFFFFF) > 0;
    }
    return result;
}

// =============================================================================
// Function 3: gcValUserStatus::gcValUserStatus(cBase *)
// Address: 0x0012fb0c, Size: 40B, Obj: gcAll_psp.obj
// =============================================================================

class cBase;

class gcValUserStatus {
public:
    cBase *mBase;     // +0x00
    void  *mVtbl;     // +0x04
    int    mPad8;     // +0x08
    int    mPadC;     // +0x0C
    gcValUserStatus(cBase *);
};

extern "C" char gcValUserStatus_vtable[];

gcValUserStatus::gcValUserStatus(cBase *base) {
    this->mBase = base;
    this->mVtbl = gcValUserStatus_vtable;
    this->mPad8 = 6;
    this->mPadC = (int)this | 1;
}

// =============================================================================
// Function 4: __getreent
// Address: 0x001aa284, Size: 40B, Obj: gMain_psp.obj
// Contains dead code: an unconditional branch at start skips over a
// null-check path. Most likely source is the classic newlib single-thread
// body plus an unreachable multi-thread fallback that was compiled anyway.
// =============================================================================

// NOTE: __getreent (0x001aa284, 40B) is NOT matched. The expected bytes
// contain 4 instructions of dead code after an unconditional `b` that skips
// them — no plain C/C++ construct reproduces that layout under SNC -O2.
// See session notes. No stub is emitted here to avoid size-colliding with
// __errno (12B) and falsely updating the DB.

// =============================================================================
// Function 5: __exception_started
// Address: 0x001bab2c, Size: 40B, Obj: gMain_psp.obj
// Pushes an exception frame onto a linked list: frame->next=top; top=&frame->next;
// frame->started=1.
// =============================================================================

struct __exception_frame {
    char _before48[48];
    unsigned char mFlag;     // +48
    char _pad49[3];
    __exception_frame *mNext;// +52
};

extern __exception_frame *g_eh_frame;     // at 0x37E518
extern __exception_frame *g_eh_top_chain; // at 0x37E54C

extern "C" void __exception_started(void) {
    __exception_frame *frame = g_eh_frame;
    frame->mNext = g_eh_top_chain;
    g_eh_top_chain = (__exception_frame *)&frame->mNext;
    frame->mFlag = 1;
}
