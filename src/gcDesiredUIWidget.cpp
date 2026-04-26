class cObject;

struct gcDesiredUIWidgetHelper {
    int _a;
    int _b;
    int _c;
    void GetText(char *) const;
};

class gcDesiredUIWidget {
public:
    cObject *Get(bool) const;
    cObject *GetObject(bool) const;
    void GetText(char *) const;
};

cObject *gcDesiredUIWidget::GetObject(bool b) const {
    return Get(b);
}

void gcDesiredUIWidget::GetText(char *buf) const {
    ((gcDesiredUIWidgetHelper *)((char *)this + 0xC))->GetText(buf);
}
