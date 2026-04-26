class cObject;

class gcDesiredEnumeration {
public:
    cObject *Get(bool) const;
    cObject *GetObject(bool) const;
};

cObject *gcDesiredEnumeration::GetObject(bool b) const {
    return Get(b);
}
