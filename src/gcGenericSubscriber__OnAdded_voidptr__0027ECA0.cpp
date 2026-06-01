// ODR-WARNING: split-TU local redeclaration of gcGenericSubscriber.
// Target: gcGenericSubscriber::OnAdded(void *) @ 0x0027eca0
// Symbol: __0fTgcGenericSubscriberHOnAddedPv
//
// The single void* param arrives in a1 (a0=this). Its bits are
// reinterpreted as a float and stored into mItems[index]. The base
// cListSubscriber::OnAdded(void*) is invoked first (it ignores a1).

class cListSubscriber {
public:
    void OnAdded(void *);
};

class gcGenericSubscriber {
public:
    char pad[0x1c];
    int mIndex;             // +0x1c
    char pad2[0x04];
    float *mItems;          // +0x24
    void OnAdded(void *);
};

void gcGenericSubscriber::OnAdded(void *p)
{
    int index = mIndex;
    float *items = mItems;
    int count = 0;
    if (items != 0) {
        count = ((int *)items)[-1] & 0x3fffffff;
    }
    if (count > 0 && index >= count) {
        return;
    }
    ((cListSubscriber *)this)->OnAdded(p);
    if (count > 0) {
        float value = *(float *)&p;
        mItems[index] = value;
    }
}
