class cObject;

class gcDesiredUIWidget {
public:
    cObject *Get(bool) const;
    cObject *GetObject(bool) const;
};

cObject *gcDesiredUIWidget::GetObject(bool b) const {
    return Get(b);
}
