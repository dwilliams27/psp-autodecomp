class gcStateMachine;
class gcState;

template <class T>
class cHandleT {
public:
    int mIndex;
};

template <class T>
class cSubHandleT {
public:
    int mIndex;
};

template <class T, class U>
class cHandlePairT {
public:
    cHandleT<T> mHandle;
    U mSubHandle;
};

extern void *D_00038890[];

struct gcStateVTableEntry {
    void *mpStateMachine;
    int mStateIndex;

    gcStateVTableEntry();
    int IsValid() const;
    void Set(const cHandlePairT<gcStateMachine, cSubHandleT<gcState> > &pair);
    void Get(cHandlePairT<gcStateMachine, cSubHandleT<gcState> > *out) const;
};

gcStateVTableEntry::gcStateVTableEntry() {
    mpStateMachine = 0;
    mStateIndex = -1;
}

int gcStateVTableEntry::IsValid() const {
    int result = 0;
    if (mpStateMachine != 0 && mStateIndex >= 0) {
        result = 1;
    }
    return (unsigned char)result;
}

void gcStateVTableEntry::Set(const cHandlePairT<gcStateMachine, cSubHandleT<gcState> > &pair) {
    int handle = pair.mHandle.mIndex;
    void *result;
    if (handle == 0) {
        result = 0;
    } else {
        unsigned short index = (unsigned short)handle;
        void *lookup = D_00038890[index];
        result = 0;
        if (lookup != 0) {
            if (*(int *)((char *)lookup + 0x30) == handle) {
                result = lookup;
            }
        }
    }
    mpStateMachine = result;
    int subIdx = -1;
    if (result != 0) {
        subIdx = pair.mSubHandle.mIndex & 0xFFFF;
    }
    mStateIndex = subIdx;
}

void gcStateVTableEntry::Get(cHandlePairT<gcStateMachine, cSubHandleT<gcState> > *out) const {
    void *sm = mpStateMachine;
    int idx = mStateIndex;
    if (idx >= 0) {
        void **data = *(void ***)((char *)sm + 0x50);
        int count = 0;
        if (data != 0) {
            count = ((int *)data)[-1];
        }
        if (idx < count) {
            char *state = (char *)data[idx];
            if (state != 0) {
                state += 0x20;
                cSubHandleT<gcState> sub;
                sub.mIndex = *(int *)state;
                cHandlePairT<gcStateMachine, cSubHandleT<gcState> > pair;
                int h = 0;
                if (sm != 0) {
                    h = *(int *)((char *)sm + 0x30);
                }
                pair.mHandle.mIndex = h;
                pair.mSubHandle = sub;
                *out = pair;
                return;
            }
        }
    }
    cHandlePairT<gcStateMachine, cSubHandleT<gcState> > empty;
    empty.mHandle.mIndex = 0;
    empty.mSubHandle.mIndex = 0;
    *out = empty;
}

class gcDesiredUIWidgetHelper {
public:
    enum gcPrimary {};
    int mPrimary;
    int mField4;
    int mField8;

    gcDesiredUIWidgetHelper(gcPrimary primary);
};

gcDesiredUIWidgetHelper::gcDesiredUIWidgetHelper(gcPrimary primary) {
    mPrimary = (int)primary;
    mField4 = 0;
    mField8 = 0;
}
