class cObject;

class gcDesiredEntityTemplate {
public:
    cObject *Get(bool) const;
    cObject *GetObject(bool) const;
};

cObject *gcDesiredEntityTemplate::GetObject(bool b) const {
    return Get(b);
}
