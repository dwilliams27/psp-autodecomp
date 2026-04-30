// gcStateInfo — decompiled from Days of Thunder (PSP)

class gcStateMachine;
class gcState;
class gcEnumeration;
class gcEnumerationEntry;
class gcEventParams;
class gcEventStackData;
class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

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
extern void *D_0037D850;

struct DeleteRecord {
    short offset;
    short _pad;
    void (*fn)(void *, void *);
};

extern "C" {
void free(void *);
void gcEnumeration__Get_intconst(void *, void *, int);
}

class gcStateInfo {
public:
    cHandlePairT<gcStateMachine, cSubHandleT<gcState> > mCurrentState; // 0-7
    char mField4;
    char mField5;
    int mField8;
    int mFieldC;
    cHandlePairT<gcStateMachine, cSubHandleT<gcState> > mField10; // 10-17
    int mField18;
    int mField1C;
    int mField20;

    gcStateInfo();
    ~gcStateInfo();
    float HandleUpdate(int dt, const gcEventParams &params, const gcEventStackData &stack);
    int IsValidState(const cHandlePairT<gcStateMachine, cSubHandleT<gcState> > &pair) const;
    void Reset(cHandleT<gcStateMachine> handle, const cHandlePairT<gcStateMachine, cSubHandleT<gcState> > &pair);
    float Send(const cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > &pair,
               const gcEventParams &params, const gcEventStackData &stack);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            void *block = *(void **)((char *)pool + 0x24);
            DeleteRecord *rec = (DeleteRecord *)(*(char **)((char *)block + 0x1C) + 0x30);
            short off = rec->offset;
            rec->fn((char *)block + off, p);
        } else {
            free(p);
        }
    }
};

// The actual layout (from asm): 9 words + bytes at 4,5. Expose as raw bytes.
struct gcStateInfoLayout {
    int mWord0;
    char mByte4;
    char mByte5;
    char _pad6;
    char _pad7;
    int mWord8;
    int mWordC;
    int mWord10;
    int mWord14;
    int mWord18;
    int mWord1C;
    int mWord20;
};

// ----- Function 2: gcStateInfo::~gcStateInfo() -----
gcStateInfo::~gcStateInfo() {
}

// ----- Function 1: gcStateInfo::gcStateInfo() -----
gcStateInfo::gcStateInfo() {
    gcStateInfoLayout *p = (gcStateInfoLayout *)this;
    p->mWord0 = 0;
    p->mByte4 = 0;
    p->mByte5 = 0;
    p->mWord8 = 0;
    p->mWordC = 0;
    p->mWord10 = 0;
    p->mWord14 = 0;
    p->mWord18 = -1;
    p->mWord1C = -1;
    p->mWord20 = 0;
}

typedef int cTimeValue;

extern "C" {
void *gcStateMachine__GetSubObject(void *, void *, int);
void cHandlePairT_GetName(const void *, char *, int, int);
}

// ----- Function 4: gcStateInfo::IsValidState -----
int gcStateInfo::IsValidState(const cHandlePairT<gcStateMachine, cSubHandleT<gcState> > &pair) const {
    char buf[256];

    if (*(void **)this == 0) return 0;

    int h = pair.mHandle.mIndex;
    void *e;
    void *sm;
    if (h == 0) {
        sm = 0;
    } else {
        e = D_00038890[h & 0xFFFF];
        sm = 0;
        if (e != 0 && *(int *)((char *)e + 0x30) == h) {
            sm = e;
        }
    }

    void *state = 0;
    if (sm != 0) {
        cSubHandleT<gcState> sub = pair.mSubHandle;
        state = gcStateMachine__GetSubObject(sm, &sub, 0);
    }
    if (state == 0) return 0;

    void *cur = *(void **)this;
    void *target = *(void **)state;
    while (cur != 0 && cur != target) {
        int h2 = *(int *)((char *)cur + 0x44);
        if (h2 == 0) {
            cur = 0;
        } else {
            void *e2;
            void *next = 0;
            e2 = D_00038890[h2 & 0xFFFF];
            if (e2 != 0 && *(int *)((char *)e2 + 0x30) == h2) {
                next = e2;
            }
            cur = next;
        }
    }
    if (cur != target) {
        buf[0] = 0;
        cHandlePairT_GetName(&pair, buf, 0, 1);
        return 0;
    }
    return 1;
}

// ----- Function 3: gcStateInfo::HandleUpdate -----
typedef cHandlePairT<gcEnumeration, cSubHandleT<gcEnumerationEntry> > EventPair;

float gcStateInfo::HandleUpdate(int dt, const gcEventParams &params, const gcEventStackData &stack) {
    gcStateInfoLayout *p = (gcStateInfoLayout *)this;
    int m18 = p->mWord18;
    cTimeValue saved_a = dt;   // sp+0x14 spill
    if (m18 > 0) {
        cTimeValue saved_b = dt;
        int new_m18 = m18 - dt;
        cTimeValue saved_c = dt;
        p->mWord18 = new_m18;
        if (new_m18 <= 0) {
            int idx = *(int *)((char *)D_0037D850 + 0x4C);
            void *e = 0;
            if (idx != 0) {
                e = D_00038890[idx & 0xFFFF];
            }
            EventPair spC;
            gcEnumeration__Get_intconst(e, &spC, 3);
            this->Send(spC, params, stack);
        }
    }
    if (p->mByte4 & 1) {
        return 0.0f;
    }
    int idx = *(int *)((char *)D_0037D850 + 0x4C);
    void *e = 0;
    if (idx != 0) {
        e = D_00038890[idx & 0xFFFF];
    }
    EventPair sp4;
    gcEnumeration__Get_intconst(e, &sp4, 2);
    return this->Send(sp4, params, stack);
}

// ----- Function 5: gcStateInfo::Reset -----
void gcStateInfo::Reset(cHandleT<gcStateMachine> handle, const cHandlePairT<gcStateMachine, cSubHandleT<gcState> > &pair) {
    gcStateInfoLayout *p = (gcStateInfoLayout *)this;

    int h = handle.mIndex;
    void *e;
    void *sm;
    if (h == 0) {
        sm = 0;
    } else {
        e = D_00038890[h & 0xFFFF];
        sm = 0;
        if (e != 0 && *(int *)((char *)e + 0x30) == h) {
            sm = e;
        }
    }

    p->mWord0 = (int)sm;
    p->mByte4 = 0;
    p->mByte5 = 0;
    cTimeValue tmpA = -1;
    p->mWord18 = -1;
    cTimeValue tmpB = -1;
    p->mWord1C = -1;
    p->mWord8 = 0;
    cTimeValue tmpC = 0;
    p->mWordC = 0;

    int h2 = pair.mHandle.mIndex;
    void *e2;
    void *sm2;
    if (h2 == 0) {
        sm2 = 0;
    } else {
        e2 = D_00038890[h2 & 0xFFFF];
        sm2 = 0;
        if (e2 != 0 && *(int *)((char *)e2 + 0x30) == h2) {
            sm2 = e2;
        }
    }

    void *state = 0;
    if (sm2 != 0) {
        cSubHandleT<gcState> sub = pair.mSubHandle;
        state = gcStateMachine__GetSubObject(sm2, &sub, 0);
    }

    if (state != 0) {
        p->mWord10 = pair.mHandle.mIndex;
        p->mWord14 = pair.mSubHandle.mIndex;
    } else {
        void *cur_sm = *(void **)this;
        if (cur_sm != 0) {
            p->mWord10 = *(int *)((char *)cur_sm + 0x48);
            p->mWord14 = *(int *)((char *)cur_sm + 0x4C);
        }
    }

    cHandlePairT<gcStateMachine, cSubHandleT<gcState> > *m10ref =
        (cHandlePairT<gcStateMachine, cSubHandleT<gcState> > *)&p->mWord10;
    if (!this->IsValidState(*m10ref)) {
        p->mWord10 = 0;
        cTimeValue tmpD = 0;
        p->mWord14 = 0;
    }
}
