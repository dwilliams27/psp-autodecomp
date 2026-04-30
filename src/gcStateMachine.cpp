class cBase;
class cFile;
class cMemPool;
class gcStateFunction;

inline void *operator new(unsigned int, void *p) { return p; }

class cType {
public:
    static cType *InitializeType(const char *, const char *, unsigned int,
                                 const cType *,
                                 cBase *(*)(cMemPool *, cBase *),
                                 const char *, const char *, unsigned int);
};

class cNamed {
public:
    static cBase *New(cMemPool *, cBase *);
};

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(bool);
    void Write(int);
    void Write(unsigned int);
    void End(void);
};

class cHandle {
public:
    int mId;
    void Write(cWriteBlock &) const;
};

class cBaseArray {
public:
    void *mData;
    void *mOwner;
    void Write(cWriteBlock &) const;
};

class cObject {
public:
    cObject(cBase *);
    void Write(cFile &) const;
};

class gcState {
public:
    char _pad0[0x20];
    unsigned int mField20;
    cHandle mField24;
    unsigned int mField28;
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

extern cType *D_000385DC;
extern cType *D_000385E0;
extern cType *D_000385E4;
extern cType *D_0009A3E8;

class gcStateMachine : public cObject {
public:
    char _pad[0x44];
    cHandle mField44;
    cHandle mField48;
    unsigned int mField4C;
    cBaseArray mField50;
    cBaseArray mField58;
    cBaseArray mField60;
    int mField68;

    gcStateMachine(cBase *);

    const cType *GetType(void) const;
    static cBase *New(cMemPool *, cBase *);
    gcState *GetSubObject(cSubHandleT<gcState>, int) const;
    int IsValid(cSubHandleT<gcState>, int) const;
    int IsValid(cSubHandleT<gcStateFunction>, int) const;
    void Write(cFile &) const;
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

// ── gcStateMachine::GetType(void) const @ 0x00259a64 ──
const cType *gcStateMachine::GetType(void) const {
    if (D_0009A3E8 == 0) {
        if (D_000385E4 == 0) {
            if (D_000385E0 == 0) {
                if (D_000385DC == 0) {
                    D_000385DC = cType::InitializeType((const char *)0x36D894,
                                                       (const char *)0x36D89C,
                                                       1, 0, 0, 0, 0, 0);
                }
                D_000385E0 = cType::InitializeType(0, 0, 2, D_000385DC,
                                                   &cNamed::New, 0, 0, 0);
            }
            D_000385E4 = cType::InitializeType(0, 0, 3, D_000385E0,
                                               0, 0, 0, 0);
        }
        D_0009A3E8 = cType::InitializeType(0, 0, 0xB6, D_000385E4,
                                           &gcStateMachine::New,
                                           (const char *)0x36D954,
                                           (const char *)0x36D964,
                                           5);
    }
    return D_0009A3E8;
}

// ── gcStateMachine::Write(cFile &) const @ 0x0010c708 ──
void gcStateMachine::Write(cFile &file) const {
    cWriteBlock wb(file, 4);
    cObject::Write(file);

    gcState **temp_s3 = *(gcState ***)((char *)this + 0x50);
    int var_a0 = 0;
    if (temp_s3 != 0) {
        var_a0 = ((int *)temp_s3)[-1];
    }
    wb.Write(var_a0);

    int var_s2 = 0;
    int var_s1 = 0;
loop:
    int var_a0_2 = 0;
    temp_s3 = *(gcState ***)((char *)this + 0x50);
    if (temp_s3 != 0) {
        var_a0_2 = ((int *)temp_s3)[-1];
    }
    if (var_s2 < var_a0_2) {
        gcState *state = *(gcState **)((char *)temp_s3 + var_s1);
        wb.Write(state != 0);
        if (state != 0) {
            wb.Write(state->mField20);
            state->mField24.Write(wb);
            wb.Write(state->mField28);
        }
        var_s2 += 1;
        var_s1 += 4;
        goto loop;
    }

    mField44.Write(wb);
    mField48.Write(wb);
    wb.Write(mField4C);
    mField50.Write(wb);
    mField58.Write(wb);
    mField60.Write(wb);
    wb.End();
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
