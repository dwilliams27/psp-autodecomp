// gcStateVTableEntry::Get(cHandlePairT<gcStateMachine, cSubHandleT<gcState> > *) const
// 0x0010c5e8, 156B, gcAll_psp.obj
//
// Looks up the gcState that the (resolved) vtable entry refers to: indexes the
// machine's state array (offset 0x50, count at [-4]) by the stored sub-handle,
// and on success returns a pair of (machine key at 0x30, state key at 0x20).
// On any failure (negative sub-handle, out-of-range, null slot) returns {0, 0}.

class gcStateMachine;
class gcState;
template <class A> class cSubHandleT;
template <class A, class B> class cHandlePairT;

template <class A, class B> class cHandlePairT {
public:
    int mHandle;     // 0x00
    int mSubHandle;  // 0x04
};

class gcStateMachine {
public:
    char pad[0x50];
    void **mStates;   // 0x50 — state pointer array (count at mStates[-1])
};

class gcStateVTableEntry {
public:
    gcStateMachine *mMachine;  // 0x00 — resolved gcStateMachine pointer
    int mSubHandle;            // 0x04 — sub-handle, or -1 when unresolved

    void Get(cHandlePairT<gcStateMachine, cSubHandleT<gcState> > *) const;
};

#pragma control mopt=0
void
gcStateVTableEntry::Get(cHandlePairT<gcStateMachine, cSubHandleT<gcState> > *out) const {
    gcStateMachine *machine = mMachine;
    int sub = mSubHandle;
    int *second = &out->mSubHandle;
    volatile int resFirst;
    volatile int stage;
    volatile int resSecond;

    if (sub >= 0) {
        void **states = machine->mStates;
        int count = 0;
        if (states != 0) {
            count = ((int *)states)[-1];
        }
        if (sub < count) {
            void *state = states[sub];
            if (state != 0) {
                int *field = (int *)((char *)state + 0x20);
                int sv = *field;
                volatile int *ps = &resSecond;
                resSecond = sv;
                int machineKey = 0;
                if (machine != 0) {
                    machineKey = *(int *)((char *)machine + 0x30);
                }
                resFirst = machineKey;
                {
                    int a = *ps;
                    int b = resFirst;
                    stage = a;
                    out->mHandle = b;
                    *second = stage;
                }
                return;
            }
        }
    }

    volatile int zFirst;
    volatile int zSecond;
    zFirst = 0;
    int zf = zFirst;
    zSecond = 0;
    out->mHandle = zf;
    *second = zSecond;
}
