#include "gcGenericSubscriber.h"

class cWriteBlock {
public:
    int _data[2];
    cWriteBlock(cFile &, unsigned int);
    void Write(int);
    void End(void);
};

class cListSubscriber {
public:
    void Attach(void);
    void Detach(void);
    void Write(cFile &) const;
};

class cSubscriberArray {
public:
    int *mData;
    cSubscriberArray &operator=(const cSubscriberArray &);
};

gcGenericSubscriber *dcast(const cBase *);

void gcGenericSubscriber::Attach(void) {
    ((cListSubscriber *)this)->Attach();
}

void gcGenericSubscriber::Detach(void) {
    ((cListSubscriber *)this)->Detach();
}

int gcGenericSubscriber::GetItem(int idx) const {
    int *arr = *(int **)((const char *)this + 0x24);
    int count = 0;
    if (arr) count = arr[-1] & 0x3FFFFFFF;
    if (count <= 0) return idx + 1;
    int result = 0;
    if (idx < 0) return result;
    if (idx >= count) return result;
    result = arr[idx];
    return result;
}

void gcGenericSubscriber::AssignCopy(const cBase *base) {
    struct Layout {
        char _p0[0x08];
        int f08;
        int f0C;
        unsigned char f10;
        char _p2[3];
        int f14;
        int f18;
        int f1C;
        int f20;
    };
    gcGenericSubscriber *other = dcast(base);
    Layout *self_l = (Layout *)this;
    Layout *other_l = (Layout *)other;
    self_l->f08 = other_l->f08;
    self_l->f0C = other_l->f0C;
    self_l->f10 = other_l->f10;
    self_l->f14 = other_l->f14;
    self_l->f18 = other_l->f18;
    self_l->f1C = other_l->f1C;
    self_l->f20 = other_l->f20;
    ((cSubscriberArray *)((char *)this + 0x24))->operator=(*(cSubscriberArray *)((char *)other + 0x24));
}

void gcGenericSubscriber::Write(cFile &file) const {
    cWriteBlock wb(file, 2);
    ((const cListSubscriber *)this)->Write(file);
    int count = 0;
    int *arr = *(int **)((const char *)this + 0x24);
    if (arr) count = arr[-1] & 0x3FFFFFFF;
    wb.Write(count);
    wb.End();
}
