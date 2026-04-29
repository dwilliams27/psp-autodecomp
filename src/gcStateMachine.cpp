class cBase;
class cMemPool;
class gcState;
class gcStateFunction;

inline void *operator new(unsigned int, void *p) { return p; }

class cObject {
public:
    cObject(cBase *);
};

template <class T>
class cSubHandleT {
public:
    cSubHandleT();
    ~cSubHandleT();
    int mIndex;
};

struct AllocRec {
    short offset;
    short _pad;
    void *(*fn)(void *, int, int, int, int);
};

class gcStateMachine : public cObject {
public:
    gcStateMachine(cBase *);

    static cBase *New(cMemPool *, cBase *);
    gcState *GetSubObject(cSubHandleT<gcState>, int) const;
    int IsValid(cSubHandleT<gcState>, int) const;
    int IsValid(cSubHandleT<gcStateFunction>, int) const;
};

// ── gcStateMachine::gcStateMachine(cBase *) @ 0x0010cc30 ──
gcStateMachine::gcStateMachine(cBase *parent) : cObject(parent) {
    *(void **)((char *)this + 4) = (void *)0x388448;
    *(int *)((char *)this + 0x44) = 0;
    *(int *)((char *)this + 0x48) = 0;
    *(int *)((char *)this + 0x4C) = 0;
    *(int *)((char *)this + 0x50) = 0;
    *(gcStateMachine **)((char *)this + 0x54) = this;
    *(int *)((char *)this + 0x58) = 0;
    *(gcStateMachine **)((char *)this + 0x5C) = this;
    *(int *)((char *)this + 0x60) = 0;
    *(gcStateMachine **)((char *)this + 0x64) = this;
    *(int *)((char *)this + 0x68) = 0;
}

// ── gcStateMachine::New(cMemPool *, cBase *) static @ 0x002599e8 ──
cBase *gcStateMachine::New(cMemPool *pool, cBase *parent) {
    void *block = ((void **)pool)[9];
    char *allocTable = *(char **)((char *)block + 0x1C);
    AllocRec *rec = (AllocRec *)(allocTable + 0x28);
    short off = rec->offset;
    void *base = (char *)block + off;
    gcStateMachine *result = 0;
    gcStateMachine *obj = (gcStateMachine *)rec->fn(base, 0x6C, 4, 0, 0);
    if (obj != 0) {
        new (obj) gcStateMachine(parent);
        result = obj;
    }
    return (cBase *)result;
}

// ── gcStateMachine::IsValid(cSubHandleT<gcState>, int) const @ 0x0010d280 ──
int gcStateMachine::IsValid(cSubHandleT<gcState> handle, int offset) const {
    if (offset < 0) {
        return 0;
    }

    volatile int *handleIndexPtr = &handle.mIndex;
    int stateIndex = *handleIndexPtr;
    if (stateIndex == 0) {
        return 0;
    }

    stateIndex = stateIndex & 0xFFFF;
    int nextIndex = stateIndex;
    nextIndex += offset;
    void **states = *(void ***)((char *)this + 0x50);
    int count = 0;
    if (states != 0) {
        count = ((int *)states)[-1];
    }
    if (nextIndex >= count) {
        return 0;
    }
    if (stateIndex < 0) {
        return 0;
    }

    count = 0;
    if (states != 0) {
        count = ((int *)states)[-1];
    }
    if (stateIndex >= count) {
        return 0;
    }

    void *state = states[stateIndex];
    int result = 0;
    if (state != 0) {
        int valid = (*handleIndexPtr == *(int *)((char *)state + 0x20));
        valid = (unsigned char)valid;
        if (valid != 0 && states[nextIndex] != 0) {
            result = 1;
        }
    }
    return (unsigned char)result;
}

// ── gcStateMachine::GetSubObject(cSubHandleT<gcState>, int) const @ 0x0010d1d4 ──
gcState *gcStateMachine::GetSubObject(cSubHandleT<gcState> handle, int offset) const {
    if (offset < 0) {
        return 0;
    }

    volatile int *handleIndexPtr = &handle.mIndex;
    int stateIndex = *handleIndexPtr;
    if (stateIndex == 0) {
        return 0;
    }

    stateIndex = stateIndex & 0xFFFF;
    int nextIndex = stateIndex;
    nextIndex += offset;
    void **states = *(void ***)((char *)this + 0x50);
    int count = 0;
    if (states != 0) {
        count = ((int *)states)[-1];
    }
    if (nextIndex >= count) {
        return 0;
    }
    if (stateIndex < 0) {
        return 0;
    }

    count = 0;
    if (states != 0) {
        count = ((int *)states)[-1];
    }
    if (stateIndex >= count) {
        return 0;
    }

    void *state = states[stateIndex];
    gcState *result = 0;
    if (state != 0) {
        int valid = (*handleIndexPtr == *(int *)((char *)state + 0x20));
        valid = (unsigned char)valid;
        if (valid != 0) {
            result = (gcState *)states[nextIndex];
        }
    }
    return result;
}

// ── gcStateMachine::IsValid(cSubHandleT<gcStateFunction>, int) const @ 0x0010d3d4 ──
int gcStateMachine::IsValid(cSubHandleT<gcStateFunction> handle, int offset) const {
    if (offset < 0) {
        return 0;
    }

    volatile int *handleIndexPtr = &handle.mIndex;
    int stateIndex = *handleIndexPtr;
    if (stateIndex == 0) {
        return 0;
    }

    stateIndex = stateIndex & 0xFFFF;
    int nextIndex = stateIndex;
    nextIndex += offset;
    void **states = *(void ***)((char *)this + 0x58);
    int count = 0;
    if (states != 0) {
        count = ((int *)states)[-1];
    }
    if (nextIndex >= count) {
        return 0;
    }
    if (stateIndex < 0) {
        return 0;
    }

    count = 0;
    if (states != 0) {
        count = ((int *)states)[-1];
    }
    if (stateIndex >= count) {
        return 0;
    }

    void *state = states[stateIndex];
    int result = 0;
    if (state != 0) {
        int valid = (*handleIndexPtr == *(int *)((char *)state + 0x3C));
        valid = (unsigned char)valid;
        if (valid != 0 && states[nextIndex] != 0) {
            result = 1;
        }
    }
    return (unsigned char)result;
}
