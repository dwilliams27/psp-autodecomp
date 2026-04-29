class cBase;

class cMemPool {
public:
    static cMemPool *GetPoolFromPtr(const void *);
};

extern "C" void free(void *);

struct cListSubscriberDeleteRecord {
    short offset;
    short pad;
    void (*fn)(void *, void *);
};

// Layout-only view for SNC's vtable-at-offset-4 virtual dispatch.
class cListSubscriberVirtualView {
public:
    cBase *mOwner;

    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual int GetRemovedIndex(void *);
};

class cListSubscriber {
public:
    cBase *mOwner;            // 0x00
    void *mTypeInfo;          // 0x04
    void *mNext;              // 0x08
    void *mPrev;              // 0x0C
    unsigned char mAttached;  // 0x10
    char _pad11[3];
    int mFirst;               // 0x14
    int mLast;                // 0x18
    int mCount;               // 0x1C
    int mNumVisible;          // 0x20

    cListSubscriber(cBase *owner);
    ~cListSubscriber();
    void SetNumVisible(int numVisible);
    void OnCleared(void);
    void Attach(void);
    void Detach(void);
    void OnAdded(void *);
    void OnRemoved(void *);
    void Select(int);

    static void operator delete(void *p) {
        cMemPool *pool = cMemPool::GetPoolFromPtr(p);
        if (pool != 0) {
            char *block = ((char **)pool)[9];
            cListSubscriberDeleteRecord *rec =
                (cListSubscriberDeleteRecord *)(((char **)block)[7] + 0x30);
            short off = rec->offset;
            void (*fn)(void *, void *) = rec->fn;
            fn(block + off, p);
        } else {
            free(p);
        }
    }
};

cListSubscriber::cListSubscriber(cBase *owner) {
    mOwner = owner;
    mTypeInfo = (void *)0x0037E718;
    mNext = 0;
    mPrev = 0;
    mAttached = 0;
    mFirst = -1;
    mLast = -1;
    mCount = 0;
    mNumVisible = 0;
}

void cListSubscriber::OnCleared(void) {
    mLast = -1;
    mFirst = -1;
    mCount = 0;
}

void cListSubscriber::Attach(void) {
    if (!mAttached) {
        mAttached = 1;
    }
}

void cListSubscriber::Detach(void) {
    if (mAttached) {
        mLast = -1;
        mFirst = -1;
        mCount = 0;
        mAttached = 0;
    }
}

void cListSubscriber::OnAdded(void *item) {
    mCount++;
    if (mFirst < 0) {
        mLast = 0;
        mFirst = 0;
    }
}

void cListSubscriber::OnRemoved(void *item) {
    int index = ((cListSubscriberVirtualView *)this)->GetRemovedIndex(item);
    int last = mLast;
    int first = mFirst;
    if (index < last) {
        mLast = last - 1;
    }
    mCount = mCount - 1;
    if (index < first) {
        first = first - 1;
    } else {
        __asm__ volatile("" ::: "memory");
    }
    Select(first);
}

cListSubscriber::~cListSubscriber() {
    mTypeInfo = (void *)0x0037E6A8;
}

void cListSubscriber::SetNumVisible(int numVisible) {
    mNumVisible = numVisible;
}
