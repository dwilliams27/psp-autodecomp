extern "C" void *memset(void *, int, unsigned int);

#pragma control sched=1

class eCollisionPair {
public:
    char _pad00[0x10];   // 0x00
    int  mBodyA;         // 0x10
    int  mBodyB;         // 0x14
    int  m18;            // 0x18
    int  m1C;            // 0x1C
    int  m20;            // 0x20
    int  m24;            // 0x24
    int  m28;            // 0x28
    int  m2C;            // 0x2C
    int  m30;            // 0x30
    int  m34;            // 0x34
    int  m38;            // 0x38
    int  m3C;            // 0x3C
    unsigned short m40;  // 0x40
    unsigned short m42;  // 0x42
    unsigned short m44;  // 0x44
    char _pad46[0x02];   // 0x46
    int  m48;            // 0x48
    char m4C[0x10];      // 0x4C
    int  m5C;            // 0x5C
    eCollisionPair() {
        m18 = 0;
        m1C = 0;
        m20 = 0;
        m24 = 0;
        m28 = 0;
        m2C = 0;
        m30 = 0;
        m34 = 0;
        m38 = 0;
        m3C = 0;
        m40 = 0;
        m42 = 0;
        m44 = 0;
        m48 = 0;
        memset(m4C, 0, 0x10);
    }
};

template <class T>
class cRedBlackTreeNode : public T {
public:
    int mLeft;
    int mRight;
    int mParent;
    cRedBlackTreeNode();
};

template <class T>
cRedBlackTreeNode<T>::cRedBlackTreeNode() : mLeft(0), mRight(0), mParent(0) {
}

template cRedBlackTreeNode<eCollisionPair>::cRedBlackTreeNode();
