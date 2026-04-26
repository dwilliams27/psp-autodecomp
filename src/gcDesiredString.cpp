class cObject;

class gcDesiredString {
public:
    cObject *Get(bool) const;
    cObject *GetObject(bool) const;
};

cObject *gcDesiredString::GetObject(bool b) const {
    return Get(b);
}
