class eShape;

class eCompoundShape {
public:
    char pad[0x80];
    eShape **mSubShapes;
    eShape *GetSubShape(int idx) const;
};

eShape *eCompoundShape::GetSubShape(int idx) const {
    return mSubShapes[idx];
}
