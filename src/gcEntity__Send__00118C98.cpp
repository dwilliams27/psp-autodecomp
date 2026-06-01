// gcEntity::Send(bool, gcEntity::gcDesiredStateMachine,
//                gcStateMessages::gcSystemMessage,
//                const gcEventParams &, const gcEventStackData &) @ 0x00118c98
// 176B, gcAll_psp.obj — non-leaf, thin forwarder.
//
// Resolves the global config singleton's enumeration handle (member at 0x4C)
// through the global handle table, fetches a cHandlePairT for the given system
// message via gcEnumeration::Get(int), then forwards to the cHandlePairT
// overload of Send.

class gcEnumeration;
class gcEnumerationEntry;

class gcEventParams {
public:
    char _data[0x58];
};

class gcEventStackData {
public:
    char _data[0x1C];
};

class gcStateMessages {
public:
    enum gcSystemMessage {
        SYSTEM_MESSAGE_0 = 0
    };
};

template <class A>
class cHandleT {
public:
    int mIndex;
    cHandleT(const cHandleT &);
};

template <class A>
class cSubHandleT {
public:
    int mSub;
};

template <class A, class B>
class cHandlePairT {
public:
    cHandleT<A> mHandle;
    cSubHandleT<B> mSub;
};

typedef cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > gcEnumPair;

class gcEnumeration {
public:
    gcEnumPair Get(int) const;
};

extern void *D_00038890[];

struct gcConfigGlobal {
    char _pad[0x4C];
    int mEnumKey;  // 0x4C
};

extern gcConfigGlobal *D_0037D850;

class gcEntity {
public:
    enum gcDesiredStateMachine {
        DESIRED_STATE_MACHINE_0 = 0
    };
    void Send(bool, gcDesiredStateMachine, gcStateMessages::gcSystemMessage,
              const gcEventParams &, const gcEventStackData &);
    void Send(bool, gcDesiredStateMachine, const gcEnumPair &,
              const gcEventParams &, const gcEventStackData &);
};

void gcEntity::Send(bool b, gcDesiredStateMachine dsm,
                    gcStateMessages::gcSystemMessage sysMsg,
                    const gcEventParams &params, const gcEventStackData &stack) {
    gcEnumeration *e = 0;
    int key = D_0037D850->mEnumKey;
    if (key != 0) {
        e = (gcEnumeration *)D_00038890[key & 0xFFFF];
    }
    Send(b, dsm, e->Get((int)sysMsg), params, stack);
}
