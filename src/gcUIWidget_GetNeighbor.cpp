class gcUIWidget {
public:
    enum gcUINeighbor {};

    int GetNeighbor(gcUINeighbor dir) const;
};

int gcUIWidget::GetNeighbor(gcUINeighbor dir) const {
    char *base = (char *)this + 0x2C;
    base += (int)dir * 8;
    __asm__ volatile("" : "+r"(base));
    base += 4;
    return *(int *)base;
}
