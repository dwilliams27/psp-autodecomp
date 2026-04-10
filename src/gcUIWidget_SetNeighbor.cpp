template <class T>
class cSubHandleT {
public:
    int mIndex;
    ~cSubHandleT() {}
};

class gcUIWidget {
public:
    enum gcUINeighbor {};

    void SetNeighbor(gcUINeighbor dir, cSubHandleT<gcUIWidget> handle);
};

void gcUIWidget::SetNeighbor(gcUINeighbor dir, cSubHandleT<gcUIWidget> handle) {
    char *base = (char *)this + (int)dir * 8;
    int val = handle.mIndex;
    base += 0x30;
    __asm__ volatile("" : "+r"(base));
    *(int *)base = val;
}
